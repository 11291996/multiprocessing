//openmp is an acronyn for open multi processing
//it is a library that allows you to parallelize your code
//openmp programs are globally sequential but locally parallel
//it follows the fork-join model

//structured block: a block of code that has a single entry and exit point
//construct: an openmp directive and the associated structured block
//region: all code encountered during the execution of a construct
//parallel region: a region that is executed by a team of threads
//a region is dynamics but a construct is static
//master thread: the thread that executes the sequential part of the code
//thread team: the set of threads that execute the parallel region
//openmp is easy but not free from the errors of other thread libraries

#include <omp.h>
#include <stdio.h>

int main() {
    omp_set_num_threads(16);

    #pragma omp parallel //openmp directive
    //construct
    {
        int ID = omp_get_thread_num();
        printf("Thread:%d\n", ID);
    }
}

//use echo $OMP_NUM_THREADS and export OMP_NUM_THREADS to check and set the global number of threads
int main() {
    omp_set_num_threads(16);
    int total = 0; //automatically shared
    int ID;

    #pragma omp parallel shared(total) private(ID)
    { //using shared and private variables
    ID = omp_get_thread_num();
    printf("Thread:%d\n", ID);
    total++;
    printf("Total:%d\n", total);
    };
    return 0;
}

//one can use a reduction variable 

int main() {
    //the same from above
    #pragma omp parallel private(ID) reduction(+:total)
    { //using reduction
    ID = omp_get_thread_num();
    printf("Thread:%d\n", ID);
    total++;
    };
    printf("Total:%d\n", total);
return 0;
}

//using omp for 
int main() {
    int i;
    int max = 24;
    int ID;

    #pragma omp parallel for private(ID) reduction(+:total) num_threads(max)
    for (i = 0; i < max ; i++) {
        ID = omp_get_thread_num();
        printf("Thread:%d\n", ID);
    }
    return 0;
}
//cannot use break or goto for omp for
//meaning loop invariant

//syncronization
//from the total example above
int main() {
    //the same from above

    #pragma omp parallel shared(total) private(ID)
    { //using shared and private variables
    //the same 
        #pragma omp critical
        total++;

    //the same
    };
    return 0;
}

//or use locks
int main() {
    //the same from above
    omp_lock_t Sync;
    omp_init_lock(&Sync);
    #pragma omp parallel shared(total) private(ID)
    { //using shared and private variables
    //the same 
        omp_set_lock(&Sync);
        total++;
        omp_unset_lock(&Sync);

    //the same
    };
    return 0;
}

//parallel sections //barrier
int main() {
    omp_set_num_threads(3);
    #pragma omp parallel sections
    {
        #pragma omp section 
        {
        printf( "Section 1 executed by thread %d\n", omp_get_thread_num() );
        }
        #pragma omp section 
        {
        printf( "Section 2 executed by thread %d\n", omp_get_thread_num() );
        }
        #pragma omp section 
        {
        printf( "Section 3 executed by thread %d\n", omp_get_thread_num() );
        }
    }
}

//outsourcing 

int main()
{ 
  int x = 1;
  #pragma omp target map(tofrom: x) //sends x to the device and back
  x = x + 1; // The copy of x on the device has a value of 2.
  printf("After the target region is executed, x = %d\n", x);
  return 0;
}