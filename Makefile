CFLAGS=-Wall -Wextra -Werror -Wno-missing-field-initializers -Wno-unused-parameter

.PHONY: all
all: test

.PHONY: cl
cl: clean

.PHONY: clean
clean:
	$(RM) test
