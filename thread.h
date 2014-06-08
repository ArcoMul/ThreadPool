#ifndef THREAD_H_
#define THREAD_H_ 

#include<stdbool.h>

#include "job.h"

typedef struct thread {
    unsigned int id;
    bool has_job;
    bool is_done;
    struct job *job;
} thread;

void* thread_run (void *ptr);
void assign_job_to_thread (struct thread *t, struct job *j);

#endif
