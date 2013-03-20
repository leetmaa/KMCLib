
Installation
=============

The installation of KMCLib should be straight forward once you have
an **-std=c++11 compatible C++ compiler**, **cppunit**, **SWIG**,
**mpi** and **Python2.7** with **numpy**
installed on your system.

.. NOTE::

   At the time of writing the make system is only configured to run on
   **Ubuntu 12.4 LTS** with **gcc 4.6.3**, **cppunit 1.12.1**, **swig 2.0.4** and
   **mpich2 3.0.2**. Support for other systems / configurations will
   be added.


Compile and test the C++ backend
---------------------------------
With all external dependencies in place you go to the KMCLib/c++/
directory, create a build directory, e.g. KMCLib/c++/build/ and from
the build directory you type ::

    cmake ..

After that, you can build the C++ unit tests by typing ::

    make tests.x

and run the tests with ::

    ./unittest/tests.x

If the tests run without any error messages you can build the backend
library and Python wrappings by typing ::

    make install

This will create the files Backend.py and _Backend.so in the
KMCLib/python/src/KMCLib/Backend/ directory, needed to run the code
from python.


Running the python unit tests
---------------------------------
After building the C++ backend you are ready to run the tests in
python. With the KMCLib/python/src directory in your
PYTHONPATH system variable, go to KMCLib/python/unittest and type ::

    python utest.py

If the python unit tests run without errors or failures your
installation is finished and you are ready to use KMCLib to perform
KMC simulations.


Full list of prerequisites / external dependencies
---------------------------------------------------
* c++11 compatible c++ compiler
* mpi
* cppunit
* swig
* Python 2.7
* numpy

