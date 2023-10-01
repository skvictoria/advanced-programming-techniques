#include <chrono> // std::chrono::seconds
#include <thread>
#include <iostream>

void pause_thread(int n)
{
    int something=n;

    std::this_thread::sleep_for(std::chrono::seconds(n)); // point to the current active thread
    // AVOID SLEEP!!!!!!!!!!!!!!!!!!!!!!! - usually bad programming.
    //  .. why: it slows the code down.

    std::cout <<"pause of" << n << " seconds ended\n";
}

int main()
{
    std::thread threads[5];

    std::cout<<"Spawning 5 thrads .. \n";

    for (int i=0; i<5; i++)
    {
        threads[i] = std::thread(pause_thread, i+1); // move-assing threads
    }

    std::cout << "done spawning thraeads . Now waiting for them to join \n";

    for(int i=0; i<5; i++)
    {
        threads[i].join();
    }

    std::cout << "All threads joined\n";

    return 0;
}