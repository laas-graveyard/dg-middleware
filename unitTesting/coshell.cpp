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
 *  Connection to a sotServerCommand, used as a relay to transmit
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
  catch (...)
    {
      std::cerr
	<< "Unexpected exception catched. Aborting..."
	<< std::endl;
      return -1;
    }
  return 0;
}
