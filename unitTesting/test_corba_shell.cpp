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

// IDL automatically generated file.
#include <dg-middleware/server-command.h>

using namespace dynamicgraph;

static const char* CLIENT_NAME = "test_corba_shell";

class TestCoShell : public ConnectToShell
{
public:
  TestCoShell (int argc, char* argv[])
    : ConnectToShell(argc,argv)
  {
  }

  int sendBuffer(std::string &buffer)
  {
    if (buffer.empty ())
      return -1;

    CorbaServer::StringStreamer_var output;

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
	      << "!!\tException catched when trying to send:"
	      << buffer
	      << "! \n... rebind"
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

  int run ()
  {
    NotifyCallback_coshell listener;
    std::cout << "Local test : "; listener.notifyClient ();


    CorbaServer::NotifyCallback_var client = listener._this ();
    if (CORBA::is_nil (client))
      throw std::runtime_error ("Argument is not a good reference");
    serverPtr_->registerClient (client, CLIENT_NAME);

    CorbaServer::StringStreamer_var output;

    long int lIVS=0;

    if (!CORBA::is_nil (serverPtr_))
      {
	try {lIVS = serverPtr_->createInputVectorSignal("comstate"); }
	catch(...)
	  {
	    std::cerr << "Unable to create comstate input signal inside coshell." << std::endl;
	    exit(-1);
	  }
      }
    std::string buffer;
    buffer = "plug pg.comref coshell.comstate";
    sendBuffer(buffer);
    std::cout << "Send " << buffer << " command " << std::endl;

    buffer = "coshell.buffer comstate 30";
    sendBuffer(buffer);
    std::cout << "Send " << buffer << " command " << std::endl;

    buffer = "OpenHRP.periodicCall addSignal coshell.synchro";
    sendBuffer(buffer);
    std::cout << "Send " << buffer << " command " << std::endl;

    buffer = "plug OpenHRP.state coshell.position";
    sendBuffer(buffer);
    std::cout << "Send " << buffer << " command " << std::endl;

    sleep(1);

    CorbaServer::DoubleSeq_var bufferOfValues;

    if (!CORBA::is_nil (serverPtr_))
      {
	try {serverPtr_->readInputVectorSignal(lIVS,bufferOfValues); }
	catch(...)
	  {
	    std::cerr << "Unable to read values from comstate." << std::endl;
	    exit(-1);
	  }
      }

    std::cout << "Data to read: " << bufferOfValues->length() <<std::endl;
    sleep(1);
    for(unsigned int i=0;i<bufferOfValues->length();i++)
      {
	std::cout << bufferOfValues[i] << std::endl;
      }

    return 0;
  }

  ~TestCoShell ()
  {
    serverPtr_->unregisterClient (CLIENT_NAME);
  }

};


int main (int argc, char* argv[])
{
  namespace po = boost::program_options;

  try
    {
      TestCoShell testcoshell (argc, argv);
      return testcoshell.run ();
    }
  catch (PrintUsage& printUsage)
    {
      std::cout << printUsage.usage_ << std::endl;
      return 1;
    }
  catch (std::exception& error)
    {
      std::cerr << error.what () << std::endl;
      return 1;
    }
  catch (...)
    {
      std::cerr
	<< "Unexpected exception catched. Aborting..."
	<< std::endl;
      return 1;
    }
  return 1;
}
