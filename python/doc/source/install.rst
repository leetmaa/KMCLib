
Installation
=============

The installation of KMCLib should be straight forward once you have
a **C++ compiler** wrapped in an **mpicxx**
script, as well as **cppunit**, **SWIG**,
**mpi** and **Python2.7** with **numpy** installed on your
system. This table shows on which systems we have successfully
installed and tested the current version of KMCLib.

+--------------------+------------+---------------------+-----------+------------+------------+
| OS                 | compiler   | mpi                 | cppunit   | Python     | SWIG       |
+====================+============+=====================+===========+============+============+
| Ubuntu 12.4 LTS    | GCC 4.6.3  | MPICH2 3.0.2        | 1.12.1    | 2.7.3      | 2.0.4      |
+--------------------+------------+---------------------+-----------+------------+------------+
| CentOS release 5.9 | GCC 4.6.3  | Intel MPI 4.0.3.008 | 1.12.1    | 2.7.2      | 2.0.4      |
+--------------------+------------+---------------------+-----------+------------+------------+

.. NOTE::
   At present **g++** is the only supported compiler


Compile and test KMCLib
------------------------
KMCLib consists of a Python frontend, with a compiled C++ backend
wrapped in Python using SWIG. To install KMCLib you thus have to
build, test and wrap the C++ part of the code before you can use
KMCLib in Python. This is fortunately a relatively fast and easy
procedure. Please follow the below steps carefully.


Step #0: Fix dependencies
..........................
Needless to say, without the above mentioned dependencies you will
not get far. Please install them properly.

The mersenne-twister pseudo random number generator used is
included in the distribution. You will have to prepare this by
typing ::

    make

in the KMCLib/c++/externals/ directory.

Please make sure all your external dependencies are in place before
you go to the next step.


Step #1: Run CMake
...................
With all external dependencies in place, go to the KMCLib/c++/
directory, create a build directory, e.g. KMCLib/c++/build/ and from
the build directory type ::

    cmake ..

If you have installed **cppunit** in a non standard location you
will have to give the search path to cmake using the -DCPPUNIT_DIR flag::

    cmake -DCPPUNIT_DIR="path/to/cppunit/library/directory" ..

.. NOTE::

   The present version of the build system assumes that your
   **mpicxx** uses **g++**. No other compilers are yet supported.
   You can check what the mpicxx
   command does on your machine by typing "mpicxx -show" in a terminal.


Step #2: Run make
......................
After CMake has created the necessary make files,
build the C++ unit tests by typing ::

    make tests.x

and run the tests with ::

    ./unittest/tests.x

If the tests run without error messages, build the backend
library and Python wrappings by typing ::

    make install

This will create the files Backend.py and _Backend.so in the
KMCLib/python/src/KMCLib/Backend/ directory, needed to run the code
from Python.

.. NOTE::
   It is possible to build and install the Python wrappings also if
   the unit tests in C++ fails to run or even compile,
   although we strongly recommend that you
   take these tests seriously.


Step #3: Run the Python unit tests
.....................................
After building and wrapping the C++ backend you are ready to run the tests in
Python. With the KMCLib/python/src directory in your
PYTHONPATH system variable, go to KMCLib/python/unittest and type ::

    python utest.py

If the Python unit tests run without errors or failures your
installation is finished and ready to use for performing
KMC simulations.

