// Copyright (C) 2013  Romain Francois
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

#ifndef Rcpp_wrap_wrap_type_h
#define Rcpp_wrap_wrap_type_h
 
namespace Rcpp{
namespace traits{
    
    template <typename T>
    struct wrap_type {
        typedef typename Rcpp::traits::wrap_type_traits<T>::wrap_category wrap_category ;
        const static bool has_matrix_interface = Rcpp::traits::matrix_interface<T>::value ;
        const static bool has_iterator = Rcpp::traits::has_iterator<T>::value ;
        const static bool is_primitive = std::is_same< wrap_category , Rcpp::traits::wrap_type_primitive_tag>::value ;
        const static bool is_enum      = std::is_enum<T>::value  ;
        const static bool is_sexp_convertible = std::is_convertible<T,SEXP>::value ;
        const static bool is_module_object = std::is_same< wrap_category , Rcpp::traits::wrap_type_module_object_tag>::value ;
        const static bool is_sugar = Rcpp::traits::is_sugar_expression<T>::value ;
         
        typedef typename std::conditional< 
            is_sexp_convertible, 
            typename Rcpp::SexpConvertibleWrapper<T>,
            typename std::conditional<
                is_module_object,
                typename Rcpp::ModuleObjectWrapper<T>,
                typename std::conditional<
                    is_primitive, 
                    typename Rcpp::PrimitiveWrapper<T>,
                    typename std::conditional<
                        is_enum,
                        Rcpp::EnumWrapper<T>,
                        typename std::conditional<
                            is_sugar,
                            typename Rcpp::SugarExpressionWrapper<T>,
                            typename std::conditional<
                                has_matrix_interface,
                                typename Rcpp::MatrixWrapper<T>,
                                typename std::conditional<
                                    has_iterator, 
                                    typename Rcpp::ContainerWrapper<T>, 
                                    typename Rcpp::Wrapper<T>
                                    >::type
                                >::type
                            >::type
                        >::type
                    >::type
                >::type 
            >::type type ;    
    } ;
    
}
}

#endif
