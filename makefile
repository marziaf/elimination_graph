CXX = g++
CXXFLAGS = -std=c++14 -g -fsanitize=address -lasan

all: test test.o lex.o fill.o

test: test.o lex.o fill.o
	${CXX} ${CXXFLAGS} -o $@ $(addprefix obj/, $^)

test.o: lex.o test.cpp
	${CXX} ${CXXFLAGS} -c test.cpp -o obj/test.o

lex.o: lib/lex.cpp
	${CXX} ${CXXFLAGS} -c lib/lex.cpp -o obj/lex.o

fill.o: lib/fill.cpp
	${CXX} ${CXXFLAGS} -c lib/fill.cpp -o obj/fill.o

clean:
	rm -r obj test