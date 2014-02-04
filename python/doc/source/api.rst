.. currentmodule:: KMCLib


KMCLib API
=======================
Under the category "KMCLib API" goes all components you may need
when performing KMC simulations with KMCLib, that are not part of the plugins.

These are the objects you get when importing with::

 from KMCLib import *

It defineds the KMCLib API for setting up and running a KMC
simulation. It is our intention that these interfaces will remain
constant over time once we have a first official release out.


KMCConfiguration
---------------------

Usage
......
A :py:class:`KMCConfiguration` consists of a :py:class:`KMCLattice`
and the element type information in each lattice site.
A :py:class:`KMCConfiguration` is given to the
:py:class:`KMCLatticeModel` to represent the initial geometry of the
simulation.

Interface
..........

.. autoclass:: KMCLib.KMCConfiguration
   :members:
   :special-members:
   :private-members:
   :exclude-members: _KMCConfiguration__checkAndSetTypes, _backend, _latticeMap

Notes
......
For a lattice KMC simulation the lattice positions do not change
during the simulation, but if *move vectors* are specified in the
processes (see the :py:class:`KMCProcess` documentation for details),
the configuration keeps track of all individual atoms occupying the
lattice sites. The :py:class:`KMCConfiguration` has internally a dual
representation of the geometry, keeping track of the type at each
lattice site, as well as the coordinates for each atom-ID. The
atom-IDs are assigned at the start of the simulation, or earlier
when otherwise asked for via any of the query functions involving
atom-IDs. The initial assignment of the atom-IDs follows the order of
the given initial types.

The :py:class:`KMCConfiguration` comes with the ability to script
itself using the :py:meth:`_script<KMCConfiguration._script>` method,
meaning that it can produce a string that, if interpreted
as a Python script, generates a copy of the configuration. This can
be a convenient way to store a configuration for later use, especially
if the configuration was first generated using a more complex Python script.

As an extra bonus :py:class:`KMCConfiguration` can also
generate a corresponding ATK script for easy visualization with
VNL from `QuantumWise`_. Since the type of configuration used in VNL must have an element
from the periodic system for each coordinate the
:py:meth:`_atkScript<KMCConfiguration._atkScript>` method
takes a dict as argument that translates the types to periodic table
elements. Any type present in the :py:class:`KMCConfiguration` which
is not in the dict will be ignored when generating the ATK script.

If you have suggestions for other scripts or file formats that you
think would be convenient if the :py:class:`KMCConfiguration` could
generate you can easily extend the class with another method. You just
follow the examples of the
:py:meth:`_script<KMCConfiguration._script>`
and
:py:meth:`_atkScript<KMCConfiguration._atkScript>` methods. We would
of course be happy to see contributions of this kind being submitted
back to us for inclusion in future releases.


KMCInteractions
---------------------

Usage
......
A :py:class:`KMCInteractions` object is given as one of the arguments
when constructing the :py:class:`KMCLatticeModel`, and contains information about *all*
interactions in the simulated system.
The :py:class:`KMCInteractions`
is constructed with a list of *all* possible elementary processes
in the simulated system as a list of :py:class:`KMCProcess` objects,
and can also be equipped with a
:py:class:`KMCRateCalculatorPlugin` by calling its
:py:meth:`setRateCalculator<KMCInteractions.setRateCalculator>` method.
An added custom rate calculator will have the ability to dynamically update the rate
constants associated with the elementary processes at different
lattice sites during the simulation.


Interface
..........

.. autoclass:: KMCLib.KMCInteractions
   :members:
   :special-members:

Notes
......
The :py:obj:`implicit_wildcards` parameter to the constructor specifies how the
local geometries of the :py:class:`KMCProcess` input should be
interpreted, and will have a crucial impact on the outcome of a simulation.
If ``implicit_wildcards=True`` (default
behavior), then the geometries of all processes will be compared to
the geometry of the lattice at the type of basis site where the
process is applicable as specified on construction of each
:py:class:`KMCProcess` object. If there are lattice sites within
the cutoff of the process that are not specified in the process
geometry these sites will be added to the process with a wild-card
marker as type, meaning that any type on the corresponding lattice
position will be regarded as a match when the process is compared to a
lattice site. If ``implicit_wildcards=False`` only wild-cards
explicitly specified in each :py:class:`KMCProcess` will be
considered. See the :py:class:`KMCProcess` documentation for further details.

KMCProcess
------------------

Usage
......
A :py:class:`KMCProcess` represents an elementary process in the KMC
simulation. Every possible way to move particles in the simulation
must be represented with :py:class:`KMCProcess` objects.
A list of :py:class:`KMCProcess` objects containing all this
information must be specified to construct the
the :py:class:`KMCInteractions` object.


Interface
..........

.. autoclass:: KMCLib.KMCProcess
   :members:
   :special-members:


Notes
......
A process can be specified with or without *move vector* input. The
simplest form of the :py:class:`KMCProcess` uses no *move vectors*. In
this case the :py:class:`KMCProcess` is constructed with the local
geometry around the type of lattice site it should be applied to, as well
as with the corresponding list of types at the lattice sites before
and after the process is applied. When the process is applied at a
lattice site the type information at the involved sites is
updated. With no *move vectors* given, the types information is updated
without keeping track of which specific atom went to which new site.
Tracking of individual atoms is some times necessary, e.g. when
calculating the mean square displacement of atoms with the built in
:py:class:`OnTheFlyMSD` analysis.
To keep track of the positions of the individual atoms (or particles)
in a simulation *move vectors* must be specified for all relevant
processes.

The :py:class:`KMCProcess` can be specified with the use of
wild-cards for matching any type at a certain lattice site. If a
wild-card is explicitly specified (using the '*' symbol as type)
it must stay in the same position in the :py:obj:`elements_before` and
:py:obj:`elements_after` lists.
Wild-cards can also be used by simply omitting the corresponding
positions in the :py:obj:`coordinates` and :py:obj:`elements` lists
and instead use the ``implicit_wildcards=True`` option when
constructing the :py:class:`KMCInteractions`. A restriction applies
that implicit wild-cards can only be used for processes that are
applicable at one type of basis site only (i.e. with ``len(basis_sites)=1``).


KMCLocalConfiguration
---------------------

Usage
......

The :py:class:`KMCLocalConfiguration` is used to internally represent local geometry
around a lattice site in the :py:class:`KMCProcess` object.


Interface
..........

.. autoclass:: KMCLib.KMCLocalConfiguration
   :members:
   :special-members:


KMCUnitCell
---------------------

Usage
......
The :py:class:`KMCUnitCell` is given as an argument when
constructing a :py:class:`KMCLattice` to use with a
:py:class:`KMCConfiguration`. The :py:class:`KMCUnitCell`
is specified both to obtain information on the basis sites to use
in the repeated primitive cell when constructing the lattice,
and also to obtain a mapping from Cartesian space to the internal
geometry based on the lattice vectors of the primitive cell. This
mapping has currently only one usage in KMCLib, when using
the :py:class:`OnTheFlyMSD` analysis where a transformation to
Cartesian coordinates is performed using the information provided
from the :py:class:`KMCUnitCell` class.


Interface
..........

.. autoclass:: KMCLib.KMCUnitCell
   :members:
   :special-members:


KMCLattice
---------------------

Usage
......

The :py:class:`KMCLattice` is used to define the topology of a lattice
KMC simulation. It consists of a :py:class:`KMCUnitCell` and information
about repetitions of this cell and the periodicity of the lattice.
The :py:class:`KMCLattice` is used for input when constructing
the :py:class:`KMCConfiguration`.

Interface
..........

.. autoclass:: KMCLib.KMCLattice
   :members:
   :special-members:


KMCLatticeModel
---------------------

Usage
......
The :py:class:`KMCLatticeModel` is the central object in
:py:mod:`KMCLib` to perform a lattice KMC simulation. The model is
constructed with a :py:class:`KMCConfiguration` and
:py:class:`KMCInteractions`, and is run by calling its
:py:meth:`run<KMCLatticeModel.run>` method with parameters given in a
:py:class:`KMCControlParameters` object.

Interface
..........

.. autoclass:: KMCLib.KMCLatticeModel
   :members:
   :special-members:

Notes
......
When running the model the :py:class:`KMCControlParameters` specify
how often the trajectory will be saved and how often information
should be printed to standard output. This is controlled with the
:py:obj:`dump_intervall` parameter in the
:py:class:`KMCControlParameters` class. If no trajectory is needed
the :py:obj:`trajectory_filename` argument can be left
empty. This will produce a warning message that no trajectory file is
specified but the simulation will run correctly only without storing
trajectory information.
If no trajectory information is needed it can still be useful to store
the last configuration to make a restart point to continue the
simulation from if needed. This can be achieved by specifying a
trajectory file name and setting the :py:obj:`dump_intervall` equal to
the :py:obj:`number_of_steps` in the :py:class:`KMCControlParameters`.
A potential drawback using such approach is that no information will
be printed to standard output during the simulation.

Two different types of trajectory formats are currently implemented.
``trajectory_type='lattice'`` gives a lattice trajectory,
where the lattice positions is specified only once, and at each step
only the type at each lattice site is stored. Using ``trajectory_type='xyz'``
instead causes the trajectory to be stored with the coordinates and
type for each atom-ID at each saved step.
This type of trajectory is *only* meaningful if the
processes specify *move_vectors* for how the individual atoms move
during the simulation.

Much flexibility and extendability is added to :py:mod:`KMCLib`
with the possibility to specify custom on-the-fly analysis objects
that can be used in the simulation. These objects must inherit from
the :py:class:`KMCAnalysisPlugin` interface and are given to the
:py:meth:`run<KMCLatticeModel.run>` method in a list using the
``analysis=[...]`` parameter. How often analysis is performed is
controlled using the :py:obj:`analysis_intervall` parameter in the
:py:class:`KMCControlParameters` class.


KMCControlParameters
---------------------

Usage
......
:py:class:`KMCControlParameters` is a container for parameters such as
the number of steps and the seed to the random number generator,
needed to run a :py:class:`KMCLatticeModel`.

Interface
..........

.. autoclass:: KMCLib.KMCControlParameters
   :members:
   :special-members:



.. _QuantumWise: http://www.quantumwise.com
