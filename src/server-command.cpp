/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * Copyright Projet JRL-Japan, 2007
 *+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 *
 * File:      ServerCommand.cpp
 * Project:   SOT
 * Author:    Torea Foissotte and Nicolas Mansard
 *
 * Version control
 * ===============
 *
 *  $Id$
 *
 * Description
 * ============
 *
 *    Generic server to launch command and get signal data from the
 *  the interpretor shell.
 *
 * ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/


/* --------------------------------------------------------------------- */
/* --- INCLUDE --------------------------------------------------------- */
/* --------------------------------------------------------------------- */

/* --- CORBA --- */
#include <omniORB4/CORBA.h>

#define VP_DEBUG
#define VP_DEBUG_MODE 45
#include <dynamic-graph/debug.h>

/* --- SOT --- */
#include <dg-middleware/server-command.h>
#include <sot-core/exception-tools.h>

#include <dynamic-graph/interpreter.h>

/* --- SOT FACTORY ----------------------------------------------------- */
#include <dynamic-graph/factory.h>

using namespace dynamicgraph;
DYNAMICGRAPH_FACTORY_ENTITY_PLUGIN( ServerCommand, "ServerCommand" );

/* --- CONSTRUCTION  --------------------------------------------------- */
ServerCommand::
ServerCommand( const std::string& name )
  :ServerAbstract( name )
{
  dgDEBUGIN(15);

  dgDEBUGOUT(15);
}

ServerCommand::
~ServerCommand( void )
{
  dgDEBUGIN(15);

  dgDEBUGOUT(15);
}


/* -------------------------------------------------------------------------- */
/* --- IMPLEMENTATION CORBA ------------------------------------------------- */
/* -------------------------------------------------------------------------- */

// #include <sot/CorbaServerGeneric.h>

// void ServerCommand::
// registerCorbaServant( void )
// {
//   genericServer->addIdl( this,getName() );

// }

/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
using namespace std;

void ServerCommand::
run( const char * cmdCorba ) throw(CORBA::SystemException)
{
  dgDEBUGIN(15);

  string cmd(cmdCorba);
  dgDEBUG(15) <<  "Run: <" << cmd <<">"<< endl;

  istringstream dummy; ostringstream dummydummy;
  try
    {g_shell.cmd( cmd,dummy,dummydummy );}
  catch( ExceptionAbstract& e )
    { dgDEBUG(1) << "Catch exeption: " << e << endl; }
  catch( ... )
    { dgDEBUG(1) << "Catch unknown exception." << endl ; }

  dgDEBUGOUT(15);
}

void ServerCommand::
getStringStream( CorbaServer::StringStreamer_out StringStreamer )
{
}


void ServerCommand::
runAndRead( const char * cmdCorba,
	    CorbaServer::StringStreamer_out os )
  throw(CORBA::SystemException)
{
  dgDEBUGIN(15);

  string line(cmdCorba),cmd;
  istringstream iss( line );  iss >> cmd;
  dgDEBUG(15) <<  "Run: ["<<cmd.length()<<"] <" << cmd << ">" << endl;

  stringstream ioss;
  try
    {g_shell.cmd( cmd,iss,ioss );}
  catch( ExceptionAbstract& e )
    {
      dgDEBUG(1) << "Catch exeption: " << e << endl;
      ioss << "Catch exeption: " << e << endl;
    }
  catch( ... )
    {
      dgDEBUG(1) << "Catch unknown exception." << endl ;
      ioss << "Catch unknown exception." << endl ;
    }

  dgDEBUG(15) << "Copy the result." << endl;
  dgDEBUGF(15,"Data = %p.",os._data);
  CorbaServer::StringStreamer_var resCorba( new CorbaServer::StringStreamer );
  string res = ioss.str();
  resCorba->length( res.length() );
  for( unsigned int i=0;i<res.length();++i )
    { resCorba[i] = res[i]; }
  os=resCorba._retn();

  /* No memory to free in resCorba, free is done by the caller. */
  dgDEBUG(15) <<  "Return: (" << res.length() << ") "<< res <<endl;

  dgDEBUGOUT(15);
}


void ServerCommand::
readVector( const char * signalNameCorba,
	    CorbaServer::DoubleSeq_out value )
  throw(CORBA::SystemException)
{
  dgDEBUGIN(15);

  std::string buffer(signalNameCorba);
  std::istringstream sigName(buffer);

  try
    {
      SignalBase<int>& sigA = g_pool.getSignal( sigName );
      Signal<ml::Vector,int>& signal
	= dynamic_cast< Signal<ml::Vector,int>& >( sigA );

      const ml::Vector& data = signal.accessCopy();
      CorbaServer::DoubleSeq_var resCorba( new CorbaServer::DoubleSeq );
      resCorba->length( data.size() );
      for( unsigned int i=0;i<data.size();++i ){ resCorba[i]=data(i); }
      value=resCorba._retn();
    }
  catch( ExceptionFactory& e )
    {
      dgDEBUG(1) << "Catch exception " << e << std::endl;
      dgDEBUG(1) << "Signal "<<buffer<<" does not exist." << endl;
    }
  catch( std::bad_cast& e )
    { dgDEBUG(1) << "STD::bad_cast: signal "<<buffer
		  <<" is not of type Vector." << endl; }
  catch( ... )
    { dgDEBUG(1) << "Signal "<<buffer
		  <<" is not define of not of type Vector." << endl; }

  dgDEBUGOUT(15);
}

/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */

long int ServerCommand::
createOutputVectorSignal( const char * signalNameCorba )
  throw(CORBA::SystemException)
{
  dgDEBUGIN(15);

  std::string signalName(signalNameCorba);

  // Does the signal exist?
  typedef dg::SignalTimeDependent< ml::Vector,int > sig_t;
  std::vector<sig_t*>::iterator its =
    std::find_if(vectorSOUT.begin(), vectorSOUT.end(),
		 boost::bind(std::equal_to<std::string>(),
			     boost::bind(&sig_t::getName, _1),
			     boost::ref(signalName))
		 );

  // If the signal already exists, return it.
  if( its!=vectorSOUT.end() ) {
    long i = std::distance(vectorSOUT.begin(), its);
    return i;
  }

  dgDEBUG(15) << "Create output signal: <" << signalName <<">."<< std::endl;
  dg::SignalTimeDependent< ml::Vector,int > * newSignal
    = new dg::SignalTimeDependent< ml::Vector,int >
    ( signalName );
  newSignal->setDependencyType( TimeDependency<int>::BOOL_DEPENDENT );

  vectorSOUT.push_back( newSignal );
  signalRegistration( *newSignal );

  dgDEBUGOUT(15);
  /* Return the rank of the new signal in the std::vector. */
  return( vectorSOUT.size()-1 );
}

long int ServerCommand::
createInputVectorSignal( const char * signalNameCorba )
  throw(CORBA::SystemException)
{
  dgDEBUGIN(15);

  std::string signalName(signalNameCorba);

  // Does the signal exist?
  typedef dg::SignalPtr< ml::Vector,int > sig_t;
  std::vector<sig_t*>::iterator its =
    std::find_if(vectorSIN.begin(), vectorSIN.end(),
		 boost::bind(std::equal_to<std::string>(),
			     boost::bind(&sig_t::getName, _1),
			     boost::ref(signalName))
		 );

  // If the signal already exists, return it.
  if( its!=vectorSIN.end() ) {
    long i = std::distance(vectorSIN.begin(), its);
    return i;
  }

  dgDEBUG(15) << "Create input signal: <" << signalName <<">."<< std::endl;
  dg::SignalPtr< ml::Vector,int > * newSignal
    = new dg::SignalPtr< ml::Vector,int >
    ( NULL,signalName );

  vectorSIN.push_back( newSignal );
  signalRegistration( *newSignal );

  dgDEBUGOUT(15);
  /* Return the rank of the new signal in the std::vector. */
  return( vectorSIN.size()-1 );
}

void ServerCommand::
readInputVectorSignal( long int signalRankCorba,
		       CorbaServer::DoubleSeq_out value )
  throw(CORBA::SystemException)
{
  dgDEBUGIN(15);

  int signalRank = signalRankCorba ;
  dg::SignalPtr<ml::Vector,int>& signal = *vectorSIN[signalRank];

  const ml::Vector& data = signal.accessCopy();
  CorbaServer::DoubleSeq_var resCorba( new CorbaServer::DoubleSeq );
  resCorba->length( data.size() );
  for( unsigned int i=0;i<data.size();++i ){ resCorba[i]=data(i); }
  value=resCorba._retn();

  dgDEBUGOUT(15);
}


void ServerCommand::
writeOutputVectorSignal( long int signalRankCorba,
			 const CorbaServer::DoubleSeq& value )
  throw(CORBA::SystemException)
{
  dgDEBUGIN(15);

  int signalRank = signalRankCorba ;
  Signal<ml::Vector,int>& signal = *vectorSOUT[signalRank];

  ml::Vector data( value.length() );
  for( unsigned int i=0;i<value.length();++i ){ data(i)=value[i]; }
  signal = data;

  dgDEBUGOUT(15);
}

/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */

void ServerCommand::
registerClient(CorbaServer::NotifyCallback_ptr client,
               const char * clientName)
  throw(CORBA::SystemException)
{
  dgDEBUGIN(15);

  std::string name(clientName);
  CallbackList::iterator iter=callbackList.find(name);
  if( iter!=callbackList.end() )
    {
      CorbaServer::NotifyCallback_ptr listen = (iter->second);
      /* TODO: destroy the previous homonyme if any. */
      //listen->destroy();
      dgDEBUG(1) << "Client " << clientName << " already defined. Rebind." << std::endl;
    }

  callbackList[name] = CorbaServer::NotifyCallback::_duplicate(client);
  dgDEBUGOUT(15);
}


void ServerCommand::
unregisterClient(const char * clientName)
  throw(CORBA::SystemException)
{
  dgDEBUGIN(15);

  std::string name(clientName);
  CallbackList::iterator iter=callbackList.find(name);
  if( iter!=callbackList.end() )
    {
      CorbaServer::NotifyCallback_ptr listen = (iter->second);
      /* TODO: free the client reference. */
      //CorbaServer::NotifyCallback_Helper::release(listen);
    }
  else
    {
      dgDEBUG(1) << "Client " << clientName << " not bound yet. Cannot unregister." << std::endl;
    }

  callbackList.erase(name);

  dgDEBUGOUT(15);
}

/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */

void ServerCommand::
notifyClients( void )
{
  dgDEBUGIN(5);
  for( CallbackList::iterator iter=callbackList.begin();
       callbackList.end()!=iter;++iter )
    {
      dgDEBUG(15) << "Client." <<std::endl;
      CorbaServer::NotifyCallback_ptr listen = (iter->second);
      dgDEBUG(15) << "Client %p=" << listen<<std::endl;
      listen->notifyClient();
    }
  dgDEBUGOUT(5);
}


void ServerCommand::
notifyClient( const std::string & name )
{
  CallbackList::iterator iter=callbackList.find(name);
  if( iter==callbackList.end() )
    {
      dgERROR << "Client <" << name << "> not registred yet." << std::endl;
      return;
    }
  iter->second->notifyClient();
}


void ServerCommand::
displayClientList( std::ostream & os )
{
  dgDEBUGIN(5);
  for( CallbackList::iterator iter=callbackList.begin();
       callbackList.end()!=iter;++iter )
    {
      os << " - " << iter->first << std::endl;
    }
  dgDEBUGOUT(5);
}

/* -------------------------------------------------------------------------- */
/* --- SHELL SOT ------------------------------------------------------------ */
/* -------------------------------------------------------------------------- */


void ServerCommand::
commandLine( const std::string& cmdLine,std::istringstream& cmdArgs,
	     std::ostream& os )
{
  dgDEBUGIN(15);
  if( cmdLine == "help" )
    {
      os << "Class ServerCommand:\n";
      ServerAbstract::commandLine( cmdLine,cmdArgs,os );
    }
  else if( cmdLine == "start" )
    {
      registerCorbaServant(this);
    }
  else if( cmdLine == "initAndStart" )
    {
      std::string corbaContext; cmdArgs >> corbaContext;
      cmdArgs >> std::ws; if(! cmdArgs.good() ) corbaContext = "";
      initCorbaServer(corbaContext);

      registerCorbaServant(this);
    }
  else if( cmdLine == "ping" )
    {
      cmdArgs >> ws;
      if( cmdArgs.good() )
        {
          std::string name; cmdArgs >> name;
          notifyClient(name);
        }
      else
        {
          notifyClients();
        }
    }
  else if( cmdLine == "clients" )
    {
      displayClientList(os);
    }

  else
    {
      ServerAbstract::commandLine( cmdLine,cmdArgs,os );
    }
  dgDEBUGOUT(15);
}
