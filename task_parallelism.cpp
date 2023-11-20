#include <iostream>
using namespace std;

int main() {
    const int maxN = 1000000000;
    int m[maxN];

    // load data into m[] (omitted) -> task1

    //calculate average -> task2
    double avrg = m[0];
    for(int j=1; j<maxN; j++) {
      avrg = avrg + m[j];
    }
    avrg = avrg / maxN;

    //sorting array -> task3
    qsort(m, maxN, sizeof(int), compare_int);

    // write results to file (omitted) -> task 4
}

//task 1 is independent of task 2, 3, 4
//task 2 is dependent on task 1 and task 3
//task 3 is dependent on task 1 and task 2
//taske 4 is dependent on task 1, 2, 3
