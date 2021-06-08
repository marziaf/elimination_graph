CXX = g++
CXXFLAGS = -std=c++11 -g -fsanitize=address -lasan -O2

all: test test.o lexp.o

test: test.o lexp.o
	${CXX} ${CXXFLAGS} -o $@ $^

test.o: test.cpp lexp.o
	${CXX} ${CXXFLAGS} -c test.cpp -o test.o

lexp.o: lexp.cpp
	${CXX} ${CXXFLAGS} -c lexp.cpp -o lexp.o

clean:
	rm *.o test