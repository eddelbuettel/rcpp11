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

#ifndef Rcpp_Module_debug_method_h
#define Rcpp_Module_debug_method_h

namespace Rcpp{
    
    template <typename Class, typename MethodClass>
    void debug_method( MethodClass& method, std::string& name, Class* object){ 
        std::string s ; 
        std::string method_name = DEMANGLE(Class) ;
        method_name += "::"  ;
        method_name += name ;
        method.signature( s, method_name.c_str() ) ;
        Rprintf( "    %70s   [object = <%p>] ...", s.c_str(), object) ;
    }

}
#endif
