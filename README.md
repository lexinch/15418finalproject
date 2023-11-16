# 15418 Final Project: Parallel Maximal Independent Set
*Team Members: Melody Cheng, Teresa Wan*

## URL
[Project Webpage URL](https://lexinch.github.io/15418finalproject/)

## Summary
We will parallelize the sequential algorithm for maximal independent set (MIS), implementing the algorithm of Luby’s algorithm and Blelloch’s algorithm. We will evaluate the speedup performance and profile our parallel implementation.

## Background
Our project aims to parallelize algorithms for solving the maximal independent set (MIS) problem in graphs. This problem, pivotal in various application areas like network theory, scheduling, and resource allocation, involves identifying the largest set of vertices with no two adjacent vertices.

The MIS problem, inherently NP-hard, poses significant computational challenges, especially in large graphs. Here, parallelism offers a promising solution. By decomposing the problem and concurrently processing subproblems, we can potentially achieve substantial performance gains.

Parallelism particularly benefits areas requiring rapid processing of large, complex graphs, such as in social network analysis or wireless network scheduling. For instance, in social networks, identifying independent user groups (an MIS problem) can be expedited. Similarly, in wireless networks, parallel MIS algorithms can efficiently allocate frequencies to ensure minimal interference.

Pseudocode for sequential algorithm to solve MIS:
```
function MIS(G):
    Initialize an empty set M
    For each vertex v in G:
        If v is not adjacent to any vertex in M:
            Add v to M
    return M
```
Pseudocode for parallel Luby’s algorithm to solve MIS:
```
function LubyMIS(G):
    Initialize MIS as an empty set
    Initialize Candidates as all vertices of G
    While Candidates is not empty:
        Assign a random weight to each vertex in Candidates
        For each vertex v in Candidates:
            If v has the highest weight among its neighbors:
                Add v to MIS
                Remove v and its neighbors from Candidates
    return MIS

```
Pseudocode for parallel Blelloch’s algorithm to solve MIS:
```
function BlellochMIS(G):
    Initialize MIS as an empty set
    Initialize Candidates as all vertices of G
    Initialize Status of each vertex as 'candidate'
    While Candidates is not empty:
        Assign a unique random number to each vertex in Candidates
        In parallel for each vertex v in Candidates:
            If v has the smallest number among its neighbors:
                Add v to MIS
                Set Status of v as 'in MIS'
            Else if any neighbor of v is 'in MIS':
                Set Status of v as 'not in MIS'
                Remove v from Candidates
    return MIS

```


## The Challenge
- Luby's Algorithm: A core part of Luby's algorithm is for each node to check its neighbors' states (whether they have also chosen to be in the MIS). This step requires efficient synchronization mechanisms to prevent race conditions and ensure that each node has the most up-to-date information about its neighbors.

- Blelloch's Algorithm: The graph coloring step requires careful consideration to avoid conflicts and ensure that no two adjacent vertices are assigned the same number in parallel.
The choice of graph representation (e.g., adjacency lists, adjacency matrices, compressed sparse row format) can significantly impact the performance of the algorithm. The representation must be conducive to parallel access and modifications.

- Workload characteristics:

    - Dynamic changes in workload: As the algorithm progresses, the workload associated with each node can change. For example, once a node is included in the MIS, it affects the status of its neighbors, dynamically altering the computation required for those parts of the graph.
    - In graph algorithms, the workload is often unevenly distributed across the processors due to the nature of the graph. Processors handling denser parts of the graph might become hotspots, while others remain idle.


## Resources
For our project, the following resources will be utilized:

- **Algorithm Research**: We will primarily use Wikipedia and other reputable web resources for researching various algorithms relevant to the maximal independent set problem. These resources will provide foundational knowledge and different algorithmic approaches.

- **Parallelization References**: Lecture slides and previous homework assignments will serve as references for implementing parallelization techniques. These materials offer valuable insights into the practical aspects of parallel computing.

- **Computing Resources**: The development and testing of the code will be conducted on the GHC machines and PSC machines.

- **Code Development**: All code for this project will be written from scratch. We will develop our own implementations of the algorithms, tailored to our specific parallelization strategies.



## Goals and Deliverables
### Plan to Achieve
- Implement the sequential algorithm of finding MIS
- Develop initial implementations of Luby's Algorithm and Blelloch's Algorithm.
- Further parallelize these two algorithms using OpenMP and OpenMPI with multiple methods, including vertex division and edge division
- Assess the performance of our parallel algorithms across various graph types and sizes, focusing on dense, sparse, and corner-dense graphs
- Implement load balancing by reassigning tasks regularly

### Hope to Achieve
- Explore the possibility of the use of SIMD and CUDA kernels within C++
- Experiment with hybrid parallel computing models that combine the strength of different parallelization techniques

## Platform Choice
For this project, we have selected multi-core GHC and PSC machines as our primary hardware platforms. These machines are chosen for their robust multi-core architecture, which is suited for parallel computation tasks.

Regarding the software, we will be utilizing C++ as the programming language. Our team's prior experience with C++, gained through earlier assignments, makes it a practical choice. Additionally, we plan to integrate OpenMP and OpenMPI libraries for parallel programming. These libraries are well-suited for implementing parallelization. Furthermore, we plan on exploring the possibility of the use of SIMD and CUDA kernels within C++. The inclusion of SIMD and CUDA is aimed at enhancing the performance of our algorithms on modern hardware, exploiting data-level parallelism and GPU computing capabilities. 

## Schedule


| Week         | To-Do                                                        |
|--------------|--------------------------------------------------------------|
| **Week 1:**<br>November 13 - November 19 |- Initial research<br>- Write Project Proposal (due November 15th)<br>- Plan out necessary code portions<br>- Start implementation of sequential version           |
| **Week 2:**<br>November 20 - November 26 | - Write code for time measurement and test cases<br>- Implement parallelized Luby’s algorithm using OpenMP                |
| **Week 3:**<br>November 27 - December 3  | - Implement parallelized Luby’s algorithm using OpenMPI and parallelized Blelloch’s algorithm<br>- Write Project Milestone Report (due December 3rd) |
| **Week 4:**<br>December 4 - December 10  | - Do more focused performance measurement and tuning<br>- Finalize OpenMP and OpenMPI parallel implementations |
| **Week 5:**<br>December 11 - December 15 | - Write Final Project Report (due December 14th)<br>- Prepare for Project Poster Session (December 15th) |


