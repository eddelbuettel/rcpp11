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

#ifndef Rcpp_AttributeProxy_h
#define Rcpp_AttributeProxy_h

namespace Rcpp{
    
template <typename CLASS>
class AttributeProxyPolicy {
public:
    
    class AttributeProxy {
    public:
        AttributeProxy( CLASS& v, const std::string& name) 
            : parent(v), attr_name(Rf_install(name.c_str()))
        {}
        
        AttributeProxy& operator=(const AttributeProxy& rhs) ;
              
        template <typename T> AttributeProxy& operator=(const T& rhs) ;
            
        template <typename T> operator T() const ;
        inline operator SEXP() const { return get() ; }
        
    private:
        CLASS& parent; 
        SEXP attr_name ;
            
        SEXP get() const ;
        void set(SEXP x ) ;
    } ;
    
    class const_AttributeProxy {
    public:
        const_AttributeProxy( const CLASS& v, const std::string& name) ;
        const_AttributeProxy& operator=(const const_AttributeProxy& rhs) ;
              
        template <typename T> operator T() const ;
        inline operator SEXP() const { return get() ; }
        
    private:
        const CLASS& parent; 
        SEXP attr_name ;
            
        SEXP get() const ;
    } ;
    
    AttributeProxy attr( const std::string& name) ;
    const_AttributeProxy attr( const std::string& name) const ;
    
} ;

}
#endif
