# Quick KMCLib installation guide.
Here is a quick guide to get you started with the installation.
More information will come as our experience with troubleshooting
this installation grows. Please send us any feedback or questions.
Note that only *g++* is supported at the moment, but we are working
on being *clang* compatible as well.

## 1) Make sure you have SWIG with python extensions installed.
Linux: "sudo apt-get install swig" Max OSX with macports:
"sudo port install swig && sudo port install swig-python"

## 2) Make sure you have CPPUNIT installed.
Download [CPPUNIT](http://sourceforge.net/projects/cppunit/) and follow the
instructions.

## 3) Build the included external dependencies.
"cd KMCLib/c++/externals"
Linux: "make".
Mac info will come, for the moment you can try the linux makefile (but make
sure you are using *g++* and not *clang* or else it won't work).

## 4) Build KMCLib C++ tests and backend.
"cd KMCLib/c++",
"mkdir build",
"cd build",
"cmake -DMPI=TRUE .." for the MPI version (requiers that you have mpi installed
 and the compiler wrapped in an *mpicxx* script), or "cmake -DMPI=FALSE .." for
 the serial version,
"make test.x",
"./unittest/test.x",
"make install"

## 5) Run the Python tests.
Place your KMCLib/python/src directory in your PYTHONPATH variable and run
"python KMCLib/python/unittest/utest.py" and each of the tests under
"KMCLib/python/functest/". We have used *python 2.7* for all development and
testing.

## DONE
You should now be ready to start setting up your KMC models. Have a look at
the functionality tests in python to get a hint at how to do it, and check the
source manual if needed.

*Good luck!*

