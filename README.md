# Array-Sum
Repository contains the code for calculating the sum of an Array using parallel programming. The elements of the Array are randomly generated and the sum is calculated using both the linear as well as the parallel methods. The results are then compared for correctness and analysed to measure the speed up. <br />

Commands for execution
```
g++ ArraySum.cpp -std=c++11 -pthread
./a.out
```

## Approach
The sum of the array with randomly generated elements from within a range is calculated using two methods:

- Standard Linear Tranversal Sum:
  - Time Complexity = O(N)
  - Space Complexity = O(1)
- Parallel Multiple Linear Traversal Sum:
  - Time Complexity = O(N/K) + O(K)
  - Space Complexity = O(K)
  - Where 'K' is the factor of parallelization (here the number of CPU cores on the machine)

For the parallel approach, the input array is divided into 'K' parts and the sum of each part is computed independenly by a unique core and stored in a smaller array of size 'K'. Finally, the sum of the resulting array is computed in a linear manner to give the sum of the entire original array.

Note: The sum of the resulting smaller array can again be computed in a parallel manner. However, considering that the value of 'K' is usually very small, the overheads due function calls, involvement of branch predictor and the disruption of the execution pipeline could out weigh the benefits. A smaller 'K' value also means that there would be fewer 'Cache Misses'. Considering these points, a linear approach was taken to compute the sum of the resulting array.

## Analysis
The data is collected for various tests by tuning the following parameters:
- Size of input array (N).
- Parallelization factor (K).

The data as well as the analysis performed on the data can be found in the 'Analysis' sub-directory.

## Known Issues:
- It is possible to measure the entire program execution time on the Windows operating system, however attempts made to measure the function exection time were not fruitful. For the purpose of the analysis, the code was run on the Ubuntu operating system.

## Useful Links:
- Array sum (C++): https://www.tutorialspoint.com/array-sum-in-cplusplus-stl
- Multithreading in C++: https://www.educative.io/blog/modern-multithreading-and-concurrency-in-cpp
