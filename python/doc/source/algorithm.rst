Algorithm
=========
:py:mod:`KMCLib` implements the VSSM or *n-fold* way algorithm
[Ref.1]_ for performing *lattice* KMC simulations. The algorithm in
its most simple form starts with an initial configuration on a lattice
and a description of all possible elementary processes in the
system with their corresponding reaction rates.

* The processes are matched with the local geometry at each
  lattice site and for each process the list of sites where it is
  available (*i.e.* where it is possible to perform the process) is
  stored.

* The total possible rate for each process is calculated and a
  process to perform is chosen with probability proportional to its
  relative total rate.

* A site to perform the chosen process at is selected at random from
  the list of sites where the chosen process is available.

* Time is propagated by drawing the length of the next time step
  randomly from an exponentially
  decreasing time distribution with the exponent given by the total
  possible rate in the system.

We have also made modifications to the algorithm to include the
possibility to run simulations with custom rates that are
calculated on-the-fly during the simulation. This important feature
adds enormous flexibility compared to only using fixed rate
processes.

The two different ways to determine the process rates (at simulation start-up
or during the simulation) can easily be combined. Some
of the processes can get their rates updated during the simulation while
other rates are given from the start of the simulation.
The rate update framework is very flexible and allows
the user to define the desired behavior via
Python code. It is *e.g.* possible to couple the update of the process rates
to global or local geometrical parameters, such that a
temperature or pressure gradient can affect the local rates of
diffusion processes differently at different sites, or that the
process rates are being modified at sites in the
vicinity of certain atom types or geometrical features that may arise
during the simulation.

The :ref:`usage examples` demonstrate how to use the custom rate
modifications, and  the algorithm is described in full detail in
[Ref.8]_ where we also show timings and scaling behavior of
:py:mod:`KMCLib`. Remember that you can
*always* look at the source code directly to investigate the
implementation.
