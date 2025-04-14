# Libraries
LIBS=utils commons pthread readline m

# Custom libraries' paths
STATIC_LIBPATHS=../utils

# Compiler flags
CDEBUG=-g -Wall -Wno-comment -DDEBUG -fdiagnostics-color=always
CTEST=-g -Wall -Wno-comment -DDEBUG -DTEST -fdiagnostics-color=always
CRELEASE=-O3 -Wall -Wno-comment -DNDEBUG -fcommon

# Arguments when executing with start, memcheck or helgrind
ARGS=kernel.config

# Valgrind flags
MEMCHECK_FLAGS=--track-origins=yes
HELGRIND_FLAGS=

# Excluded source files (eg: main() function)
TEST_EXCLUDE=src/main.c