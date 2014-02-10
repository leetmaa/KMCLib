# Copyright (c)  2013  Mikael Leetmaa
#
# This file is part of the KMCLib project distributed under the terms of the
# GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
#



# Installing is the last task, so this one goes here.
all: ./src/mersenne-twister-master/INSTALL_STAMP ./src/cppunit-1.12.1/INSTALL_STAMP


# -----------------------------------------------------------------------------
# Target to force unpack.
# -----------------------------------------------------------------------------

./zip/FORCE_UNPACK:
# Touch the file if not done.
	touch ./zip/FORCE_UNPACK

# -----------------------------------------------------------------------------
# Unpack.
# -----------------------------------------------------------------------------

# Unpack the mersenne-twister.
./src/mersenne-twister-master/UNPACK_STAMP: ./zip/cslarsen_mersenne_twister_04e512d825.zip ./zip/FORCE_UNPACK
	unzip -o ./zip/cslarsen_mersenne_twister_04e512d825.zip -d ./src && \
	touch ./src/mersenne-twister-master/UNPACK_STAMP

# Unpack cppunit.
./src/cppunit-1.12.1/UNPACK_STAMP: ./zip/cppunit-1.12.1.tar.gz ./zip/FORCE_UNPACK
	cp ./zip/cppunit-1.12.1.tar.gz ./src && \
	cd ./src/ && \
	tar -xzvf cppunit-1.12.1.tar.gz && \
	rm  cppunit-1.12.1.tar.gz && \
	touch cppunit-1.12.1/UNPACK_STAMP && \
	cd -

# -----------------------------------------------------------------------------
# Patch.
# -----------------------------------------------------------------------------

# Patch the Mersenne-Twister

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
	echo "namespace mt {" > tmp.h && \
	echo "" >> tmp.h && \
	cat mersenne-twister.h >> tmp.h && \
	echo "} // Namespace KMCLib patch" >> tmp.h && \
	cat tmp.h | sed s/"RAND_MAX"/"RAND_MAX_MT"/g | sed s/"float"/"double"/g > cslarsenmt.h && \
	echo "// Patching" > tmp.cpp &&\
	echo "#include <stdio.h>" >> tmp.cpp &&\
	echo "#include <stdint.h>" >> tmp.cpp &&\
	echo "#include \"cslarsenmt.h\"" >> tmp.cpp &&\
	echo "#include <limits>" >> tmp.cpp &&\
	echo "#ifndef UINT32_MAX" >> tmp.cpp &&\
	echo "#define UINT32_MAX  (std::numeric_limits<uint32_t>::max())" >> tmp.cpp &&\
	echo "#endif" >> tmp.cpp &&\
	echo "using namespace mt;" >> tmp.cpp &&\
	echo "" >> tmp.cpp &&\
	cat mersenne-twister.cpp >> tmp.cpp &&\
	cat tmp.cpp | sed s/"#include \"mersenne-twister.h\""/""/g | sed s/"MT\[0\] = value;"/"index = 0; MT\[0\] = value;"/g | sed s/"float"/"double"/g | sed s/"1.0f"/"1.0"/g > cslarsenmt.cpp && \
	touch PATCH_STAMP && \
	cd -


# Patch cppunit.
./src/cppunit-1.12.1/PATCH_STAMP: ./src/cppunit-1.12.1/UNPACK_STAMP
	cd ./src/cppunit-1.12.1/ && \
	echo "rm -rf conftest.dSYM" >> configure && \
	cd -

# -----------------------------------------------------------------------------
# Build.
# -----------------------------------------------------------------------------

# Build the patched mersenne-twister.
./src/mersenne-twister-master/BUILD_STAMP: ./src/mersenne-twister-master/PATCH_STAMP
	cd ./src/mersenne-twister-master/ && \
	$(CXX) -O3 -fPIC -o cslarsenmt.o -c cslarsenmt.cpp && \
	touch BUILD_STAMP && \
	cd -

# Build cppunit.
./src/cppunit-1.12.1/BUILD_STAMP: ./src/cppunit-1.12.1/PATCH_STAMP
	cd ./src/cppunit-1.12.1/ && \
	./configure LDFLAGS="-ldl" --prefix="${CURDIR}/lib/cppunit" && \
	make && \
	make check && \
	make install && \
	touch BUILD_STAMP && \
	cd -


# -----------------------------------------------------------------------------
# Install.
# -----------------------------------------------------------------------------


# Install the mersenne-twister.
./src/mersenne-twister-master/INSTALL_STAMP: ./src/mersenne-twister-master/BUILD_STAMP
# Generate the header and copy the files over.
	cd ./src/mersenne-twister-master/ && \
	cp cslarsenmt.o ../../obj/ && \
	cp cslarsenmt.h ../../include/externals/ && \
	touch INSTALL_STAMP && \
	cd -

# Install cppunit.
./src/cppunit-1.12.1/INSTALL_STAMP: ./src/cppunit-1.12.1/BUILD_STAMP
	cd lib/cppunit && \
	mv include/cppunit/ ../../include/ && \
	touch INSTALL_STAMP && \
	cd -


# -----------------------------------------------------------------------------
# Cleanup.
# -----------------------------------------------------------------------------

clean:
	rm -r ./src/mersenne-twister-master/ && \
	rm -r ./src/cppunit-1.12.1/ && \
	rm ./obj/*.o && \
	rm ./lib/*.a && \
	rm ./include/externals/*.h &&\
	rm -r ./include/cppunit
