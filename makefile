CXX = g++
CXXFLAGS = -std=c++11 -g -fsanitize=address -lasan -O2

all: test test.o lexp.o

test: test.o lexp.o
	${CXX} ${CXXFLAGS} -o $@ $(addprefix obj/, $^)

test.o: lexp.o
	${CXX} ${CXXFLAGS} -c test.cpp -o obj/test.o

lexp.o:
	${CXX} ${CXXFLAGS} -c lib/lexp.cpp -o obj/lexp.o

clean:
	rm -r obj test