#ifndef _SOT_CORBACLIENT_H_
#define _SOT_CORBACLIENT_H_

#include <string>

#include <omniORB4/CORBA.h>

/*! Connection to a servant in the <sot.context> context.
 *
 */
class CorbaConnection
{
 public:
  // Please notice that changing these values may engage your physical
  // integrity ;).
  static const int DEFAULT_OMNIORB_NAMESERVICE_PORT = 2809;
  static const std::string DEFAULT_OMNIORB_NAMESERVICE_HOST;

 protected:
  std::string progname_;
  CORBA::ORB_ptr m_orb;
  PortableServer::POA_var poa;
  CosNaming::NamingContext_var rootContext;
  CosNaming::Name name;

 public:

  /// \brief Main constructor.
  ///
  /// The argument specifies the current program name (i.e. argv[0]).
  /// It is used for human-display only and can be initialized to any string.
  CorbaConnection (const std::string& progname)
    : progname_ (progname)
    {}

  /// \brief Connect to CORBA service.
  ///
  /// Connects to CORBA service.
  ///
  /// This supports two differents mode:
  /// - either no host/port is given to resolve the name service
  ///   and it is done using the local default omniORB configuration
  ///   (usually through the omniORB.cfg file).
  /// - or if a host or port is given, the default configuration is overrided
  ///   and the specified server hosts/ports are used.
  ///   Default values are provided when specifying only one value
  ///  (either port or host).
  ///   * Host: localhost
  ///   * Port: 2809
  ///
  /// See in the omniORB manual
  /// (http://omniorb.sourceforge.net/omni41/omniORB/omniORB004.html#toc22)
  /// the InitRef setting to configure the location of the NameService.
  CORBA::Object_ptr connectToServant
    (const std::string& m_ServerName,
     const std::string& m_ServerKind,
     const std::string& m_HostNameServer = std::string (),
     int nameServicePort = -1);

  CORBA::Object_ptr reconnectToServant( void ) { return resolve(); }

 protected:
  CORBA::Object_ptr resolve( void );
};

#endif /* _SOT_CORBACLIENT_H_ */
