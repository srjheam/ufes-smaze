VALGRINDOUT = valgrind-out.txt

FLAGS = -Wall -Wextra -Wno-unused-result -Wno-unused-parameter -g -I src/search -I src/ed -L . -lsearch -led -lm

DEPS = horario.h
OBJ = horario.o main.o

LIBED_DEPS = $(patsubst %.c,%.o,$(wildcard ./src/ed/*.c)) 
LIBSEARCH_DEPS = $(patsubst %.c,%.o,$(wildcard ./src/search/*.c)) 

all: main

%.o: %.c %.h
	gcc -c -o $@ $< $(FLAGS)

libed.a: $(LIBED_DEPS)
	ar -crs libed.a $(LIBED_DEPS)

libsearch.a: $(LIBSEARCH_DEPS)
	ar -crs libsearch.a $(LIBSEARCH_DEPS)

main: main.c libed.a libsearch.a 
	gcc -o main main.c $(FLAGS)

clean:
	rm -f main libed.a libsearch.a $(LIBSEARCH_DEPS) $(LIBED_DEPS) jheam_ross_2022100890.zip $(VALGRINDOUT)

zip: clean
	zip -r jheam_ross_2022100890.zip . \
		-x dados/\* \
		   scripts/\* \
		   *.zip \
		   *.txt
	
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
