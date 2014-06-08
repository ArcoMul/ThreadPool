#include<stdio.h>
#include<pthread.h>
#include "thread.h"

void* thread_run (void *ptr)
{
    struct thread *thread = (struct thread*) ptr;
    thread->id = (unsigned int) pthread_self();
    thread->is_done = false;
    thread->has_job = false;
    printf("Thread with id %u created\n", thread->id);

    // Run!
    while (!thread->is_done) {
        if (thread->has_job) {
            printf("Execute job on thread %u\n", thread->id);
            thread->job->f();
            printf("Finished executing job on thread %u\n", thread->id);
        }
        thread->has_job = false;
    }
    pthread_exit(NULL);
    return;
}

void assign_job_to_thread (struct thread *t, struct job *j)
{
    t->job = j;
    t->has_job = true;
}
