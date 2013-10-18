// Copyright (C) 2013 Romain Francois
//
// This file is part of Rcpp11.
//
// Rcpp11 is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 2 of the License, or
// (at your option) any later version.
//
// Rcpp11 is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Rcpp11.  If not, see <http://www.gnu.org/licenses/>.

#ifndef Rcpp__vector__eval_methods_h
#define Rcpp__vector__eval_methods_h

namespace Rcpp{
namespace internal{

    template <int RTYPE, template <class> class StoragePolicy> 
    class eval_methods {} ;
    
    template <template <class> class StoragePolicy> 
    class eval_methods<EXPRSXP, StoragePolicy> {
    public:
        SEXP eval() ;
        SEXP eval(SEXP) ;
    } ;
      
}
}
#endif