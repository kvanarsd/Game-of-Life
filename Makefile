CC = clang
CFLAGS = -Wall -Wextra -Werror -Wpedantic
LIBFLAGS = -lm -lncurses
EXEC = life
OBJECTS = universe.o life.o 

all: $(EXEC)

$(EXEC): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(EXEC) $(OBJECTS) $(LIBFLAGS)

universe.o: universe.c
	$(CC) $(CFLAGS) -c universe.c
	
life.o: life.c
	$(CC) $(CFLAGS) -c life.c 

clean:
	rm -f $(EXEC) $(OBJECTS)
format:
	clang-format -i -style=file *.[ch]

scan-build: clean
	scan-build --use-cc=$(CC) make
