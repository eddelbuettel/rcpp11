//
// sapply.h:  sapply
//
// Copyright (C) 2010 - 2011 Dirk Eddelbuettel and Romain Francois
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

#ifndef Rcpp__sugar__sapply_h
#define Rcpp__sugar__sapply_h

namespace Rcpp{
namespace sugar{

template <int RTYPE, bool NA, typename T, typename Function, bool NO_CONVERSION>
class Sapply : public VectorBase< 
	Rcpp::traits::r_sexptype_traits<
		typename std::result_of<Function(typename Rcpp::traits::storage_type<RTYPE>::type)>::type
	>::rtype , 
	true ,
	Sapply<RTYPE,NA,T,Function,NO_CONVERSION>
> {
public:
	typedef typename std::result_of<Function(typename Rcpp::traits::storage_type<RTYPE>::type)>::type result_type ; 
	
	const static int RESULT_R_TYPE = 
		Rcpp::traits::r_sexptype_traits<result_type>::rtype ;
	
	typedef Rcpp::VectorBase<RTYPE,NA,T> VEC ;
	typedef typename Rcpp::traits::r_vector_element_converter<RESULT_R_TYPE>::type converter_type ;
	typedef typename Rcpp::traits::storage_type<RESULT_R_TYPE>::type STORAGE ;
	
	typedef typename Rcpp::traits::Extractor< RTYPE, NA, T>::type EXT ;
	
	Sapply( const VEC& vec_, Function fun_ ) : vec(vec_.get_ref()), fun(fun_){
	    RCPP_DEBUG( "Sapply With Converter = %s", DEMANGLE(Sapply) )
	    RCPP_DEBUG( "Sapply Converter = %s", DEMANGLE(converter_type) ) 
	}
	
	inline STORAGE operator[]( int i ) const {
		STORAGE res = converter_type::get( fun( vec[i] ) );
		return res ;
	}
	inline int size() const { return vec.size() ; }
	         
private:
	const EXT& vec ;
	Function fun ;
	
} ;


template <int RTYPE, bool NA, typename T, typename Function>
class Sapply<RTYPE,NA,T,Function,true> : public VectorBase< 
	Rcpp::traits::r_sexptype_traits<
		typename std::result_of<Function(typename Rcpp::traits::storage_type<RTYPE>::type)>::type
	>::rtype , 
	true ,
	Sapply<RTYPE,NA,T,Function,true>
> {
public:
	typedef typename std::result_of<Function(typename Rcpp::traits::storage_type<RTYPE>::type)>::type result_type ; 
	const static int RESULT_R_TYPE = 
		Rcpp::traits::r_sexptype_traits<result_type>::rtype ;
	
	typedef Rcpp::VectorBase<RTYPE,NA,T> VEC ;
	typedef typename Rcpp::traits::storage_type<RESULT_R_TYPE>::type STORAGE ;
	
	typedef typename Rcpp::traits::Extractor< RTYPE, NA, T>::type EXT ;
	
	Sapply( const VEC& vec_, Function fun_ ) : vec(vec_.get_ref()), fun(fun_){
	    RCPP_DEBUG( "Sapply  = %s", DEMANGLE(Sapply) )
	}
	
	inline STORAGE operator[]( int i ) const {
		return fun( vec[i] ) ;
	}
	inline int size() const { return vec.size() ; }
	         
private:
	const EXT& vec ;
	Function fun ;
	
} ;
	

} // sugar

template <int RTYPE, bool NA, typename T, typename Function >
inline sugar::Sapply<
    RTYPE,NA,T,Function, 
    std::is_same< 
        typename std::result_of<Function(typename Rcpp::traits::storage_type<RTYPE>::type )>::type ,  
        typename Rcpp::traits::storage_type< traits::r_sexptype_traits< typename std::result_of<Function(typename Rcpp::traits::storage_type<RTYPE>::type)>::type >::rtype >::type
    >::value
> 
sapply( const Rcpp::VectorBase<RTYPE,NA,T>& t, Function fun ){
    typedef typename std::result_of<Function(typename Rcpp::traits::storage_type<RTYPE>::type)>::type result_type ; 
	return sugar::Sapply<RTYPE,NA,T,Function, 
	std::is_same< 
        result_type,  
        typename Rcpp::traits::storage_type< traits::r_sexptype_traits<result_type>::rtype >::type
    >::value >( t, fun ) ;
}

} // Rcpp

#endif
