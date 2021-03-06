// Copyright (C) 2012 Dirk Eddelbuettel and Romain Francois
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

#ifndef Rcpp_macros_interface_h
#define Rcpp_macros_interface_h

#define RCPP_GENERATE_CTOR_ASSIGN__(__CLASS__)      \
typedef StoragePolicy<__CLASS__> Storage ; \
typedef AttributeProxyPolicy<__CLASS__> AttributePolicy ; \
__CLASS__( const __CLASS__& other ){              \
    Storage::copy__(other) ;                               \
}                                                 \
__CLASS__( __CLASS__&& other ){                   \
    Storage::steal__(other) ;                              \
}                                                 \
__CLASS__& operator=( __CLASS__&& other ){        \
    return Storage::steal__( other );                      \
}                                                 \
__CLASS__& operator=(const __CLASS__& rhs) {      \
    return Storage::copy__(rhs) ;                          \
}                                                 \

#define RCPP_POLICIES(__CLASS__)                 \
    public RObjectStorage<__CLASS__>,            \
    public SlotProxyPolicy<__CLASS__>,           \
    public AttributeProxyPolicy<__CLASS__>       \

#define RCPP_API_CLASS(__CLASS__)   \
template < template <class> class StoragePolicy > class __CLASS__ : \
    public StoragePolicy<__CLASS__<StoragePolicy>>,            \
    public SlotProxyPolicy<__CLASS__<StoragePolicy>>,           \
    public AttributeProxyPolicy<__CLASS__<StoragePolicy>>       \
   
#define RCPP_API_CLASS_DECL(__CLASS__)  \
    template < template <class> class StoragePolicy > class __CLASS__##_Impl ; \
    using __CLASS__ = __CLASS__##_Impl<RObjectStorage> ;
    
#endif
