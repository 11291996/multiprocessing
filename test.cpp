#include <iostream>
#include <thread>
#include <mutex>
#include <queue>

using namespace std;

template<typename T>

class threadsafe_queue {
    private:
        mutex mut;
        queue<T> data_queue;
        condition_variable data_cond;
    public:
        void push(T new_value) {
        lock_guard<mutex> lk(mut);
        data_queue.push(new_value);
        data_cond.notify_all();
        }
        void wait_and_pop(T& value) {
            unique_lock<mutex> lk(mut);
            data_cond.wait(lk,[this]{return !data_queue.empty();});
            value=data_queue.front();
            data_queue.pop();
        }
};

int main() {
    return 0;
}