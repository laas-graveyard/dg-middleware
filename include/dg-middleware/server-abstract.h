/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * Copyright Projet JRL-Japan, 2007
 *+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 *
 * File:      ServerAbstract.h
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
