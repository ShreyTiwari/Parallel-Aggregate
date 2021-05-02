/**
 * @file ArraySum.cpp
 *
 * @brief This file implements an algorithm for a parallel aggregator.
 *
 * @author Shrey Tiwari
 * Contact: shreymt@gmail.com
 */

/* 
    - Standard Linear Tranversal Aggregate:
        - Time Complexity = O(N)
        - Space Complexity = O(1)

    - Parallel Multiple Linear Traversal Aggregate:
        - Time Complexity = O(N/K) + O(K)
        - Space Complexity = O(K)
        - Where 'K' is the factor of parallelization

    COMMANDS:
        - g++ ParallelAggregate.cpp -std=c++11 -pthread
        - ./a.out

    NOTE:
    1. Currently the program assumes equal chunk sizes for all partitions. Hence, 'SIZE' should be divisible by 'CORES'.
    2. Time Measurements will work on Linux only.
*/


// Importing the required header files
#include <iostream>
#include <time.h>
#include <unistd.h>
#include <vector>
#include <thread>

// Defining the input size for the array
#define SIZE 1024

// Defining total number of CPU cores
#define CORES 16

// Defining the range for array elements
#define LOWER 0
#define UPPER 10

using namespace std;

// Struct to hold aggregation results
typedef struct result
{
    int x;
    int y;
} Result;


// Helper Function to help measure the execution time (works only on Ubuntu)
double time_elapsed(struct timespec *start, struct timespec *end) 
{
	double t;
	t = (end->tv_sec - start->tv_sec); // diff in seconds
	t += (end->tv_nsec - start->tv_nsec) * 0.000000001; //diff in nanoseconds
	return t;
}

// Helper Function to compare the results from two approaches
int is_equal(Result a, Result b)
{
    if((a.x == b.x) && (a.y == b.y))
        return 1;

    return 0;
}


// Helper Function to randomly fill in the array with intergers in specified range
void fill_array_random(short int *array)
{
    srand(time(0));

    // printf("The array is: \n")

    for(int i = 0; i < SIZE; i++)
    {
        array[i] = (rand() % (UPPER - LOWER + 1)) + LOWER;

        // printf("%d ", array[i]);
    }

    // printf("\n");
}


// Function to calculate the array aggregates in a linear way
Result linear_aggregate(short int *array, int start, int end)
{
    int x = 0, y = 0;
    
    // printf("\nI    X    Y\n");

    for(int i = start; i <= end; i++)
    {
        x += array[i];
        y += x;

        // printf("%d    %d    %d\n", i, x, y);
    }

    // printf("\n");

    Result linear_result = {x, y};
    return linear_result;
}


// Function executed by each thread
void thread_sum(short int *array, int start, int end, Result *res, int index)
{
    res[index] = linear_aggregate(array, start, end);
}


// Function to calculate the array aggregates parallely
Result parallel_aggregate(short int *array)
{
    // Meta data for the threads to execute
    Result res[CORES];
    int chunk_size = SIZE/CORES;

    // Launch the parallel threads
    vector<thread> threads;
    for(int i = 0; i < CORES; i++)
        threads.push_back(thread(thread_sum, array, (i * chunk_size), ((i+1) * chunk_size - 1), res, i));

    for (auto &th : threads)
        th.join();

    // Linearly sum the resulting array
    int x = 0, y = 0;
    for(int i = 0; i < CORES; i++)
    {
        y += res[i].y + (x*chunk_size); // Adding the constant offset of x to y (from previous partitions)
        x += res[i].x;
    }

    Result parallel_result = {x, y};
    return parallel_result;
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
    Result linear_result1 = linear_aggregate(arr, start, end);
    Result linear_result2 = linear_aggregate(arr, start, end);
    Result linear_result3 = linear_aggregate(arr, start, end);
    clock_gettime(CLOCK_REALTIME, &l_end);

    // Very varied results if tested back to back. Could be due to CPU spikes.
    // Adding cool down period results in much more consistency.
    usleep(1000000);

    clock_gettime(CLOCK_REALTIME, &p_start);
    Result parallel_result1 = parallel_aggregate(arr);
    Result parallel_result2 = parallel_aggregate(arr);
    Result parallel_result3 = parallel_aggregate(arr);
    clock_gettime(CLOCK_REALTIME, &p_end);

    printf("-------------------------------------------------------------------------------\n");
    if (!is_equal(linear_result1, linear_result2) || !is_equal(linear_result2, linear_result3))
    {
        printf("ERROR: The computed totals did not match (Linear Computation Case).\n");
    }

    if (!is_equal(parallel_result1, parallel_result2) || !is_equal(parallel_result2, parallel_result3))
    {
        printf("ERROR: The computed totals did not match (Parallel Computation Case).\n");
    }
    
    if (!is_equal(linear_result1, parallel_result1))
    {
        printf("ERROR: The computed totals did not match.\n");
    }
    else
    {
        printf("The array size is: %d\nThe value of K is: %d\nThe range is: %d to %d\n", SIZE, CORES, LOWER, UPPER);
        printf("The array sum (X) is: %d\n", parallel_result1.x);
        printf("The array aggregate (Y) is: %d\n", parallel_result1.y);

        // We are averaging the time over the three runs
        printf("Time taken using linear approach: %lf microseconds\n", (time_elapsed(&l_start, &l_end) * 1e6)/3);
        printf("Time taken using parallel approach: %lf microseconds\n", (time_elapsed(&p_start, &p_end) * 1e6)/3);
    }
    printf("-------------------------------------------------------------------------------\n");
    
    return 0;
}
