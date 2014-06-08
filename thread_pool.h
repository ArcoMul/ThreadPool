#ifndef THREAD_POOL_H_
#define THREAD_POOL_H_

#include<stdbool.h>

#include "job.h"
#include "thread.h"

typedef struct thread_pool {
    int pool_size;
    struct thread threads[2];
    bool is_empty;
    struct job *job_queue_first;
    struct job *job_queue_last;
} thread_pool;

int init_thread_pool (struct thread_pool *p, int n);
void process_thread_pool(struct thread_pool *p);
void thread_pool_queue_task(struct thread_pool *p, void (*f)(void));
void stop_thread_pool (struct thread_pool *p);

#endif
