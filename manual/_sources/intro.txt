.. currentmodule:: KMCLib

Introduction
=============

Welcome to the :py:mod:`KMCLib` documentation!

:py:mod:`KMCLib` is a
Python/C++ implementation of the *lattice* KMC method distributed
under the GPLv3 license. See the :ref:`about` section for details.

You can always find the latest stable version of the program
here: https://github.com/leetmaa/KMCLib/releases

If you use KMCLib in your research please **cite** the
scientific paper describing the program [Ref.8]_.

This documentation describes version 1.1


Why KMC?
---------------------------
KMC is a well known method for accelerated dynamics simulations of
atomistic systems. The method builds on the
separation of timescales between the fast vibrational motion on the
one hand and the slow events of interest on the other. Only the slow
events are simulated while all other motion in the system is regarded
as equilibrated when each slow (and therefore rare) event takes
place. Accurate calculations of energies for the elementary processes
are typically combined with transition state theory estimates of the
corresponding rates to build the input to the KMC simulation: an
initial configuration and a list of elementary processes that governs
the dynamics of the system.
Since no motion that takes place on a short time scale
is explicitly treated KMC simulations can often reach long time scales
that are difficult to access with other methods.

The *lattice* KMC method implemented in :py:mod:`KMCLib` is a
particularly efficient form of KMC suitable for
simulating systems where the configuration can be expressed on a
lattice in space, and where all process in the system take place on this
lattice by changing the types associated with the lattice sites.
Systems suitable for *lattice* KMC simulations are typically ordered
materials such as the bulk of (defect) crystalline solids and their surfaces.
:py:mod:`KMCLib` implements the *lattice* KMC method.

Several good description of the KMC method exists in the literature.
*Arthur F. Voter* has written an introductory text to KMC
simulations [Ref.1]_ that I find particularly appealing.
There is also a vast bulk of research
literature on the subject, from the first publications of the
method [Ref.2]_, [Ref.3]_, to several more recent reviews
(e.g. [Ref.4]_, [Ref.5]_, [Ref.6]_, [Ref.9]_), as well
as many research papers using the KMC method.

Why KMCLib?
---------------------------

Several implementations of the KMC method exist already.
So why would you need another one?

Searching the `webofknowledge`_ publication database for papers using
KMC you find that for the last couple of years several hundred
papers using KMC were published each year. But only a small
fraction of these publications used any of the publicly available
KMC codes. Clearly the available codes do not meet everyone's needs.
A plausible explanation is that the KMC algorithm is simple enough so
that anyone with some programming experience can write her own
code. Combine this with the need for custom solutions for many
systems you end up in a situation where most research groups have
their own implementation of the method. This situation is not ideal.
Since the majority of these codes never makes it to the public much
work is wasted re-inventing the wheel rather than focusing on the
models design.

The :py:mod:`KMCLib` project started from the need of a flexible KMC
code  that did not require re-compilation to setup or modify a KMC model.
We also needed the ability to study diffusion in complex bulk systems
that required more flexibility than what could easily be obtained using
the common approach of pre-defining all possible processes before
the start of the simulation. For complex systems with many possible
interactions this list easily grows out of hand. With the introduction
of the custom rate calculator interface
(:py:class:`KMCRateCalculatorPlugin`) we were able to combine
well-defined processes on a lattice with rates that depend in a
complex way on the surroundings.

KMCLib can be used for studying reactions and diffusion of any surface
or bulk system that can be expressed on a lattice in space, but is
particularly well equipped for

* **Bulk diffusion studies**, that are made easy through the built in mean square
  displacement (MSD) analysis and the ability to track individual
  atoms during the simulation.


* **Extensions to the conventional KMC algorithm**, where use of the
  custom rate calculator can solve the complexity problem where the
  number of elementary processes grows large for complex systems
  with long range interactions.


* *ab initio* KMC schemes with many complex interactions in dense bulk or
  complex surface systems, where the :py:mod:`KMCLib` **parallelization**
  scheme over process-site pairs to match is most efficient.

  '

Other KMC codes
................
If you don't find :py:mod:`KMCLib` suitable for your needs I strongly
encourage you to consider any of the other available KMC codes before
you decide to implement the method yourself.

For those who like a GUI to work with, and who don't mind
using a fortran compiler for setting up and running models,
there is the `kmos`_ code.

The `SPPARKS`_ code requires you to write some C++ code to implement your model
but provides a good alternative for massively parallel KMC simulations.

A list of other available codes is provided by the kmos authors `here`_.

.. _webofknowledge: http://www.webofknowledge.com/
.. _here: https://kmos.readthedocs.org/en/latest/troubleshooting/faq.html
.. _kmos: http://mhoffman.github.io/kmos/
.. _SPPARKS: http://spparks.sandia.gov/




