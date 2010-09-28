/*
 * Copyright 2010,
 * François Bleibel,
 *
 * CNRS/AIST
 *
 * This file is part of dg-middleware.
 * dg-middleware is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 * dg-middleware is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.  You should
 * have received a copy of the GNU Lesser General Public License along
 * with dg-middleware.  If not, see <http://www.gnu.org/licenses/>.
 */

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
