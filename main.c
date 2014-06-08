#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include "shared.h"
#include "job.h"
#include "thread.h"
#include "thread_pool.h"

typedef struct player {
    int x;
    int y;
    pthread_mutex_t mutex;
} player;

void render (void *ptr)
{
    if (DEBUG) printf("Render\n");
    struct player *p = (struct player*) ptr;
    char *world[5][5];
    int row;
    int col;
    for (row = 0; row < 5; row++) {
        for (col = 0; col < 5; col++) {
            world[row][col] = "X";
        }
    }

    pthread_mutex_lock (&p->mutex);
    world[p->y][p->x] = "O";
    pthread_mutex_unlock (&p->mutex);

    printf("\n");
    for (row = 0; row < 5; row++) {
        for (col = 0; col < 5; col++) {
            printf("%s", world[row][col]);
        }
        printf("\n");
    }
}

void update (void *ptr)
{
    if (DEBUG) printf("Update\n");
    struct player *p = (struct player*) ptr;
    pthread_mutex_lock (&p->mutex);
    p->x += 1;
    if (p->x > 5) {
        p->y += 1;
        p->x = 0;
    }
    if (p->y > 5) {
        p->y = 0;
        p->x = 0;
    }
    pthread_mutex_unlock (&p->mutex);
}

int main ()
{
    if (DEBUG) printf("Start program\n");

    int thread_count = 8;

    if (DEBUG) printf("Create thread pool\n");

    // Create pool
    thread_pool p;

    // Init pool
    init_thread_pool (&p, thread_count);

    if (DEBUG) printf("Thread pool successfuly created\n");

    player plyr;
    plyr.x = 0;
    plyr.y = 0;
    pthread_mutex_init(&plyr.mutex, NULL);

    bool running = true;
    int tick = 0;
    struct timespec t;
    t.tv_sec = 0;
    t.tv_nsec = 100000000;
    while (running)
    {
        if (DEBUG) printf("Run tick %d\n", tick);

        process_thread_pool(&p);

        thread_pool_queue_task(&p, &update, &plyr); 
        if (tick % 3 == 0) {
            thread_pool_queue_task(&p, &render, &plyr); 
        }

        if (DEBUG) printf("End tick %d\n", tick);
        nanosleep(&t, NULL);
        tick++;
    }

    if (DEBUG) printf("Finished program\n");

    return 0;
}
