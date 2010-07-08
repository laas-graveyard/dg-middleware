/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * Copyright Projet JRL-Japan, 2008
 *+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 *
 * File:      CORBAReference.h (from DecisionByHFSM)
 * Project:   SOT
 * Author:    Olivier Stasse
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
