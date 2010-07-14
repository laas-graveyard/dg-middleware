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

/* --- CORBA IDL --- */
#include <server-command-corba.h>

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

class NotifyCallback_coshell
  : public virtual POA_CorbaServer::NotifyCallback
{
public:
  ~NotifyCallback_coshell ()
  {
    std::cout << "Destroyed." << std::endl;
  }
  virtual void notifyClient ()
  {
    std::cout << "Notified!" << std::endl;
  }
};

static const char* CLIENT_NAME = "coshell";

namespace
{
  void displayCorbaInfo (const std::string& sotServiceName,
			 const std::string& sotServiceKind,
			 const std::string& nameServiceHost,
			 int nameServicePort);

  void displayCorbaInfo (const std::string& sotServiceName,
			 const std::string& sotServiceKind,
			 const std::string& nameServiceHost,
			 int nameServicePort)
  {
    boost::format corbaInfo
      (
       "Corba information:\n"
       "\tStack of tasks service name: %1%\n"
       "\tStack of tasks service kind: %2%\n"
       "\tName service host:           %3%\n"
       "\tName service port:           %4%\n"
       );
    corbaInfo
      % sotServiceName
      % sotServiceKind
      % ((nameServiceHost.empty ())
	 ? "default" : nameServiceHost.c_str ())
      % ((nameServicePort < 0)
	 ? "default" : boost::lexical_cast<std::string> (nameServicePort));
    std::cout << corbaInfo.str () << std::endl;
  }
} // end of anonymous namespace.


class PrintUsage
{
public:
  PrintUsage (const std::string& usage)
    : usage_ (usage)
  {}

  const std::string usage_;
};

class CoShell
{
public:
  CoShell (int argc, char* argv[])
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

  ~CoShell ()
  {
    serverPtr_->unregisterClient (CLIENT_NAME);
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

private:
  CorbaServer::SOT_Server_Command_var serverPtr_;
  CorbaConnection connection_;
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
