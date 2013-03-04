""" Module for easy KMC from python. """

from KMCLocalConfiguration import KMCLocalConfiguration
from KMCInteractions import KMCInteractions
from KMCConfiguration import KMCConfiguration
from KMCLattice import KMCLattice
from KMCLatticeModel import KMCLatticeModel
from KMCUnitCell import KMCUnitCell
from KMCControlParameters import KMCControlParameters
from KMCRateCalculatorPlugin import KMCRateCalculatorPlugin
from Utilities.SaveAndReadUtilities import KMCInteractionsFromScript
from Utilities.SaveAndReadUtilities import KMCConfigurationFromScript
from KMCProcess import KMCProcess

__all__ = ['KMCLocalConfiguration', 'KMCInteractions', 'KMCConfiguration',
           'KMCLattice', 'KMCLatticeModel', 'KMCUnitCell',
           'KMCControlParameters', 'KMCInteractionsFromScript',
           'KMCConfigurationFromScript', 'KMCRateCalculatorPlugin',
           'KMCProcess']

# Trick to intialize and finalize MPI only once.
from Backend import Backend

# Init MPI.
Backend.MPICommons.init()

# Make sure to finalize MPI on exit.
def killme():
    Backend.MPICommons.finalize()

import atexit
atexit.register(killme)

