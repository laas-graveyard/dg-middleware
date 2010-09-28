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

#ifndef __SOT_CORBA_REFERENCE_HH_
#define __SOT_CORBA_REFERENCE_HH_

#include <string>

#ifdef OMNIORB4
#include <omniORB4/CORBA.h>
#endif /* OMNIORB4 */


class CORBAReference
{
 public:

  /*! \brief Default constructor */
  CORBAReference();

  /*! \brief Default destructor */
  ~CORBAReference();

  /*! \brief Initialize the ORB. */
  bool InitORB();

  /*! \brief Returns the reference to a CORBA object. */
  CORBA::Object_ptr getObjectReference(std::string ServerID,
				       std::string ServerKind);

  /*! \brief setting the hostname name service. */
  bool SetNameServiceHostname(std::string &TheHostName);

  /*! \brief getting the name service hostname. */
  const std::string & GetNameServiceHostname() const;

  /*! \brief setting the port number. */
  bool SetPortNumber(unsigned short &PortNumber);

  /*! \brief getting the port number. */
  const unsigned short & GetPortNumber() const;

  /*! \brief Initialize the root context. */
  void InitRootContext();

  /*! \brief Returns the root context. */
  const CosNaming::NamingContext_var & getRootContext() const;

  /*! \brief Returns the ORB. */
  const CORBA::ORB_var & getORB() const;

 private:

  /*! \brief Identifier to the Object Request Broker. */
  CORBA::ORB_var m_orb;

  /*! \brief Store the port number. */
  unsigned short m_PortNumber;

  /*! \brief Store the hostname. */
  std::string m_HostName;

  /*! \brief Store the context. */
  CosNaming::NamingContext_var m_rootContext;

};

#endif /* _SOT_CORBA_REFERENCE_HH_ */
