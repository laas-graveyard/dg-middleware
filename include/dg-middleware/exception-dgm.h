/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * Copyright Projet JRL-Japan, 2007
 *+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 *
 * File:      exception-tools.h
 * Project:   SOT
 * Author:    Nicolas Mansard
 *
 * Version control
 * ===============
 *
 *  $Id$
 *
 * Description
 * ============
 *
 *
 * ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/


#ifndef __DGM_EXCEPTION_H
#define __DGM_EXCEPTION_H

/* --------------------------------------------------------------------- */
/* --- INCLUDE --------------------------------------------------------- */
/* --------------------------------------------------------------------- */


#include <dynamic-graph/exception-abstract.h>

/* --------------------------------------------------------------------- */
/* --- API ------------------------------------------------------------- */
/* --------------------------------------------------------------------- */

#if defined (WIN32)
#  if defined (exception_dgm_EXPORTS)
#    define DGM_EXPORT __declspec(dllexport)
#  else
#    define DGM_EXPORT __declspec(dllimport)
#  endif
#else
#  define DGM_EXPORT
#endif


/* --------------------------------------------------------------------- */
/* --- CLASS ----------------------------------------------------------- */
/* --------------------------------------------------------------------- */

namespace dynamicgraph {

/* \class ExceptionDgMiddleware
 */
class DGM_EXPORT ExceptionDgMiddleware
:public ExceptionAbstract

{
 public:
  enum ErrorCodeEnum
    {
      GENERIC = ExceptionAbstract::TOOLS
      ,CORBA
    };

  static const std::string EXCEPTION_NAME;
  virtual const std::string& getExceptionName( void ) const { return EXCEPTION_NAME; }

public:

  ExceptionDgMiddleware ( const ExceptionDgMiddleware::ErrorCodeEnum& errcode,
		       const std::string & msg = "" );
  ExceptionDgMiddleware( const ExceptionDgMiddleware::ErrorCodeEnum& errcode,
			const std::string & msg,const char* format, ... );
  virtual ~ExceptionDgMiddleware( void ) throw() {};


};

} // namespace



#endif /* #ifndef __DGM_EXCEPTION_H */

/*
 * Local variables:
 * c-basic-offset: 2
 * End:
 */
