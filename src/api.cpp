// api.cpp:  Rcpp api
//
// Copyright (C) 2012 - 2013  Dirk Eddelbuettel and Romain Francois
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
  
#define COMPILING_RCPP11

#include <Rcpp.h>
#include "internal.h" 
#include <cxxabi.h>

namespace Rcpp {

    // Evaluator
    SEXP Rcpp_eval__impl(SEXP expr_, SEXP env) {
        RCPP_DEBUG( "Rcpp_eval( expr = <%p>, env = <%p> )", expr_, env ) 
        Scoped<SEXP> expr = expr_ ;

        reset_current_error() ; 

        Environment RCPP = Environment::Rcpp11_namespace(); 
        static SEXP tryCatchSym = NULL, evalqSym, conditionMessageSym, errorRecorderSym, errorSym ;
        if (!tryCatchSym) {
            tryCatchSym               = ::Rf_install("tryCatch");
            evalqSym                  = ::Rf_install("evalq");
            conditionMessageSym       = ::Rf_install("conditionMessage");
            errorRecorderSym          = ::Rf_install(".rcpp_error_recorder");
            errorSym                  = ::Rf_install("error");
        }
        RCPP_DEBUG( "  [Rcpp_eval] RCPP = " ) 
        
        Scoped<SEXP> call = Rf_lang3( 
            tryCatchSym, 
            Rf_lang3( evalqSym, expr, env ),
            errorRecorderSym
        ) ;
        SET_TAG( CDDR(call), errorSym ) ;
        /* call the tryCatch call */
        Scoped<SEXP> res  = ::Rf_eval( call, RCPP );
        
        if( error_occured() ) {
            Scoped<SEXP> current_error        =  rcpp_get_current_error() ;
            Scoped<SEXP> conditionMessageCall = ::Rf_lang2(conditionMessageSym, current_error) ;
            Scoped<SEXP> condition_message    = ::Rf_eval(conditionMessageCall, R_GlobalEnv) ;
            std::string message(CHAR(::Rf_asChar(condition_message)));
            throw eval_error(message) ;
        }

        return res ;
    }
                   
    std::string demangle__impl( const std::string& name ){
        std::string real_class ;
        int status =-1 ;
        char *dem = 0;
        dem = abi::__cxa_demangle(name.c_str(), 0, 0, &status);
        if( status == 0 ){
            real_class = dem ;
            free(dem);
        } else {
            real_class = name ;
        }
        return real_class ;
    }

    static SEXP get_last_call(){
        SEXP sys_calls_symbol = Rf_install( "sys.calls" ) ;
        Scoped<SEXP> sys_calls_expr = Rf_lang1(sys_calls_symbol) ;   
        Scoped<SEXP> calls = Rf_eval( sys_calls_expr, R_GlobalEnv ) ;
        SEXP res = calls ;
        while( !Rf_isNull(CDR(res)) ) res = CDR(res); 
        return CAR(res) ;
    }
    
    static SEXP get_exception_classes( const std::string& ex_class) {
        Scoped<SEXP> res = Rf_allocVector( STRSXP, 4 );
        SET_STRING_ELT( res, 0, Rf_mkChar( ex_class.c_str() ) ) ;
        SET_STRING_ELT( res, 1, Rf_mkChar( "C++Error" ) ) ;
        SET_STRING_ELT( res, 2, Rf_mkChar( "error" ) ) ;
        SET_STRING_ELT( res, 3, Rf_mkChar( "condition" ) ) ;
        return res;
    }
    
    static SEXP make_condition(const std::string& ex_msg, SEXP call, SEXP cppstack, SEXP classes){
        Scoped<SEXP> res     = Rf_allocVector( VECSXP, 3 ) ;
        Scoped<SEXP> message = Rf_mkString( ex_msg.c_str() ) ;
        RCPP_SET_VECTOR_ELT( res, 0, message ) ;
        RCPP_SET_VECTOR_ELT( res, 1, call ) ;
        RCPP_SET_VECTOR_ELT( res, 2, cppstack ) ;
        Scoped<SEXP> names = Rf_allocVector( STRSXP, 3 ) ;
        SET_STRING_ELT( names, 0, Rf_mkChar( "message" ) ) ;
        SET_STRING_ELT( names, 1, Rf_mkChar( "call" ) ) ;
        SET_STRING_ELT( names, 2, Rf_mkChar( "cppstack" ) ) ;
        Rf_setAttrib( res, R_NamesSymbol, names ) ;
        Rf_setAttrib( res, R_ClassSymbol, classes ) ;
        return res ;
    }
    
    static SEXP exception_to_r_condition( const std::exception& ex){
        std::string ex_class = Rcpp::demangle( typeid(ex).name() ) ;
        std::string ex_msg   = ex.what() ; 
        
        Scoped<SEXP> cppstack  = rcpp_get_stack_trace() ;
        Scoped<SEXP> call      = get_last_call() ;
        Scoped<SEXP> classes   = get_exception_classes(ex_class) ;
        Scoped<SEXP> condition = make_condition( ex_msg, call, cppstack, classes ) ; 
        rcpp_set_stack_trace( R_NilValue ) ;
        return condition ;
    }
    
    void forward_exception_to_r__impl( const std::exception& ex){
        SEXP stop_sym  = Rf_install( "stop" ) ;
        Scoped<SEXP> condition = exception_to_r_condition(ex) ;
        Scoped<SEXP> expr = Rf_lang2( stop_sym , condition );
        Rf_eval( expr, R_GlobalEnv ) ;
    }

    SEXP exception_to_try_error__impl( const std::exception& ex ){
        return string_to_try_error(ex.what());
    }
    
    SEXP string_to_try_error__impl( const std::string& str){
        // form simple error condition based on a string
        Scoped<SEXP> simpleErrorExpr = ::Rf_lang2(::Rf_install("simpleError"), Rf_mkString(str.c_str()));
        Scoped<SEXP> simpleError = Rf_eval(simpleErrorExpr, R_GlobalEnv);
    	
        Scoped<SEXP> tryError = Rf_mkString( str.c_str() ) ;
        Rf_setAttrib( tryError, R_ClassSymbol, Rf_mkString("try-error") ) ; 
        Rf_setAttrib( tryError, Rf_install( "condition") , simpleError ) ; 
        return tryError;
    }

    const char * type2name__impl(int sexp_type) {
        switch (sexp_type) {
        case NILSXP:	return "NILSXP";
        case SYMSXP:	return "SYMSXP";
        case RAWSXP:	return "RAWSXP";
        case LISTSXP:	return "LISTSXP";
        case CLOSXP:	return "CLOSXP";
        case ENVSXP:	return "ENVSXP";
        case PROMSXP:	return "PROMSXP";
        case LANGSXP:	return "LANGSXP";
        case SPECIALSXP:	return "SPECIALSXP";
        case BUILTINSXP:	return "BUILTINSXP";
        case CHARSXP:	return "CHARSXP";
        case LGLSXP:	return "LGLSXP";
        case INTSXP:	return "INTSXP";
        case REALSXP:	return "REALSXP";
        case CPLXSXP:	return "CPLXSXP";
        case STRSXP:	return "STRSXP";
        case DOTSXP:	return "DOTSXP";
        case ANYSXP:	return "ANYSXP";
        case VECSXP:	return "VECSXP";
        case EXPRSXP:	return "EXPRSXP";
        case BCODESXP:	return "BCODESXP";
        case EXTPTRSXP:	return "EXTPTRSXP";
        case WEAKREFSXP:	return "WEAKREFSXP";
        case S4SXP:		return "S4SXP";
        default:    return "<unknown>";
        }
    }

    #if defined(__GNUC__)
    #if defined(WIN32) || defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__) || defined(__CYGWIN__) || defined(__sun)
        // Simpler version for Windows and *BSD 
        SEXP stack_trace__impl( const char* file, int line ){
            Rcpp::List trace = Rcpp::List::create( 
                Rcpp::Named( "file"  ) = file, 
                Rcpp::Named( "line"  ) = line, 
                Rcpp::Named( "stack" ) = "C++ stack not available on this system"
            ) ;
            trace.attr("class") = "Rcpp_stack_trace" ;
            return trace ;
        }
    #else 
    #include <execinfo.h>
    
        static std::string demangler_one( const char* input){
            static std::string buffer ;
            buffer = input ;
            buffer.resize( buffer.find_last_of( '+' ) - 1 ) ;
            buffer.erase(
                buffer.begin(), 
                buffer.begin() + buffer.find_last_of( ' ' ) + 1
            ) ;
            return Rcpp::demangle( buffer) ;
        }
        
        SEXP stack_trace__impl( const char *file, int line) {
            const size_t max_depth = 100;
            size_t stack_depth;
            void *stack_addrs[max_depth];
            char **stack_strings;
        
            stack_depth = backtrace(stack_addrs, max_depth);
            stack_strings = backtrace_symbols(stack_addrs, stack_depth);
        
            std::string current_line ;
            
            Rcpp::CharacterVector res( stack_depth - 1) ;
            std::transform( 
                stack_strings + 1, stack_strings + stack_depth, 
                res.begin(), 
                demangler_one 
            ) ;
            free(stack_strings); // malloc()ed by backtrace_symbols
            
            Rcpp::List trace = Rcpp::List::create( 
                Rcpp::Named( "file"  ) = file, 
                Rcpp::Named( "line"  ) = line, 
                Rcpp::Named( "stack" ) = res ) ;
            trace.attr("class") = "Rcpp_stack_trace" ;
            return trace ;
        }
    #endif 
    #else /* !defined( __GNUC__ ) */
        SEXP stack_trace__impl( const char *file, int line) {
            return R_NilValue ;
        }
    #endif   

    namespace {
        unsigned long RNGScopeCounter = 0;
    }
    
    unsigned long enterRNGScope__impl() {       
        if (RNGScopeCounter == 0)
            GetRNGstate();       
        return ++RNGScopeCounter;
    }
    
    unsigned long exitRNGScope__impl() {
        RNGScopeCounter--;
        if (RNGScopeCounter == 0)
            PutRNGstate();
        return RNGScopeCounter ;
    }

}

