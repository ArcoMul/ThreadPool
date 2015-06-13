#ifndef THREAD_POOL_H_
#define THREAD_POOL_H_

#include<stdbool.h>

#include "job.h"
#include "thread.h"

typedef struct thread_pool {
    int pool_size;
    pthread_t worker_id;
    int job_count;
    struct thread *threads;
    bool is_empty;
    struct job *job_queue_first;
    struct job *job_queue_last;
} thread_pool;

int thread_pool_init (struct thread_pool *p, int n);
void thread_pool_start (struct thread_pool *p);
void* thread_pool_run (void *ptr);
void thread_pool_process(struct thread_pool *p, int current_time);
void thread_pool_queue_task(struct thread_pool *p, void (*function)(void *ptr), void *data, char name[10], int start_time, int deadline);
void thread_pool_next_job (struct thread_pool *p);
bool thread_pool_is_empty (struct thread_pool *p);
void thread_pool_stop (struct thread_pool *p);

#endif
