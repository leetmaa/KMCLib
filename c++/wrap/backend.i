/*
  Copyright (c)  2012  Mikael Leetmaa

  This file is part of the KMCLib project distributed under the terms of the
  GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
*/


%module(directors="1") Backend
%{
#include "latticemodel.h"
#include "latticemap.h"
#include "configuration.h"
#include "interactions.h"
#include "process.h"
#include "coordinate.h"
#include "matchlistentry.h"
#include "simulationtimer.h"
#include "ratecalculator.h"
%}

%feature("director") SimpleDummyBaseClass;
%feature("director") RateCalculator;

#pragma SWIG nowarn=503
#pragma SWIG nowarn=509
#pragma SWIG nowarn=314
#pragma SWIG nowarn=389

%include "std_string.i"
%include "std_vector.i"
%include "std_map.i"

%template(StdVectorString) std::vector<std::string>;
%template(StdVectorDouble) std::vector<double>;
%template(StdVectorInt) std::vector<int>;
%template(StdVectorBool) std::vector<bool>;
%template(StdVectorProcess) std::vector<Process>;
%template(StdVectorCoordinate) std::vector<Coordinate>;
%template(StdVectorMinimalMatchListEntry) std::vector<MinimalMatchListEntry>;
%template(StdVectorStdVectorDouble) std::vector<std::vector<double> >;
%template(StdMapStringInt) std::map<std::string,int>;

%include "latticemodel.h"
%include "latticemap.h"
%include "configuration.h"
%include "interactions.h"
%include "process.h"
%include "coordinate.h"
%include "matchlistentry.h"
%include "simulationtimer.h"
%include "ratecalculator.h"


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

