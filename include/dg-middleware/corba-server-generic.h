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

#ifndef DG_MIDDLEWARE_CORBA_SERVER_GENERIC_H
# define DG_MIDDLEWARE_CORBA_SERVER_GENERIC_H
# include <list>
# include <iostream>
# include <string>

# include <dg-middleware/corba.hh>

class CorbaServerGeneric
{

 public:
  CorbaServerGeneric( const std::string & contextName );
  ~CorbaServerGeneric( void );

 public:
  void init( void );
  template< class IdlReference >
    void addIdl( IdlReference idl, const std::string & idlStrName );
  void run( bool onlyOnce=false );
  void stop( void );

 protected:
  CORBA::ORB_var orb;
  CosNaming::NamingContext_var hppContext;
  PortableServer::POA_var poa;

  typedef PortableServer::ObjectId* IdlId;
  typedef std::list<IdlId> IdlList;
  IdlList idlList;

  std::string serverName;

  //protected:
  public:

  void initOrb(void);
  void createThreadPolicy( bool isMultiThread = true );
  void createContext( const std::string & contextStrName );
  void activateContext( void );
  template< class IdlReference >
    void activateIdl( IdlReference idl);
  template< class IdlReference >
    void bindIdl( IdlReference idl, const std::string & idlStrName );

};


# include "corba-server-generic.t.cpp"
#endif //! DG_MIDDLEWARE_CORBA_SERVER_GENERIC_H
