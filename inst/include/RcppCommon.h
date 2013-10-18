//
// RcppCommon.h:  common include and defines statements
//
// Copyright (C) 2008 - 2009 Dirk Eddelbuettel
// Copyright (C) 2009 - 2013 Dirk Eddelbuettel and Romain Francois
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

#ifndef RcppCommon_h
#define RcppCommon_h

// #define RCPP_DEBUG_LEVEL 1

#include <Rcpp/platform/compiler.h>
#include <Rcpp/macros/macros.h>

// include R headers, but set R_NO_REMAP and access everything via Rf_ prefixes
#define R_NO_REMAP
#include <R.h>
#include <Rinternals.h>
#include <R_ext/Complex.h>
#include <R_ext/Parse.h>
#include <R_ext/Rdynload.h>
#include <R_ext/Callbacks.h>
#include <Rversion.h>
#include <Rcpp/Scoped.h>

#define RCPP_SET_VECTOR_ELT SET_VECTOR_ELT 
// #define RCPP_SET_VECTOR_ELT(v,i,x) { Rprintf("SET_VECTOR_ELT(%p, %d, %p). file = %s, line = %d\n", (SEXP)v, i, (SEXP)x, __FILE__, __LINE__) ; SET_VECTOR_ELT( (SEXP)v,i,(SEXP)x) ; } 

#include <cstdint>
#include <array>
#include <type_traits>
#include <iterator>
#include <exception>
#include <iostream>
#include <sstream>
#include <string>
#include <list>
#include <forward_list>
#include <stack>
#include <queue>

#include <map>
#include <set>
#include <stdexcept>
#include <vector>
#include <deque>
#include <functional>
#include <numeric>
#include <algorithm>
#include <complex>
#include <limits>
#include <typeinfo>
#include <tuple>
#include <utility>

#include <Rcpp/routines.h>
#include <Rcpp/complex.h>
#include <Rcpp/barrier.h>

#include <Rcpp/exceptions.h>
#include <Rcpp/Demangler.h>

namespace Rcpp{
    
    inline const char* short_file_name(const char* file) {
        std::string f(file) ;
        return f.substr( f.find_last_of("/") + 1 ).c_str() ;
    }

    class String ;
    template <typename CLASS> class RObjectStorage ;
    template <typename CLASS> class NoProtectStorage ;
    
    template <int RTYPE, template <class> class StoragePolicy > 
    class Vector_Impl ;
    
    template <int RTYPE>
    using Vector = Vector_Impl<RTYPE, RObjectStorage> ;
    
    template <int RTYPE> class Matrix ;
    using CharacterVector = Vector<STRSXP> ;
    using List = Vector<VECSXP> ; 
    using ExpressionVector = Vector<EXPRSXP> ; 
    
    RCPP_API_CLASS_DECL(RObject) 
    RCPP_API_CLASS_DECL(Function) 
    RCPP_API_CLASS_DECL(Language) 
    RCPP_API_CLASS_DECL(Pairlist) ;
    RCPP_API_CLASS_DECL(Environment) ;
    RCPP_API_CLASS_DECL(Promise) ;
    RCPP_API_CLASS_DECL(WeakReference) ;
    RCPP_API_CLASS_DECL(S4) ;
    RCPP_API_CLASS_DECL(Formula) ;
    RCPP_API_CLASS_DECL(Reference) ;
    
    template < template <class> class StoragePolicy > class Symbol_Impl ;
    using Symbol = Symbol_Impl<NoProtectStorage> ;
    
    template < template <class> class StoragePolicy > class DataFrame_Impl ;
    using DataFrame = DataFrame_Impl<RObjectStorage> ;
    
}	
namespace Rcpp{
    inline SEXP Rcpp_PreserveObject(SEXP x){ 
        if( x != R_NilValue ) {
            R_PreserveObject(x); 
        }
        return x ;
    }

    inline void Rcpp_ReleaseObject(SEXP x){
        if (x != R_NilValue) {
            R_ReleaseObject(x); 
        }
    }

    inline SEXP Rcpp_ReplaceObject(SEXP x, SEXP y){
        if( x == R_NilValue ){
            Rcpp_PreserveObject( y ) ;    
        } else if( y == R_NilValue ){
            Rcpp_ReleaseObject( x ) ;
        } else {
            // if we are setting to the same SEXP as we already have, do nothing 
            if (x != y) {
                
                // the previous SEXP was not NULL, so release it 
                Rcpp_ReleaseObject(x);
                
                // the new SEXP is not NULL, so preserve it 
                Rcpp_PreserveObject(y);
            }
        }
        return y ;
    } 
}

#include <Rcpp/traits/traits.h>
#include <Rcpp/Named.h>

#include <Rcpp/internal/caster.h>
#include <Rcpp/internal/r_vector.h>
#include <Rcpp/r_cast.h>

#include <Rcpp/api/bones/bones.h>

#include <Rcpp/internal/export.h>
#include <Rcpp/internal/r_coerce.h>
#include <Rcpp/as.h>
#include <Rcpp/InputParameter.h>
#include <Rcpp/is.h>

#include <Rcpp/vector/VectorBase.h>
#include <Rcpp/vector/MatrixBase.h>

#include <Rcpp/wrap/wrap.h>

#include <Rcpp/internal/Proxy_Iterator.h>
#include <Rcpp/internal/const_Proxy_Iterator.h>
#include <Rcpp/internal/converter.h>

#include <Rcpp/sugar/sugar_forward.h>

#include <Rcpp/iostream/iostream.h>
#include <Rcpp/longlong.h>

#endif
