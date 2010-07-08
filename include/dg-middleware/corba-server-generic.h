
#include <list>
#include <iostream>
#include <string>
#include <omniORB4/CORBA.h>



class CorbaServerGeneric
{

 public:
  CorbaServerGeneric( const std::string & contextName );
  ~CorbaServerGeneric( void );

 public:
  void init( void );
  template< class IdlReference >
    void addIdl( IdlReference idl, const std::string & idlStrName );
  void run( bool onlyOnce=false );
  void stop( void );

 protected:
  CORBA::ORB_var orb;
  CosNaming::NamingContext_var hppContext;
  PortableServer::POA_var poa;

  typedef PortableServer::ObjectId* IdlId;
  typedef std::list<IdlId> IdlList;
  IdlList idlList;

  std::string serverName;

  //protected:
  public:

  void initOrb(void);
  void createThreadPolicy( bool isMultiThread = true );
  void createContext( const std::string & contextStrName );
  void activateContext( void );
  template< class IdlReference >
    void activateIdl( IdlReference idl);
  template< class IdlReference >
    void bindIdl( IdlReference idl, const std::string & idlStrName );

};


#include "corba-server-generic.t.cpp"

