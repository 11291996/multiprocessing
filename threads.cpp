#include <stdio.h>
#include <unistd.h> // for sleep()
                    // system call
//main thread
int main(void){
    printf("Hello from the main thread.\n");
    sleep (60); // take a rest for 60 sec...
    return 0; 
}
//using POSIX threads
#include <pthread.h>
pthread_t my_thread;
void* threadF (void *arg) {
printf("Hello from our first POSIX thread.\n"); sleep(60); //thread number is different
return 0;
}
//defining thread creation function
int pthread_create( //Purpose: create a new thread.
pthread_t *tid, //pointer to thread ID or address of thread ID
const pthread_attr_t *atrr, //pointer to thread attributes
void * (*start_routine) (void *), //pointer to function that thread will execute
void *arg //pointer to argument of thread function
);
//*attr is used to specify the attributes of the thread
//thread can be detached or joinable
//detached will be terminated when the function is finished
//joinable will not be terminated and can be joined later 
int pthread_join( //purpose is to wait for a thread to terminate.
  pthread_t tid, //thread id to be terminated //it gets value not pointer or address
  void ** status, //stores the thread's status whether it is terminated or not
);
//joining a thread multiple times is not allowed
//joinable threads can be detached 
pthread_detach()
//thread termination 
pthread_exit() //terminates itself 
//this can be used in other threads to terminate the thread
//or pthread_join() can return the thread to upper thread
//this returns a void pointer for pthread_join()
//pthread_join() prepares a pointer to store the return value of the thread
//using creation function
int main(void) {
    pthread_create(&my_thread, NULL, threadF, NULL);
    printf("Hello from the main thread.\n"); //thread number is different
    pthread_join(my_thread, NULL);
return 0;
}
//g++ –o hello_thr hello_thr.cpp -lpthread must be used to compile the thread application
//ps -M | grep user to check the process number
//top -o threads -pid PID number to chekch the thread number

//thread synchronization
//header file isostream is not thread safe
#include <iostream>
using namespace std;

void* threadF (void *arg) {
    cout << "Hello from our first POSIX thread.\n";
    return 0;
}

pthread_t my_thread;
int main() {
    pthread_create(&my_thread, NULL, threadF, NULL);
    cout << "Hello from the main thread.\n";
    pthread_join(my_thread, NULL);
return 0;}
//this may result Helllo ffrom outhre,,, //mixed output
//to deal with this one can use osyncstream
#include <ostream> //this library is not in standard library now
//uses mutex to control cout
void* threadF (void *arg) {
    osyncstream(cout) << "Hello from our first POSIX thread.\n";
    return nullptr;
}

pthread_t my_thread;
int main() {
    pthread_create(&my_thread, NULL, threadF, NULL);
    osyncstream(cout) << "Hello from the main thread.\n";
    pthread_join(my_thread, NULL);
return 0;}
//this will result in Hello from the main thread. Hello from our first POSIX thread.

//the execution order is determined by creation of the thread
void* threadF (void *arg) {
    printf("T.a\n");
    printf("T.b\n");
}

int main(void){
    printf("M.a\n");
    pthread_create(&mythread, NULL, threadF, NULL);
    printf("M.b\n");
    printf("M.c\n");
    pthread_join(my_thread, NULL);
    printf("M.d\n");
    return 0;
}
//meaning output like T.a, T.b, M.a is impossible

//however threads can be executed in parallel and this order is controlled by the OS
void* threadF (void *arg) {
    int x = 0;
    printf("T.%d\n", x++);
    printf("T.%d\n", x);
}

int main(void) {
    printf("M.a\n");
    pthread_create(&mythread, NULL, threadF, NULL);
    int y = 0;
    printf("M.%d\n", y++);
    printf("M.%d\n", y);
    pthread_join(my_thread, NULL);
    printf("M.d\n");
    return 0;
}
//meaning from the code above outputs like M.a, M.0, M.1, T.0, T.1 or M.a, T.0, T.1, M.0, M.1 are possible

//threads share same memory space, so variables are also shared
int i = 0;
pthread_t my_thread;
    void* threadF (void *arg) {
        printf("Hello, i is %d.\n", i);
        return NULL;
}
int main(void){
    i = 1;
    pthread_create(&my_thread, NULL, threadF, NULL); //this will print 1, since i is shared
    pthread_join(my_thread, NULL);
    return 0; 
}
//OpenMP is used to implement divide and conquer algorithms for fork-joining
#include <omp.h>
//routine to compute matrix vector product y = Ax
int main() {
#pragma omp parallel for shared(A) //this sets the multithread operation
    for (local_int_t i=0; i<nrow; i++) {
    double sum = 0.0;
    const double * const cur_vals = A.matrixValues[i]; 
    const local_int_t * const cur_inds = A.mtxIndL[i]; 
    const int cur_nnz = A.nonzerosInRow[i];
    for (int j=0; j< cur_nnz; j++)
    sum += cur_vals[j]*xv[cur_inds[j]];
    yv[i] = sum; }
}
//multi thread loop operation example 
#include<stdio.h>
#include<pthread.h>
#define NUMT x // x = number_of_threads
int args[NUMT]; //array of arguments for functions below //can have arguments as its elements
pthread_t threadIDs[NUMT]; //thread ids stored in pthread_t class
//defining thread function
void * tfunc (void * p) {
    printf("thread arg is %d\n", * (int *) p );
}
//create threads 
int main(void) {
    int i;
    for (i=0; i < NUMT; i++) { //creating threads with thread ids stored 
    args[i] = i; //initializing arguments for thread i 
    pthread_create(&threadIDs[i], NULL,
                   tfunc, (void *) &arg[i] ); //gets the int argument of the thread function from array above
                   //if the argument is not in array but a value in one address, data race will occur and make problems
    }
    for (i=0; i < NUMT; i++) {
    pthread_join(threadIDs[i], NULL); //joining via thread ids
    }
    return 0;
}
//getting more than one argument
#include<stdio.h>
#include<pthread.h>
pthread_t threadID;
struct multi_arg {
    int range;
    int tomato;
};
//defining thread function
void * tfunc (void * p) {
    struct multi_arg * ptr = (struct multi_arg *) p;
    printf("range: %d, tomato %d\n",
    ptr->range, ptr->tomato);
}
int main(void) {
    //getting multiple arguments
    struct multi_arg ma;
    ma.range = 1;
    ma.tomato = 2;
    pthread_create(&threadID, NULL, &tfunc, (void *) &ma );
    pthread_join(threadID, NULL);
return 0; 
}
//life time issue
//define thread
#define NUM_THREADS 5
//define thread function
void *PrintHello(void *threadid)
{
long tid = (long)threadid;
printf("Hello World! It's me, thread #%d!\n", tid); pthread_exit(NULL);
}
//main function
int main (long argc, char *argv[]) {
    pthread_t threads[NUM_THREADS]; //this has to be defined outside of main function
    //or when other main function calls the threads, there will be race condition 
    long rc, t;
    for(t=0; t<NUM_THREADS; t++) {
        printf("In main: creating thread %ld\n", t);
        rc = pthread_create(&threads[t], NULL, PrintHello, (void *)t);
        if (rc){
            printf("ERROR pthread_create returned %ld\n", rc);
            exit(-1);
        }
    }
    pthread_exit(NULL); //since this ends the main thread and life time issue can occur
}
exit() // this function ends all thread and the process itself
//this function is equivalent to finish the main function
pthread_t thr;
void * Hello(void * p) {
    sleep(20);
    printf("Hello World!\n");
    pthread_exit(NULL);
}
int main(void) {
    pthread_create(&thr, NULL, Hello, NULL);
    //pthread_exit(NULL); 
    //exit(); all these will end the main thread and the process
} //the end of main function will end the main thread and the process
//to get thread status from above, global variables must be used
void * Hello_1 (void * p) {
    int ret1;
    ret1 = 0;
    sleep(20);
    printf("Hello World!\n");
    pthread_exit( (void *) &ret1);
}
//printing 
int main (int argc, char *argv[]) {
    void * rPtr;
    pthread_t thr; 
    pthread_create(&thr, NULL, Hello_1, NULL);
    pthread_join(thr, &rPtr); //storing Hello_1's pthread_exit() pointer to rPtr
    printf("Thread returned %d\n", * (int *) rPtr); //printing ret1 
    //this will always print 0, since ret1 is gone when thr is terminated
    //so store it outside of Hello_1
}
int ret2;
void * Hello_2 (void * p) {
    ret2 = 1;
    sleep(5);
    printf("Hello World!\n");
    return (void *) &ret2; //just returning void pointer is fine //os will eventually terminate the thread
}
int main (int argc, char *argv[]) {
    void * rPtr;
    pthread_t thr; 
    pthread_create(&thr, NULL, Hello_2, NULL);
    pthread_join (thr, &rPtr); //storing Hello_2's pthread_exit() pointer to rPtr
    printf("Thread returned %d\n", * (int *) rPtr); //printing ret2 //now it prints well 
}
#include <thread> //this built in libarary is used to create threads
void threadF(string msg) {
    cout << "my_thread says: " << msg << endl;
}
int main(void) {
    // Constructs thread object and starts execution of threadF():
    thread my_thread(threadF, "Hello"); //starts thread right away
    // Join my_thread:
    my_thread.join();
return 0; 
}
//use g++ -std=c++11 -pthread -o test test.cpp to compile
//this is far more easier to pass non integer arguments
//but only void return type is allowed
//also if the thread is not joined or detached, the program will be terminated
//syncronized printing
#define NUMT 20
#include <cstdio> // avoid synchronization issue in iostream
vector<thread> threads; //sequential container
void threadF(int id, int k) { printf("My args: %d, %d\n", id, k);
}
int main(void) {
for (int i=0; i<NUMT; i++) { threads.push_back(thread(threadF, i, i+1));
}
for (int i=0; i<NUMT; i++) { threads[i].join(); //threads[i].detach();
}
return 0; }
//using async to multithread
#include <future> //imports async, future
bool is_prime (int x) {
for (int i=2; i<x; ++i) if (x%i==0) return false; return true;
}
int main (void) {
  //call function asynchronously
future<bool> ft = async (is_prime, 444444443); //meaning this function will be executed in another thread
  //do something while waiting for function to set future:
  cout << "checking, please wait" << endl;
  //blocks the main flow until return value is available
  //since ft is a future object
  bool x = ft.get(); 
  cout << "444444443 "
            << (x?"is":"is not") << " prime.\n";
return 0; }
//strtok is not thread safe //string to token
//char * strtok (char * str, const char * delimiters)
int main() {
char buffer[] = "foo bar car";
char * token1 = strtok(buffer, " ");
cout << token1 << endl;
char * token2 = strtok(NULL, " "); //this might return third token
cout << token2 << endl;
char * token3 = strtok(NULL, " "); //if this is executed first
cout << token3 << endl;
}
//use strtok_r instead from glibc
//char * strtok_r (char * str, const char * delimiters, char ** saveptr)
//it uses saveptr to store the state between calls
//just like Hello_1 and Hello_2 example above
//making it thread safe
int main() {
char buffer[] = "foo bar car";
char * saveptr;
char * token1 = strtok_r(buffer, " ", &saveptr;
cout << token1 << endl;
char * token2 = strtok_r(NULL, " ", &saveptr);
cout << token2 << endl;
char * token3 = strtok_r(NULL, " ", &saveptr);
cout << token3 << endl;
}