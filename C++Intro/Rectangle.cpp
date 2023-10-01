#include <iostream>
#include "Rectangle.h"

// 1. template
// 2. extern
// 3. stack space grow or shrink if you pass by reference

void Rectangle::set_values(int x, int y)
{
    // restricted to rectangle
    width = x;
    height = y;
}

void set_values(int x, int y)
{
    // global function
}

int main()
{
    Rectangle rect;
    rect.set_values(3,4);
    std::cout << "Area: " << rect.area();
    return 0;
}