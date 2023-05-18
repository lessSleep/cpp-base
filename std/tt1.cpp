#include <bits/stdc++.h>
using namespace std;
#define N 100000
  
// A comparator function used by qsort
int compare(const void * a, const void * b)
{
    return ( *(int*)a - *(int*)b );
}

int arr[N], dupArr[N];

static void DoSetup(const benchmark::State& state) {   
  
    // seed for random input
    srand(time(NULL));

    // to measure time taken by qsort and sort
    clock_t begin, end;
    double time_spent;

    // generate random input
    for (int i = 0; i < N; i++)
        dupArr[i] = arr[i] = rand()%100000;
  

}

static void csort(benchmark::State& state) {
    // Code inside this loop is measured repeatedly
    for (auto _ : state) {
        qsort(arr, N, sizeof(int), compare);
    }

}
// Register the function as a benchmark
BENCHMARK(csort);

static void cppsort(benchmark::State& state) {
    // Code before the loop is not measured
    for (auto _ : state) {
        sort(dupArr, dupArr + N);
    }
}

BENCHMARK(cppsort);