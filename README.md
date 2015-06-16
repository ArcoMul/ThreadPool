ThreadPool
==========

A threadingpool to run an arbitrary number of 'tasks' on an arbitrary number of 'threads'. 

The repository includes two examples to demonstrate the thread pool.

Example 'Calculations'
-------------------

This example will calculate the first thousand prime numbers in a backwards way. Every task
of the threadpool calculates one prime number. This example demonstrates how a threadpool
benefits when doing a lot of the same tasks in a asynchronios way. The example will run in
the shortest time when using at least the amount of processors available.

**To run the example:**

- Clone the repository
- Run `$ make calculations`
- Run `$ ./calculations`
- The program will run for some time and then print the prime numbers 1 to 1000

Example 'Rendering'
-------------------

This example will print a 'world' of X's and a O will walk through it in a smooth way. The
example demonstrates a over simplified render engine. The update function of the world
runs as often as possible, while the render function runs only 20 times per second.

**To run the example:**

- Clone the repository
- Run `$ make rendering`
- Run `$ ./rendering`
- A field with X's will print and a O is walking through it

Development
-----------

- Set the DEBUG variable in shared.c to true or false to enable/disable debugging
