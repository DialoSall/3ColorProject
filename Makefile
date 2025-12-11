CXX = clang++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -Iinclude

SRC_MAIN = src/main.cpp src/graph.cpp src/sorting_machine.cpp src/algorithm.cpp src/utilities.cpp
SRC_TEST = tests/test_graphs.cpp src/graph.cpp src/sorting_machine.cpp src/algorithm.cpp src/utilities.cpp
SRC_BENCH = tests/benchmark.cpp src/graph.cpp src/sorting_machine.cpp src/algorithm.cpp src/utilities.cpp

all: color3 test_graphs bench

color3: $(SRC_MAIN)
	$(CXX) $(CXXFLAGS) -o $@ $(SRC_MAIN)

test_graphs: $(SRC_TEST)
	$(CXX) $(CXXFLAGS) -o $@ $(SRC_TEST)

bench: $(SRC_BENCH)
	$(CXX) $(CXXFLAGS) -o $@ $(SRC_BENCH)

clean:
	rm -f color3 test_graphs bench
