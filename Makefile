CXX=g++
CXXFLAGS=-O2 -Wall -pedantic -std=c++11
main:	main.o Node.o NodesFactory.o
		$(CXX) -o main Node.o NodesFactory.o main.o $(CXXFLAGS)
main.o:	main.cpp
		$(CXX) -o main.o -c main.cpp $(CXXFLAGS)
Node.o: Node.cpp Node.hpp
		$(CXX) -o Node.o -c Node.cpp $(CXXFLAGS)
NodesFactory.o: NodesFactory.cpp NodesFactory.hpp
		$(CXX) -o NodesFactory.o -c NodesFactory.cpp $(CXXFLAGS)
clean:
		rm -f *.o
