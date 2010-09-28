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

#ifndef __SOT_SERVER_ABSTRACT_HH__
#define __SOT_SERVER_ABSTRACT_HH__

/* --------------------------------------------------------------------- */
/* --- INCLUDE --------------------------------------------------------- */
/* --------------------------------------------------------------------- */

/* SOT */
#include <dynamic-graph/entity.h>
#include <boost/thread.hpp>

//class CorbaServerGeneric;
#include <dg-middleware/corba-server-generic.h>

namespace dg = dynamicgraph;

class ServerAbstract
: public dg::Entity
{
 public:
  const static std::string DEFAULT_CONTEXT_NAME;

 protected: //private:
  static CorbaServerGeneric * genericServer;
  static boost::thread * serverThread;

 public:

  ServerAbstract( const std::string& name );
  virtual ~ServerAbstract ( void ) {}

  void commandLine( const std::string& cmdLine,
                    std::istringstream& cmdArgs,
		    std::ostream& os );

 public:
  static void initCorbaServer( const std::string contextName = "" );
  static void closeCorbaServer( void );

 protected:
  //virtual void registerCorbaServant( void ) = 0;
  template< class ClassCorbaPlugin >
    void registerCorbaServant( ClassCorbaPlugin* idl )
    { genericServer->addIdl( idl,idl->getName() ); }

 public:
};

#endif // #ifndef __SOT_SERVER_ABSTRACT_HH__
