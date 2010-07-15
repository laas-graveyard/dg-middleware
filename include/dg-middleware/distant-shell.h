/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * Copyright Projet JRL-Japan, 2007
 *+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 *
 * File:      DistantShell.h
 * Project:   SOT
 * Author:    Nicolas Mansard
 *
 * Version control
 * ===============
 *
 *  $Id$
 *
 * Description
 * ============
 *
 *
 * ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/



#ifndef __SOT_DISTANTSHELL_HH__
#define __SOT_DISTANTSHELL_HH__

/* --------------------------------------------------------------------- */
/* --- INCLUDE --------------------------------------------------------- */
/* --------------------------------------------------------------------- */


#include <dynamic-graph/interpreter.h>
#include <dynamic-graph/contiifstream.h>
//#include <pthread.h>

#ifdef HAVE_LIBBOOST_THREAD
#include <boost/thread.hpp>
#endif

/* --------------------------------------------------------------------- */
/* --- API ------------------------------------------------------------- */
/* --------------------------------------------------------------------- */

#if defined (WIN32) 
#  if defined (distant_shell_EXPORTS)
#    define DistantShell_EXPORT __declspec(dllexport)
#  else  
#    define DistantShell_EXPORT __declspec(dllimport)
#  endif 
#else
#  define DistantShell_EXPORT
#endif

namespace dg = dynamicgraph;

/* --------------------------------------------------------------------- */
/* --- CLASS ----------------------------------------------------------- */
/* --------------------------------------------------------------------- */

class DistantShell_EXPORT DistantShell
{
 public:
  std::ofstream osOwn;
 protected:
  dg::Contiifstream filein;
  dg::Interpreter& shell;
  std::ostream & os;
/*   pthread_t loopThread; */
/*   pthread_attr_t loopThreadAttr; */

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

  void inputFile( const std::string& filename ) { filein = filename; }

  void loop( const int latency = LATENCY_DEFAULT ); // in ms
  void loopInThread( void );
  void terminate( void ) { endLoop = true; }
  void setLatency( const unsigned int& l ) { latency = l; }
};



#include <dynamic-graph/entity.h>

class DistantShell_EXPORT DistantShellPlugin
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




