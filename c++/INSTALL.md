# Quick install guide for KMCLib 1.1

Here is a quick guide to get you started with the installation.
More information will come as our experience with troubleshooting
this installation grows. Please send us any feedback or questions.

For Linux we have tested the *g++* and *Intel* compilers, and for
Mac OSX both *g++* and *clang* should work. Note that the compiler
you use must have support for *C++11*. See the full manual for details.


## 1) Make sure you have SWIG with python extensions installed.

    Linux (Ubuntu):
    $ sudo apt-get install swig

    Max OSX with macports:
    $ sudo port install swig
    $ sudo port install swig-python

There might other ways to do this for your particular setup.

## 2) Build the included external dependencies.

    $ cd KMCLib/c++/externals

    Linux with g++:
    $ CXX=g++ CC=gcc make

    Mac OSX with clang:
    $ CXX='clang++ -stdlib=libc++' CC=clang make


## 3) Build KMCLib C++ tests and backend.

    $ cd KMCLib/c++
    $ mkdir build
    $ cd build

    Linux (serial version):
    $ cmake ..

    Linux (parallel version):
    $ cmake -DMP=mpicxx ..

Where you replace mpicxx with any other MPI wrapper you'd rather
use, just make sure it wraps the same compiler as the one you used
for building the external dependencies.

    Mac OSX (clang, serial version):
    $ CXX=clang++ CC=clang cmake ..

    Mac OSX (g++, serial version, with Python from macports):
    $ CXX=g++ CC=gcc cmake -DMACPORT=TRUE ..

Finally:

    $ make test.x
    $ ./unittest/test.x
    $ make install

## 4) Run the Python tests.
Place your "KMCLib/python/src" directory in your PYTHONPATH variable.

Using bash this looks like:

    export PYTHONPATH=<your install path>/KMCLib/python/src:${PYTHONPATH}

in your .bashrc or .bash_profile depending on system.

Now, run

    python KMCLib/python/unittest/utest.py

and each of the tests under "KMCLib/python/functest/".

We have used *python 2.7* for all development and
testing. If you are using Mac OSX with clang then make sure you are using the
machin python /usr/bin/python2.7 or you might end up with an error saying
"Fatal Python error: PyThreadState_Get: no current thread".

## DONE
You should now be ready to start setting up your KMC models. Have a look at
the functionality tests in python to get a hint at how to do it, and check the
source manual if needed.

*Good luck!*
