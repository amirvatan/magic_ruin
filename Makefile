CC=gcc

RM=rm

CFLAGS=-c -Wall -O3

LDFLAGS=-lSDL2

SOURCES=$(find . -iname "*.c")

OBJECTS=$(find . -iname "*.o")


.PHONY: all
all: $(OBJECTS)

$(OBJECTS): $(SOURCES)
	$CC $(SOURCES) 
.c.o:
	$(CC) $(CFLAGS) $< -o $@

.o.: 
	$(CC) $^ $(LDFLAGS) -o $@

.PHONY: clean
clean:
	-@ $(RM) *.o 
	-@ $(RM) $(EXECS)

