VALGRINDOUT = valgrind-out.txt

FLAGS = -Wall -Wextra -Wno-unused-result -Wno-unused-parameter -g

DEPS = horario.h
OBJ = horario.o main.o

LIBED_DEPS = $(patsubst %.c,%.o,$(wildcard ./src/ed/*.c)) 
LIBSEARCH_DEPS = $(patsubst %.c,%.o,$(wildcard ./src/search/*.c)) 

all: main

%.o: %.c %.h
	gcc $(FLAGS) -c -o $@ $< 

libed.a: $(LIBED_DEPS)
	ar -crs libed.a $(LIBED_DEPS)

libsearch.a: $(LIBSEARCH_DEPS)
	ar -crs libsearch.a $(LIBSEARCH_DEPS)

main: main.c libed.a libsearch.a 
	gcc $(FLAGS) -o main main.c -I src/search -I src/ed -L . -led -lsearch -lm

clean:
	rm -f main libed.a libsearch.a $(LIBSEARCH_DEPS) $(LIBED_DEPS)

zip: clean
	zip -r jheam_ross_2022100890.zip .
	
run:
	./main

valgrind: all
	rm -f $(VALGRINDOUT)

	valgrind --leak-check=full \
			 --show-leak-kinds=all \
			 --track-origins=yes \
			 --verbose \
			 --log-file=$(VALGRINDOUT) \
			 ./main

	@echo "Head to $(VALGRINDOUT)"
