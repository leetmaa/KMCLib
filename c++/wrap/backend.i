/*
  Copyright (c)  2012  Mikael Leetmaa

  This file is part of the KMCLib project distributed under the terms of the
  GNU General Public License version 3, see <http://www.gnu.org/licenses/>.
*/


%module(directors="1") Backend
%{
#include "../src/latticemodel.h"
#include "../src/configuration.h"
%}

%include "std_string.i"
%include "std_vector.i"

%template(StdVectorString) std::vector<std::string>;
%template(StdVectorDouble) std::vector<double>;
%template(StdVectorStdVectorDouble) std::vector<std::vector<double> >;

%include "../src/latticemodel.h"
%include "../src/configuration.h"


