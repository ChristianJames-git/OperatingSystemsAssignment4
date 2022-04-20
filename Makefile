# Christian James 823672623

# CXX Make variable for compiler
CXX=g++
# Make variable for compiler options
#	-std=c++11  C/C++ variant to use, e.g. C++ 2011
#	-g          include information for symbolic debugger e.g. gdb
CXXFLAGS=-std=c++11 -g -pthread

# Rules format:
# target : dependency1 dependency2 ... dependencyN
#     Command to make target, uses default rules if not specified

# First target is the one executed if you just type make
# make target specifies a specific target

# $^ is an example of a special variable.  It substitutes all dependencies
rideshare : main.o consumer.o producer.o broker.o io.o
	$(CXX) $(CXXFLAGS) -o rideshare $^

main.o : main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp

consumer.o : consumer.h consumer.cpp
	$(CXX) $(CXXFLAGS) -c consumer.cpp

producer.o : producer.h producer.cpp
	$(CXX) $(CXXFLAGS) -c producer.cpp

broker.o : broker.h broker.cpp
	$(CXX) $(CXXFLAGS) -c broker.cpp

io.o : io.h io.cpp ridesharing.h
	$(CXX) $(CXXFLAGS) -c io.cpp

clean :
	rm *.o