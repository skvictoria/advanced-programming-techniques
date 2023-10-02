# Optimization

## Common Tips
- Pass Structures and Classes by Reference: Saves you a copy.
- Use void as return type if not returning anything.
- Use Initialization list in constructors.
- Use +=, -=, *=, etc operators whenever possible to save creating temporary copies.
- Delcare variables only in the scope that they are needed.
- Avoid repeated dynamic memory allocations. Try to reuse memory when every possible.
    - Memory Pool Class
- Think about memory access when doing calculations.
    - Single Thread: Try access adjacent data (cache line)
        - 2D array: Go through the columns first and go to the next row.
    - Multi Thread: Try access at least one cache line apart from another thread.
- Function Calls are expensive. (Requires two jumps plus function stack allocation.): So, use iteration over recursion.
- Use inline keyword to reduce function calls.
- Be smart about condition Testing order.
    ```
    if(bTestthree)
    {
        // put simple conditions first.
    }
    else if(isconditionShortTestTrue())
    {

    }
    else if(isconditionLongTestTrue())
    {
        // put long running test functions at end.
    }
    ```

    ```
    if(bTestOne && bTestTwo && isConditionLongTestTrue())
    {
        // if bTestOne is often false, then place it in the front.
    }
    ```

- For loop Unrolling.
    - Use ++x instead of x++.
    - Normal loop vs. Loop Unrolling
        ```
        // normal loop.
        for(x = 0; x < 100; ++x)
        {
            delete(x);
        }
        ```

        ```
        // loop unrolling: multi cores can operate at the same time.
        for(x = 0; x < 100; x += 5)
        {
            delete(x);
            delete(x+1);
            delete(X+2);
            delete(x+3);
            delete(x+4);
        }
        ```