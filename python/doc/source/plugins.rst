Plugin Interfaces
=======================
KMCLib can easily be extended through the use of plugins,
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
The KMCRateCalculatorPluin is used to customize the calculation of
rates in the KMC simulation. A user defined RateCalculator plugin must
inherit from the KMCRateCalculatorPlugin base class as in this example:

.. code-block:: python

    from KMCLib.KMCRateCalculatorPlugin import KMCRateCalculatorPlugin

    # Define the derived class.
    class CustomPlugin(KMCRateCalculatorPlugin):

        def rate(self, ... ):
            # Implement the custom rate routine.
            ...

        # Override the other base class API functions if needed.
        ...

The derived class is then given to the KMCInteractions
object to be used in the KMC simulation.

.. code-block:: python

    # Construct the interactions object.
    interactions = KMCInteractions( ... )

    # Set the custom rate calculator.
    interactions.setRateCalculator(rate_calculator=CustomPlugin)

    # Construct the KMCLatticeModel.
    model = KMCLatticeModel(interactions=interactions, ... )

When running a KMC simulation with a custom rate calculator defined,
its rate( ... ) function will be called each time the rate of an available
process is needed. The rate( ... ) function receives information about
the geometry and the types before and after the process, as well as
the unmodified rate constant associated with the process. It is then
up to the user to handle the geometric and types information to
compute a new rate based on the given information. The
rate( ... ) function should return a valid rate for the particular
process for the given geometry.

Note that the rate(...) function of the custom rate calculator plugin
could be called millions of times during a typical KMC
simulation production run. It is therefore highly recommendable that
code efficiency is kept in mind when implementing this type of custom
extension.

Interface
.................

.. autoclass:: KMCLib.KMCRateCalculatorPlugin
   :members: initialize, rate, cutoff
   :special-members:

