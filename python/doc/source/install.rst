
Installation
=============

The installation of :py:mod:`KMCLib` is fairly simple and we have tried
to keep the external dependencies to a minimum. These install
instructions works for the different Linux and Mac systems we have
installed and tested on. If you have successfully installed on another
system we would be happy to receive your updates to these install
instructions.

This table shows on which systems we have successfully
installed and tested the current version of KMCLib. If you have
information for other systems, compilers or library versions please
let us know.

+--------------------+------------+---------------------+-----------+------------+------------+
| OS                 | Compiler   | MPI                 | cppunit   | Python     | SWIG       |
+====================+============+=====================+===========+============+============+
| Ubuntu 12.4 LTS    | GCC 4.6.3  | MPICH2 3.0.2        | 1.12.1    | 2.7.3      | 2.0.4      |
+--------------------+------------+---------------------+-----------+------------+------------+
| CentOS release 5.9 | GCC 4.6.3  | Intel MPI 4.0.3.008 | 1.12.1    | 2.7.2      | 2.0.4      |
+--------------------+------------+---------------------+-----------+------------+------------+
| Mac OSX 10.8.5     | GCC 4.2.1  | --                  | 1.12.1    | 2.7.2      | 2.0.9      |
+--------------------+------------+---------------------+-----------+------------+------------+
| Mac OSX 10.5.8     | GCC 4.2.1  | Open MPI 1.2.3      | 1.12.1    | 2.7.2      | 1.3.31     |
+--------------------+------------+---------------------+-----------+------------+------------+


Compiler
-----------
To compile :py:mod:`KMCLib` you will need a **C++ compiler** installed on your
system. The current version only supports the **g++** compiler. If
you want to use another compiler you will have to modify the make
system accordingly. If you compile :py:mod:`KMCLib` using
another compiler please let us know so we can include your updates in
the official release.


MPI
------
To build the *parallel* version of the code you will also need an **MPI**
implementation installed on your system, and you will be asked to
specify the compiler wrapper script you want to use, such as e.g. **mpicxx**.


Make system
-----------
The make system is based on **CMake**. Installing **CMake**
can be done by typing on Linux::

    sudo apt-get install swig

and on Mac OSX using macports::

    sudo port install swig && sudo port install swig-python


SWIG
-----------
To generate the Python wrappings for the C++ code you must have
**SWIG** with python extensions installed.
On Linux::

    sudo apt-get install swig

On Mac OSX with macports::

    sudo port install swig && sudo port install swig-python



Build the externals
----------------------
All other external dependencies are included with
:py:mod:`KMCLib`. These are the **CPPUNIT** library for C++ unit tests,
and a Mersenne-Twister implementation.

You can build the externals by typing::

    cd KMCLib/c++/externals
    CXX=g++ make


Build C++ and test the backend
--------------------------------
For the build to work (on both Linux and Mac OSX) you will have to make
sure that **CMAke** can find your python headers. This could be
done by finding where they are located and placing them in your
``CPLUS_INCLUDE_PATH``. Some thing like::

    export CPLUS_INCLUDE_PATH=/usr/include/python2.7:${CPLUS_INCLUDE_PATH}

in your ``.bashrc`` file typically helps. Note that the exact path may
vary on different systems.

Now, go to the ``KMCLib/c++`` directory and create the build director::

    cd KMCLib/c++
    mkdir build
    cd build

On Linux::

    cmake -DMPI=mpiwrapper ..

for the MPI version or::

    cmake ..

for the serial version. Replace ``mpiwrapper`` with the mpi compiler wrapper script you want to use, e.g. ``mpicxx``.

Mac with g++ and Python from macports instead do::

    cmake -DCMAKE_CXX_COMPILER=g++ -DCMAKE_C_COMPILER=gcc -DMACPORT=TRUE ..

or leave ``-DMACPORT`` out to use machine Python. Add the
``-DMPI=mpiwrapper`` option to build the parallel version.

On all systems you then type::

    make test.x
    ./unittest/test.x
    make install

To make the tests, run the tests and install the ``Backend.py`` module.


Run the Python tests
-----------------------

Put your ``KMCLib/python/src`` in your ``PYTHONPATH``. If you use bash
this can be done by putting::

   export PYTHONPATH=/home/leetmaa/projects/KMCLib/python/src:${PYTHONPATH}

in your ``.bashrc`` file.

Now you should run the test by typing::

    python KMCLib/python/unittest/utest.py

and finally run each of the tests in the ``KMCLib/python/functest/``
directory.

If the Python unit tests run without errors or failures your
installation is finished and ready to use for performing
KMC simulations.
Have a look at the functionality tests in ``KMCLib/python/functest/``
to get a hint at how to do it, and you should also check this manual,
particular the introductory usage example.


