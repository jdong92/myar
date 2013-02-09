CC = icc
CXX = icpc
CFLAGS = -Wall -std=c99 -openmp -O3 -g -I.
CXXFLAGS = -Wall -openmp -O3 -g
#overkill on the flags, but that doesn't hurt anything
LDFLAGS = -lrt -lpthread 
#replace this if you want to change the output name
TARGET = myar

SRC = myar.c

#any headers go here
INCLUDES = 

#any .c or .cpp files go here
SOURCE = myar.c

#default is to compile
default:	compile

#depends on all of you source and header files
compile: ${SOURCE} ${INCLUDES}
#this assumes you actually are linking all of the source files together
	${CC} ${CFLAGS} ${SOURCE} -o ${TARGET} ${LDFLAGS}

debug: ${SOURCE} ${INCLUDES}
	${CC} ${CFLAGS} ${SOURCE} -o ${TARGET} ${LDFLAGS} -DDEBUG

dvi: ${TARGET}.tex 
#	pygmentize the input source file -- THIS NAME SHOULD BE SAFE
	pygmentize -f latex -o __${SRC}.tex ${SRC}
#	run latex twice to get references correct
	latex ${TARGET}.tex
#	you can also have a bibtex line here
#	bibtex $(TARGET).tex
	latex $(TARGET).tex
#	remove the pygmentized output to avoid cluttering up the directory
	rm __${SRC}.tex

ps: dvi
	dvips -R -Poutline -t letter ${TARGET}.dvi -o ${TARGET}.ps

pdf: ps
	ps2pdf ${TARGET}.ps
