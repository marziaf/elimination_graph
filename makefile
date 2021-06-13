CXX = g++
CXXFLAGS = -std=c++14 -g -fsanitize=address -lasan 
LDLIBS = -isystem benchmark/include -Lbenchmark/build/src  -lpthread  -lbenchmark

ODIR=obj
_OBJ = lex.o fill.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

all: test bm

test: test.cpp $(OBJ) test_cases.o
	${CXX} ${CXXFLAGS} -o $@ $^

bm: bm.cpp $(OBJ) test_cases.o
	${CXX} $< ${CXXFLAGS} ${LDLIBS}  -o $@  $(OBJ) test_cases.o

test_cases.o: test_cases.cpp
	${CXX} ${CXXFLAGS} -c -o $@ $^

obj/%.o: lib/%.cpp
	${CXX} ${CXXFLAGS} -c -o $@ $^

clean:
	rm obj/* test

# https://devhints.io/makefile