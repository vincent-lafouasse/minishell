NAME = minishell

SRCS = $(shell find src -name '*.c')

CC ?= cc
CFLAGS += -Wall -Wextra -g3
# CFLAGS += -Werror
CPPFLAGS += -Isrc -MMD -MP
LDFLAGS += -lreadline

OBJS := $(SRCS:%=build/%.o)
DEPS := $(OBJS:.o=.d)

LIBFT_PATH = ./lib/libft
LIBFT = $(LIBFT_PATH)/libft.a
LIBFT_INCLUDE_DIR = $(LIBFT_PATH)/include
LIBFT_MAKE_DIR = $(LIBFT_PATH)

CPPFLAGS += -I$(LIBFT_INCLUDE_DIR)
LDFLAGS += $(LIBFT)

LIBS = $(LIBFT)

.PHONY: all
all: build

.PHONY: build
build: $(NAME)

.PHONY: run
run: build
	./$(NAME)

# linking stage
$(NAME): $(OBJS) $(LIBS)
	$(CC) $(OBJS) $(LDFLAGS) $(LDLIBS) -o $@

# compiling stage
build/%.c.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(LIBFT):
	make -C $(LIBFT_MAKE_DIR)

.PHONY: re
re: fclean build

.PHONY: clean
clean:
	rm -rf build

.PHONY: fclean
fclean: clean
	rm -rf $(NAME)

.PHONY: test
test: $(LIBS)
	cmake -S test -B build/test
	cmake --build build/test
	GTEST_COLOR=1 ctest --test-dir build/test $(CTEST_OPT)

.PHONY: vtest
ifneq ($(TEST_WITH_MEMCHECK),)
vtest: CTEST_OPT += -T memcheck
endif
vtest: CTEST_OPT += -V
vtest: test

.PHONY: test_libft
test_libft: $(LIBFT)
	cmake -S lib/libft/test -B build/test_libft
	cmake --build build/test_libft
	GTEST_COLOR=1 ctest --test-dir build/test_libft -V

.PHONY: update
update: fclean
	mkdir -p build
	bear  --output build/compile_commands.json -- make build

# aliases
.PHONY: b c u r t vt
b: build
c: clean
u: update
r: run
t: test
vt: vtest

-include $(DEPS)
