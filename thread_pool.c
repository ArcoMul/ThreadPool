#include<pthread.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "thread_pool.h"

int init_thread_pool (struct thread_pool *p, int n)
{
    printf("Init thread pool, create %d threads\n", n);

    p->is_empty = true;
    p->pool_size = n;
    
    int i = 0;
    int err;
    while (i < p->pool_size) {
        pthread_t id;
        err = pthread_create(&id, NULL, &thread_run, &(p->threads[i]));
        if (err != 0)
            printf("Can't create thread :[%s]\n", strerror(err));
        else
            printf("Thread created successfully\n");
        i++;
    }
    return 0;
}

void process_thread_pool(struct thread_pool *p)
{
    if (p->is_empty) return;

    int i = 0;
    while (i < p->pool_size && !p->is_empty) {
        if (!p->threads[i].has_job) {
            assign_job_to_thread(&(p->threads[i]), p->job_queue_first);
            if (p->job_queue_first->has_next) {
                p->job_queue_first = p->job_queue_first->next;
            } else {
                p->is_empty = true;
            }
        }
        i++;
    }
}

void thread_pool_queue_task(struct thread_pool *p, void (*f)(void))
{
    printf ("Add job to thread pool\n");

    // Initialize job
    job *j; 
    j = malloc(sizeof(struct job));
    j->has_next = false;
    j->f = f;

    if (p->is_empty) {
        p->job_queue_first = j;
        p->job_queue_last = j;
        p->is_empty = false;
        return;
    }
    p->job_queue_last->next = j;
    p->job_queue_last->has_next = true;
    p->job_queue_last = j;
}

void stop_thread_pool (struct thread_pool *p)
{
    int i = 0;
    while (i < p->pool_size) {
        p->threads[i].is_done = true;
        i++;
    }
}
