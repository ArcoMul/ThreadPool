#ifndef THREAD_H_
#define THREAD_H_ 

#include<stdbool.h>

#include "job.h"

typedef struct thread {
    pthread_t id;
    bool has_job;
    bool is_done;
    struct job *job;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} thread;

void* thread_run (void *ptr);
void assign_job_to_thread (struct thread *t, struct job *j);

#endif
