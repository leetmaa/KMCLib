# KMCLib - A Kinetic Monte Carlo Python/C++ library

## License and Copying
Developed by Mikael Leetmaa at the Royal Institute of Technology (KTH), Sweden.

Copyright (c)  2012-2013  Mikael Leetmaa See 'LICENSE.txt' for license details.

## KMCLib
KMCLib is a collection of Python front-end functions, classes and interfaces for performing efficient lattice KMC simulations, with all computationally heavy back-end functionality implemented in C++.

The KMC simulations can be performed either with pre-defined rates for all possible processes, or the rates can be updated on the fly with any external code interfaced with KMCLib via a well defined custom rate calculator plugin interface. Analysis of the simulation results are either performed on the fly via an analysis plugin interface, or after the simulation is finished by examining the produced trajectory file.

## Use KMCLib in your own research
A first official release is planned during the second half of 2013, so please note that some important functionality is still missing, and there might be severe bugs or issues with the program not yet solved (or even detected). Functionality will continuously be added and more analysis alternatives will be implemented as we start testing the code against more realistic research problems. I would therefore strongly advise against using this program for research purposes before we are finished with the scientific tests and evaluations.

If you would be interested to use this code in your own research, or if you are interested in collaborations, please don't hesitate to send me an e-mail: leetmaa@kth.se


Mikael Leetmaa, Stockholm June 10 2013

