//
// which_max.h:  which.max
//
// Copyright (C) 2012   Dirk Eddelbuettel and Romain Francois
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

#ifndef Rcpp__sugar__which_max_h
#define Rcpp__sugar__which_max_h

namespace Rcpp{
namespace sugar{

template <int RTYPE, bool NA, typename T>
class WhichMax {
public:
    typedef typename Rcpp::VectorBase<RTYPE,NA,T> VEC_TYPE ;
    typedef typename Rcpp::traits::storage_type<RTYPE>::type STORAGE ;
	WhichMax(const VEC_TYPE& obj_ ) : obj(obj_){}
	
	int get() const {
	    STORAGE current = obj[0] ;
	    STORAGE min = current ;
	    int index = 0 ;
	    if( Rcpp::traits::is_na<RTYPE>(current) ) return NA_INTEGER ;
	    int n = obj.size() ;
	    for( int i=1; i<n; i++){
		    current = obj[i] ;
		    if( Rcpp::traits::is_na<RTYPE>(current) ) return NA_INTEGER ;
		    if( current > min ){
		        min = current ;
		        index = i ;
		    }
		}
		return index ;
	}
	
private:
    const VEC_TYPE& obj ;	
	
} ;
   
template <int RTYPE, typename T>
class WhichMax<RTYPE,false,T> {
public:
    typedef typename Rcpp::VectorBase<RTYPE,false,T> VEC_TYPE ;
    typedef typename Rcpp::traits::storage_type<RTYPE>::type STORAGE ;
	WhichMax(const VEC_TYPE& obj_ ) : obj(obj_){}
	
	int get() const {
	    STORAGE current = obj[0] ;
	    STORAGE min = current ;
	    int index = 0 ;
	    int n = obj.size() ;
	    for( int i=1; i<n; i++){
		    current = obj[i] ;
		    if( current > min ){
		        min = current ;
		        index = i ;
		    }
		}
		return index ;
	}
	
private:
    const VEC_TYPE& obj ;	
	
} ;

    
} // sugar



template <int RTYPE, bool NA, typename T>
int which_max( const VectorBase<RTYPE,NA,T>& t ){
	return sugar::WhichMax<RTYPE,NA,T>(t).get() ; 
}
  
} // Rcpp
#endif

