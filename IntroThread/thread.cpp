// reference: https://cplusplus.com/reference/multithreading/

// THE BENEFITS WHEN USING THREAD
// assumption: thread not interacting each other.
// 1. It can speed things up
// 2. To do jobs (If you go to the task manager, then there are threads Handling the gui displays, )

// how do we create those threads?
// windows / linux has different operating system that would run different kinds of thread programming.
// c++ does: support standards for threading interface. (windows, mac, ubuntu compatible)


// thread is a class.
// thread : concurrently or parallelized.
// this_thread

// thread example from https://cplusplus.com/reference/thread/thread/
#include <iostream>       // std::cout
#include <thread>         // std::thread

#include "function.h"

int main() 
{
    // hardware_concurrency() is static function. we did not create any instances.
    std::cout << "This system can execute " << std::thread::hardware_concurrency() << "threads" << std::endl;
    std::thread foofirst (foo);     // spawn new thread that calls foo() // we need to give its version of main
    std::thread barsecond (bar,0);  // spawn new thread that calls bar(0) // we need to give its version of main

    // .. as soon as foofirst and barsecond execute, the foo and bar is executed.
    // .. here now we have 3 threads (including the main)

    std::cout << "main, foo and bar now execute concurrently...\n";

    // We are now creating child threads
    // join is a blocking function: main got BLOCK until foo and bar gets finished.
    //    .. however: blocking function means that thread is stuck. 
    //    .. it cannot do anything 
    //    .. however, join is efficient because if join does not exist, then it cannot clean up what it has supposed to do.
    // synchronize threads:
    foofirst.join();                // pauses until first finishes
    barsecond.join();               // pauses until second finishes

    std::cout << "foo and bar completed.\n";

    return 0;
}