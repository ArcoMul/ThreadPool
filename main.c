#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include "shared.h"
#include "job.h"
#include "thread.h"
#include "thread_pool.h"

/**
 * Player object
 */
typedef struct player {
    int x;
    int y;
    pthread_mutex_t mutex;
} player;

/**
 * Renders the world with the player on it
 */
void render (void *ptr)
{
    if (DEBUG) printf("Render\n");

    // Get the player
    struct player *p = (struct player*) ptr;

    // Create the world 5x5 character array
    char *world[5][5];
    int row;
    int col;
    for (row = 0; row < 5; row++) {
        for (col = 0; col < 5; col++) {
            world[row][col] = "X";
        }
    }

    // Put the player in the world
    pthread_mutex_lock (&p->mutex);
    world[p->y][p->x] = "O";
    pthread_mutex_unlock (&p->mutex);

    // Output the world (and the player)
    printf("\n");
    for (row = 0; row < 5; row++) {
        for (col = 0; col < 5; col++) {
            printf("%s", world[row][col]);
        }
        printf("\n");
    }
}

/**
 * Updates the position of the player
 */
void update (void *ptr)
{
    if (DEBUG) printf("Update\n");

    // Get the player
    struct player *p = (struct player*) ptr;

    // Make the player walk one position on the x axis
    // and one down if it reaches the end of the row
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

    int thread_count = sysconf( _SC_NPROCESSORS_ONLN );

    if (DEBUG) printf("Create thread pool with %d threads\n", thread_count);

    // Create pool
    thread_pool p;

    // Init pool
    init_thread_pool (&p, thread_count);

    if (DEBUG) printf("Thread pool successfuly created\n");

    player plyr;
    plyr.x = 0;
    plyr.y = 0;
    pthread_mutex_init(&plyr.mutex, NULL);
    
    // Keep track of time while running
    clock_t start_time, current_time;
    start_time = current_time = clock();

    // How often the app is rendered
    int frame = 0;

    // If the app is running
    bool running = true;
    
    // Amount of updates of the app
    int tick = 0;

    // How long to sleep
    struct timespec t;
    t.tv_sec = 0;
    t.tv_nsec = 10000000; // 100000000 = 0.1 sec

    while (running)
    {
        if (DEBUG) printf("Run tick %d\n", tick);

        current_time = clock();
        float time_since_start = (((float) current_time - (float) start_time) / 1000000.0F ) * 1000;  

        if (DEBUG) printf("%f miliseconds since start\n", time_since_start);

        // Update every tick
        thread_pool_queue_task(&p, &update, &plyr, "update", time_since_start, 1000); 

        // Only render every second
        if (time_since_start / 100 > frame) {
            thread_pool_queue_task(&p, &render, &plyr, "render", time_since_start, 1000); 
            frame++;
        }

        // Process all the threads
        process_thread_pool(&p, time_since_start);

        if (DEBUG) printf("End tick %d\n", tick);

        // Keep track of the amount of ticks
        tick++;

        // Sleep for a bit
        nanosleep(&t, NULL);
    }

    if (DEBUG) printf("Finished program\n");

    return 0;
}
