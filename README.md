# 15418 Final Project: Parallel Maximal Independent Set
*Team Members: Melody Cheng, Teresa Wan*

## URLs
[Project Proposal URL](https://lexinch.github.io/15418finalproject/proposal/proposal.html)

[Project Milestone URL](https://lexinch.github.io/15418finalproject/milestone/milestone.html)

[Project Final Report URL](https://docs.google.com/document/d/1yRcRD2ahBrTF_eNbuy1lZ05uuSNCfnLLmSuDRfu0Q-c/edit?usp=sharing)

## Instructions to compile and run the code
To build the project, run:
```
make
```
To clean the build (remove the missolver executable), run:
```
make clean
```
To run the executable with different flags, you can execute commands like:
```
# Generate test cases
cd test
python3 check.py 

# Run different versions with test cases
./missolver -seq -f test/complete-5000.txt #-seq means sequential version
./missolver -lubyopenmp -f test/complete-5000.txt #-lubyopenmp means OpenMP parallel for version of Luby's algorithm
./missolver -lubytask -f test/complete-5000.txt #-lubytask means OpenMP task version of Luby's algorithm
./missolver -lubympi -f test/complete-5000.txt #-lubympi means OpenMPI version of Luby's algorithm
./missolver -randprio -f test/complete-5000.txt #-randprio means openMP implementation of random priority parallel algorithm
OMP_NUM_THREADS=4 ./missolver -lubyopenmp #sets the number of threads to 4, missing -f flag sets program to run the default test case, which is same as complete-5000.txt
```
