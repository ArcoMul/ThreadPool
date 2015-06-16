#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <pthread.h>
#include <math.h>
#include <unistd.h>
#include "shared.h"
#include "job.h"
#include "thread.h"
#include "thread_pool.h"

int WORLD_WIDTH = 30;
int WORLD_HEIGHT = 10;
int WALK_SPEED = 20;
int FRAMES_PER_SEC = 20;

/**
 * Player object
 */
typedef struct player {
    float x;
    float y;
    double last_update;
    pthread_mutex_t mutex;
} player;

pthread_mutex_t render_mutex;

double get_miliseconds ()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    double time_in_mill = (tv.tv_sec) * 1000 + (tv.tv_usec) / 1000 ;
    return time_in_mill;
}

/**
 * Renders the world with the player on it
 */
void render (void *ptr)
{
    if (DEBUG) printf("Render\n");

    // Get the player
    struct player *p = (struct player*) ptr;

    // Create the world 5x5 character array
    char *world[WORLD_HEIGHT][WORLD_WIDTH];
    int row;
    int col;
    for (row = 0; row < WORLD_HEIGHT; row++) {
        for (col = 0; col < WORLD_WIDTH; col++) {
            world[row][col] = "X";
        }
    }

    // Put the player in the world
    pthread_mutex_lock (&p->mutex);
    world[(int)(p->x / WORLD_WIDTH)][((int)p->x) % WORLD_WIDTH] = "O";
    pthread_mutex_unlock (&p->mutex);

    // Output the world (and the player)
    pthread_mutex_lock (&render_mutex);
    printf("\n");
    for (row = 0; row < WORLD_HEIGHT; row++) {
        for (col = 0; col < WORLD_WIDTH; col++) {
            printf("%s", world[row][col]);
        }
        printf("\n");
    }
    pthread_mutex_unlock (&render_mutex);
    if (DEBUG) printf("Rendered!\n");
}

/**
 * Updates the position of the player
 */
void update (void *ptr)
{
    if (DEBUG) printf("Update\n");

    // Get the player
    struct player *p = (struct player*) ptr;

    double current_time = get_miliseconds();

    // Make the player walk one position on the x axis
    // and one down if it reaches the end of the row
    pthread_mutex_lock (&p->mutex);
    double seconds_since_last_update = (current_time - p->last_update) / 1000;
    p->x += (float) WALK_SPEED * seconds_since_last_update;
    if (p->x > WORLD_WIDTH * WORLD_HEIGHT) {
        p->x -= WORLD_WIDTH * WORLD_HEIGHT;
    }
    p->last_update = current_time;
    pthread_mutex_unlock (&p->mutex);
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

    player plyr;
    plyr.x = 0;
    plyr.y = 0;
    plyr.last_update = get_miliseconds();
    pthread_mutex_init(&plyr.mutex, NULL);

    pthread_mutex_init(&render_mutex, NULL);
    
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

    double starttime = get_miliseconds();

    thread_pool_start(&p, false);

    while (running)
    {
        float time_since_start = (get_miliseconds() - starttime) / 1000;

        if (DEBUG) printf("%f seconds since start\n", time_since_start);

        thread_pool_queue_task(&p, &update, &plyr, "update", 100); 

        // Only render every second
        if (time_since_start * FRAMES_PER_SEC > frame) {
            // Update every tick
            thread_pool_queue_task(&p, &render, &plyr, "render", 1000); 
            frame++;
        }

        // Keep track of the amount of ticks
        tick++;

        // Sleep for a bit
        nanosleep(&t, NULL);
    }

    pthread_join(p.worker_id, NULL);

    if (DEBUG) printf("Finished program\n");

    pthread_exit(NULL);

    return 0;
}
