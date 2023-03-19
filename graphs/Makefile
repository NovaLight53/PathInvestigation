TARGETS = graph
CXX = clang++
CXXFLAGS = -std=c++20 -Wall -Wextra -pedantic

all: $(TARGETS)

graph: graph.o
	$(CXX) -o graph graph.o

graph.o: graph.cpp graph.hpp
	$(CXX) -c $(CXXFLAGS) graph.cpp

clean:
	rm -rf *.o $(TARGETS)