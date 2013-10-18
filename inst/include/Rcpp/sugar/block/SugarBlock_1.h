//
// SugarBlock.h:  sugar functions
//
// Copyright (C) 2010 - 2011 Dirk Eddelbuettel and Romain Francois
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

#ifndef RCPP_SUGAR_BLOCK_1_H
#define RCPP_SUGAR_BLOCK_1_H

namespace Rcpp{
namespace sugar{                         

template <bool NA, typename OUT, typename U1, typename T1>
class SugarBlock_1 : public Rcpp::VectorBase< Rcpp::traits::r_sexptype_traits<OUT>::rtype , NA, SugarBlock_1<NA,OUT,U1,T1> > {
public:
    typedef OUT (*FunPtr)(U1) ;
    SugarBlock_1( FunPtr ptr_, const T1 & vec_) : ptr(ptr_), vec(vec_){}
        
    inline OUT operator[]( int i) const { 
        return ptr( vec[i] ) ;
    }
    inline int size() const { return vec.size() ; }
        
private:
    FunPtr ptr ;
    const T1& vec ;
};

} // sugar
} // Rcpp

#define SB1_T VectorBase<REALSXP,NA,T>
                                                            
#define SUGAR_BLOCK_1(__NAME__,__SYMBOL__)                                                \
    namespace Rcpp{                                                                       \
    template <bool NA, typename T>                                                        \
    inline sugar::SugarBlock_1<NA,double,double,SB1_T >                                   \
    __NAME__(                                                                             \
        const SB1_T& t                                                                    \
    ){                                                                                    \
        return sugar::SugarBlock_1<NA,double,double,SB1_T >(                              \
                __SYMBOL__ , t                                                            \
        ) ;                                                                               \
    }                                                                                     \
}
	
#endif
