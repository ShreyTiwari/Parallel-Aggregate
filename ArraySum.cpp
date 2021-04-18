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
#include <unistd.h>
#include <vector>
#include <thread>

// Defining the input size for the array (4000k)
#define SIZE 4096000

// Defining total number of CPU cores
#define CORES 8

// Defining the range for array elements
#define LOWER -5
#define UPPER 5

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
int linear_sum(short int *array, int start, int end)
{
    int sum = 0;
    for(int i = start; i <= end; i++)
        sum += array[i];

    return sum;
}


// Function executed by each thread
void thread_sum(short int *array, int start, int end, int *res, int index)
{
    res[index] = linear_sum(array, start, end);
}


// Function to calculate the sum of the array parallely
int parallel_sum(short int *array)
{
    // Meta data for the threads to execute
    int res[CORES];
    int chunk_size = SIZE/CORES;

    // Launch the parallel threads
    vector<thread> threads;
    for(int i = 0; i < CORES; i++)
        threads.push_back(thread(thread_sum, array, (i * chunk_size), ((i+1) * chunk_size - 1), res, i));

    for (auto &th : threads)
        th.join();

    // Linearly sum the resulting array
    int sum = 0;
    for(int i = 0; i < CORES; i++)
        sum += res[i];

    return sum;
}

// Code execution begins here
int main ()
{
    // Object of timespec to calculate execution time
    struct timespec l_start, l_end, p_start, p_end;

    short int arr[SIZE];
    fill_array_random(arr);

    int start = 0, end = SIZE - 1;

    clock_gettime(CLOCK_REALTIME, &l_start);
    int l_sum1 = linear_sum(arr, start, end);
    int l_sum2 = linear_sum(arr, start, end);
    int l_sum3 = linear_sum(arr, start, end);
    clock_gettime(CLOCK_REALTIME, &l_end);

    // Very varied results if tested back to back. Could be due to CPU spikes.
    // Adding cool down period results in much more consistency.
    usleep(1000000);

    clock_gettime(CLOCK_REALTIME, &p_start);
    int p_sum1 = parallel_sum(arr);
    int p_sum2 = parallel_sum(arr);
    int p_sum3 = parallel_sum(arr);
    clock_gettime(CLOCK_REALTIME, &p_end);

    printf("-------------------------------------------------------------------------------\n");
    if ((l_sum1 != l_sum2) || (l_sum2 != l_sum3))
    {
        printf("ERROR: The computed totals did not match (Linear Computation Case).\n");
    }
    if ((p_sum1 != p_sum2) || (p_sum2 != p_sum3))
    {
        printf("ERROR: The computed totals did not match (Parallel Computation Case).\n");
    }
    
    if (l_sum1 != p_sum1)
    {
        printf("ERROR: The computed totals did not match.\n");
    }
    else
    {
        printf("The array size is: %d\nThe value of K is: %d\nThe range is: %d to %d\n", SIZE, CORES, LOWER, UPPER);
        printf("The array sum is: %d\n", l_sum1);

        // We are averaging the time over the three runs
        printf("Time taken using linear approach: %lf microseconds\n", (time_elapsed(&l_start, &l_end) * 1e6)/3);
        printf("Time taken using parallel approach: %lf microseconds\n", (time_elapsed(&p_start, &p_end) * 1e6)/3);
    }
    printf("-------------------------------------------------------------------------------\n");
    
    return 0;
}
