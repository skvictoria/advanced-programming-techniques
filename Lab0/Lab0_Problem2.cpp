/*
Author: Seulgi Kim
Class: ECE6122 A
Last Date Modified: 3/9/2023

Description:

This code gets natural number from user and outputs multiples of 3 or 5 and its sum.
*/
#include <iostream> // std::cout
#include <vector>   // std::vector
#include <numeric>  // accumulate

// input status (0 if terminate, 1 if processing)
enum eStatus
{
    TERMINATE = 0,
    PROCESSING      // default
};

/**
 * @brief This method check the incoming input and extract valid input.
 * @param strInputStream The incoming input from user.
 * @param u32ValidNum    Returns the valid integer input.
 * @return 1 if succeed extracting valid input, 0 if not.
*/
uint8_t ExtractValidInput(const std::string strInputStream, uint32_t& u32ValidNum)
{
    uint8_t u8Success = 1U;
    char cSubString;

    // check if all digits in strInputstream has 0~9.
    if (strInputStream.length() > 0U)
    {
        for (uint8_t u8StrIdx = 0U; u8StrIdx < strInputStream.length(); u8StrIdx++)
        {
            cSubString = (strInputStream.substr(u8StrIdx, 1))[0];
            // if any one of the input is not digit, failed.
            if (isdigit(cSubString) == 0)
            {
                u8Success = 0U;
            }
        }
    }
    else
    {
        u8Success = 0U;
    }

    if (u8Success == 1U)
    {
        // extract valid input.
        u32ValidNum = stoi(strInputStream);
        if (u32ValidNum == eStatus::TERMINATE)
        {
            u8Success = 0U;
        }
    }

    return u8Success;
}

/**
 * @brief This method extract the multiples of the targeted number.
 * @param s32TargetNum The targeted number for multiplies.
 * @param u32InputNum  The entered number from user.
 * @param vStore       Returns all of the multiples of targeted number.
*/
void SolveMultiple(const int32_t s32TargetNum, const uint32_t u32InputNum, std::vector<uint32_t>& vStore)
{
    uint8_t u8HasStarted = 0U;

    std::cout << "The multiples of " << s32TargetNum << " below " << u32InputNum << " are: ";
    for (uint32_t u32Idx = 1U; u32Idx < u32InputNum; u32Idx++)
    {
        if (u32Idx % s32TargetNum == 0)
        {
            if (u8HasStarted == 1U)
            {
                std::cout << ", ";
            }
            std::cout << u32Idx;
            u8HasStarted = 1U;
            vStore.push_back(u32Idx);
        }
    }
    std::cout << "." << std::endl;
}

int main()
{
    // input number shall be greater than or equal to 0.
    uint32_t u32InputNum = eStatus::PROCESSING;
    std::string strInputStream;
    uint8_t u8GotValid = 0U;

    while (u32InputNum != eStatus::TERMINATE)
    {
        std::vector<uint32_t> vMultiples;
        
        // while getting invalid input,
        while (u8GotValid == 0U)
        {
            // ask input to user.
            std::cout << "Please enter a natural number (0 to quit): ";

            // get input.
            getline(std::cin, strInputStream);

            // if the input is valid,
            if (ExtractValidInput(strInputStream, u32InputNum) == 1U)
            {
                u8GotValid = 1U;
            }
            // if the input is invalid because the input is equal to 0,
            else if (u32InputNum == eStatus::TERMINATE)
            {
                break;
            }
            // else the input is invalid,
            else
            {
                ;
            }
        } // end of while.
        
        if (u32InputNum == eStatus::TERMINATE)
        {
            std::cout << "Program terminated." << std::endl;
            break;
        }

        SolveMultiple(3, u32InputNum, vMultiples);
        SolveMultiple(5, u32InputNum, vMultiples);

        std::cout << "The sum of all multiples is: " << accumulate(vMultiples.begin(), vMultiples.end(), 0) << "." << std::endl;
        
        u8GotValid = 0U;
        std::vector<uint32_t>().swap(vMultiples);
    } // end of while.
    std::cout << "Have a nice day!" << std::endl;
    
    return 0;
}