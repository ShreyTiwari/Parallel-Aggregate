/**
 * @file ArraySum.cpp
 *
 * @brief This file implements two approaches to calculating the Sum of an Array.
 *
 * @author Shrey Tiwari
 * Contact: shreymt@gmail.com
 *
 */

// Importing the required header files
#include <iostream>
#include <time.h>

// Defining the input size for the array as 1 million elements (2 ^ 20)
#define SIZE 1024

// Defining the range for array elements
#define LOWER -2
#define UPPER 2

// Defining total number of CPU cores
#define CORES 4

using namespace std;


// Helper Function to help measure the execution time (works only on Ubuntu)
double time_elapsed(struct timespec *start, struct timespec *end) 
{
	double t;
	t = (end->tv_sec - start->tv_sec); // diff in seconds
	t += (end->tv_nsec - start->tv_nsec) * 0.000000001; //diff in nanoseconds
	return t;
}


// Helper Function to randomly fill in the array with intergers in specified range
void fill_array_random(short int *array)
{
    srand(time(0));

    for(int i = 0; i < SIZE; i++)
        array[i] = (rand() % (UPPER - LOWER + 1)) + LOWER;
}


// Function to calculate the sum of the array in a linear way
int linear_sum(short int *array)
{
    int sum = 0;
    for(int i = 0; i < SIZE; i++)
        sum += array[i];

    return sum;
}


// Function to calculate the sum of the array parallely
int parallel_sum(short int *array)
{
    return 0;
}

// Code execution begins here
int main ()
{
    // Object of timespec to calculate execution time
    struct timespec l_start, l_end, p_start, p_end;

    short int arr[SIZE];
    fill_array_random(arr);

    clock_gettime(CLOCK_REALTIME, &l_start);
    int l_sum1 = linear_sum(arr);
    int l_sum2 = linear_sum(arr);
    int l_sum3 = linear_sum(arr);
    clock_gettime(CLOCK_REALTIME, &l_end);

    clock_gettime(CLOCK_REALTIME, &p_start);
    int p_sum1 = parallel_sum(arr);
    int p_sum2 = parallel_sum(arr);
    int p_sum3 = parallel_sum(arr);
    clock_gettime(CLOCK_REALTIME, &p_end);

    printf("-------------------------------------------------------------------------------\n");
    //if(l_sum != p_sum)
    if ((l_sum1 != l_sum2) || (l_sum2 != l_sum3))
    {
        printf("ERROR: The computed totals did not match (Linear Computation Case).\n");
    }
    if ((p_sum1 != p_sum2) || (p_sum2 != p_sum3))
    {
        printf("ERROR: The computed totals did not match (Parallel Computation Case).\n");
    }
    
    //if (l_sum1 != p_sum1)
    if (l_sum1 == p_sum1)
    {
        printf("ERROR: The computed totals did not match.\n");
    }
    else
    {
        printf("The array sum is: %d\n", l_sum1);

        // We are averaging the time over the three runs
        printf("Time taken using linear approach: %lf microseconds\n", (time_elapsed(&l_start, &l_end) * 1e6)/3);
        printf("Time taken using parallel approach: %lf microseconds\n", (time_elapsed(&p_start, &p_end) * 1e6)/3);
    }
    printf("-------------------------------------------------------------------------------\n");
    
    return 0;
}
