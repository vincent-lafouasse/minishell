EXEC = tests.out

BUILD_DIR = ./build
SRC_DIR   = ./src

SRCS := $(shell find $(SRC_DIR) -name '*.c')
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:%.o=%.d)

INC_DIRS   = ../include
INC_DIRS  += ./include
INC_DIRS  += ./Unity/include
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

LIBS     = -lft
LDFLAGS  = -L..

LIBS    += -lunity
LDFLAGS += -LUnity

CC = cc
CFLAGS   = -std=gnu99
CFLAGS  += -g3
CFLAGS  += -Wall -Wextra
#CFLAGS += -Werror

CPPFLAGS = $(INC_FLAGS) -MMD -MP


# link libbsd only on Linux, not on macOS
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
	LIBS += -lbsd
endif

.PHONY: test
test: clean run

.PHONY: run
run: build
	@echo
	@echo =============== TEST OUTPUT =============================================
	@./$(BUILD_DIR)/$(EXEC)

.PHONY: build
build: $(BUILD_DIR)/$(EXEC)

# Linking
$(BUILD_DIR)/$(EXEC): $(OBJS)
	make -C ..
	make -C Unity
	echo Linking Test Binary
	$(CC) $^ -o $@ $(LDFLAGS) $(LIBS)

# Compile C 
$(BUILD_DIR)/%.c.o: %.c
	echo Compile $<
	mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	@echo Cleanup tests
	@$(RM) -r $(BUILD_DIR)

.PHONY: format
format:
	clang-format -i $(shell find $(SRC_DIR) -name '*.c')
	clang-format -i $(shell find './include' -name '*.h')

# LSP stuff, don't worry about it
.PHONY: update
update:
	make -C .. clean
	make clean
	mkdir -p $(BUILD_DIR)
	bear --output $(BUILD_DIR)/compile_commands.json -- make -C .. build && make build

.PHONY: t r b f c
t: test
r: run
b: build
f: format
c: clean

-include $(DEPS)
