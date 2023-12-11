//smallest shared resource 
#include <iostream>
#include <pthread.h>
using namespace std;

#define MAX 2
#define MAX_ITER 100000000

volatile long counter = 0; //uses the value and does not make variable memory in this program 

void * thr_f(void * arg) {
    long l; 
    for (int i = 0; i < MAX_ITER; i++) {
        counter++;
    }
}

int main() {
    pthread_t tid[MAX]; 
    int t = 0; //creating thread id 
    for (t = 0; t < MAX; t++) {
        pthread_create(&tid[t], NULL, thr_f, NULL); //creating threads
    }
    for (t = 0; t < MAX; t++) {
        pthread_join(tid[t], NULL); //joining threads
    }
    printf("Expected value: %lld, observed counter" " value: %lld\n", MAX_ITER * MAX, counter);
    printf("You experienced %lld lost updates,  or %d%% of all accesses to the shared variable.\n",
    MAX_ITER * MAX - counter, 100 - 100 * counter / (MAX_ITER * MAX));
}
//to solve this 
//use atomic libary
#include <atomic>
#include <thread>
#include <vector>

atomic<long> counter(0); //template is below
vector<thread> threads;

void threadF() {
    unsigned long l;
    for (l = 0; l < MAX_ITER; l++) {
        counter++;
    }
}

int main(void) {
    for (int i = 0; i < MAX; i++) {
        threads.push_back(thread(threadF));
    }

    for (int i = 0; i < MAX; i++) {
        threads[i].join();
    }

    // Use .load() to read the value of the atomic variable
    printf("Expected value: %lld, observed counter value: %lld\n", (long long)(MAX_ITER * MAX), (long long)(counter.load()));
    printf("You experienced %lld lost updates, or %d%% of all accesses to the shared variable.\n",
           (long long)(MAX_ITER * MAX - counter.load()), 100 - (int)(100 * counter.load() / (MAX_ITER * MAX)));
}
//atomic
template <class T> struct atomic; //template for atomic struct
template <> struct atomic<Integral>; //specialization for Integral types
template <class T> struct atomic<T*>; //pointer is also possible

int main() {
  a=0; //operator= (assignment)
  a.is_lock_free(); //returns true if atomic variable is lock-free
  a.store(2); //store
  a.load(); //load
  atomic<float> a; //T a //conversion to T
  a.exchange(3); //store the data and return the old value
  float b = 2;
  float c = 3;
  a.compare_exchange_weak(b, c)//if b != a, then b = a and return True, else a = c, False
  a.compare_exchange_strong(b, c)//same but retry if fail //one can use weak and while loop to do the same
  a.fetch_add(2); //a = a + 2 atomically //return the old value
  a.fetch_sub(2); //a = a - 2 atomically //return the old value
  a.fetch_and(2); //a = a & 2 atomically //return the old value
  a.fetch_or(2); //a = a | 2 atomically //return the old value
  a.fetch_xor(2); //a = a ^ 2 atomically //return the old value
  a++; //a = a + 1 atomically
  a--; //a = a - 1 atomically
  a+=2; //a = a + 2 atomically
  a-=2; //a = a - 2 atomically
  a&=2; //a = a & 2 atomically
  a|=2; //a = a | 2 atomically
  a^=2; //a = a ^ 2 atomically
}
//mutex //included in pthread.h
long counter = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; //mutex is a struct
void * thr_f(void * arg) {
    long l;
    for (l = 0; l < MAX_ITER; l++) {
        pthread_mutex_lock(&mutex); //lock
        counter++;
        pthread_mutex_unlock(&mutex); //unlock
    }
}
//use the main functioin of pthread above 
//using trylock
//when another thread meets a lock, it will wait until certain amount of time or blocked 
//trylock will make this process faster //more efficient
void * thr_f(void * arg) {
    long l;
    while (pthread_mutex_trylock(&mutex) != 0) {
        //do nothing 
        //pthread_mutex_unlock(&mutex); 
        //this should give an error since lock cannot be unlocked by other thread
    }
    for (l = 0; l < MAX_ITER; l++) {
        counter++;
        pthread_mutex_unlock(&mutex); //unlock
    }
}
//dynamic allocation
int main() {
    pthread_mutex_t * mylock = new pthread_mutex_t; //dynamic allocation in running program
    pthread_mutex_init(mylock, NULL); //initializing the mutex
    //do something
    pthread_mutex_destroy(mylock); //destroying the mutex
    delete mylock; //deleting the mutex
}
//example of dynamic allocation in linked list
struct node {
    pthread_mutex_t * node_lock; //mutex for each node
    int counter;
    struct node * next;
};
//testing
int main() {
    struct node * head = new node;
    head->counter = 0;
    head->next = NULL;
    pthread_mutex_init(head->node_lock, NULL);
}
//defining thread function
void incrementEvenListNodes (struct Node * ptr) {
    while (ptr != NULL) {
        pthread_mutex_lock(&ptr->node_lock); //locks for each node
        if (ptr->counter % 2 == 0) {
            ptr->counter++;
        }
        pthread_mutex_unlock&ptr->node_lock);
        ptr = ptr->next;
    }
}
//combine both above and write linked list counter program

//monitor 
static long counter = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
//monitor applied functions
void increment_ctr() {
    pthread_mutex_lock(&mutex);
    counter++;
    pthread_mutex_unlock(&mutex);
}

void read_ctr() {
    long ret;
    pthread_mutex_lock(&mutex);
    ret = counter;
    pthread_mutex_unlock(&mutex);
}
//serialization 
 int long_computation(int)
    {
        int i;
        for (i = 0; i < 100000000; i++) {
        }
        return 0;
    }
//this makes serialization worse
unsigned long counter = 0;

void * thr_f(void * arg) {
    long l;
    for (l = 0; l < MAX_ITER; l++) {
        pthread_mutex_lock(&mutex);
        counter += long_computation(l);
        pthread_mutex_unlock(&mutex);
    }
}
//this makes serialization better
void * thr_f(void * arg) {
    long l, tmp;
    for (l = 0; l < MAX_ITER; l++) {
        tmp = long_computation(l);
        pthread_mutex_lock(&mutex);
        counter += tmp;
        pthread_mutex_unlock(&mutex);
    }
}
//deadlock
//self deadlock
void * thr_f(void * arg) {
    pthread_mutex_lock(&mutex);
    pthread_mutex_lock(&mutex); //cannot lock itself from this outside
    pthread_mutex_unlock(&mutex);
}
//ABBA deadlock
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER;
void * thr_f1(int &a) {
    pthread_mutex_lock(&mutex1);
    int b;
    b = a;
    pthread_mutex_unlock(&mutex1);
}
void * thr_f2(int &b) {
    pthread_mutex_lock(&mutex2);
    int a;
    b = a;
    pthread_mutex_unlock(&mutex2);
}
//cannot access each other
//philosopher deadlock
#define MAX 5
pthread_t thr[MAX];
pthread_mutex_t m[MAX];

int thr_args[MAX];

void * tfunc (void * arg) {
    long i = (long) arg; // thread id: 0..4 
    for (;;) { // repeat forever
        pthread_mutex_lock( &m[i] ); //accessing left chopstick //aquiring mutex i
        pthread_mutex_lock( &m[(i + 1) % MAX] ); //accessing right chopstick //aquiring mutex i + 1
        //% will give 1 when i + 1 = 6
        printf("Philosopher %d is eating...\n", i);
        pthread_mutex_unlock(&m[i]);
        pthread_mutex_unlock(&m[(i + 1) % MAX]);
    }
}

int main() {
    int j;
    for (j = 0; j < MAX; j++) {
    pthread_mutex_init(&m[j], NULL);
    }
    for (j = 0; j < MAX; j++) {
        thr_args[j] = j;
        pthread_create(&thr[j], NULL, tfunc, (void *)&thr_args[j]);
    }
    pthread_exit(NULL); //waits for all threads to finish
}
//solution
void * tfunc (void * arg) {
  int i = *(int *)arg;
  for (;;) {
    if (  i < ((i + 1) % MAX) ) { // enforce locking hierarchy
      pthread_mutex_lock(&m[i]); 
      pthread_mutex_lock(&m[(i + 1) % MAX]); 
    } else {
      pthread_mutex_lock(&m[(i + 1) % MAX]);  //5 = 1 and
      pthread_mutex_lock(&m[i]); 
    }
    printf("Philosopher %d is eating...\n", i);
    pthread_mutex_unlock(&m[i]); // order of unlock operations does not matter!
    pthread_mutex_unlock(&m[(i + 1) % MAX]); 
  }
}
//c++17 mutex //not pthread_mutex
#include <algorithms>
#include <list>
#include <mutex> //mutex


list<int> some_list; //get list container
mutex some_mutex; //from mutex header

//lock guard //for deadlock free and mutual exclusion
void add_to_list(int new_value) {
  lock_guard<mutex> guard(some_mutex); //ensures RAII for mutexes
  some_list.push_back(new_value); //locked in the constructor and unlocked in the destructor 
  //though there is no unlock(), returning or exiting function will unlock the mutex
}

bool list_contrains(int value_to_find) {
  lock_guard<mutex> guard(some_mutex); //the same
  return find(some_list.begin(), some_list.end(), value_to_find) \
  != some_list.end();
}
//applying lock guard 
class some_big_object; 
void swap(some_big_object &lhs, some_big_object &rhs);
class X {
  private: 
  some_big_object some_detail;
  mutex m;
  public:
  X(some_big_object const &sd) : some_detail(sd) { //X gets some_big_object as a variable
    friend void swap(X &lhs, X &rhs) {
      if (&lhs == &rhs) //when swaping function approaches the same memory, it does not do anything
        return; //it may try to lock the same memory twice if this part is gone //self mutex avoided
      lock(lhs.m, rhs.m); //locking two mutexes
      lock_guard<mutex> lock_a(lhs.m, adopt_lock); //ensures mutexes are always unlocked -> deadlock avoided
      lock_guard<mutex> lock_b(rhs.m, adopt_lock); //adopt lock is standard class that implies lock guard that mutexes are already locked 
      //which is true in this case
      swap(lhs.some_detail, rhs.some_detail)
    }
  }
}
//scoped lock
void swap(X &lhs, X &rhs) {
  if (&lhs == &rhs)
    return
  scoped_lock guard(lhs.m, rhs.m); //scoped lock enables list mutex arguments -> multi mutex RAII
  swap(lhs.some_detail, rhs.some_detail);
}
//shared_mutex
shared_mutex entry_mutex;

void readSharedData() {
  shared_lock<shared_mutex> lk(entry_mutex); //multi mutex
  //some reading by multiple threads locked
}
void updateSharedData() {
  lock_guard<shared_mutex> lk(entry_mutex); //only single thread can write though
  //some writing by a thread locked
} //still faster than single read and single write

//alignas for false sharing
#include <iostream>
#include <thread>

alignas(64) long long num1 = 0; //padding applied via alignas library
alignas(64) long long num2 = 0;

//counting functions
void fun1() {
    for (long long i = 0; i < 1000000000; i++)
        num1 += 1;
}

void fun2() {
    for (long long i = 0; i < 1000000000; i++)
        num2 += 1;
}

int main() {

    std::thread t1(fun1); //using two threads
    std::thread t2(fun2);   

    t1.join(); t2.join();
} //compile with g++ -std=c++11 -lpthread -o test test.cpp
//test with time ./test using alignas and not from above 
//m1 chip does not show much difference but intel chips do