#include <omp.h>
#include <stdio.h>

int main()
{ 
  int x = 1;
  #pragma omp target map(tofrom: x)
  x = x + 1; // The copy of x on the device has a value of 2.
  printf("After the target region is executed, x = %d\n", x);
  return 0;
}