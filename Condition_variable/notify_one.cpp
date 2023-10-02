#include <mutex>
#include <iostream>
#include <condition_variable>
#include <thread>

std::condition_variable produce, consume;
std::mutex mtx;
int cargo;

void consumer()
{
    std::unique_lock<std::mutex> lck(mtx);
    while(cargo == 0)
    {
        consume.wait(lck);
    }
    std::cout << cargo << "\n";
    cargo = 0;
    produce.notify_one();
}

void producer(int id)
{
    std::unique_lock<std::mutex> lck(mtx);
    while(cargo != 0)
    {
        produce.wait(lck);
    }
    cargo = id;
    consume.notify_one();
}

int main()
{
    std::thread consumers[10], producers[10];

    return 0;
}