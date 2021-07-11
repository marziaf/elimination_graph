CXX = g++
CXXFLAGS = -std=c++14 -g 
DEBUGFLAGS = -Wall -Wextra -pedantic -fsanitize=address -lasan
LDLIBS = -lbenchmark -lpthread -isystem benchmark/include -Lbenchmark/build/src

ODIR=obj
_OBJ = lex.o fill.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

all: test bm space

test: test.cpp $(OBJ) obj/test_cases.o
	${CXX} ${CXXFLAGS} ${DEBUGFLAGS}  -o $@ $^

bm: bm.cpp $(OBJ) obj/test_cases.o
	${CXX} -O2 $< ${LDLIBS} ${CXXFLAGS}  -o $@ $(OBJ) obj/test_cases.o

space: spacebm.cpp  $(OBJ) obj/test_cases.o
	${CXX} -O0 $< ${CXXFLAGS}  -o $@ $(OBJ) obj/test_cases.o

obj/test_cases.o: test_cases.cpp
	${CXX} ${CXXFLAGS} -c -o $@ $^

obj/%.o: lib/%.cpp
	${CXX} ${CXXFLAGS} -c -o $@ $^

clean:
	rm obj/* test bm

# https://devhints.io/makefile