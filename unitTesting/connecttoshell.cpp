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
  try
    {
      serverPtr_ =
	CorbaServer::SOT_Server_Command::_narrow (corba_obj);
    }
  catch (CORBA::TRANSIENT& exception)
    {
      std::cerr << "Failed to connect to the stack of tasks." << std::endl
		<< "1. Double check that the server is started." << std::endl
		<< "2. Does the server and client version match?" << std::endl
		<< std::endl
		<< "Minor code: " << exception.minor () << std::endl;
      throw;
    }
  
  if(CORBA::is_nil (serverPtr_))
    throw std::runtime_error ("Failed to connect to the server.");
  dgDEBUG(1) << "Cast [OK]. Run the shell." << std::endl;
}


