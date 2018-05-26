# Concurrent Fundamentals in CPlusPlus
Implement concurrent primitives such as locks, barriers, and data structures
 using standard thread library in C++.
 
## TODO
- [ ] work stealing queue
- [ ] ctrie
- [ ] concurrent hash table

## Usage
```
mkdir build
cmake ..
make
# run the executable in build/
```

## Experiments

### Locks
The benchmark is run on `Intel(R) Xeon(R) CPU E5-2630 v3 @ 2.40GHz` with 32 cores and the tests having
100 threads push and pop 1000 elements to a shared vector.

- centralized locks

```
[mutex] elapsed 0.0685262s
[mutex] elapsed 0.0372867s
[test and set lock] elapsed 2.07016s
[test and set lock] elapsed 0.389452s
[test and test and set lock] elapsed 0.0861447s
[test and test and set lock] elapsed 0.159365s
[ticket lock] elapsed 913.909s
[ticket lock] elapsed 912.928s
[lamport_bakery_lock] elapsed 854.521s
[lamport_bakery_lock] elapsed 835.092s
[lamport_fast_lock] elapsed 1.38185s
[lamport_fast_lock] elapsed 1.41983s
```
- scalable locks

```
[tas lock with exponential backoff] elapsed 0.013401s
[tas lock with exponential backoff] elapsed 0.0127451s
[tatas lock with exponential backoff] elapsed 0.101308s
[tatas lock with exponential backoff] elapsed 0.166205s
[ticket lock with exponential backoff] elapsed 717.829s
[ticket lock with exponential backoff] elapsed 625.013s
[mcs_test] elapsed 552.605s
[mcs_test] elapsed 261.17s
```

- lockfree stack
```
[lockfree_ds::treiber_stack_no_free] elapsed 0.52083s
[lockfree_ds::treiber_stack_no_free] elapsed 0.642559s
[lockfree_ds::treiber_stack_with_hp] elapsed 0.520429s
[lockfree_ds::treiber_stack_with_hp] elapsed 1.21502s
[lockfree_ds::msqueue_no_free] elapsed 2.8808s
[lockfree_ds::msqueue_no_free] elapsed 3.74271s
[lockfree_ds::msqueue_with_hp] elapsed 2.70002s
[lockfree_ds::msqueue_with_hp] elapsed 3.35095s
```

## Reference
- [Shared Memory Synchronization](https://www.morganclaypool.com/doi/abs/10.2200/S00499ED1V01Y201304CAC023)
- [csc458](https://www.cs.rochester.edu/u/sree/courses/csc-258/spring-2018/)
- [CPlusPlus Concurrency in Action](https://www.manning.com/books/c-plus-plus-concurrency-in-action-second-edition)
- [Hazard pointers: safe memory reclamation for lock-free objects](https://ieeexplore.ieee.org/abstract/document/1291819/)
- [Lock-freedom without garbage collection](https://aturon.github.io/blog/2015/08/27/epoch/)
- [fearless-concurrency-with-hazard-pointers](http://ticki.github.io/blog/fearless-concurrency-with-hazard-pointers/)
- [concurrencykit](https://github.com/concurrencykit/ck)
- [cilk](http://supertech.csail.mit.edu/papers/PPoPP95.pdf)