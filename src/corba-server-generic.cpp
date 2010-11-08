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

#include <cstdlib>
#include <dg-middleware/corba-server-generic.h>

/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
CorbaServerGeneric::
CorbaServerGeneric(  const std::string & name )
  : serverName(name)
{
}

CorbaServerGeneric::
~CorbaServerGeneric( void )
{
}

/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
void CorbaServerGeneric::
init( void )
{
  initOrb();
  createThreadPolicy();
  createContext( serverName );
  activateContext();
}

/* Call the init of ORB. */
void CorbaServerGeneric::
initOrb( void )
{
  /* --- ORB init. --- */
  try
    {
      int argc = 1;
      char* arg0 = strdup("CorbaServerGeneric");
      char *argv[] = {arg0};
      orb = CORBA::ORB_init(argc, argv);
      free (arg0);
      if(CORBA::is_nil(orb))
        {
          std::cerr << "CorbaServer: failed to initialize ORB" << std::endl;
          throw "TODO";
        }

    } HPP_RETHROW("ORB init.");
}

/** Make the CORBA object single-threaded to avoid GUI crash. */
/* Create a single threaded policy object. */
void CorbaServerGeneric::
createThreadPolicy(bool)
{
  try
    {

      poa = PortableServer::POA
        ::_narrow(orb->resolve_initial_references("RootPOA"));



//       PortableServer::POA_var rootPoa = PortableServer::POA
//         ::_narrow(orb->resolve_initial_references("RootPOA"));

//       PortableServer::ThreadPolicy_var threadPolicy;
//       if (inMultiThread)
//         {threadPolicy = rootPoa->create_thread_policy(PortableServer::ORB_CTRL_MODEL);}
//       else
//         {threadPolicy = rootPoa->create_thread_policy(PortableServer::MAIN_THREAD_MODEL);}

//       /* Copy the object in the list. */
//       CORBA::PolicyList policyList;
//       policyList.length(1);
//       policyList[0] = PortableServer::ThreadPolicy::_duplicate(threadPolicy);
//       poa = rootPoa->create_POA("child",
//                                 PortableServer::POAManager::_nil(),
//                                 policyList);

//       /* ThreadPolicy was duplicate in policyList -> destroy. */
//       threadPolicy->destroy();
//       /* Note: is it necessary to duplicate? Just build in policyList? */
    } HPP_RETHROW("Thread policy");
}

/** Bind a context to the corba root, with a given name.
 */
void CorbaServerGeneric::
createContext( const std::string & contextStrName )
{

  try {

    /* Obtains a reference to the root context of the Name service
     * and narrows the reference returned. */
    CosNaming::NamingContext_var rootContext = CosNaming::NamingContext::
      _narrow(orb->resolve_initial_references("NameService"));
    if( CORBA::is_nil(rootContext) )
      {
        std::cerr << "Failed to narrow the root naming context." << std::endl;
        throw "TODO";
      }

    /* Create the name of the context. */
    CosNaming::Name contextName;
    contextName.length(1);
    contextName[0].id   = CORBA::string_dup(contextStrName.c_str());
    contextName[0].kind = CORBA::string_dup("context");
    /* Note on kind: The kind field is used to indicate the type
     * of the object. This is to avoid conventions such as that used
     * by files (name.type -- e.g. hpp.ps = postscript etc.). */

    /* Bind a context called <contextName> to the root context. */
    try{ hppContext = rootContext->bind_new_context(contextName); }
    catch(CosNaming::NamingContext::AlreadyBound& ex)
      {
        /* If the context already exists, this exception will be raised.
         * In this case, just resolve the name and assign hppContext
         * to the object returned. */
        hppContext = CosNaming::NamingContext::
          _narrow(rootContext->resolve(contextName));
        if( CORBA::is_nil(hppContext) )
          {
            std::cerr << "Failed to narrow naming context." << std::endl;
            throw "TODO";
          }
      }
  } HPP_RETHROW("Creating context");

  return;
}


void CorbaServerGeneric::
activateContext( void )
{
  poa->the_POAManager()->activate();
}

/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
/** \brief If CORBA requests are pending, process them.
 */
void CorbaServerGeneric::
run(bool onlyOnce)
{
  if( onlyOnce )
    {
      if (orb->work_pending())  { orb->perform_work(); }
    }
  else
    { /* Enter in the Corba control loop. Never return. */
      orb->run();
    }
}

/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
void CorbaServerGeneric::
stop( void )
{
  for( IdlList::iterator iter=idlList.begin();iter!=idlList.end();++iter )
    {
      std::cout << __LINE__ << std::endl;
      IdlId ref = *iter;
      std::cout << __LINE__ << std::endl;
      poa->deactivate_object(*ref);
      std::cout << __LINE__ << std::endl;
    }
  std::cout << __LINE__ << std::endl;
  idlList.clear();
  std::cout << __LINE__ << std::endl;

  orb->shutdown(0);
  std::cout << __LINE__ << std::endl;
}

