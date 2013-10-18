//
// Formula.h:  formula
//
// Copyright (C) 2010 - 2012 Dirk Eddelbuettel and Romain Francois
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

#ifndef Rcpp__Formula_h
#define Rcpp__Formula_h

#include <RcppCommon.h>
#include <Rcpp/Language.h>

namespace Rcpp{ 

    template < template <class> class StoragePolicy >
    class Formula_Impl : public Language_Impl<StoragePolicy> {
    public:
        typedef typename Language_Impl<StoragePolicy>::Storage Storage ;
        
        Formula_Impl(SEXP x){
            switch( TYPEOF( x ) ){
            case LANGSXP:
                if( ::Rf_inherits( x, "formula") ){
                    Storage::set__( x );
                } else{
                    Storage::set__( internal::convert_using_rfunction( x, "as.formula") ) ;
                }
                break;
            case EXPRSXP:
            case VECSXP:
                /* lists or expression, try the first one */
                if( ::Rf_length(x) > 0 ){
                    SEXP y = VECTOR_ELT( x, 0 ) ;
                    if( ::Rf_inherits( y, "formula" ) ){
                        Storage::set__( y ) ;  
                    } else{
                        SEXP z = internal::convert_using_rfunction( y, "as.formula") ;
                        Storage::set__( z ) ;
                    }
                } else{
                    throw not_compatible( "cannot create formula from empty list or expression" ) ; 
                }
                break;
            default:
                Storage::set__( internal::convert_using_rfunction( x, "as.formula") ) ;
            }
        }

        explicit Formula_Impl( const std::string& code ) {
            Storage::set__(internal::convert_using_rfunction( ::Rf_mkString(code.c_str()), "as.formula") ) ;
        }

    } ; 

} // namespace Rcpp

#endif
