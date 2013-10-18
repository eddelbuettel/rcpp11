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

#ifndef Rcpp_Module_method_h
#define Rcpp_Module_method_h

    template <typename OUT, typename... Args>
    self& method( const char* name_, OUT (Class::*fun)(Args...), const char* docstring = 0, ValidMethod valid = &yes ){
        typedef OUT (Class::*MemberFunctionType)(Args...)  ;
        if( is_debugging )
            AddMethod( name_, new Debug_CppMethod_Impl<Class,MemberFunctionType, false,OUT,Args...>( fun, name_ ), valid, docstring) ;
        else 
            AddMethod( name_, new CppMethod_Impl<Class,MemberFunctionType, false,OUT,Args...>( fun ), valid, docstring) ;
        return *this ;
    }
    
    template <typename OUT, typename... Args>
    self& method( const char* name_, OUT (Class::*fun)(Args...) const, const char* docstring = 0, ValidMethod valid = &yes ){
        typedef OUT (Class::*MemberFunctionType)(Args...) const  ;
        if( is_debugging ) 
            AddMethod( name_, new Debug_CppMethod_Impl<Class,MemberFunctionType,true,OUT,Args...>( fun, name_ ), valid, docstring ) ;
        else
            AddMethod( name_, new CppMethod_Impl<Class,MemberFunctionType,true,OUT,Args...>(fun), valid, docstring ) ;
        return *this ;
    }
    
    template <typename OUT, typename... Args>
    self& nonconst_method( const char* name_, OUT (Class::*fun)(Args...), const char* docstring = 0, ValidMethod valid = &yes ){
        typedef OUT (Class::*MemberFunctionType)(Args...)  ;
        if(is_debugging)
            AddMethod( name_, new Debug_CppMethod_Impl<Class,MemberFunctionType,false,OUT,Args...>( fun, name_ ) , valid, docstring ) ;
        else 
            AddMethod( name_, new CppMethod_Impl<Class,MemberFunctionType,false,OUT,Args...>( fun ) , valid, docstring ) ;
        return *this ;
    }
    template <typename OUT, typename... Args>
    self& const_method( const char* name_, OUT (Class::*fun)(Args...) const, const char* docstring = 0, ValidMethod valid = &yes ){
        typedef OUT (Class::*MemberFunctionType)(Args...) const ;
        if( is_debugging )
            AddMethod( name_, new Debug_CppMethod_Impl<Class,MemberFunctionType,true,OUT,Args...>( fun, name_), valid, docstring ) ;
        else
            AddMethod( name_, new CppMethod_Impl<Class,MemberFunctionType,true,OUT,Args...>( fun ), valid, docstring ) ;    
        return *this ;
    }
	
    	template <typename OUT, typename... Args>
	self& method( const char* name_, OUT (*fun)(Class*, Args...), const char* docstring = 0, ValidMethod valid = &yes ){
        typedef OUT (*MemberFunctionType)(Class*, Args...) ;
        if( is_debugging )
            AddMethod( name_, new Debug_CppMethod_Impl<Class,MemberFunctionType,false,OUT,Args...>( fun, name_ ), valid, docstring ) ;    
        else 
            AddMethod( name_, new CppMethod_Impl<Class,MemberFunctionType,false,OUT,Args...>( fun ), valid, docstring ) ;
        return *this ;
	}

#endif
