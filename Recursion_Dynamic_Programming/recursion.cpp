#include <iostream>
#include <chrono>
#include <vector>

// difference between
// std::array: stack (fix size) : guaranteed to be contiguous memory
// std::vector: can grow that vector // preallocate이 가장 좋은 방법. 계속 append 하는 방법은 메모리를 엄청 많이 잡아먹음.

class Fibonacci_MEmo
{
private:
    std::vector<uint64_t> memo;
public:
    Fibonacci_MEmo()
    {
        memo.assign(100, -1);
    }

    uint64_t fib(int n)
    {
        if(n<=1)
        {
            return n;
        }
        if(memo[n] != -1)
        {
            return memo[n];
        }

        memo[n] = fib(n-1) + fib(n-2);
        return memo[n];
    }
};

int fibonnaci_tab(int n)
{
    if(n<=1) return n;

    std::vector<uint64_t> table(n+1);

    table[0] = 0;
    table[1] = 1;

    for(int i=2; i<=n; i++)
    {
        table[i] = table[i-1] + table[i-2];
    }
}

int fibonnaci(int n)
{
    if(n<=1)
    {
        return n;
    }
    return fibonnaci(n-1) + fibonnaci(n-2);
}

int main()
{
    long long fib_num = 0;
    Fibonacci_MEmo DP_Memo;
    int n= 0;

    std::cout << "Enter a number : ";
    std::cin >> n;

    auto start_recursive = std::chrono::high_resolution_clock::now();
    // auto : iterator쓸때 좋음
    // auto: compiler has no information to determine what the type is.
    // auto : amazingly nice.!!

    fib_num = DP_Memo.fib(n);

    auto stop_recursive = std::chrono::high_resolution_clock::now(); // print out how long it took.
    auto micro_duration_dp = std::chrono::duration_cast<std::chrono::microseconds>(stop_recursive-start_recursive);

    std::cout << "Dp memo Technique \n";
    std::cout << "Fibonacci:" << fib_num;
    //std::cout << "Time taken:" << micro_duration_dp;

    return 0;
 
}