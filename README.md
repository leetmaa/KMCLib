# KMCLib - a general framework for lattice kinetic Monte Carlo (KMC) simulations

## License and Copying
Developed by Mikael Leetmaa at the Royal Institute of Technology (KTH), Sweden.

Copyright (c)  2012-2013  Mikael Leetmaa. Released under the GPLv3 license, see 'LICENSE.txt' for details.

## KMCLib
KMCLib is a collection of Python front-end functions, classes and interfaces for performing efficient lattice KMC simulations, with all computationally heavy back-end functionality implemented in C++.

The KMC simulations can be performed either with pre-defined rates for all possible processes, or the rates can be updated on the fly with any external code interfaced with KMCLib via a well defined custom rate calculator plugin interface. Analysis of the simulation results are either performed on the fly via an analysis plugin interface, or after the simulation is finished by examining the produced trajectory file.

## Use KMCLib in your own research
KMCLib is now in version 1.0 BETA-2. All features for version 1.0 are implemented and all to date known issues are resolved. We will keep updating with more usage examples and a more complete manual before the final release of version 1.0. If you happen to find any bugs or issues, or if you have suggestions for improvements, please let me know.

If you would be interested to use this code in your research, or if you are interested in collaborations, please don't hesitate to send me an e-mail: leetmaa@kth.se


Mikael Leetmaa, December 27 2013

