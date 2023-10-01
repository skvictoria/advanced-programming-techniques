// static members in classes
#include <iostream>
using namespace std;

class Dummy {
public:
    static int n;
    Dummy () { n++; };
};

int Dummy::n=0; // static variable is only useful in the function.
// all instances, same variable.

// hw: https://cplusplus.com/doc/tutorial/templates/ 읽어오기.

int main ()
{
    Dummy a;
    Dummy b[5];
    cout << a.n << '\n';
    Dummy * c = new Dummy;
    cout << Dummy::n << '\n';
    delete c;

    return 0;
}