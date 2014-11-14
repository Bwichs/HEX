# Brian Wichers bwichers@ucsc.edu
# Matthew Wang majwang@ucsc.edu
# Makefile code modified from cmps12b Makefile

MKFILE    = Makefile

CC       = g++ -std=c++11 -Wall -Wextra -Wno-deprecated -Wno-sign-compare -lGL -lglut

CPPSOURCE   = hex.cpp glut_test.cpp
CPPHEADER   = hex.h
OBJECTS   = ${CPPSOURCE:.cpp=.o}
EXECBIN   = test

all : ${EXECBIN}

${EXECBIN} : 
	${CC} hex.cpp glut_test.cpp -o test
	./test


%.o : %.cpp
	${CC} -cpp $<


