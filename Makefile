# Define compiler and flags
CXX = mpic++
CXXFLAGS = -fopenmp -std=c++14 -lpthread

# Define source files and target
SRC = src/main.cpp src/seq_mis.cpp src/luby_mis.cpp src/luby_mpi.cpp src/rand_priority_mis.cpp src/luby_mp_task.cpp
TARGET = missolver

# Default target
all: $(TARGET)

# Build the project
$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

# Clean the build
clean:
	rm -f $(TARGET)

# PHONY targets
.PHONY: all clean
