CXX = g++
CXXFLAGS = -std=c++14 -g -fsanitize=address -lasan 
LDLIBS = -isystem benchmark/include -Lbenchmark/build/src  -lpthread  -lbenchmark

ODIR=obj
_OBJ = lex.o fill.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

all: test bm

test: test.cpp $(OBJ)
	${CXX} ${CXXFLAGS} -o $@ $^

bm: bm.cpp $(OBJ)
	${CXX} $< ${CXXFLAGS} ${LDLIBS}  -o $@  $(OBJ)

obj/%.o: lib/%.cpp
	${CXX} ${CXXFLAGS} -c -o $@ $^

clean:
	rm obj/* test

# https://devhints.io/makefile