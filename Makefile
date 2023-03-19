CXX = clang++
CXXFLAGS = --std=c++2a -g -Wextra -pedantic -O3

TARGETS = pathSim analysis

all:$(TARGETS)

pathSim: pathSim.o simulation.o hashQueue.o player.o
	$(CXX) -o pathSim pathSim.o hashQueue.o player.o simulation.o

analysis: analysis.o
	$(CXX) -o analysis analysis.o

pathSim.o: player.hpp hashQueue.cpp pathSim.cpp
	$(CXX) $(CXXFLAGS) -c pathSim.cpp

analysis.o: analysis.cpp
	$(CXX) $(CXXFLAGS) -c analysis.cpp
player.o: player.hpp player.cpp
	$(CXX) $(CXXFLAGS) -c player.cpp

hashQueue.o: hashQueue.cpp hashQueue.hpp player.hpp
	$(CXX) $(CXXFLAGS) -c hashQueue.cpp

simulation.o: hashQueue.hpp player.hpp simulation.cpp simulation.hpp
	$(CXX) $(CXXFLAGS) -c simulation.cpp

clean: 
	rm -rf *.o $(TARGETS)