#include<stdio.h>
#include<pthread.h>
#include "shared.h"
#include "thread.h"

bool thread_is_done (struct thread *t)
{
    return t->is_done;
}

/**
 * Run function of this thread
 */
void* thread_run (void *ptr)
{
    struct thread *thread = (struct thread*) ptr;

    // Start values
    thread->id = pthread_self();
    thread->is_done = false;
    thread->has_job = false;

    if (pthread_mutex_init(&thread->mutex, NULL) != 0)
    {
        printf("mutex init failed\n");
        return;
    }

    if (pthread_cond_init (&thread->cond, NULL) != 0)
    {
        printf("cond init failed\n");
        return;
    }

    if (DEBUG) printf("Thread with id %u created\n", (unsigned int) thread->id);

    // Run!
    pthread_mutex_lock(&thread->mutex);
    while (!thread_is_done(thread))
    {
        // If the thread has a job, execute it
        pthread_cond_wait(&thread->cond, &thread->mutex);
        if (thread->has_job) {
            if (DEBUG) printf("Execute job %d on thread %u\n", thread->job->id, (unsigned int) thread->id);
            thread->job->function(thread->job->data);
            if (DEBUG) printf("Finished executing job %d on thread %u\n", thread->job->id, (unsigned int) thread->id);
            thread->has_job = false;
        }
    }
    pthread_mutex_unlock(&thread->mutex);
    if (DEBUG) printf("Exit thread #%u\n", (unsigned int) thread->id);
    pthread_exit(NULL);
    return;
}

/**
 * Give this thread a job
 */
void assign_job_to_thread (struct thread *t, struct job *j)
{
    pthread_mutex_lock(&t->mutex);
    if (DEBUG) printf("Assign job %d to thread %u\n", j->id, (unsigned int) t->id);
    t->job = j;
    t->has_job = true;
    if (DEBUG) printf("Job assigned\n");
    pthread_cond_signal(&t->cond);
    pthread_mutex_unlock(&t->mutex);
}


void thread_stop (struct thread *t)
{
    pthread_mutex_lock(&t->mutex);
    if (DEBUG) printf("Stop thread %u\n", (unsigned int) t->id);
    t->is_done = true;
    pthread_cond_signal(&t->cond);
    pthread_mutex_unlock(&t->mutex);
}
