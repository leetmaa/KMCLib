.. currentmodule:: KMCLib


Plugin Interfaces
=======================
:py:mod:`KMCLib` can easily be extended through the use of plugins,
where the user can write extension to the code and get them easily
integrated with the core functionality simply by deriving from a
specified plugin base class and overloading certain functions
on the base class.

The plugins will always have a Python interface class to inherit from,
also in the case when the actual code will be called from within the
C++ core routines. This allows the user to write plugin extensions in Python
without having to recompile the underlying C++ code.

Calls from C++ to Python are managed through SWIG Director
classes. These calls inevitably come with an overhead compared to
calling compiled code within C++ (see the SWIG `documentation
<http://www.swig.org>`_ for details).
The overhead should fortunately in most
realistic cases be negligible. On the other hand what happens inside
the custom Python functions could very well be time consuming enough to
require special attention and implementation in a compiled
language. Such techniques will however not be covered here.


KMCRateCalculatorPlugin
-------------------------
The :py:class:`KMCRateCalculatorPluin` is used to customize the calculation of
rates in the KMC simulation. A user defined :py:class:`RateCalculator`
plugin must
inherit from the :py:class:`KMCRateCalculatorPlugin` base class as in this example:

.. code-block:: python

    from KMCLib.KMCRateCalculatorPlugin import KMCRateCalculatorPlugin

    # Define the derived class.
    class CustomPlugin(KMCRateCalculatorPlugin):

        def rate(self, ... ):
            # Implement the custom rate routine.
            ...

        # Override the other base class API functions if needed.
        ...

The derived class is then given to the :py:class:`KMCInteractions`
object to be used in the KMC simulation.

.. code-block:: python

    # Construct the interactions object.
    interactions = KMCInteractions( ... )

    # Set the custom rate calculator.
    interactions.setRateCalculator(rate_calculator=CustomPlugin)

    # Construct the KMCLatticeModel.
    model = KMCLatticeModel(interactions=interactions, ... )

When running a KMC simulation with a custom rate calculator defined,
its :py:meth:`rate<KMCRateCalculatorPlugin.rate>` method will be called each time the rate of an available
process is needed. The :py:meth:`rate<KMCRateCalculatorPlugin.rate>` method receives information about
the geometry and the types before and after the process, as well as
the unmodified rate constant associated with the process. It is then
up to the user to handle the geometric and types information to
compute a new rate based on the given information. The
:py:meth:`rate<KMCRateCalculatorPlugin.rate>` method should return a valid rate for the particular
process for the given geometry.

Note that the :py:meth:`rate<KMCRateCalculatorPlugin.rate>` method of the custom rate calculator plugin
could be called millions of times during a typical KMC
simulation production run. It is therefore highly recommendable that
code efficiency is kept in mind when implementing this type of custom
extension.

Interface
.................

.. autoclass:: KMCLib.KMCRateCalculatorPlugin
   :members: initialize, rate, cutoff
   :special-members:



KMCAnalysisPlugin
-------------------------
The KMCAnalysisPlugin is used to perform on-the-fly custom analysis
during the KMC simulation. A list of instantiated KMCAnalysisPlugin
objects can be given to the :py:meth:`run<KMCLatticeModel.run>` function of the :py:class:`KMCLatticeModel`.
Each user-defined custom analysis object must inherit from the
:py:class:`KMCAnalysisPlugin` base class.
Three functions are available to overload for setting up, handling
updates after a step and finalizing the analysis.
Here is an example:

.. code-block:: python

    from KMCLib.KMCAnalysisPlugin import KMCAnalysisPlugin

    # Define the custom analysis class.
    class CustomAnalysis(KMCAnalysisPlugin)
        """ Custom analysis class """
        def __init__(self, ... ):
            # Initialize the object.
            ...

        # Overload the interface functions.
        def setup(self, step, time, configuration):
            ...

        def registerStep(self, step, time, configuration):
            ...

        def finalize(self):
            ...

    # Instantiate the custom analysis object.
    my_analysis1 = CustomAnalysis(...)
    my_analysis2 = CustomAnalysis(...)
    ...

    # Setup the list of analysis objects.
    analysis = [my_analysis1, my_analysis2, ... ]

    # Setup the kmc lattice model to run (see documentation for parameters)
    kmc_model = KMCLatticeMode(...)

    # Start the simulation with custom on-the-fly analysis every 10:th step.
    kmc_mode.run( ...
                  control_parameters=KMCControlParameters( ...
                                                           analysis_interval=10)
                  analysis=analysis)

Note that how often the :py:meth:`registerStep<KMCAnalysisPlugin.registerStep>`
function is called during the simulation is controlled via the
:py:obj:`analysis_interval` parameter given to the :py:class:`KMCControlParameters`.


Interface
.................

.. autoclass:: KMCLib.KMCAnalysisPlugin
   :members: setup, registerStep, finalize, __init__

