# Copyright (c)  2013  Mikael Leetmaa
#
# This file is part of the KMCLib project distributed under the terms of the
# GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
#



# -----------------------------------------------------------------------------
# This is what we want to install.
# -----------------------------------------------------------------------------

# Installing is the last task, so this one goes here.
all: ./src/mersenne-twister-master/INSTALL_STAMP


# -----------------------------------------------------------------------------
# Target to force mersenne-twister unpack.
# -----------------------------------------------------------------------------

./zip/FORCE_UNPACK:
# Touch the file if not done.
	touch ./zip/FORCE_UNPACK


# -----------------------------------------------------------------------------
# Unpack mersenne-twister.
# -----------------------------------------------------------------------------

./src/mersenne-twister-master/UNPACK_STAMP: ./zip/cslarsen_mersenne_twister_04e512d825.zip ./zip/FORCE_UNPACK
# Unpack the mersenne-twister.
	unzip -o ./zip/cslarsen_mersenne_twister_04e512d825.zip -d ./src && \
	touch ./src/mersenne-twister-master/UNPACK_STAMP


# -----------------------------------------------------------------------------
# Patch mersenne-twister.
# -----------------------------------------------------------------------------

# In the .cpp file:
# 1) Make sure UINT32_MAX is defined.
# 2) Change name of the header file to include to 'cslarsenmt.h'.
# 3) Use a separate namespace.
# 4) Make sure to reset the 'index' variable when seeding.
# 5) Change from 'float' to 'double'.

# In the .h file:
# 1) Use a separate namespace.
# 2) Make sure not to redefine RAND_MAX.
# 3) Change from 'float' to 'double'.

./src/mersenne-twister-master/PATCH_STAMP: ./src/mersenne-twister-master/UNPACK_STAMP
# Patch the .cpp and .h files.
	cd ./src/mersenne-twister-master/ && \
	echo "namespace mt {\n" > tmp.h && \
	cat mersenne-twister.h >> tmp.h && \
	echo "\n} // Namespace KMCLib patch" >> tmp.h && \
	cat tmp.h | sed s/"RAND_MAX"/"RAND_MAX_MT"/g | sed s/"float"/"double"/g > cslarsenmt.h && \
	echo "// Patching\n#include <stdio.h>\n#include <stdint.h>\n#include \"cslarsenmt.h\"\n#include <limits>\n#ifndef UINT32_MAX\n#define UINT32_MAX  (std::numeric_limits<uint32_t>::max())\n#endif\nusing namespace mt;\n" > tmp.cpp &&\
	cat mersenne-twister.cpp >> tmp.cpp &&\
	cat tmp.cpp | sed s/"#include \"mersenne-twister.h\""/""/g | sed s/"MT\[0\] = value;"/"index = 0; MT\[0\] = value;"/g | sed s/"float"/"double"/g | sed s/"1.0f"/"1.0"/g > cslarsenmt.cpp && \
	touch PATCH_STAMP && \
	cd -


# -----------------------------------------------------------------------------
# Build the patched mersenne-twister.
# -----------------------------------------------------------------------------

./src/mersenne-twister-master/BUILD_STAMP: ./src/mersenne-twister-master/PATCH_STAMP
# Build the library.
	cd ./src/mersenne-twister-master/ && \
	$(CXX) -O3 -fPIC -o cslarsenmt.o -c cslarsenmt.cpp && \
	touch BUILD_STAMP && \
	cd -


# -----------------------------------------------------------------------------
# Installe the mersenne-twister so that the CMake build system finds it.
# -----------------------------------------------------------------------------

./src/mersenne-twister-master/INSTALL_STAMP: ./src/mersenne-twister-master/BUILD_STAMP
# Generate the header and copy the files over.
	cd ./src/mersenne-twister-master/ && \
	cp cslarsenmt.o ../../obj/ && \
	cp cslarsenmt.h ../../include/externals/ && \
	touch INSTALL_STAMP && \
	cd -


# -----------------------------------------------------------------------------
# Cleanup by simply deleting everything.
# -----------------------------------------------------------------------------

clean:
	rm -r ./src/mersenne-twister-master/ && \
	rm ./obj/*.o && \
	rm ./include/externals/*.h

