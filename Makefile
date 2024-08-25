NAME = minishell

SRCS = $(shell find src -name '*.c')

CFLAGS = -Wall -Wextra -g3
# CFLAGS += -Werror
CPPFLAGS = -Isrc -MMD -MP
LDFLAGS = -lreadline

OBJS := $(SRCS:%=build/%.o)
DEPS := $(OBJS:.o=.d)

LIBS = 

.PHONY: all
all: build

.PHONY: build
build: $(NAME)

.PHONY: run
run: build
	./$(NAME)

# linking stage
$(NAME): $(OBJS) $(LIBS)
	cc $(OBJS) $(LDFLAGS) $(LDLIBS) -o $@

# compiling stage
build/%.c.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@
