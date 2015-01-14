#ifndef THREAD_POOL_H_
#define THREAD_POOL_H_

#include<stdbool.h>

#include "job.h"
#include "thread.h"

typedef struct thread_pool {
    int pool_size;
    int job_count;
    struct thread threads[8];
    bool is_empty;
    struct job *job_queue_first;
    struct job *job_queue_last;
} thread_pool;

int init_thread_pool (struct thread_pool *p, int n);
void process_thread_pool(struct thread_pool *p, int current_time);
void thread_pool_queue_task(struct thread_pool *p, void (*function)(void *ptr), void *data, char name[10], int start_time, int deadline);
void thread_pool_next_job (struct thread_pool *p);
void stop_thread_pool (struct thread_pool *p);

#endif
