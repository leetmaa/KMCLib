# Copyright (c)  2013-2016  Mikael Leetmaa
#
# This file is part of the KMCLib project distributed under the terms of the
# GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
#



# Installing is the last task, so this one goes here.
all: ./src/cppunit-1.12.1/INSTALL_STAMP ./src/md5/INSTALL_STAMP

md5: ./src/md5/INSTALL_STAMP

cppunit: ./src/cppunit-1.12.1/INSTALL_STAMP

# -----------------------------------------------------------------------------
# Target to force unpack.
# -----------------------------------------------------------------------------

./zip/FORCE_UNPACK:
# Touch the file if not done.
	touch ./zip/FORCE_UNPACK

# -----------------------------------------------------------------------------
# Unpack.
# -----------------------------------------------------------------------------

# Unpack cppunit.
./src/cppunit-1.12.1/UNPACK_STAMP: ./zip/cppunit-1.12.1.tar.gz ./zip/FORCE_UNPACK
	cp ./zip/cppunit-1.12.1.tar.gz ./src && \
	cd ./src/ && \
	tar -xzvf cppunit-1.12.1.tar.gz && \
	rm  cppunit-1.12.1.tar.gz && \
	touch cppunit-1.12.1/UNPACK_STAMP && \
	cd -

# Unpack the md5 code.
./src/md5/UNPACK_STAMP: ./zip/md5.tar.gz ./zip/FORCE_UNPACK
	cp ./zip/md5.tar.gz ./src && \
	cd ./src/ && \
	tar -xzvf md5.tar.gz && \
	rm md5.tar.gz && \
	touch md5/UNPACK_STAMP && \
	cd -

# -----------------------------------------------------------------------------
# Patch.
# -----------------------------------------------------------------------------

# Patch cppunit.
./src/cppunit-1.12.1/PATCH_STAMP: ./src/cppunit-1.12.1/UNPACK_STAMP
	cd ./src/cppunit-1.12.1/ && \
	echo "rm -rf conftest.dSYM" >> configure && \
	cd -

# -----------------------------------------------------------------------------
# Build.
# -----------------------------------------------------------------------------

# Build cppunit.
./src/cppunit-1.12.1/BUILD_STAMP: ./src/cppunit-1.12.1/PATCH_STAMP
	cd ./src/cppunit-1.12.1/ && \
	./configure LDFLAGS="-ldl" --prefix="${CURDIR}/lib/cppunit" && \
	make && \
	make check && \
	make install && \
	cp -r ./src/cppunit/.libs/libcppunit* ../../lib/cppunit/lib/ && \
	touch BUILD_STAMP && \
	cd -

# Build the md5 code.
./src/md5/BUILD_STAMP: ./src/md5/UNPACK_STAMP
	cd ./src/md5/ && \
	$(CXX) -O3 -fPIC -o md5.o -c md5.c && \
	touch BUILD_STAMP && \
	cd -

# -----------------------------------------------------------------------------
# Install.
# -----------------------------------------------------------------------------

# Install cppunit.
./src/cppunit-1.12.1/INSTALL_STAMP: ./src/cppunit-1.12.1/BUILD_STAMP
	cd lib/cppunit && \
	mv include/cppunit/ ../../include/ && \
	touch INSTALL_STAMP && \
	cd -

# Install md5.
./src/md5/INSTALL_STAMP: ./src/md5/BUILD_STAMP
	cd ./src/md5/ && \
	mv md5.o ../../obj/ && \
	cp md5.h ../../include/externals/ && \
	touch INSTALL_STAMP && \
	cd -

# -----------------------------------------------------------------------------
# Cleanup.
# -----------------------------------------------------------------------------

clean:
	rm -r ./src/cppunit-1.12.1/ && \
	rm -r ./src/md5/ && \
	rm ./include/externals/*.h && \
	rm -r ./include/cppunit && \
	rm -r ./lib/cppunit/ && \
	rm ./obj/*.o

clean-md5:
	rm -r ./src/md5/ && \
	rm ./include/externals/md5.h &&\
	rm ./obj/md5.o

clean-cppunit:
	rm -r ./src/cppunit-1.12.1/ && \
	rm -r ./include/cppunit/ && \
	rm -r ./lib/cppunit/
