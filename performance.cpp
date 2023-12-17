#include<papi.h> //papi is not a standard library //should be installed online

long long int end = 100000000;

int main() {
    int retval = PAPI_library_init(PAPI_VER_CURRENT);
    if(retval != PAPI_VER_CURRENT)
        fail( __FILE__, __LINE__, "PAPI_library_init", retval );
    long long cyc_startcalc = PAPI_get_real_cyc(); //starts hardward cycle
    long long usec_startcalc = PAPI_get_real_usec(); //starts human clock
    for (int i = 0; i < end; i ++) {}; //code being checked  
    long long cyc_finish = PAPI_get_real_cyc(); //ends hardward cycle
    long long usec_finish = PAPI_get_real_usec(); //starts huamn clock
    printf("Duration (cycles): %s\n", cyc_finish - cyc_startcalc);
    printf("Duration (usecs): %s\n", usec_finish - usec_startcalc);
}