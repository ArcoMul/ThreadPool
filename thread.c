#include<stdio.h>
#include<pthread.h>
#include "shared.h"
#include "thread.h"

/**
 * Run function of this thread
 */
void* thread_run (void *ptr)
{
    struct thread *thread = (struct thread*) ptr;

    // Start values
    thread->id = (unsigned int) pthread_self();
    thread->is_done = false;
    thread->has_job = false;

    if (DEBUG) printf("Thread with id %u created\n", thread->id);

    // Run!
    while (!thread->is_done)
    {
        // If the thread has a job, execute it
        if (thread->has_job) {
            if (DEBUG) printf("Execute job %d on thread %u\n", thread->job->id, thread->id);
            thread->job->function(thread->job->data);
            if (DEBUG) printf("Finished executing job %d on thread %u\n", thread->job->id, thread->id);
        }
        // Otherwise save that the thread doesn't have a job
        thread->has_job = false;
    }
    pthread_exit(NULL);
    return;
}

/**
 * Give this thread a job
 */
void assign_job_to_thread (struct thread *t, struct job *j)
{
    if (DEBUG) printf("Assign job %d to thread %u\n", j->id, t->id);
    t->job = j;
    t->has_job = true;
}
