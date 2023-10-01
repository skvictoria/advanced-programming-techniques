/*
Author: Seulgi Kim
Date last modified: 24/8/2023
Organization: ECE4122_6122 Class

Description:

Simple Function to take in an integer and display the square back to the user.
*/

#include <iostream>

// function prototypes
// if not defined, then link error
int32_t SquareIt(int32_t s32Num);

int main()
{
    int32_t s32iNum = 0;
    std::cout << "Please Enter the number: "; 
        // std : standard library
        // cout : console out (cout belongs to the namespace std)
    std::cin >> s32iNum; // blocking code, do not return until the user input something.
    std::cout << "Your Number squared is: " << s32iNum * s32iNum;
    std::cout << "Your Number squared is: " << SquareIt(s32iNum); // function call

    return 0;
}

int32_t SquareIt(int32_t s32Num)
{
    return s32Num * s32Num;
}