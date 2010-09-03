/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * Copyright Projet JRL-JAPAN, Tsukuba, 2007
 *+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 *
 * File:      coshell.cc
 * Project:   SOT
 * Author:    Nicolas Mansard from Torea Foissotte (test_corba_shell)
 *
 * Version control
 * ===============
 *
 *  $Id$
 *
 * Description
 * ============
 *
 *  Connection to a sotServerCommand, used as a relay to transmitt
 * shell instruction to the sotInterpretor running inside OpenHRP.
 *
 * ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#define VP_DEBUG
#define VP_DEBUG_MODE 45

#include <cassert>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/program_options.hpp>

#include <dynamic-graph/debug.h>

#include "CorbaConnection.h"

// IDL automatically generated file.
#include <server-command.h>
#include "connecttoshell.h"

using namespace dynamicgraph;

ConnectToShell::ConnectToShell (int argc, char* argv[])
  : serverPtr_ (),
    connection_ (std::string (argv[0]))
{
  namespace po = boost::program_options;
  po::options_description desc ("Allowed options");
  desc.add_options ()
    ("sot-service-name,c",
     po::value<std::string>()->default_value ("coshell"),
     "stack of tasks service name")
    ("sot-service-kind,k",
     po::value<std::string>()->default_value ("servant"),
     "stack of tasks servive kind")
    ("name-service-host,r",
     po::value<std::string>()->default_value (""),
     "name service host")
    ("name-service-port,p",
     po::value<int>()->default_value (-1),
     "name service port")
    
    ("help,h", "produce help message")
    ;
  
  po::variables_map vm;
  po::store (po::parse_command_line (argc, argv, desc), vm);
  po::notify (vm);
  
  if (vm.count ("help"))
    {
      std::stringstream ss;
      ss << "Usage: " << argv[0] << " [options]" << std::endl
	 << desc << std::endl
	 << "Report bugs to <hpp@laas.fr>" << std::endl;
      throw PrintUsage (ss.str ());
    }
  
  std::string sotServiceName = vm["sot-service-name"].as<std::string> ();
  std::string sotServiceKind = vm["sot-service-kind"].as<std::string> ();
  std::string nameServiceHost = vm["name-service-host"].as<std::string> ();
  int nameServicePort = vm["name-service-port"].as<int> ();
  
  displayCorbaInfo (sotServiceName, sotServiceKind,
		    nameServiceHost, nameServicePort);

  dgDEBUG(15) << "Connection ..." << std::endl;
  CORBA::Object_ptr corba_obj
    = connection_.connectToServant (sotServiceName,
				    sotServiceKind,
				    nameServiceHost,
				    nameServicePort);
  
  dgDEBUG(15) << "Connection [OK]. Cast ... " << std::endl;
  serverPtr_ =
    CorbaServer::SOT_Server_Command::_narrow (corba_obj);
  
  if(CORBA::is_nil (serverPtr_))
    throw std::runtime_error ("Failed to connect to the server.");
  dgDEBUG(1) << "Cast [OK]. Run the shell." << std::endl;
}


