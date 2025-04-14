include settings.mk

# Check if tests folder exists
ifneq ($(wildcard tests/*),)
TESTS_ENABLED=1
endif

# Set compiler options
CC=gcc

# Set prerrequisites
SRCS_C += $(shell find src/ -iname "*.c")
SRCS_H += $(shell find src/ -iname "*.h")
DEPS = $(foreach STL,$(STATIC_LIBPATHS),$(STL:%=%/bin/lib$(notdir $(STL)).a))

# Set test prerrequisites
ifeq ($(TESTS_ENABLED),1)
TESTS_C += $(shell find tests/ -iname "*.c")
TESTS_H += $(shell find tests/ -iname "*.h")
endif

# Set header paths to (-I)nclude
IDIRS += $(addsuffix /src,$(STATIC_LIBPATHS) .)

# Set library paths to (-L)ook
LIBDIRS = $(addsuffix /bin,$(STATIC_LIBPATHS))

# Set intermediate objects
OBJS = $(patsubst src/%.c,obj/%.o,$(SRCS_C))

# Set test intermediate objects
ifeq ($(TESTS_ENABLED),1)
TEST_OBJS = $(filter-out $(TEST_EXCLUDE), $(TESTS_C)) $(patsubst src/%.c,obj/%.o,$(filter-out $(TEST_EXCLUDE), $(SRCS_C)))
endif

# Set binary targets
BIN = bin/$(shell cd . && pwd | xargs basename)

# Set test binary targets
ifeq ($(TESTS_ENABLED),1)
TEST = bin/$(shell cd . && pwd | xargs basename)_tests.out
endif

.PHONY: all
all: debug $(TEST)

.PHONY: debug
debug: CFLAGS = $(CDEBUG)
debug: $(BIN)

.PHONY: release
release: CFLAGS = $(CRELEASE)
release: $(BIN)

.PHONY: clean
clean:
	-rm -rfv obj bin

$(BIN): $(OBJS) | $(dir $(BIN))
	$(CC) $(CFLAGS) -o "$@" $^ $(IDIRS:%=-I%) $(LIBDIRS:%=-L%) $(LIBS:%=-l%)

obj/%.o: src/%.c $(SRCS_H) $(DEPS) | $(dir $(OBJS))
	$(CC) $(CFLAGS) -c -o "$@" $< $(IDIRS:%=-I%)


ifeq ($(TESTS_ENABLED),1)
$(TEST): CFLAGS = $(CDEBUG)
$(TEST): $(TEST_OBJS) | $(dir $(TEST))
	$(CC) $(CFLAGS) -o "$@" $^ $(IDIRS:%=-I%) $(LIBDIRS:%=-L%) $(RUNDIRS:%=-Wl,-rpath,%) $(LIBS:%=-l%) -lcspecs
endif


.SECONDEXPANSION:
$(DEPS): $$(shell find $$(patsubst %bin/,%src/,$$(dir $$@)) -iname "*.c" -or -iname "*.h")
	make --no-print-directory -C $(patsubst %bin/,%,$(dir $@)) 3>&1 1>&2 2>&3 | sed -E 's,(src/|tests/)[^ ]+\.(c|h)\:,$(patsubst %bin/,%,$(dir $@))&,' 3>&2 2>&1 1>&3

$(sort $(dir $(BIN) $(OBJS))):
	mkdir -pv $@

.PHONY: start
start: debug
	valgrind --tool=none ./$(BIN) $(ARGS)

.PHONY: daemon
daemon:
	@test $(shell which entr) || entr
	while sleep 0.1; do \
		find src/ | entr -d make start --no-print-directory; \
	done


.PHONY: memcheck
memcheck: debug
	valgrind --leak-check=full $(MEMCHECK_FLAGS) ./$(BIN) $(ARGS)

.PHONY: helgrind
helgrind: debug
	valgrind --tool=helgrind $(HELGRIND_FLAGS) ./$(BIN) $(ARGS)


################################################################################

ifeq ($(TESTS_ENABLED),1)
.PHONY: test
test: $(TEST)
	valgrind --tool=none ./$(TEST)

.PHONY: test-daemon
test-daemon:
	@test $(shell which entr) || entr
	while sleep 0.1; do \
		find tests/ src/ | entr -d make test --no-print-directory; \
	done

.PHONY: test-memcheck
test-memcheck: $(TEST)
	valgrind --leak-check=full $(MEMCHECK_FLAGS) ./$(TEST)

.PHONY: test-helgrind
test-helgrind: $(TEST)
	valgrind --tool=helgrind $(HELGRIND_FLAGS) ./$(TEST)
endif

