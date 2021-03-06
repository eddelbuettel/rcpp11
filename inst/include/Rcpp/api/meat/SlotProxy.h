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

#ifndef Rcpp_api_meat_SlotProxy_h
#define Rcpp_api_meat_SlotProxy_h

namespace Rcpp{

    template <typename CLASS>
    typename SlotProxyPolicy<CLASS>::SlotProxy& SlotProxyPolicy<CLASS>::SlotProxy::operator=( const SlotProxy& rhs ){
        set( rhs.get() ) ;
        return *this ;    
    }
    
    template <typename CLASS>
    SEXP SlotProxyPolicy<CLASS>::SlotProxy::get() const {
        return R_do_slot( parent, slot_name ) ;    
    }
    
    template <typename CLASS>
    void SlotProxyPolicy<CLASS>::SlotProxy::set( SEXP x) {
        parent = R_do_slot_assign(parent, slot_name, x);
    }

    template <typename CLASS>
    typename SlotProxyPolicy<CLASS>::SlotProxy SlotProxyPolicy<CLASS>::slot(const std::string& name) {
        SEXP x = static_cast<CLASS&>(*this) ;
        if( !Rf_isS4(x) ) throw not_s4() ;
        return SlotProxy( static_cast<CLASS&>(*this) , name ) ;
    }
 
    template <typename CLASS>
    template <typename T> 
    typename SlotProxyPolicy<CLASS>::SlotProxy& SlotProxyPolicy<CLASS>::SlotProxy::operator=( const T& rhs ){
        set( wrap( rhs ) ) ;
        return *this ;
    }
    
    template <typename CLASS>
    template <typename T>
    SlotProxyPolicy<CLASS>::SlotProxy::operator T() const {
        return as<T>(get()) ;    
    }

    
    
    template <typename CLASS>
    SlotProxyPolicy<CLASS>::const_SlotProxy::const_SlotProxy( const CLASS& v, const std::string& name) 
        : parent(v), slot_name(Rf_install(name.c_str()))
    {
        if( !R_has_slot( v, slot_name) ){
            throw no_such_slot() ; 
        }        
    }

    template <typename CLASS>
    SEXP SlotProxyPolicy<CLASS>::const_SlotProxy::get() const {
        return R_do_slot( parent, slot_name ) ;    
    }
    
    template <typename CLASS>
    typename SlotProxyPolicy<CLASS>::const_SlotProxy SlotProxyPolicy<CLASS>::slot(const std::string& name) const {
        SEXP x = static_cast<const CLASS&>(*this) ;
        if( !Rf_isS4(x) ) throw not_s4() ;
        return const_SlotProxy( static_cast<const CLASS&>(*this) , name ) ;
    }

    
}
#endif
