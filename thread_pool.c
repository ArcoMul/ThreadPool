#include<pthread.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "shared.h"
#include "thread_pool.h"

int init_thread_pool (struct thread_pool *p, int n)
{
    if (DEBUG) printf("Init thread pool, create %d threads\n", n);

    // Start values
    p->is_empty = true;
    p->pool_size = n;
    p->job_count = 0;
    
    // Create all the threads
    int i = 0;
    int err;
    while (i < p->pool_size) {
        pthread_t id;
        p->threads[i].has_job = false;
        err = pthread_create(&id, NULL, &thread_run, &(p->threads[i]));
        if (err != 0)
            if (DEBUG) printf("Can't create thread :[%s]\n", strerror(err));
        else
            if (DEBUG) printf("Thread created successfully\n");
        i++;
    }
    return 0;
}

/**
 * Assigns new jobs to idle threads, drops job which are over their deadline
 */
void process_thread_pool(struct thread_pool *p, int current_time)
{
    if (p->is_empty) return;

    // Count the amount of jobs left
    job *j = p->job_queue_first;
    int job_count = 1;
    while (j->has_next) {
        j = j->next;
        job_count++;
    }

    // Count the amount of idle threads
    int thread_count = 0;
    int i = 0;
    while (i < p->pool_size) {
        if (!p->threads[i].has_job) {
            thread_count++;
        } else {
            if (p->threads[i].has_job) {
                if (DEBUG) printf("Has job\n");
                if (DEBUG) printf("Thread %d has job with name %s\n", p->threads[i].id, p->threads[i].job->name);
            }
        }
        i++;
    }

    if (DEBUG) printf("Process %d jobs on %d threads\n", job_count, thread_count);

    i = 0;
    while (i < p->pool_size && !p->is_empty)
    {
        // Check if the thread doesn't have a job already
        if (!p->threads[i].has_job)
        {
            // Skip all the jobs which passed their deadline
            while (!p->is_empty && p->job_queue_first->start_time + p->job_queue_first->deadline < current_time) {
                if (DEBUG) printf("Drop task deadline %d is bigger than %d", p->job_queue_first->deadline, current_time);
                thread_pool_next_job(p);
            }
            
            // Assign the job to a thread
            assign_job_to_thread(&(p->threads[i]), p->job_queue_first);

            // Drop the job and advance to the next one
            thread_pool_next_job(p);
        }
        i++;
    }
}

/**
 * Advance to the next job in the thread
 */
void thread_pool_next_job (struct thread_pool *p)
{
    job *first = p->job_queue_first;
    if (first == NULL) {
        printf("No jobs to free\n");  
    } else if (first->has_next) {
        p->job_queue_first = first->next;
        free(first);
    } else {
        p->is_empty = true;
        free(first);
    }
}

/**
 * Add a new task/job
 */
void thread_pool_queue_task(struct thread_pool *p, void (*function)(void *data), void *data, char name[10], int start_time, int deadline)
{
    if (DEBUG) printf ("Add job %d to thread pool\n", p->job_count);

    // Initialize job
    job *j; 
    j = malloc(sizeof(struct job));
    j->has_next = false;
    j->function = function;
    j->data = data;
    j->id = p->job_count++;
    j->start_time = start_time;
    j->deadline = deadline;
    strcpy(j->name, name);

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

/**
 * Check if threadpool is empty
 */
bool thread_pool_empty (struct thread_pool *p)
{
    return p->is_empty;
}

/**
 * Stop thread pull, set all threads on done
 */
void thread_pool_stop (struct thread_pool *p)
{
    if (DEBUG) printf("Stop thread pool\n");
    int i = 0;
    while (i < p->pool_size) {
        p->threads[i].is_done = true;
        pthread_join((pthread_t) p->threads[i].id, NULL);
        i++;
    }
}
