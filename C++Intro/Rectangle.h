#pragma once // 
// or we can do like
#ifndef Rectangle_H_
    #define Rectangle_H_

    class Rectangle
    {
        int width, height; // default: private
        // if we use struct: then the default is public.
    public:
        void set_values(int, int);
        inline int area() // tell compiler that if there are area(), then replace it.
        // you do not need to function call(quite expensive)
        // iteration whenever possible to the funciton call
        {
            return width*height;
        }
    };

#endif