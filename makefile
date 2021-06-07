CXX = g++
CXXFLAGS = -std=c++11 -g -fsanitize=address -lasan -O0

all: test

test: test.o lexp.o
	${CXX} ${CXXFLAGS} -o $@ $^

test.o: test.cpp lexp.o
	${CXX} ${CXXFLAGS} -c test.cpp -o test

lexp.o: lexp.cpp
	${CXX} ${CXXFLAGS} -c lexp.cpp -o lexp