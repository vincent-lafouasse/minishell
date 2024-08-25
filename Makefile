NAME = minishell

SRC = $(shell find src -name '*.c')

CPPFLAGS = -Isrc
LDFLAGS = -lreadline

.PHONY: all
all: build

.PHONY: build
build: $(NAME)

$(NAME):
	gcc $(CPPFLAGS) $(LDFLAGS) $(SRC) -o $(NAME)

