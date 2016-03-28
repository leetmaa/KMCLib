## *KMCLib* - a general framework for lattice kinetic Monte Carlo (KMC) simulations

### License and copying

    Developed by Mikael Leetmaa at the Royal Institute of Technology (KTH), Sweden.
    Copyright (c)  2012-2016  Mikael Leetmaa
    Released under the GPLv3 license, see 'LICENSE.txt' for details.

When you use *KMCLib* in your research please **cite** the paper describing the code:

    Mikael Leetmaa and Natalia V. Skorodumova
    “KMCLib: A general framework for lattice kinetic Monte Carlo (KMC) simulations”
    Comput. Phys. Commun. 185 (2014) 2340
    DOI: 10.1016/j.cpc.2014.04.017

If you use version *1.1* or later I would also ask you to include this reference:

    Mikael Leetmaa and Natalia V. Skorodumova
    “KMCLib 1.1: Extended random number support and technical updates to the KMCLib general framework for kinetic Monte-Carlo simulations“
    Comput. Phys. Commun.
    DOI: 10.1016/j.cpc.2015.06.016

The mean square displacement algorithm is described here:

    Mikael Leetmaa and Natalia V. Skorodumova
    “Mean square displacements with error estimates from non-equidistant time-step kinetic Monte Carlo simulations“
    Comput. Phys. Commun.
    DOI: 10.1016/j.cpc.2015.02.006


### *KMCLib* is now in version 2.0 alpha

*KMCLib* is a general framework for *lattice* kinetic Monte Carlo (KMC) simulations, to simulate the time evolution of systems of up to millions of particles in one, two or three dimensions. *KMCLib* is organized as a collection of Python frontend functions, classes and interfaces for performing efficient *lattice* KMC simulations, with all computationally heavy backend functionality implemented in C++.

Version *2.0* introduces support for having more than one particle per lattice site in the simulations. This was implemented to enable simulations of gas through porous solids as described in this paper:

    Amber Mace, Mikael Leetmaa and Aatto Laaksonen
    “Temporal Coarse Graining of CO2 and N2 Diffusion in Zeolite NaKA: From the Quantum Scale to the Macroscopic“
    Journal of chemical theory and computation 11 (10), 4850-4860

Version *2.0* also comes with improved performance. A framework for writing custom rate calculators in C++ has been added for performance sensitive applications. Once your custom rate calculator is prototyped and tested in Python you can port it to C++ for increased performance. A caching mechanism for custom rates is now also in place, that can significantly reduce computational time for time consuming custom rates calculations.

### The combination of features makes *KMCLib* unique
* Python user interface - well documented, easily extended.
* C++ backend library for maximizing efficiency.
* Easy to prototype KMC models - no need to recompile source to run new models.
* Arbitrarily complex elementary processes can be used.
* On-the-fly evaluation of custom rate expressions written in Python.
* Built in on-the-fly mean square displacement (MSD) analysis to facilitate diffusion studies.
* MPI parallelism without loss of accuracy.

### Well documented with usage examples and install instructions
The code manual (http://leetmaa.github.io/KMCLib/manual-v1.1/) includes full documentation of the Python interface and several usage examples, as well as installation instructions for both Linux and Mac.

There is at the time of writing no manual for version *2.0*. All features in version *1.1* should be unchanged however. If ever in doubt please check the source code for details.

### Known issues
Version *2.0* is still in an *alpha* stage. The set of features might change before the full version *2.0*, and bugs may be found and fixed, however the quality of the code at this stage should meet the same standards with respect to quality as the previous full version *1.1*. There are at the time of writing no known major bugs or issues, but as always, if you happen to find any bugs or issues, or if you have suggestions for improvements, please let me know.

### Obtain a copy
The program is distributed under the GPLv3 license and can be obtained from the *KMCLib* git repository https://github.com/leetmaa/KMCLib/releases/tag/v2.0-a1

### Use *KMCLib* in your own research
If you would be interested to use this code in your research and need any assistance, or if you are interested in collaborations, please don't hesitate to send me an e-mail: leetmaa@kth.se


Mikael Leetmaa, Mars 28 2016


