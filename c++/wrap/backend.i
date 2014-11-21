/*
  Copyright (c)  2012-2013  Mikael Leetmaa

  This file is part of the KMCLib project distributed under the terms of the
  GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
*/

// Get rid of the warning messages from compiling the auto generated code.
#pragma SWIG nowarn=503
#pragma SWIG nowarn=509
#pragma SWIG nowarn=314
#pragma SWIG nowarn=389

// Define the content of our modeule.
%module(directors="1") Backend
%{
#include "latticemodel.h"
#include "latticemap.h"
#include "configuration.h"
#include "interactions.h"
#include "process.h"
#include "customrateprocess.h"
#include "coordinate.h"
#include "matchlistentry.h"
#include "typebucket.h"
#include "matchlist.h"
#include "simulationtimer.h"
#include "ratecalculator.h"
#include "mpicommons.h"
#include "ontheflymsd.h"
#include "random.h"
%}

// Use directors on the RateCalculator for using the python callback.
%feature("director") SimpleDummyBaseClass;
%feature("director") RateCalculator;

// Exception handling for overloaded RateCalculators in Python.
%feature("director:except") {
    if ($error != NULL) {
        throw Swig::DirectorMethodException();
    }
}
%exception {
    try { $action }
    catch (Swig::DirectorException &e) { SWIG_fail; }
}

// Include SWIG files for the std containers.
%include "std_vector.i"
%include "std_map.i"
%include "std_string.i"
%include "std_pair.i"

 // Define the templates to use in Python.
%template(StdVectorString) std::vector<std::string>;
%template(StdVectorStdVectorString) std::vector<std::vector<std::string> >;
%template(StdVectorDouble) std::vector<double>;
%template(StdVectorInt) std::vector<int>;
%template(StdVectorBool) std::vector<bool>;
%template(StdVectorProcess) std::vector<Process>;
%template(StdVectorProcessPtr) std::vector<Process*>;
%template(StdVectorCustomRateProcess) std::vector<CustomRateProcess>;
%template(StdVectorProcessBucketMatchListEntry) std::vector<ProcessBucketMatchListEntry>;
%template(StdVectorConfigBucketMatchListEntry) std::vector<ConfigBucketMatchListEntry>;
%template(StdVectorStdVectorInt) std::vector<std::vector<int> >;
%template(StdVectorStdVectorDouble) std::vector<std::vector<double> >;
%template(StdVectorCoordinate) std::vector<Coordinate>;
%template(StdVectorStdPairCoordinate) std::vector<std::pair<Coordinate, Coordinate> >;
%template(StdVectorStdVectorCoordinate) std::vector<std::vector<Coordinate> >;
%template(StdMapStringInt) std::map<std::string,int>;
%template(StdVectorStdMapStringInt) std::vector<std::map<std::string,int> >;
%template(StdVectorStdPairIntInt) std::vector<std::pair<int, int> >;
%template(StdVectorTypeBucket) std::vector<TypeBucket>;

// Include the definitions.
%include "latticemodel.h"
%include "latticemap.h"
%include "configuration.h"
%include "interactions.h"
%include "process.h"
%include "customrateprocess.h"
%include "coordinate.h"
%include "matchlist.h"
%include "typebucket.h"
%include "matchlistentry.h"
%include "simulationtimer.h"
%include "ratecalculator.h"
%include "mpicommons.h"
%include "ontheflymsd.h"
%include "random.h"


// This extends the Coordinate class with python indexing support.
%extend Coordinate
{
    double __getitem__(unsigned int i)
    {
        // Check bounds.
        if (i > 2)
        {
            throw std::out_of_range("Coordinate index out of range.");
        }
        return (*self)[i];
    };

    void __setitem__(unsigned int i, const double value)
    {
        // Check bounds.
        if (i > 2)
        {
            throw std::out_of_range("Coordinate index out of range.");
        }
        (*self)[i] = value;
    };
};


// This extends the TypeBucket class with python indexing support.
%extend TypeBucket
{
    int __getitem__(unsigned int i)
    {
        // Check bounds.
        if (static_cast<int>(i) > (*self).size() - 1)
        {
            throw std::out_of_range("TypeBucket index out of range.");
        }
        return (*self)[i];
    };

    void __setitem__(unsigned int i, const int value)
    {
        // Check bounds.
        if (static_cast<int>(i) > (*self).size() - 1)
        {
            throw std::out_of_range("TypeBucket index out of range.");
        }
        (*self)[i] = value;
    };

    int __len__()
    {
        return (*self).size();
    }
};
