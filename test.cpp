#include <iostream>
#include <mutex>
#include <vector>

using namespace std;

const auto workers = {"Peter", "Clara", "Carl"};

auto on_completion = []() noexcept {
    cout << "phase done\n"; // locking not needed here
};

barrier sync_point(ssize(workers), on_completion);

auto work = [&](string name) {
    string product = " " + name + " worked\n";
    osyncstream(cout) << product; // atomic
    sync_point.arrive_and_wait();
    product = " " + name + " cleaned\n";
    osyncstream(cout) << product;
    sync_point.arrive_and_wait();
};

int main() {
    vector<jthread> threads;
    threads.reserve(size(workers));
    for (auto const& worker : workers)
        threads.emplace_back(work, worker);
}