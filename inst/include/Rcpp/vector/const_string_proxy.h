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

#ifndef Rcpp__vector__const_string_proxy_h
#define Rcpp__vector__const_string_proxy_h
 
namespace Rcpp{
namespace internal{
	
	template<int RTYPE> class const_string_proxy {
	public:
		
		typedef typename Rcpp::Vector<RTYPE> VECTOR ;
		
		const_string_proxy() : parent(0), index(-1){};
		
		/**
		 * Creates a proxy
		 *
		 * @param v reference to the associated character vector
		 * @param index index 
		 */
		const_string_proxy( const VECTOR& v, int index_ ) : parent(&v), index(index_) {
				RCPP_DEBUG( "const_string_proxy( VECTOR& = <%p>, index_ = %d) ", v.get__(), index_ ) ;
		}
			
		const_string_proxy( const const_string_proxy& other ) : parent(other.parent), index(other.index){} ;
		
		const_string_proxy& operator=( const const_string_proxy& ) = delete ; 
		
		void import( const const_string_proxy& other){
			parent = other.parent ;
			index  = other.index ;
		}
		
		/**
		 * rhs use. Retrieves the current value of the 
		 * element this proxy refers to.
		 */
		operator SEXP() const {
			return get() ;
		}
		
		/**
		 * rhs use. Retrieves the current value of the 
		 * element this proxy refers to and convert it to a 
		 * C string
		 */
		operator /* const */ char*() const {
		    return const_cast<char*>( CHAR(get()) );
		}
		 
		 
		/**
		 * Prints the element this proxy refers to to an 
		 * output stream
		 */
		template <int RT>
		friend std::ostream& operator<<(std::ostream& os, const const_string_proxy<RT>& proxy);
		
		template <int RT>
		friend std::string operator+( const std::string& x, const const_string_proxy<RT>& proxy);
		
		const VECTOR* parent; 
		int index ;
		inline void move( int n ){ index += n ;}
		
		inline SEXP get() const {
			return STRING_ELT( *parent, index ) ;
		}
		
		inline int size() const { return strlen( begin() ) ; }
		bool operator==( const char* other){
			return strcmp( begin(), other ) == 0 ;
		}
		bool operator!=( const char* other){
			return strcmp( begin(), other ) != 0 ;
		}
		
		bool operator==( const const_string_proxy& other){
			return strcmp( begin(), other.begin() ) == 0 ;
		}
		bool operator!=( const const_string_proxy& other){
			return strcmp( begin(), other.begin() ) != 0 ;
		}
		
		
		private:
		    typedef const char* iterator ;
		    typedef const char& reference ;
		
		    inline iterator begin() const { return CHAR( STRING_ELT( *parent, index ) ) ; }
		    inline iterator end() const { return begin() + size() ; }
		
			static std::string buffer ;
		
	} ;
	
	template <int RT>
	bool operator<( const const_string_proxy<RT>& lhs, const const_string_proxy<RT>& rhs) {
		return strcmp( 
			const_cast<char *>(lhs.begin() ), 
			const_cast<char *>(rhs.begin())
			) < 0 ;
	}

	template <int RT>
	bool operator>( const const_string_proxy<RT>& lhs, const const_string_proxy<RT>& rhs) {
		return strcmp( 
			const_cast<char *>(lhs.begin() ), 
			const_cast<char *>(rhs.begin())
			) > 0 ;
	}
	
	template <int RT>
	bool operator>=( const const_string_proxy<RT>& lhs, const const_string_proxy<RT>& rhs) {
		return strcmp( 
			const_cast<char *>(lhs.begin() ), 
			const_cast<char *>(rhs.begin())
			) >= 0 ;
	}
	
	template <int RT>
	bool operator<=( const const_string_proxy<RT>& lhs, const const_string_proxy<RT>& rhs) {
		return strcmp( 
			const_cast<char *>(lhs.begin() ), 
			const_cast<char *>(rhs.begin())
			) <= 0 ;
	}
		
	template<int RTYPE> std::string const_string_proxy<RTYPE>::buffer ;
	
	inline std::ostream& operator<<(std::ostream& os, const const_string_proxy<STRSXP>& proxy) {
	    os << static_cast<char*>(proxy) ;
	    return os;
	}
	
	inline std::string operator+( const std::string& x, const const_string_proxy<STRSXP>& y ){
		return x + static_cast<const char*>(y) ;
	}
		
}
}

#endif
