##v2.0 alpha (Mars 28 2016)

Version *2.0* introduces support for having more than one particle per lattice site in the simulations. This was implemented to enable simulations of gas through porous solids. Version *2.0* also comes with improved performance. A framework for writing custom rate calculators in C++ has been added for performance sensitive applications. Once your custom rate calculator is prototyped and tested in Python you can port it to C++ for increased performance. A caching mechanism for custom rates is now also in place, that can significantly reduce computational time for time consuming custom rates calculations.

##v1.1 (May 28 2015)
***KMCLib* is now in version 1.1**

Version *1.1* introduces support for different random number generators, and increased support for different platforms and compiles. Random numbers are now generated using the standard (C++11) library and *KMCLib* from now on supports the use of both the Mersenne-Twister and the RANLUX PRNG:s through the standard C++ library. Additionally the standard minimal PRNG is also available, as well as true random numbers if you have a random device installed. The make system is upgraded to provide support for additional platforms and compilers. We now support the **clang** compiler on Mac OSX, and both the **gcc** and the **Intel C++ compiler** under Linux. Support for the **Intel C++ compiler** was requested for use in particular on large computer clusters with little flexibility to change compilers. At the time of writing there are no known bugs or issues, but as always, if you happen to find any bugs or issues, or if you have suggestions for improvements, please let me know.


##v1.1-b1 (December 17 2014)
***KMCLib* is now in version 1.1 BETA-1**

After discovering an issue with the external Mersenne-twister random number generator included in the previous version of *KMCLib*, we have decided to make the switch over to using **C++11** and the Mersenne-twister random number generator from the standard (C++11) library. This means you will no longer be able to use *KMCLib* with a compiler older than **gcc 4.6**. With this update we also took the opportunity to upgrade the make system to provide support for additional platforms and compilers. Several users have reported difficulties installing *KMCLib* on Mac OSX. From version 1.1 we will support using the **clang** compiler, which we believe will make the installation procedure a lot smoother on Mac OSX. *KMCLib 1.1* will also have support for using the **Intel C++ compiler** under Linux. This was requested for use in particular on large computer clusters with little flexibility to change compilers. The *KMCLib 1.1* release is still in BETA, meaning some minor updates and bug-fixes can be expected in the near future; the installation documentation for the new platforms is not yet done but will soon be updated, and the clang support is not yet fully implemented and tested.


##v1.0 (May 6 2014)
***KMCLib* version 1.0 is ready for release.**

*KMCLib* is a general framework for *lattice* kinetic Monte Carlo (KMC) simulations, to simulate the time evolution of systems of up to millions of particles in one, two or three dimensions. *KMCLib* is organized as a collection of Python frontend functions, classes and interfaces for performing efficient *lattice* KMC simulations, with all computationally heavy backend functionality implemented in C++.

The combination of features that makes *KMCLib* unique include:

* Python user interface - well documented, easily extended.
* C++ backend library for maximizing efficiency.
* Easy to prototype KMC models - no need to recompile source to run new models.
* Arbitrarily complex elementary processes can be used.
* On-the-fly evaluation of custom rate expressions written in Python.
* Built in on-the-fly mean square displacement (MSD) analysis to facilitate diffusion studies.
* MPI parallelism without loss of accuracy.

After carefully testing the code and removing bugs and issues before the release of version 1.0 there are *no* known bugs or issues at the time of writing.
The code manual (http://leetmaa.github.io/KMCLib/manual-v1.0/) includes full documentation of the Python interface and several usage examples, as well as installation instructions for both Linux and Mac.
The program is distributed under the GPLv3 license and can be obtained from the *KMCLib* git repository https://github.com/leetmaa/KMCLib/releases/tag/v1.0


##v1.0-b2 (Deceber 27 2013)
KMCLib is now in version 1.0 BETA-2. All features for version 1.0 are implemented and all to date known issues are resolved. We will keep updating with more usage examples and a more complete manual before the final release of version 1.0. If you happen to find any bugs or issues, or if you have suggestions for improvements, please let me know.

##v1.0-b1 (Deceber 3 2013)
KMCLib is now in version 1.0 BETA. We are fairly confident that the code works as intended and we have started using it in our own research. Before you start using the code in *your* research you should be aware that there is still room for minor updates and improvements before the first official release later this year. We will keep updating with more usage examples, more analysis functionality and a more complete manual, but there could also eventually be some minor changes to the API. The list of known issues is very short and involves nothing that affects the outcome of the simulations, but if you happen to find any bugs or issues, or if you have suggestions for improvements, please let me know.

