ThreadPoolMake: job.h thread.h thread.c thread_pool.h thread_pool.c
	gcc -pthread thread.c thread_pool.c main.c -o ThreadPool
