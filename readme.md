ThreadPool
==========

A threadingpool to run certain tasks in their own thread and schedule these tasks to be run on a certain amount of threads.

Small example terminal game / simulation included.

The render() and update() function will be outsourced to the threading pool. render() will be run every _x_ seconds, while update() is run as much as possible.
