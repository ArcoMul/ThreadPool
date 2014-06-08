#include<stdio.h>

#include "job.h"
#include "thread.h"
#include "thread_pool.h"

void print_something ()
{
    printf("Print something\n");
}

void print_something_long ()
{
    sleep(2);
    printf("Print something long\n");
}

int main ()
{
    printf("Start program\n");

    int thread_count = 2;

    printf("Create thread pool\n");

    // Create pool
    thread_pool p;

    // Init pool
    init_thread_pool (&p, thread_count);

    printf("Thread pool successfuly created\n");

    bool running = true;
    int tick = 0;
    while (running)
    {
        printf("Run tick %d\n", tick);

        if (tick > 5) {
            stop_thread_pool(&p);
            running = false;
        }

        process_thread_pool(&p);

        if (tick == 2) {
            thread_pool_queue_task(&p, &print_something); 
            thread_pool_queue_task(&p, &print_something_long); 
        }

        printf("End tick %d\n", tick);
        sleep(1);
        tick++;
    }

    printf("Finished program\n");

    return 0;
}
