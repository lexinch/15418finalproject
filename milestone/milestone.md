# Project Milestone

## Project: Parallel Maximal Independent Set
### Team Members: Melody Cheng, Teresa Wan

---

### Summary of Work Completed

Throughout the initial phase of our project, we've achieved significant milestones in addressing the complex problem of solving the maximal independent set (MIS). Our work began with an intensive research phase where we delved into various algorithms pertinent to this challenge. Initially, we planned to code and analyze Luby's algorithm and Blelloch's algorithm. However, upon an in-depth investigation, we discerned that Blelloch's algorithm was less efficient in terms of runtime and less suitable for parallel programming. This insight led us to pivot our focus towards the Random-priority parallel algorithm which not only matches the runtime of Luby's algorithm (O(log n)) but also surpasses Blelloch's in exploiting parallel programming capabilities. The foundation of our project was further solidified by establishing a robust coding environment and framework, and meticulously crafting test cases to ensure the integrity of our algorithms.

Our progression into the practical aspects of the project involved first implementing the sequential algorithm for solving MIS. An essential part of this phase was developing a mechanism to verify the correctness of the output generated by our code, which upon rigorous testing proved to be accurate. Subsequently, we ventured into parallel computing, starting with the implementation of Luby's algorithm using OpenMP. After extensive testing and validation of its correctness, we proceeded to adapt Luby's algorithm for OpenMPI. Currently, we are addressing some bugs encountered in the OpenMPI version. In parallel, we successfully implemented the Random-priority algorithm with OpenMP and have confirmed its correctness. Throughout this journey, we have navigated through several challenges including the generation of suitable test cases, setting up a conducive programming environment, and the intricate process of debugging our implementations. These challenges have been essential in enhancing our understanding and skills, thereby contributing significantly to our learning goal.

---

### Alignment with Previous Goals

Initially, we aimed to implement and compare the performance of Luby’s algorithm and Blelloch’s algorithm using OpenMP and OpenMPI frameworks. However, after some research, we shifted our goals to instead compare Luby’s algorithm and the random-priority parallel algorithm. So far, we have successfully implemented Luby's algorithm in OpenMP and the random-priority algorithm in OpenMP. However, we faced challenges with the OpenMPI implementation of Luby's algorithm, which is not yet functioning as expected. This deviation from our initial goal is a setback, but we have made significant progress in most areas. Unfortunately, due to time constraints and the complexity involved, we've realized it's not feasible to parallelize both algorithms in both OpenMP and OpenMPI within our project timeline.

Moving forward, our focus will be on debugging and correcting the OpenMPI implementation of Luby's algorithm. We are also planning to enhance the performance of Luby's algorithm for a more comprehensive comparison between its OpenMP and OpenMPI implementations. While this adjustment in our goals means we won't parallelize both algorithms in both frameworks as initially planned, we are confident in our ability to achieve the corrected and optimized implementation of Luby's algorithm in both OpenMP and OpenMPI.

---

### Adjusted Goals

**Plan to Achieve:**
- Implement the sequential algorithm of finding MIS (achieved)
- Develop initial implementations of Luby’s Algorithm and random-priority algorithm (achieved)
- Further parallelize Luby’s Algorithm with OpenMP and OpenMPI
- Parallelize random-priority Algorithm with OpenMP
- Fine-tune Luby’s Algorithm with OpenMP
- Assess the performance of our parallel algorithms across various graph types and sizes focusing on dense, sparse, and corner-dense graphs

**Hope to Achieve:**
- Fine-tune Luby’s Algorithm with OpenMPI
- Fine-tune random-priority algorithm with OpenMP

---

### Updated Project Schedule

| Week | To-Do |
|------|-------|
| December 4 - December 7 | Measure performance of Luby’s algorithm using OpenMP (Teresa & Melody), Fine-tune performance by adjusting parameters etc. (Teresa & Melody) |
| December 8 - December 10 | Debug Luby’s algorithm using OpenMPI (Teresa & Melody), Measure performance (Teresa & Melody), Reach Goal: Improve performance of Luby’s algorithm using OpenMPI, Reach Goal: Achieve good performance with random-priority parallel algorithm using OpenMP |
| December 11 - December 15 | Write Makefile to make compiling and running easy (Teresa & Melody), Write Final Project Report and prepare for Project Poster Session (Teresa & Melody) |

---

### Plans for Poster Session

We are planning a comprehensive and engaging format using a series of slides. It starts with an introductory slide that outlines