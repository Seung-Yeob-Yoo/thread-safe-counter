# thread-safe-counter

1. Compare performance
=>
Input = 10000

Mutex: 0.001 sec
![tscounter](https://user-images.githubusercontent.com/83752097/121810552-56e8e480-cc9c-11eb-91b0-ab72b1fd6ed9.png)

Semaphore: 1.412 sec 
![sem_counter](https://user-images.githubusercontent.com/83752097/121810571-6a944b00-cc9c-11eb-9bb7-017604cba888.png)

2. Analysis(why?)
=>
Mutex : Only one thread holding the lock enters (A mutex can be unlocked by who locked it)

Semaphore : Multiple enters with a counter (There is no lock owner concept)

Mutex use hardware for only one thread. In contrast, Semaphore use hardware for many threads.
Therefore, Mutex is faster than Semaphore. 

3. OS environment
=>
WSL2
