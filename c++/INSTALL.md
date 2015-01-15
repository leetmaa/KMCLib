# Quick KMCLib installation guide.
Here is a quick guide to get you started with the installation.
More information will come as our experience with troubleshooting
this installation grows. Please send us any feedback or questions.
Note that only *g++* is supported at the moment, but we are working
on being *clang* compatible as well.

## 1) Make sure you have SWIG with python extensions installed.
Linux: "sudo apt-get install swig" Max OSX with macports:
"sudo port install swig && sudo port install swig-python"

## 2) Build the included external dependencies.
This includeds CppUnit and a mersenne-twister implementation.
"cd KMCLib/c++/externals",
"CXX=g++ make"

## 3) Build KMCLib C++ tests and backend.
"cd KMCLib/c++",
"mkdir build",
"cd build",
Linux: "cmake -DMPI=mpiwrapper .." for the MPI version or "cmake .." for
 the serial version. Replace "mpiwrapper" with the mpi compiler wrapper script you want to use, e.g. "mpicxx".
Mac with g++ and python from macports: "cmake -DCMAKE_CXX_COMPILER=g++ -DCMAKE_C_COMPILER=gcc -DMACPORT=TRUE .." or leave -DMACPORT out to use machine Python.
"make test.x",
"./unittest/test.x",
"make install"

## 4) Run the Python tests.
Place your "KMCLib/python/src" directory in your PYTHONPATH variable and run
"python KMCLib/python/unittest/utest.py" and each of the tests under
"KMCLib/python/functest/". We have used *python 2.7* for all development and
testing.

## DONE
You should now be ready to start setting up your KMC models. Have a look at
the functionality tests in python to get a hint at how to do it, and check the
source manual if needed.

*Good luck!*


# Quick install guide for KMCLib 1.1 on Mac OSX with clang.

## 1) Make sure you have SWIG with python extensions installed.
How you do this is up to you. For Mac OSX with macports:
"sudo port install swig && sudo port install swig-python",
but there might be other ways.

## 2) Build the included external dependencies.
"cd KMCLib/c++/externals",
"CXX='clang++ -stdlib=libc++'  make"

## 3) Build KMCLib C++ tests and backend.
"cd KMCLib/c++",
"mkdir build",
"cd build",
Mac with clang: "CXX=clang++ cmake .."
"make test.x",
"./unittest/test.x",
"make install"

## 4) Run the Python tests.
Place your "KMCLib/python/src" directory in your PYTHONPATH variable and run
"python KMCLib/python/unittest/utest.py" and each of the tests under
"KMCLib/python/functest/". We have used *python 2.7* for all development and
testing.

## DONE
You should now be ready to start setting up your KMC models. Have a look at
the functionality tests in python to get a hint at how to do it, and check the
source manual if needed.
