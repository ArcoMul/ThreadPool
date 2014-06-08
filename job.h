#ifndef JOB_H_
#define JOB_H_

#include<stdbool.h>

typedef void (*job_function)(void);

typedef struct job {
    bool has_next;
    job_function f;
    struct job *next;
} job;

#endif
