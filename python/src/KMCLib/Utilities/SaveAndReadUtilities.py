""" Module for holding common save and read utility functions. """


# Copyright (c)  2012  Mikael Leetmaa
#
# This file is part of the KMCLib project distributed under the terms of the
# GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
#

from KMCLib.Exceptions.Error import Error
from KMCLib.CoreComponents.KMCConfiguration import KMCConfiguration
from KMCLib.CoreComponents.KMCInteractions import KMCInteractions
from KMCLib.CoreComponents.KMCLatticeModel import KMCLatticeModel

def KMCConfigurationFromScript(script_file_path):
    """
    Utility function to read a KMCConfiguration from a script file.

    :param script_file_path: The script file path to read.
    :type script_file_path: str

    :return: The KMCConfiguration saved in the file.
    """
    # Get the stored configurations.
    return getScriptComponent(script_file_path, KMCConfiguration)


def KMCInteractionsFromScript(script_file_path):
    """
    Utility function to read a KMCInteractions from a script file.

    :param script_file_path: The script file path to read.
    :type script_file_path: str

    :return: The KMCInteractions saved in the file.
    """
    # Get the stored configurations.
    return getScriptComponent(script_file_path, KMCInteractions)


def KMCLatticeModelFromScript(script_file_path):
    """
    Utility function to read a KMCLatticeModel from a script file.

    :param script_file_path: The script file path to read.
    :type script_file_path: str

    :return: The KMCLatticeModel saved in the file.
    """
    # Get the stored configurations.
    return getScriptComponent(script_file_path, KMCLatticeModel)


def getScriptComponent(script_file_path, component_type):
    """
    Utility function to get a component of a certain type from a
    script file.

    :param script_file_path: The script file path to read.
    :type script_file_path: str

    :param component_type: The class type of the component to extract.
    :type component_type: class

    :return: The first component found that matches the type.
    """
    global_dict = {}
    local_dict  = {}
    try:
        execfile(script_file_path, global_dict, local_dict)
    except IOError as e:
        raise Error("The script file '%s' failed to load or does not exist."%(script_file_path))

    # Search through the local dict and return when found.
    for key in local_dict.keys():
        test_obj = local_dict[key]
        if isinstance(test_obj, component_type):
            return test_obj

    # Nothing was found, return None.
    return None
