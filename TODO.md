# TODO

## Implementation
- [x] Barebones matrix dot product with no optimization
- [x] Dot product with loop order: i-k-j (1)
- [x] Dot product with loop order: k-j-i (2)
- [x] Validate matrix size
- [ ] Apply block optimization to code that requires RAM access
- [ ] Vectorize code via compiler 
- [ ] Multi-core vectorized code wihtout HT
- [ ] Adapt code (1 or 2) to run in
    - [ ] all SMX of a Kepler GPU
    - [ ] all cores of an Intel Knights Landing
    **n.b** adapt data structures to comply with cache sizes

## Report
- [x] Describe cluster hardware platform
- [x] Describe laptop hardware paltform
- [x] Roofline model for cluster node 662
- [ ] Roofline model for laptop
    - [ ] Add ceilings according to matrix multiplication
- [ ] PAPI performance counters
- [x] Matrix size that fits in L1 cache of 662
- [x] Matrix size that fits in L2 cache of 662
- [x] Matrix size that fits in L3 cache of 662
- [ ] Measure execution times for dot-product function
    - [ ] Use K-best, with K=3 and sample size=8
- [ ] Use PAPI data to analyze best execution time
    - [ ] Estimate: RAM accesses per instruction; bytes transferred to/from the RAM, with and without transposed matrices
    - [ ] Confirm values with PAPI
- [ ] For each data set size:
    - [ ] Estimate number of FP op's for each implementation
    - [ ] Plot achieved performance in roofline model
    - [ ] miss rate on memory reads for each cache: L1, L2, L3
- [ ] Interpret obtained results
    - [ ] CPU bound or memory bandwidth bound
    - [ ] Performance bottlenecks
    - [ ] ...
- [ ] Describe benefits of block optimization
- [ ] Measure execution times for vectorized dot-product
    - [ ] Use K-best, with K=3 and sample size=8
- [ ] Measure execution times for multi-core vectorized dot-product
    - [ ] Use K-best, with K=3 and sample size=8
- [ ] Complement table with:
    - [ ] Data transfer times between the CPU-device 
    - [ ] Accelerator-CPU
    only for the largest data set