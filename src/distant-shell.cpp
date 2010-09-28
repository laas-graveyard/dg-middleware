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

#include <dg-middleware/distant-shell.h>
#include <dynamic-graph/debug.h>

#include <dynamic-graph/factory.h>
using namespace dynamicgraph;

#ifdef WIN32
#include <Windows.h>
#endif

DistantShell::
~DistantShell( void )
{
  dgDEBUGIN(5);
  endLoop = true;
  
#ifdef HAVE_LIBBOOST_THREAD
  threadLoop->join();
#endif

  //usleep(1000*1000);
  dgDEBUGOUT(5);
}


void DistantShell::
loop( const int lat )
{
  dgDEBUGIN(15);
  while(! endLoop )
    {
      dgDEBUG(45) <<  lat <<std::endl;
      if( filein.loop() )
	{
	  while( filein.ready() )
	    {
	      std::istringstream iss( filein.next() );
	      std::string cmdLine; 
	      dgDEBUG(35) << cmdLine << std::endl;
	      iss>>cmdLine;
	      shell.cmd( cmdLine,iss,os );
	    }
	}
#ifndef WIN32
	usleep( lat*1000 );
#else	  
	Sleep(lat);
#endif
    }
  dgDEBUGOUT(15);
}

void* 
__staticLoop( void* autoref )
{
  DistantShell* ref = static_cast<DistantShell *>( autoref );
  ref->loop();
  return NULL;
}

void DistantShell::
loopInThread( void )
{
  dgDEBUGIN(5);
#ifdef HAVE_LIBBOOST_THREAD
//   pthread_attr_init(&loopThreadAttr);
//   pthread_create(&loopThread,&loopThreadAttr,__staticLoop,this);
//  threadLoop = new boost::thread( boost::bind( &DistantShell::loop,this ) );
  threadLoop = new boost::thread (boost::bind( &DistantShell::loop,this,
					       latency ));
#else
  #ifndef WIN32
  #warning Unable to compile with libboost_thread
  #else
  #pragma message ( "Unable to compile with libboost_thread\n" ) 
  #endif
  dgDEBUG(5) << "No boost:thread. Unable to launch in thread." <<std::endl;
#endif
  dgDEBUGOUT(5);
}


/* --------------------------------------------------------------------- */
/* --------------------------------------------------------------------- */
/* --------------------------------------------------------------------- */



DYNAMICGRAPH_FACTORY_ENTITY_PLUGIN(DistantShellPlugin,"DShell");

using namespace std;

DistantShellPlugin::
DistantShellPlugin( const std::string& name )
  : Entity(name)
    , dshell( g_shell,"/tmp/nmansard/"+name )
{
  dgDEBUGINOUT(5);
}


DistantShellPlugin::
~DistantShellPlugin( void )
{
  dgDEBUGINOUT(5);
}


void DistantShellPlugin::
commandLine( const std::string& cmdLine,
	     std::istringstream& cmdArgs,
	     std::ostream& os )
{

  dgDEBUGIN(15) << "CMD = " << cmdLine <<endl;

  if( cmdLine == "help") 
    {
      os << "distant shell: "<<endl
	 << "  - open <file>" << endl
	 << "  - start" << endl
	 << "  - stop" << endl
	 << "  - latency <uint>" << endl;
    }
  else if( cmdLine == "open" )
    {
      std::string f; cmdArgs >> f; 
      dshell.inputFile(f);
    }
  else if( cmdLine == "start" )
    {
      dshell.loopInThread();
    }
  else if( cmdLine == "stop" )
    {
      dshell.terminate();
    }
  else if( cmdLine == "latency" )
    {
      unsigned int l; cmdArgs >> l;
      dshell.setLatency(l);
    }
  else
    Entity::commandLine( cmdLine,cmdArgs,os );
  
  dgDEBUGOUT(15);

}

