
# Copyright (c)  2012  Mikael Leetmaa
#
# This file is part of the KMCLib project distributed under the terms of the
# GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
#


# A simplest possible makefile will do for the moment.

CC = g++
CCFLAGS   = -fPIC -O3 -c
SOFLAGS   = -fPIC -Wall -Wextra -shared -flat_namespace
TEST_LINK = -lcppunit

SRC_CPPFILES  = $(wildcard ./src/*.cpp)
SRC_HFILES    = $(SRC_CPPFILES:.cpp=.h)
SRC_OBJ       = $(SRC_CPPFILES:.cpp=.o)

TEST_CPPFILES = $(wildcard ./unittest/*.cpp)
TEST_HFILES   = $(TEST_CPPFILES:.cpp=.h)
TEST_OBJ      = $(TEST_CPPFILES:.cpp=.o)

.PHONY:wrap

all: backend tests

# Compile the source.
src: $(SRC_OBJ) $(SRC_HFILES)

# Generate and compile the wrappings.
wrap: ./wrap/backend.i $(SRC_HFILES)
	swig -python -c++ ./wrap/backend.i ; \
	$(CC) -o ./wrap/backend_wrap.o $(CCFLAGS) ./wrap/backend_wrap.cxx

# Create the shared library to use.
bundle: src wrap
	$(CC) $(SOFLAGS) ./src/*.o ./wrap/*.o -o _Backend.so

# Make the backend for python.
backend: bundle
	mv _Backend.so ../python/src/KMCLib/Backend/_Backend.so ;
	mv ./wrap/Backend.py ../python/src/KMCLib/Backend/Backend.py

# Rule for compiling .o files, simply depending on all src headers for now.
%.o: %.cpp  $(patsubst %.o,%.h,$@) $(SRC_HFILES)
	$(CC) $(CCFLAGS) $< -o $@

# Compile the tests.
tests: $(SRC_OBJ) $(TEST_OBJ)
	$(CC) -o run_tests.x $^ $(TEST_LINK)

# Clean everything.
clean-wrap:
	rm ./wrap/backend_wrap.* ; \
	rm ../python/src/KMCLib/Backend/_Backend.so ; \
	rm ../python/src/KMCLib/Backend/Backend.py

# Clean the .o and *~ files.
clean:
	rm ./src/*.o      ./src/*~      ; \
	rm ./wrap/*.o     ./wrap/*~     ; \
	rm ./unittest/*.o ./unittest/*~ ; \
	rm run_tests.x

