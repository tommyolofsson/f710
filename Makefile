CFLAGS=-Wall -Wextra -Werror -Wno-missing-field-initializers -I . -O3

.PHONY: all
all: test

test: test.o f710.o

.PHONY: clean
clean:
	$(RM) test
	$(RM) *.o
