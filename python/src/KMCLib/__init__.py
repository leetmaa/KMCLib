""" Module for easy KMC from python. """

from CoreComponents.KMCLocalConfiguration import KMCLocalConfiguration
from CoreComponents.KMCInteractions import KMCInteractions
from CoreComponents.KMCProcess import KMCProcess
from CoreComponents.KMCConfiguration import KMCConfiguration
from CoreComponents.KMCLattice import KMCLattice
from CoreComponents.KMCLatticeModel import KMCLatticeModel
from CoreComponents.KMCUnitCell import KMCUnitCell
from CoreComponents.KMCControlParameters import KMCControlParameters
from Analysis.OnTheFlyMSD import OnTheFlyMSD
from Analysis.TimeStepDistribution import TimeStepDistribution
from Utilities.SaveAndReadUtilities import KMCInteractionsFromScript
from Utilities.SaveAndReadUtilities import KMCConfigurationFromScript
from PluginInterfaces.KMCRateCalculatorPlugin import KMCRateCalculatorPlugin
from PluginInterfaces.KMCAnalysisPlugin import KMCAnalysisPlugin
from Backend.Backend import MPICommons

__all__ = ['KMCLocalConfiguration', 'KMCInteractions', 'KMCConfiguration',
           'KMCLattice', 'KMCLatticeModel', 'KMCUnitCell',
           'KMCControlParameters', 'KMCInteractionsFromScript',
           'KMCConfigurationFromScript', 'KMCRateCalculatorPlugin',
           'KMCAnalysisPlugin', 'KMCProcess', 'OnTheFlyMSD',
           'TimeStepDistribution', 'MPICommons']

# Print the header when the module is loaded.
Utilities.PrintUtilities.printHeader()

# Trick to intialize and finalize MPI only once.
MPICommons.init()

# Make sure to finalize MPI on exit.
def killme():
    MPICommons.finalize()

import atexit
atexit.register(killme)

