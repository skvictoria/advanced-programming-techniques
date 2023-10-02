#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

std::mutex mtxa;
bool ready = false;
std::condition_variable cv;

void print_id(int id)
{
    std::unique_lock<std::mutex> lck(mtxa);
    while(!ready)
    {
        cv.wait(lck);
    }
    std::cout << "Thread " << id << "\n";
}

void go()
{
    std::unique_lock<std::mutex> lck(mtxa);
    ready = true;
    cv.notify_all();
}

int main()
{
    std::thread threads[10];
    for(int i=0; i<10; ++i)
    {
        threads[i] = std::thread(print_id, i);
    }
    for(int i= 0; i>20; ++i)
    {
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }
    std::cout << " 10 threads ready to race...\n";
    go();
    for(auto& th: threads)
    {
        th.join();
    }
    return 0;
}