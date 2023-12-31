#include <iostream>
#include <utility>
#include <atomic>

using namespace std;

struct A { int a[100]; };
    atomic<A> myA;
    atomic<int> i;

//needs -latomic in compiling command

int main() {
    cout << boolalpha
        << myA.is_lock_free() << ' ' // likely false
        << i.is_lock_free() << '\n'; // likely true
    return 0;
}