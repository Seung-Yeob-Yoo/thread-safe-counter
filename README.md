# thread-safe-counter

1. Compare performance
=>
Input = 10000
Mutex: 0.001 sec
Semaphore: 0.125 sec 

2. Analysis(why?)
=>
Mutex : Only one thread holding the lock enters (A mutex can be unlocked by who locked it)

Semaphore : Multiple enters with a counter (There is no lock owner concept)

Mutex use hardware for only one thread. In contrast, Semaphore use hardware for many threads.
Therefore, Mutex is faster than Semaphore. 

3. OS environment
=>
WSL2
