/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * Copyright Projet JRL-Japan, 2007
 *+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 *
 * File:      ServerAbstract.cpp
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


/* --------------------------------------------------------------------- */
/* --- INCLUDE --------------------------------------------------------- */
/* --------------------------------------------------------------------- */

#include <dynamic-graph/debug.h>

/* SOT */
//#include <sot/CorbaServerGeneric.h>
#include <dg-middleware/server-abstract.h>
#include <boost/bind.hpp>

using namespace dynamicgraph;

/* --------------------------------------------------------------------- */
/* --- CLASS ----------------------------------------------------------- */
/* --------------------------------------------------------------------- */

const std::string ServerAbstract::DEFAULT_CONTEXT_NAME = "sot";
CorbaServerGeneric * ServerAbstract::genericServer = NULL;
boost::thread * ServerAbstract::serverThread = NULL;

ServerAbstract::
ServerAbstract(const std::string &name)
  :Entity(name)
{
}


void ServerAbstract::
initCorbaServer( const std::string inContextName )
{
  if( NULL!=genericServer ) return;

  dgDEBUG(15) << "Decide the context name." << std::endl;
  std::string contextName;
  if( inContextName.length()>0 )
    contextName = inContextName; else contextName = DEFAULT_CONTEXT_NAME;

  dgDEBUG(15) << "Create and init the generic server with context <"
               << contextName << ">." << std::endl;
  genericServer = new CorbaServerGeneric(contextName);
  genericServer->init();

  dgDEBUG(15) << "/* Run the server in a new thread. */" << std::endl;
  serverThread = new boost::thread( boost::bind(&CorbaServerGeneric::run,
                                                genericServer,false) );
}


void ServerAbstract::
closeCorbaServer( void )
{
  if( NULL==genericServer ) return;

  dgDEBUG(5) << "Stop the server." << std::endl;
  genericServer->stop();

  dgDEBUG(5) << "Wait for the corresponding thread to finish." << std::endl;
  serverThread->join();

  dgDEBUG(5) << "And finally free the memory." << std::endl;
  delete genericServer; genericServer=NULL;
  delete serverThread; serverThread=NULL;
}


/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */

#include <dynamic-graph/pool.h>

void ServerAbstract::
commandLine( const std::string& cmdLine,std::istringstream& cmdArgs,std::ostream& os )
{
  if( cmdLine == "help" )
    {
      os << "ServerAbstract:"<<std::endl
	 << "  - initCorba \t\t\t:register the context in the nameservice." << std::endl;

      Entity::commandLine( cmdLine,cmdArgs,os );
    }
  else if( cmdLine == "initCorba" )
    {
      std::string corbaContext; cmdArgs >> corbaContext;
      cmdArgs >> std::ws; if(! cmdArgs.good() ) corbaContext = "";
      initCorbaServer(corbaContext);
    }
  else
    {      Entity::commandLine( cmdLine,cmdArgs,os );    }
}




