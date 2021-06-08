CXX = g++
CXXFLAGS = -std=c++11 -g -fsanitize=address -lasan -O2

all: test test.o lex.o fill.o

test: test.o lex.o fill.o
	${CXX} ${CXXFLAGS} -o $@ $(addprefix obj/, $^)

test.o: lex.o
	${CXX} ${CXXFLAGS} -c test.cpp -o obj/test.o

lex.o:
	${CXX} ${CXXFLAGS} -c lib/lex.cpp -o obj/lex.o

fill.o:
	${CXX} ${CXXFLAGS} -c lib/fill.cpp -o obj/fill.o

clean:
	rm -r obj test