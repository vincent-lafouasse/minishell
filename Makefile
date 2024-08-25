SRC = $(shell find src -name '*.c')

CPPFLAGS = -Isrc
LDFLAGS = -lreadline

.PHONY: all
all:
	gcc $(CPPFLAGS) $(LDFLAGS) $(SRC) -o minishell
