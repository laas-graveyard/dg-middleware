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

#ifndef __SOT_DISTANTSHELL_HH__
# define __SOT_DISTANTSHELL_HH__

# ifdef HAVE_LIBBOOST_THREAD
#  include <boost/thread.hpp>
# endif

# include <dynamic-graph/interpreter.h>
# include <dynamic-graph/contiifstream.h>
# include <dg-middleware/config.hh>


namespace dg = dynamicgraph;

class DG_MIDDLEWARE_DLLAPI DistantShell
{
 public:
  std::ofstream osOwn;
 protected:
  dg::Contiifstream filein;
  dg::Interpreter& shell;
  std::ostream & os;

#ifdef HAVE_LIBBOOST_THREAD
  boost::thread* threadLoop;
#else
  int* threadLoop;
#endif

  bool endLoop;

  unsigned int latency;
  static const unsigned int LATENCY_DEFAULT = 10;

 public:
  DistantShell(dg::Interpreter& shell_,const std::string& fn  )
    :osOwn(fn.c_str()),shell(shell_),os(osOwn)
    ,endLoop(false),latency(LATENCY_DEFAULT){ }

  DistantShell( const std::string& filename
		   ,dg::Interpreter& shell_
		   ,std::ostream& os_ )
    : filein( filename ),shell(shell_),os(os_)
    ,endLoop(false),latency(LATENCY_DEFAULT)
    {}

  DistantShell( const std::string& filename
		   ,dg::Interpreter& shell_
		   ,const std::string& fn )
    : osOwn(fn.c_str()),filein( filename ),shell(shell_),os(osOwn)
    ,endLoop(false),latency(LATENCY_DEFAULT)
    {}

  ~DistantShell( void );

  void inputFile (const std::string& filename)
  {
    filein = dg::Contiifstream (filename);
  }

  void loop( const int latency = LATENCY_DEFAULT ); // in ms
  void loopInThread( void );
  void terminate( void ) { endLoop = true; }
  void setLatency( const unsigned int& l ) { latency = l; }
};



#include <dynamic-graph/entity.h>

class DG_MIDDLEWARE_DLLAPI DistantShellPlugin
: public dg::Entity
{
 protected:
  DistantShell dshell;

 public:
  static const std::string CLASS_NAME;
  virtual const std::string& getClassName( void ) const { return CLASS_NAME; }

 public:
  DistantShellPlugin( const std::string& name );
  virtual ~DistantShellPlugin( void );

 public:
  virtual void commandLine( const std::string& cmdLine,std::istringstream& cmdArgs,
			    std::ostream& os );


};


#endif /* #ifndef __SOT_DISTANTSHELL_HH__ */




