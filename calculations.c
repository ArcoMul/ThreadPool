#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include "shared.h"
#include "job.h"
#include "thread.h"
#include "thread_pool.h"

pthread_mutex_t counter_mutex;

int counter = 0;

void task1 ()
{
    pthread_mutex_lock(&counter_mutex);
    counter++;
    pthread_mutex_unlock(&counter_mutex);
}

void task2 ()
{
    pthread_mutex_lock(&counter_mutex);
    counter++;
    pthread_mutex_unlock(&counter_mutex);
}

int main ()
{
    if (DEBUG) printf("Start program\n");

    int thread_count = sysconf( _SC_NPROCESSORS_ONLN ) + 1;

    if (DEBUG) printf("Create thread pool with %d threads\n", thread_count);

    // Create pool
    thread_pool p;

    // Init pool
    thread_pool_init (&p, thread_count);

    if (DEBUG) printf("Thread pool successfuly created\n");

    if (pthread_mutex_init(&counter_mutex, NULL) != 0)
    {
        printf("\n mutex init failed\n");
        return 1;
    }

    int i;
    for (i = 0; i < 10; i++)
    {
        thread_pool_queue_task(&p, &task1, (void*)0, "task1", 0, 10000); 
        thread_pool_queue_task(&p, &task2, (void*)0, "task2", 0, 10000); 
    }

    thread_pool_start(&p);

    pthread_join(p.worker_id, NULL);
    pthread_mutex_destroy(&counter_mutex);

    printf("Final coutner value: %d\n", counter);

    printf("Finished program\n");

    pthread_exit(NULL);

    return 0;
}
