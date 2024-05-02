This library provides an implementation of a balanced histogram. Users can specify the maximum number of elements for the histogram bins. Whenever a bin exceeds this maximum, it is split to ensure that the maximum number of elements stay withing the bounds.

# How to use
Include the header into you code.
The histogram class, i.e. `balancing_histogram::hist<T, N>`, builds a balanced histogram of of elements of type `T` such that none of the bins exceeds `N` elements. Note that `T` should be supported by `numeric_limits.

The histogram class supports the following methods:
  1. `void insert(T elem)`: Insert an element of type `T` in the histogram

  2. `std::vector<T> get_bin_boundaries()`: Returns the boundries of the bins. If there are $k$ bins, then a vector of length $k+1$ is returned. The first value is the smalled element in the histogram. The next $k$ values are the highest values in each bin
  
  3. `void print()`: Prints the histogram on `stdout` 
