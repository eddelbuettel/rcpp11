//
// Range.h:  
//
// Copyright (C) 2010 - 2013 Dirk Eddelbuettel and Romain Francois
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

#ifndef RCPP_SUGAR_RANGE_H
#define RCPP_SUGAR_RANGE_H

namespace Rcpp{

    class Range : public VectorBase<INTSXP,false, Range >{
    public:
        Range( int start_, int end__ ) : start(start_), end_(end__){
            if( start_ > end__ ){
                throw std::range_error( "upper value must be greater than lower value" ) ;
            }
        }
                
        inline int size() const{
            return end_ - start + 1;
        }
                
        inline int operator[]( int i) const {
            return start + i ;
        }
                
        Range& operator++() {
            start++ ; end_++ ;
            return *this ;
        }
        Range operator++(int) {
            Range orig(*this) ;
            ++(*this);
            return orig ;
        }
                
        Range& operator--() {
            start-- ; end_-- ;
            return *this ;
        }
        Range operator--(int) {
            Range orig(*this) ;
            --(*this);
            return orig ;
        }
                
        Range& operator+=(int n) {
            start += n ; end_ += n ;
            return *this ;
        }
                
        Range& operator-=(int n) {
            start -= n ; end_ -= n ;
            return *this ;
        }
                
        Range operator+( int n ){
            return Range( start + n, end_ + n ) ;
        }
        Range operator-( int n ){
            return Range( start - n, end_ - n ) ;
        }
                
        inline int get_start() const { return start ; }
        inline int get_end() const { return end_ ; }
                
    private:
        int start ;
        int end_ ;
    } ;
        
} 

#endif
