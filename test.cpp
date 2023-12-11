#include <iostream>
#include <thread>
#include <chrono>

alignas(64) long long num1 = 0; //바뀐 부분
alignas(64) long long num2 = 0; //바뀐 부분
long long num3 = 0;

void fun1() {
    for (long long i = 0; i < 1000000000; i++)
        num1 += 1;
}

void fun2() {
    for (long long i = 0; i < 1000000000; i++)
        num2 += 1;
}

void fun3() {
    for (long long i = 0; i < 2000000000; i++) {
        num3 += 1;
    }
}
int main() {
    auto beginTime = std::chrono::high_resolution_clock::now();

    std::thread t1(fun1);   //Multi Thread 실행
    std::thread t2(fun2);   //Multi Thread 실행

    t1.join(); t2.join();

    auto endTime = std::chrono::high_resolution_clock::now(); 
    std::chrono::duration<double> resultTime = endTime - beginTime;

    printf("%lld\n", num1 + num2);
    std::cout << resultTime.count() << std::endl;
    printf("--------------------\n");
    beginTime = std::chrono::high_resolution_clock::now();

    fun3(); //Single Thread 실행
    
    endTime = std::chrono::high_resolution_clock::now();
    resultTime = endTime - beginTime;
    printf("%lld\n", num3);
    std::cout << resultTime.count() << std::endl;
}