#ifndef JOB_H_
#define JOB_H_

#include<stdbool.h>

typedef void (*job_function)(void *data);

typedef struct job {
    bool has_next;
    int id;
    job_function function;
    void *data;
    struct job *next;
    char name[10];
    double start_time;
    double deadline;
} job;

#endif
