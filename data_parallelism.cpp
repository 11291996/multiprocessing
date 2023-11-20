#include <iostream>
using namespace std;

//scalar registor used in cpu
int main() {
    int a[4] = {1,2,3,4}; int b[4] = {1,2,3,4}; int c[4]; int i;
    for (i=0; i < 4; i++) 
    {
        c[i] = a[i] + b[i];
        cout << c[i] << endl;
    }
    return 0;
}

#include <smmintrin.h> 
#include <emmintrin.h> 
#include <stdio.h>

//vector registors 
//data parallelism in cpu
//different extension is applied by the header file above 
//Intel x86: SSE, AVX, AVX2, AVX-512, ARM: Neon 128-bit SIMD extensions, ...
int main() {
    __m128i A = _mm_set_epi32(4, 3, 2, 1); //intrinsic -> a procedure provided by the compiler
    __m128i B = _mm_set_epi32(2, 2, 2, 2);
    __m128i C = _mm_add_epi32(A, B); //add vectors A and B
    int res = _mm_extract_epi32(C, 3); //extract element //at index 3
    printf("Result: %d\n", res);
    return 0;
}