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

// IDL automatically generated file.
#include <dg-middleware/server-command.h>

#ifndef _CONNECT_TO_SOT_H_
#define _CONNECT_TO_SOT_H_
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

class ConnectToShell
{
 public:
	ConnectToShell (int argc, char* argv[]);
  ~ConnectToShell (){};
  virtual int run ()=0;

 protected:
  CorbaServer::SOT_Server_Command_var serverPtr_;
  CorbaConnection connection_;
};


#endif /* _CONNECT_TO_SOT_H_ */
