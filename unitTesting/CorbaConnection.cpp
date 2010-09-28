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

#include "CorbaConnection.h"

#include <iostream>
#include <string>

#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>

const std::string
CorbaConnection::DEFAULT_OMNIORB_NAMESERVICE_HOST = "localhost";

namespace
{
  std::string prettyPrintNameServiceHost (const std::string& host);
  std::string prettyPrintNameServicePort (int port);

  std::string prettyPrintNameServiceHost (const std::string& host)
  {
    return (host.empty ()) ? "default" : host;
  }

  std::string prettyPrintNameServicePort (int port)
  {
    return (port < 0)
      ? std::string ("default") : boost::lexical_cast<std::string> (port);
  }
} // end of anonymous namespace.

CORBA::Object_ptr CorbaConnection::
connectToServant(const std::string& m_ServerName,
		 const std::string& m_ServerKind,
		 const std::string& m_HostNameServer,
		 int nameServicePort)
{
  boost::format errorMsg
    ("%1% (host = %2%, port = %3%).");

  std::string nameServiceHost = m_HostNameServer;

  // Connect to the name server.
  try {

    if (nameServicePort < 0 && m_HostNameServer.empty ())
      {
	int argc = 1;
	char* argv[] = {const_cast<char*> (progname_.c_str ())};
	m_orb = CORBA::ORB_init (argc, argv);
      }
    else
      {
	if (nameServicePort < 0)
	  nameServicePort = DEFAULT_OMNIORB_NAMESERVICE_PORT;
	if (nameServiceHost.empty ())
	  nameServiceHost = DEFAULT_OMNIORB_NAMESERVICE_HOST;

	boost::format fmt ("NameService=corbaloc:iiop:%d:%d/NameService");
	fmt % nameServiceHost % nameServicePort;

	int argc = 1;
	char* argv[] =
	  {
	    const_cast<char*> (progname_.c_str ()),
	    "-ORBInitRef",
	    const_cast<char*> (fmt.str ().c_str ())
	  };
	m_orb = CORBA::ORB_init (argc, argv);
      }

    if (CORBA::is_nil (m_orb))
      throw "TODO";
  }
  catch (...)
    {
      errorMsg
	% "Failed to connect to the name server"
	% prettyPrintNameServiceHost (nameServiceHost)
	% prettyPrintNameServicePort (nameServicePort);
      std::cerr << errorMsg.str () << std::endl;
      return CORBA::Object::_nil ();
    }

  // Activate the poa.
  CORBA::Object_var root_poa = m_orb->resolve_initial_references ("RootPOA");
  poa = PortableServer::POA::_narrow (root_poa);
  poa->the_POAManager ()->activate ();

  // Obtain a reference to the root context of the Name service.
  try
    {
      rootContext =
	CosNaming::NamingContext::_narrow
	(m_orb->resolve_initial_references ("NameService"));
      if (CORBA::is_nil (rootContext))
	throw "TODO";
    }
  catch (...)
    {
      errorMsg
	% "Failed to narrow the root naming context"
	% prettyPrintNameServiceHost (nameServiceHost)
	% prettyPrintNameServicePort (nameServicePort);
      std::cerr << errorMsg.str () << std::endl;
      return CORBA::Object::_nil();
    }

  // Create a name object, containing the name test/context.
  name.length (2);
  name[0].id   = "sot";
  name[0].kind = "context";
  name[1].id   = m_ServerName.c_str();
  name[1].kind = m_ServerKind.c_str();

  return resolve ();
}

CORBA::Object_ptr CorbaConnection::
resolve( void )
{
  try
    {
      return rootContext->resolve(name);
    }
  catch (CosNaming::NamingContext::NotFound& ex)
    {
      std::cerr << "Context not found." << std::endl;
    }
  catch(CORBA::COMM_FAILURE& ex)
    {
      std::cerr
	<< "Caught system exception COMM_FAILURE "
	<< "-- unable to contact the naming service." << std::endl;
    }
  catch(CORBA::SystemException&)
    {
      std::cerr
	<< "Caught a CORBA::SystemException while using the naming service."
	<< std::endl;
    }
  std::cerr
    << "...failed to find the Server " << name[0].id << std::endl;
  return CORBA::Object::_nil();
}
