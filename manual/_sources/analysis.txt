.. _`analysis`:


.. currentmodule:: KMCLib

Analysis
=========

All built in analysis in :py:mod:`KMCLib` is implemented using the
:py:class:`KMCAnalysisPlugin` interface and is performed on-the-fly
during the simulation. At present there are two analysis options
included, the :py:class:`OnTheFlyMSD` for mean square displacement
analysis and the :py:class:`TimeStepDistribution` analysis for
recoding the distribution of time steps during the simulation. On-the-fly
analysis objects are used by giving them as as a arguments to the
:py:meth:`run<KMCLatticeModel.run>` function of the
:py:class:`KMCLatticeModel`. Have a look at the :ref:`usage examples`
for details.

How often analysis is performed during the simulation is controlled
by the :py:obj:`analysis_interval` parameter of the
:py:class:`KMCControlParameters`. For future releases we will consider
to let each analysis object
itself decide how often it should be used, but for the moment all analysis
is performed equally often.


On-the-fly mean square displacement (MSD)
------------------------------------------

Usage
......
The :py:class:`OnTheFlyMSD` analysis is used by giving it as an
argument to the :py:meth:`run<KMCLatticeModel.run>` function of the
:py:class:`KMCLatticeModel`. Have a look at the :ref:`usage examples`
for details.

Interface
............

.. autoclass:: KMCLib.OnTheFlyMSD
   :members:
   :special-members:

Notes
......
The :py:class:`OnTheFlyMSD` analysis implements the method described
in [Ref.7]_. It produces the MSD curves with correct error estimates
for Brownian motion but can overestimate the errors for confined
motion or diffusion with flow. See [Ref.7]_ for details.

You have to give the element type to track as argument to the
constructor. If you want to calculate the MSD of several different
types simultaneously you can instantiate and use several different
:py:class:`OnTheFlyMSD` objects at the same time.

Note that to be meaningful this analysis requires that move vectors
are given to the processes so that individual atom movements can be traced.
See the :py:class:`KMCProcess` documentation for details.

Future versions might include a way to automatically obtain a
reasonable starting guess for the parameters. For the moment, however,
you will have to find a good set of parameters yourself. The method
can be sensitive and you should make sure you run with a reasonable
bin-size, max time and number of history steps. You should be careful
and plot the resulting MSD curve after a short simulation to make
sure your parameters are reasonable before you run and longer production
simulations to estimate the diffusion constants.

One way to estimate reasonable parameters is to look at the
:py:class:`TimeStepDistribution` of a short simulation with the same
system and chose a :py:obj:`bin_size` roughly equal to the mean of the
time step distribution, and use as many bins as you have history steps.


Time-step distribution
-----------------------

Usage
.........
The :py:class:`TimeStepDistribution` analysis is used by giving it as an
argument to the :py:meth:`run<KMCLatticeModel.run>` function of the
:py:class:`KMCLatticeModel` in the same way as the
:py:class:`OnTheFlyMSD` analysis. You can have a look at the
:ref:`usage examples` for details.


Interface
............

.. autoclass:: KMCLib.TimeStepDistribution
   :members:
   :special-members:

Notes
......
The :py:class:`TimeStepDistribution` analysis is very simple and could
serve as a good example if you want to implement your own
analysis. Have a look at the source code.


Your contributions
-------------------
I have implemented the type of analysis I needed for the specific
projects I was working on. There is often no point in guessing what
people need. Better to wait for your input. If you have suggestions of
what type of analysis would be useful please let me
know.

With a little bit of Python knowledge implementing your own
analysis module is simple. Follow the example of the
:py:class:`TimeStepDistribution` and have a look at the
:py:class:`KMCAnalysisPlugin` documentation. If you have analysis
contributions you wish to share, or perhaps even include in the next official
release don't hesitate to let me know.






