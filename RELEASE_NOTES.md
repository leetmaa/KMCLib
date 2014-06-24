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

