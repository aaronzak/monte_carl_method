/* File:    ex5.2_omp_trap1_no_crit.c
 * Purpose: Try to estimate definite integral (or area under curve) using 
 *          trapezoidal rule.  This version doesn't include a critical
 *          directive.  So beware:  it has a race condition
 *
 * Compile: gcc -g -Wall -fopenmp -o ex5.2_omp_trap1_no_crit 
 *             ex5.2_omp_trap1_no_crit.c
 * Usage:   ./ex5.2_omp_trap1_no_crit <number of threads>
 *
 * Input:   a, b, n
 * Output:  estimate of integral from a to b of f(x)
 *          using n trapezoids.
 *
 * Notes:   
 *   1.  The function f(x) is hardwired.
 *   2.  In this version, each thread explicitly computes the integral
 *       over its assigned subinterval.
 *   3.  This version assumes that n is evenly divisible by the 
 *       number of threads
 *
 * IPP:  Section 5.2.1 (pp. 216 and ff.) and Exercise 5.2
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
 #include <time.h>


void Usage(char* prog_name);
double f(double x);    /* Function we're integrating */
void getPi(long long int n, double* global_result_p);

int main(int argc, char* argv[]) {
   
   

   double  global_result = 0.0;  /* Store result in global_result */
   //double  a, b;                 /* Left and right endpoints      */
   long long int     n;                    /* Total number of trapezoids    */
   int     thread_count;

   if (argc != 2) Usage(argv[0]);
   thread_count = strtol(argv[1], NULL, 10);
   printf("Enter n\n");
   scanf("%ll ", &n);
   if (n % thread_count != 0) Usage(argv[0]);

#  pragma omp parallel num_threads(thread_count) 
   getPi(n, &global_result);

   printf("With n = %d tosses, our estimate\n", n);
   printf("of pi  = %lld\n",
      global_result/thread_count);
   return 0;
}  /* main */

/*--------------------------------------------------------------------
 * Function:    Usage
 * Purpose:     Print command line for function and terminate
 * In arg:      prog_name
 */
void Usage(char* prog_name) {

   fprintf(stderr, "usage: %s <number of threads>\n", prog_name);
   fprintf(stderr, "   number of tosses must be evenly divisible by\n");
   fprintf(stderr, "   number of threads\n");
   exit(0);
}  /* Usage */



/*------------------------------------------------------------------
 * Function:    Trap
 * Purpose:     Use trapezoidal rule to estimate definite integral
 * Input args:  
 *    a: left endpoint
 *    b: right endpoint
 *    n: number of trapezoids
 * Output arg:
 *    integral:  estimate of integral from a to b of f(x)
 */
void getPi(long long int n, double* global_result_p) {
   double  h, x, my_result;
   double  local_a, local_b;
   int  i, local_n;
   int my_rank = omp_get_thread_num();
   int thread_count = omp_get_num_threads();
   long long int hits;

//perform calculation
 //  h = (b-a)/n; 
   local_n = n/thread_count;  
  /* local_a = a + my_rank*local_n*h; 
   local_b = local_a + local_n*h; 
   my_result = (f(local_a) + f(local_b))/2.0; */
      
   long int distance;
   hits=0; //number of hits
   for (i = 0; i <= local_n-1; i++) {
      srand(time(NULL));
      int a = (rand()%2001);
      a=(a/1000) -1;
      int b= rand()%2001;
      b=b/1000 -1;
      distance = a*a + b*b;
      if(distance<=1){
         h++;
      }

     
   }
   double pi=4*(hits/(double)n);
   my_result = pi;

//#  pragma omp critical 
   *global_result_p += my_result; 
}  /* Trap */