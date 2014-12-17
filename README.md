## *KMCLib* - a general framework for lattice kinetic Monte Carlo (KMC) simulations

### License and copying

    Developed by Mikael Leetmaa at the Royal Institute of Technology (KTH), Sweden.
    Copyright (c)  2012-2014  Mikael Leetmaa
    Released under the GPLv3 license, see 'LICENSE.txt' for details.

When you use *KMCLib* in your research please **cite** the paper describing the code

    Mikael Leetmaa and Natalia V. Skorodumova
    “KMCLib: A general framework for lattice kinetic Monte Carlo (KMC) simulations”
    Comput. Phys. Commun. (2014) DOI: 10.1016/j.cpc.2014.04.017


### *KMCLib* is now in version 1.1 BETA-1

*KMCLib* is a general framework for *lattice* kinetic Monte Carlo (KMC) simulations, to simulate the time evolution of systems of up to millions of particles in one, two or three dimensions. *KMCLib* is organized as a collection of Python frontend functions, classes and interfaces for performing efficient *lattice* KMC simulations, with all computationally heavy backend functionality implemented in C++.

After discovering an issue with the external Mersenne-twister random number generator included in the previous version of *KMCLib*, we have decided to make the switch over to using **C++11** and the Mersenne-twister random number generator from the standard (C++11) library. This means you will no longer be able to use *KMCLib* with a compiler older than **gcc 4.6**. With this update we also took the opportunity to upgrade the make system to provide support for additional platforms and compilers. Several users have reported difficulties installing *KMCLib* on Mac OSX. From version 1.1 we will support using the **clang** compiler, which we believe will make the installation procedure a lot smoother on Mac OSX. *KMCLib 1.1* will also have support for using the **Intel C++ compiler** under Linux. This was requested for use in particular on large computer clusters with little flexibility to change compilers.

### The combination of features makes *KMCLib* unique
* Python user interface - well documented, easily extended.
* C++ backend library for maximizing efficiency.
* Easy to prototype KMC models - no need to recompile source to run new models.
* Arbitrarily complex elementary processes can be used.
* On-the-fly evaluation of custom rate expressions written in Python.
* Built in on-the-fly mean square displacement (MSD) analysis to facilitate diffusion studies.
* MPI parallelism without loss of accuracy.

### Well documented with usage examples and install instructions
The code manual (http://leetmaa.github.io/KMCLib/manual-v1.0/) includes full documentation of the Python interface and several usage examples, as well as installation instructions for both Linux and Mac.

### Known issues
The *KMCLib 1.1* release is still in BETA, meaning some minor updates and bug-fixes can be expected in the near future; the installation documentation for the new platforms is not yet done but will soon be updated, and the clang support is not yet fully implemented and tested. As allways, if you find any bugs or issues, or if you have suggestions for improvements, please let me know.

### Obtain a copy
The program is distributed under the GPLv3 license and can be obtained from the *KMCLib* git repository https://github.com/leetmaa/KMCLib/releases/tag/v1.1-b1

### Use *KMCLib* in your own research
If you would be interested to use this code in your research and need any assistance, or if you are interested in collaborations, please don't hesitate to send me an e-mail: leetmaa@kth.se


Mikael Leetmaa, December 17 2014


