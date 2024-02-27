#include <iostream>
using namespace std;
void test(int *p)
{
    p = new int[10];
}
int main()
{
    int *p;
    test(p);
    p[0] = 1;
    cout << p[0];
    return 0;
}