// example on this
#include <iostream>
using namespace std;

class Dummy {
public:
    bool isitme (Dummy& param);
};

bool Dummy::isitme (Dummy& param)
{
    if (&param == this)  // this operator gives you the memory location of the current object.
                        // in this case, this operator points at &a.
                        // static function이 아닌 이상, class 내의 method는 항상 object가 있어야 하기 때문이다.
    {
        return true;
    }
    else
    {
        return false;
    }
}

int main () {
    Dummy a; // stack memory location
    Dummy a1;
    Dummy a2;
    Dummy* b = &a; // b is going to point the memory location of a.
    Dummy* b1 = &a1;
    Dummy* b2 = &a2;
    if ( b->isitme(a) )
        cout << "yes, &a is b\n";
    return 0;
}