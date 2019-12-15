OBJS=scanner.o parser.o ast.o interpreter.o table.o code.o compiler.o
INTERM=scanner.c parser.c parser.h
PROGRAM=./compiler
CFLAGS=-g -Wall -Wno-unused-function -Wno-unused-variable

all: $(PROGRAM)

scanner.c: scanner.flex parser.h
	flex -o scanner.c scanner.flex

parser.c parser.h: parser.bison
	bison --verbose --defines=parser.h -o parser.c parser.bison

$(PROGRAM): $(OBJS)
	$(CC) -o $(PROGRAM) $(OBJS) -lm

clean:
	rm -f $(PROGRAM) $(OBJS) $(INTERM) *.xml parser.output

test: tests/*
	for file in $^ ; do \
		echo $${file} ; \
		$(PROGRAM) $${file} ; \
		echo '' ; \
	done
