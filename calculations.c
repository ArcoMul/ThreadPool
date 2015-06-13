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

void prime_number (void* ptr)
{
    int* input = (int*) ptr;
    int wanted_prime_n = input[0];
    int current_n = 3, prime_n, i;

    // Loop through prime numbers
    for (prime_n = 1; prime_n <= wanted_prime_n; )
    {
        // Loop through every number
        for (i = 2; i <= current_n - 1; i++)
        {
            if (current_n % i == 0)
            break;
        }
        // Current number is only dividable by itself
        if (i == current_n)
        {
            if (prime_n == wanted_prime_n)
                break;
            prime_n++;
        }
        current_n++;
    }

    // The number we had when we breaked out of the loop
    // is the number belonging to the n'th prime number
    // we were looking for
    input[1] = current_n;
}

int main (int argc, char *argv[])
{
    if (DEBUG) printf("Start program\n");

    int thread_count;
    if (argc > 1 && isdigit(*argv[1]) && atoi(argv[1]) > 0) {
        thread_count = atoi(argv[1]);
    } else {
        thread_count = sysconf( _SC_NPROCESSORS_ONLN ) + 1;
    }

    if (DEBUG) printf("Create thread pool with %d threads\n", thread_count);

    // Create pool
    thread_pool p;

    // Init pool
    thread_pool_init (&p, thread_count);

    if (DEBUG) printf("Thread pool successfuly created\n");

    int n = 1000;
    int input[n][2];
    int i;
    for (i = 0; i < n; i++)
    {
        input[i][0] = i + 1;
        thread_pool_queue_task(&p, &prime_number, (void*)&input[i], "prime", 0, 10000); 
    }

    clock_t start_time, current_time;
    start_time = current_time = clock();

    thread_pool_start(&p);

    pthread_join(p.worker_id, NULL);

    current_time = clock();
    float time_since_start = (((float) current_time - (float) start_time) / CLOCKS_PER_SEC);  

    for (i = 0; i < n; i++)
    {
        printf("Prime number %d = %d\n", input[i][0], input[i][1]);
    }

    printf("Calculations where done in %f\n", time_since_start);

    printf("Finished program\n");

    pthread_exit(NULL);

    return 0;
}
