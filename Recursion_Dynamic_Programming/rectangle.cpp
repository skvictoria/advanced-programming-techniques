// pointer to classes example
#include <iostream>
using namespace std;

class Rectangle {
  int width, height;
public:
    Rectangle(int x, int y) : width(x), height(y) {}
    int area(void) { return width * height; }
};


int main()
{
    // object has its own lifespan.
    Rectangle obj (3, 4); // stack에 처음 생김. 끝날때 clean up 할 필요 없음(automotaically clean up by program itself)
    Rectangle * foo, * bar, * baz;
    foo = &obj; // stack???????
    // pointer : easy to point to differnet location 
    bar = new Rectangle (5, 6); // heap memory. they will stay there, so operating system should free them up.
    // heap memory 와 stack memory에 쌓이는 거 구분하기!
    baz = new Rectangle[2] { {2,5}, {3,6} };
    cout << "obj's area: " << obj.area() << '\n';
    cout << "*foo's area: " << foo->area() << '\n';
    cout << "*bar's area: " << bar[0].area() << '\n'; // 해도 됨.
    //cout << "*bar's area: " << bar->area() << '\n';
    cout << "baz[0]'s area:" << baz[0].area() << '\n';
    cout << "baz[1]'s area:" << baz[1].area() << '\n';       
    delete bar; // heap memory 에 있는 것들은 무조건 delete해주지 않으면 memory leak 남.
    delete[] baz; // delete vs. delete[] -> if delete baz 하면 baz[0]만 delete해줌.
    // smart pointer 쓰면 garbage cleaner 기능을 대신해줌.
    return 0;
}