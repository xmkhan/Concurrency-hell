CXX = u++ #compiler
CXXFLAGS = -g -multi -Wall -Wno-unused-label -MMD	# compiler flags
MAKEFILE_NAME = ${firstword ${MAKEFILE_LIST}}	# makefile name

OBJECTS0 = printer.o bank.o nameserver.o parent.o watcard.o watcardoffice.o bottlingplant.o truck.o vendingmachine.o student.o main.o
EXEC0 = northpole

OBJECTS = ${OBJECTS0}
DEPENDS = ${OBJECTS:.o=.d}
EXECS = ${EXEC0}

#############################################################

all : ${EXECS}						# build all executables

${EXEC0} : ${OBJECTS0}
	${CXX} ${CXXFLAGS} $^ -o $@

#############################################################

.PHONY : all clean

${OBJECTS} : ${MAKEFILE_NAME}			# OPTIONAL : changes to this file => recompile

-include ${DEPENDS}				# include *.d files containing program dependences

clean :						# remove files that can be regenerated
	rm -f *.d *.o ${EXEC0} ${EXEC00} ${EXECS}
