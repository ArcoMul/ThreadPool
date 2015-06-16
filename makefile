rendering: shared.h shared.c job.h thread.h thread.c thread_pool.h thread_pool.c rendering.c
	gcc -pthread shared.c thread.c thread_pool.c rendering.c -o rendering -g

calculations: shared.h shared.c job.h thread.h thread.c thread_pool.h thread_pool.c calculations.c
	gcc -pthread shared.c thread.c thread_pool.c calculations.c -o calculations -g
