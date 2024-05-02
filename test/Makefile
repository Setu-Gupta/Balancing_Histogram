CXX = g++
CXX_FLAGS = -pedantic -Wall -Wextra -Werror -std=c++20

OPT_FLAGS = -Ofast -march=native -flto
DBG_FLAGS = -g3 -Og -ggdb

INCLPATH = ../headers
LIBSRC = $(wildcard $(INCLPATH)/*.h)

SRC = $(wildcard ./*.cc)
SRC += $(wildcard ./*.cpp)

.PHONY: release
release: CXX_FLAGS += $(OPT_FLAGS)
release: test

.PHONY: debug
debug: CXX_FLAGS += $(DBG_FLAGS)
debug: test

test: $(SRC) $(LIBSRC)
	$(CXX) -I$(INCLPATH) $(CXX_FLAGS) $(SRC) -o $@

.PHONY: clean
clean:
	rm test
