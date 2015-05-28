
Installation
=============

The installation of :py:mod:`KMCLib` is fairly simple and we have tried
to keep the external dependencies to a minimum. These install
instructions works for the different Linux and Mac systems we have
installed and tested on. If you have successfully installed on another
system we would be happy to receive your updates to these install
instructions.

This table shows on which systems we have successfully
installed and tested the current version of KMCLib. Looking at the
span of different compiler and OS versions that work it is reasonable
to assume that the program can be successfully installed on many other
systems as well. If you have
information for other systems, compilers, or library versions please
let us know.


+----------------------------------------------------------------------------------------------------------------------+
| VERSION 1.1                                                                                                          |
+--------------------+-------------------------------------+---------------------+-----------+------------+------------+
| OS                 | Compiler                            | MPI                 | cppunit   | Python     | SWIG       |
+====================+=====================================+=====================+===========+============+============+
| Ubuntu 14.4 LTS    | GCC 4.8.2                           | OpenMPI 1.8.4       | 1.13.0    | 2.7.6      | 2.0.11     |
+--------------------+-------------------------------------+---------------------+-----------+------------+------------+
| Ubuntu 12.4 LTS    | GCC 4.6.3                           | MPICH2 1.4.1        | 1.12.1    | 2.7.3      | 2.0.4      |
+--------------------+-------------------------------------+---------------------+-----------+------------+------------+
| CentOS release 6.6 | icpc-15.0.1 (gcc 4.8.2 runtime lib) | Intel MPI 5.0.2.044 | 1.12.1    | 2.7.3      | 1.3.4      |
+--------------------+-------------------------------------+---------------------+-----------+------------+------------+
| Mac OSX 10.10.3    | Clang 6.1                           | --                  | 1.12.1    | 2.7.6      | 3.0.2      |
+--------------------+-------------------------------------+---------------------+-----------+------------+------------+
| Mac OSX 10.9.5     | Clang 6.0                           | --                  | 1.12.1    | 2.7.5      | 3.0.5      |
+--------------------+-------------------------------------+---------------------+-----------+------------+------------+
| Mac OSX 10.8.5     | Clang 4.2                           | --                  | 1.12.1    | 2.7.2      | 2.0.9      |
+--------------------+-------------------------------------+---------------------+-----------+------------+------------+


Compiler
-----------
To compile :py:mod:`KMCLib` you will need a **C++11 compiler** installed on your
system. The current version supports the **g++ (GCC)**, **icpc(Intel)** and
**Clang (Mac OSX)** compilers. If you want to use another compiler you
will have to modify the make system accordingly. If you compile
:py:mod:`KMCLib` using another compiler please let us know so we can
include your updates in the official release.


MPI
------
To build the *parallel* version of the code you will also need an **MPI**
implementation installed on your system, and you will be asked to
specify the compiler wrapper script you want to use, such as e.g. **mpicxx**.
The current version of :py:mod:`KMCLib` has been tested using **MPICH2**,
**OpenMPI** and **Intel-MPI** as indicated in the table above.

Make system
-----------
The make system is based on **CMake**. Installing **CMake**
can be done by typing on Ubuntu::

    sudo apt-get install swig

and on Mac OSX using macports::

    sudo port install swig && sudo port install swig-python


SWIG
-----------
To generate the Python wrappings for the C++ code you must have
**SWIG** with python extensions installed.
On Ubuntu::

    sudo apt-get install swig

On Mac OSX with macports::

    sudo port install swig && sudo port install swig-python



Build the externals
----------------------
All other external dependencies are included with
:py:mod:`KMCLib`. For the current release this consists only of the
**CPPUNIT** library for C++ unit tests. If you are on a Linux system
and already have cppunit installed you can skip building the
externals (or install it using your package management system)
however if you are using the clang compiler on Mac building
the externals is required.

You build the externals by typing::

    cd KMCLib/c++/externals
    CC=<c-compiler> CXX=<c++-compiler> make

Where you replace <c-compiler> and <c++-compiler> with your compiler.
See the table above for a list of tested compilers for different systems.

Using Clang on Mac OSX 10.8 you will have to use ``CXX="clang++ -stdlib=libc++"``.


Build C++ and test the backend
--------------------------------
For the build to work (on both Linux and Mac OSX) you will have to make
sure that **CMake** can find your python headers. This could be
done by finding where they are located and placing them in your
``CPLUS_INCLUDE_PATH``. Some thing like::

    export CPLUS_INCLUDE_PATH=/usr/include/python2.7:${CPLUS_INCLUDE_PATH}

in your ``.bashrc`` file typically does it, or::

    export CPLUS_INCLUDE_PATH=/opt/local/Library/Frameworks/Python.framework/Versions/2.7/include/python2.7:${CPLUS_INCLUDE_PATH}

in your ``.bash_profile`` file if you are on Mac OSX.
**Note that the exact path may vary on different systems.** If you do
not know what is right for your system you should try to find where
your ``Python.h`` file is located, for the same version of python you
intend to use when running the program later.

Now, go to the ``KMCLib/c++`` directory and create the build director::

    cd KMCLib/c++
    mkdir build
    cd build

On Linux::

    cmake -DMPI=<mpiwrapper> ..

for the MPI version or::

    cmake ..

for the serial version. Replace ``<mpiwrapper>`` with the mpi compiler wrapper script you want to use, e.g. ``mpicxx``.

On older versions of Mac OSX using **clang** you will have to do::

    CC=clang CXX="clang++ -stdlib=libc++" cmake ..

while on newer versions **OSX 10.9** and **OSX 10.10** with **clang 6** use::

    CC=clang CXX=clang++ cmake ..


Add the ``-DMPI=<mpiwrapper>`` option to build the parallel version.
(Note that this has not been tested on mac.)

On all systems you then type::

    make test.x
    ./unittest/test.x
    make install

To make the tests, run the tests and install the ``Backend.py`` module.

If the ``make install`` step complains about not finding ``Python.h``
then your CPLUS_INCLUDE_PATH variable has not been set correctly.



Run the Python tests
-----------------------

Put your ``KMCLib/python/src`` in your ``PYTHONPATH``. If you use bash
this can be done by putting::

   export PYTHONPATH=<absolute-path>/KMCLib/python/src:${PYTHONPATH}

in your ``.bashrc`` file (or in ``.bash_profile`` on Mac OSX), where
you replace ``<absolute-path>`` with the absolute path to the
directory where KMCLib was installed.

Now you should run the test by typing::

    python KMCLib/python/unittest/utest.py

and finally run each of the tests in the ``KMCLib/python/functest/``
directory.

If the Python unit tests run without errors or failures your
installation is finished and ready to use for performing KMC simulations.
Have a look at the functionality tests in ``KMCLib/python/functest/``
to get a hint at how to do it, and you should also check this manual,
particular the introductory usage example.


