#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include "shared.h"
#include "job.h"
#include "thread.h"
#include "thread_pool.h"

void addition (void* ptr)
{
    int* s = (int*) ptr;
    s[2] = s[0] + s[1];
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

    int n = 1000;
    int input[n][3];
    int i;
    for (i = 0; i < n; i++)
    {
        input[i][0] = i;
        input[i][1] = i;
        thread_pool_queue_task(&p, &addition, (void*)&input[i], "addition", 0, 10000); 
        printf("%d + %d = ?\n", input[i][0], input[i][1]);
    }

    clock_t start_time, current_time;
    start_time = current_time = clock();

    thread_pool_start(&p);

    pthread_join(p.worker_id, NULL);

    current_time = clock();
    float time_since_start = (((float) current_time - (float) start_time) / CLOCKS_PER_SEC);  

    for (i = 0; i < n; i++)
    {
        printf("%d + %d = %d\n", input[i][0], input[i][1], input[i][2]);
    }

    printf("Calculations where done in %f\n", time_since_start);

    printf("Finished program\n");

    pthread_exit(NULL);

    return 0;
}
