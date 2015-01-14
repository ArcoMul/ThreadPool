ThreadPoolMake: shared.h shared.c job.h thread.h thread.c thread_pool.h thread_pool.c
	gcc -pthread shared.c thread.c thread_pool.c main.c -o ThreadPool -g
