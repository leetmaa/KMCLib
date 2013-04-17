External Dependencies
======================
This folder holds all external libraries needed for the c++ part of KMCLib
that are shipped with the distribution.

mersenne-twister
-----------------
The "mersenne-twister" implementation we use here is written by
Christian Stigen Larsen < csl@sublevel3.org >
and can be found at
< https://github.com/cslarsen/mersenne-twister >

The patches we have applied to make this mersenne-twister implementation a
(for our purposes) good enough replacement for the c++11 construction
   std::mt19937 rng;
   std::generate_canonical<double, 32>(rng);
can be found in the Makefile.

License
========
Each external library includes its own license information.
Please see those files for details.

