# Import all from KMCLib
from KMCLib import *

# Setup a valid KMCUnitCell.
unit_cell = KMCUnitCell(cell_vectors=[[2.8,0.0,0.0],
                                      [0.0,3.2,0.0],
                                      [0.0,0.5,3.0]],
                        basis_points=[[0.0,0.0,0.0],
                                      [0.5,0.5,0.5],
                                      [0.25,0.25,0.75]])

# Setup the lattice.
lattice = KMCLattice(unit_cell=unit_cell,
                     repetitions=(4,4,1),
                     periodic=(True,True,False))

types = ['a','a','a','a','b','b',
         'a','a','a','b','b','b',
         'b','b','a','a','b','a',
         'b','b','b','a','b','a',
         'b','a','a','a','b','b',
         'b','b','b','b','b','b',
         'a','a','a','a','b','b',
         'b','b','a','b','b','a']

# Setup the configuration.
config = KMCConfiguration(lattice=lattice,
                          types=types,
                          possible_types=['a','c','b'])

