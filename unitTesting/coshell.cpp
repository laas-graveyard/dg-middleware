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

class coshell__INIT
{
public:
  coshell__INIT ()
  {
    DebugTrace::openFile("/tmp/coshell.txt");
  }
};
coshell__INIT coshell_initiator;


static const char* CLIENT_NAME = "coshell";

class CoShell : public ConnectToShell
{
public:
  CoShell (int argc, char* argv[])
    : ConnectToShell(argc,argv)
  {
  }

  int run ()
  {
    NotifyCallback_coshell listener;
    std::cout << "Local test : "; listener.notifyClient ();


    CorbaServer::NotifyCallback_var client = listener._this ();
    if (CORBA::is_nil (client))
      throw std::runtime_error ("Argument is not a good reference");
    serverPtr_->registerClient (client, CLIENT_NAME);

    CorbaServer::StringStreamer_var output;

    const std::string prompt ("> ");

    while (std::cin.good () && !std::cin.eof ())
      {
	std::cout << prompt;

	std::string buffer;
	std::getline (std::cin, buffer);

	dgDEBUG(5) << buffer.size () << ": " << buffer << std::endl;

	if (buffer.empty ())
	  continue;
	if (buffer == "exit")
	  break;

	// Send the command to the distant server and print the result.
	if (!CORBA::is_nil (serverPtr_))
	  {
	    try
	      {
		serverPtr_->runAndRead (buffer.c_str (), output);
		for (unsigned int i = 0; i < output->length (); ++i)
		  std::cout << output[i];
	      }
	    catch (...)
	      {
		std::cerr
		  << "!!\tException catched when trying to communicate! ... rebind"
		  << std::endl;
		serverPtr_ =
		  CorbaServer::SOT_Server_Command::_narrow
		  (connection_.reconnectToServant ());
		serverPtr_->runAndRead (buffer.c_str (), output);
		for (unsigned int i = 0; i < output->length (); ++i)
		  std::cout << output[i];
	      }
	  }
	else
	  std::cerr << "!!\tCorba not plug. Nothing sent."<< std::endl;
      }
    std::cout << std::endl;
    return 0;
  }

  ~CoShell ()
  {
    serverPtr_->unregisterClient (CLIENT_NAME);
  }

};


int main (int argc, char* argv[])
{
  namespace po = boost::program_options;

  try
    {
      CoShell coshell (argc, argv);
      return coshell.run ();
    }
  catch (PrintUsage& printUsage)
    {
      std::cout << printUsage.usage_ << std::endl;
      return 0;
    }
  catch (std::exception& error)
    {
      std::cerr << error.what () << std::endl;
      return 1;
    }
  catch(CORBA::Exception& exception)
    {
      boost::format fmt
	("A CORBA exception has been raised (exception name: ``%1%'').");
      fmt % exception._name ();
      std::cerr << fmt.str () << std::endl;
      return 1;
    }
  catch (...)
    {
      std::cerr
	<< "Unexpected exception catched. Aborting..."
	<< std::endl;
      return -1;
    }
  return 0;
}
