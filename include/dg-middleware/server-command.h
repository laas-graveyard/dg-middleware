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

struct tsData
{
  double ts;
  std::vector<double> data;
};

typedef struct s_dataStack
{
  std::vector<tsData> stackOfTsData;
  unsigned int index;
  bool turnaround;

  s_dataStack()
  { index=0; turnaround=false;}

  void inc()
  {
    index++;
    if (index==stackOfTsData.size())
      {
	index=0;
	turnaround=true;
      }
  }
} dataStack;

class ServerCommand
:public ServerAbstract
,virtual public POA_CorbaServer::SOT_Server_Command
  //  ,virtual public PortableServer::RefCountServantBase
{
 protected:
  std::vector< dg::SignalTimeDependent< ml::Vector,int >* > vectorSOUT;
  std::vector< dg::SignalPtr< ml::Vector,int >* > vectorSIN;
  std::vector< dataStack > vectorSINStored;
  std::map<std::string, int> vectorName2Rank;

  void bufferSignal(std::istringstream & iss);

 public: /* --- CONSTRUCTION --- */
  ServerCommand( const std::string& name );
  virtual ~ServerCommand( void );
 protected:
   /*! \brief Signal to buffer input values.
     This signal should be called periodically in order to
     store data. To store a signal data, send to ParseCmd:
     buffer signalname sizeofbuffer
     sizeofbuffer should be an integer.
    */
   double & synchroValue(double &aValue, int time);

   int &OneInternalStep(int &dummy, int time);

  public: /* --- OUTPUT SIGNAL --- */
   /*! \brief Signal existing only to store data. */
   dg::SignalTimeDependent<double, int> synchroSOUT;
   dg::SignalTimeDependent<int, int> OneInternalStepS;
   /* --- INPUT SIGNAL --- */
   /*! \brief Real state position values. */
   dg::SignalPtr<ml::Vector,int> jointPositionSIN;


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
  void  readSeqInputVectorSignal(const CorbaServer::SeqOfRank& signalRanks,
  				 CorbaServer::SeqOfDoubleSeq_out values)
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
