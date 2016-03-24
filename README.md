# CS1541_Project2
Project #2 (Due April 7)
NOTE: This assignment can be done by a team of two students (do not have to be the same team of project 1).
The goal of this assignment is to build a simple cache simulator to study the effect of different cache parameters
on performance. Your cache simulator, called cache.c, should accept six command line arguments;
1. trace file name (same as in project 1)
2. trace_view_on (same as in project 1)
3. the size of the cache in Kilo Bytes (should restrict it to be a power of 2)
4. the size of each cache block in bytes (should check that it is a power of 2)
5. the associativity of the cache sets (should restrict it to be a power of 2)
6. the replacement policy (0 for LRU and 1 for FIFO)
A simple program, cache.c, is provided (similar to CPU.c) to read the trace file and extract the load and store
commands. You should modify this program to simulate the actions of the L1 data cache as a result of executing
the load and store instructions. Assume that all the blocks in the cache are initially invalid and count the number
of cache accesses as well as the cache misses, cache hits and write backs during the execution of the trace. The
file skeleton.h provides a generic skeleton of the functions and data structures that you may use in your simulation.
Use the same traces as in project 1 to test your simulator reporting the total number of cache accesses, cache hits,
cache misses and write backs.
Experiment 1: To examine the effect of the block size, consider 4 cache sizes (1KB, 16KB and 128KB) with 4-
way set associativity and LRU replacement. Compare the miss rate for block sizes of 4B, 16B, 32B, 64B, 128B
and 256B. Produce two plots similar to the one on page 391 of the text book, one for each of the long traces.
Produce two corresponding graphs to express the number of bytes written back (the y axis being the number of
bytes written back instead of the miss rate).
Experiment 2: To examine the effect of the replacement policy, consider a 4KB cache with 32B blocks and 4-
way associativity. Use a bar graph to plot the miss rate for each of the two long traces and 2 replacement policies
(FIFO and LRU). Your plot should have 4 bars.
Experiment 3: To examine the effect of associativity, consider a 4KB cache with 32B blocks and LRU
replacement. Use a bar graph to plot the miss rate for associativity = 1, 4 and 8. This plot will have 6 bars, three
for each long trace.
What to submit (email to TA):
1) One file with your source code for the cache.c (which includes all functions and declarations) simulator
which takes 6 arguments in the following order; the input trace file, the trace_view_on switch, the
cache_size, the block_size, the associativity and the replacememt_policy (0 for LRU or 1 for FIFO).
2) One file with
a. The result of running your simulation with trace_view_on = 0 for each long trace file and the
parameters specified in each experiment
b. The plots specified in each experiment
c. Your explanation of the results and the conclusion that you draw from each experiment
