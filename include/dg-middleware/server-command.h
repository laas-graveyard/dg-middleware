/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * Copyright Projet JRL-Japan, 2007
 *+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 *
 * File:      ServerCommand.h
 * Project:   SOT
 * Author:    Torea FOISSOTTE and Nicolas Mansard
 *
 * Version control
 * ===============
 *
 *  $Id$
 *
 * Description
 * ============
 *
 * inspired by sotServerTeleop
 *
 * ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#ifndef __SOT_SERVER_COMMAND_H__
#define __SOT_SERVER_COMMAND_H__

/* --------------------------------------------------------------------- */
/* --- INCLUDE --------------------------------------------------------- */
/* --------------------------------------------------------------------- */

/* --- SOT --- */
#include <dg-middleware/server-abstract.h>
#include <dynamic-graph/signal-ptr.h>
#include <dynamic-graph/signal-time-dependent.h>

/* --- CORBA IDL --- */
#include <server-command-corba.h>

/* --- BOOST --- */
#ifdef HAVE_LIBBOOST_THREAD
#include <boost/thread.hpp>
#endif

/* --- MAAL --- */
#include <MatrixAbstractLayer/boost.h>
namespace ml = maal::boost;

/* --------------------------------------------------------------------- */
/* --- CLASS ----------------------------------------------------------- */
/* --------------------------------------------------------------------- */

namespace dg = dynamicgraph;

class ServerCommand
:public ServerAbstract
,virtual public POA_CorbaServer::SOT_Server_Command
  //  ,virtual public PortableServer::RefCountServantBase
{
 protected:
  std::vector< dg::SignalTimeDependent< ml::Vector,int >* > vectorSOUT;
  std::vector< dg::SignalPtr< ml::Vector,int >* > vectorSIN;

 public: /* --- CONSTRUCTION --- */
  ServerCommand( const std::string& name );
  virtual ~ServerCommand( void );

 public:  /* --- sot ENTITY --- */
  static const std::string CLASS_NAME;
  virtual const std::string& getClassName( void ) const { return CLASS_NAME; }

  void notifyClient( const std::string & name );
  void notifyClients( void );
  void displayClientList( std::ostream & os );

  void commandLine( const std::string& cmdLine,
		    std::istringstream& cmdArgs,
		    std::ostream& os );

 public: /* --- CORBA implementation --- */

  void run( const char * cmd )
    throw(CORBA::SystemException);
  void runAndRead( const char * cmd,
		   CorbaServer::StringStreamer_out os )
    throw(CORBA::SystemException);

  void getStringStream( CorbaServer::StringStreamer_out StringStreamer ) ;
  void readVector( const char * cmd,
		   CorbaServer::DoubleSeq_out value )
    throw(CORBA::SystemException);

  long int createOutputVectorSignal( const char * signalNameCorba )
    throw(CORBA::SystemException);
  long int createInputVectorSignal( const char * signalNameCorba )
    throw(CORBA::SystemException);

  void readInputVectorSignal( long int signalRank,
			      CorbaServer::DoubleSeq_out value )
    throw(CORBA::SystemException);
  void writeOutputVectorSignal( long int signalRank,
				const CorbaServer::DoubleSeq& value )
    throw(CORBA::SystemException);

  /* Callback registration */
  typedef std::map<std::string,CorbaServer::NotifyCallback_var> CallbackList;
  CallbackList callbackList;

  void registerClient(CorbaServer::NotifyCallback_ptr client,
                      const char * clientName)
    throw(CORBA::SystemException);
  void unregisterClient(const char * clientName)
    throw(CORBA::SystemException);



};

#endif // #ifndef __SOT_SERVER_COMMAND_H__
