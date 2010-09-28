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

/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
#define HPP_RETHROW(stringError) catch( ... ) { std::cerr << "(#" << __LINE__ << "): Error catched: " << stringError << std::endl; throw; }

template< class hppIdlReference >
void CorbaServerGeneric::
addIdl( hppIdlReference idlRef, const std::string & idlStrName )
{
  activateIdl(idlRef);
  bindIdl(idlRef,idlStrName);
}

template< class hppIdlReference >
void CorbaServerGeneric::
activateIdl( hppIdlReference idlRef )
{
  try {
    idlList.push_back( poa->activate_object( idlRef ) );
  }
  HPP_RETHROW("Binding idl");
}

template< class hppIdlReference >
void CorbaServerGeneric::
bindIdl( hppIdlReference idlRef, const std::string & idlStrName )
{
  CosNaming::Name idlName;
  CORBA::Object_var idlObj = idlRef->_this();

  try {
    idlName.length(1);
    idlName[0].id   = CORBA::string_dup(idlStrName.c_str());
    idlName[0].kind = CORBA::string_dup("servant");

    hppContext->bind(idlName,idlObj);
  }
  catch(CosNaming::NamingContext::AlreadyBound& ex)
    {
      std::cerr << "Warning naming context already bound" << std::endl;
      hppContext->rebind(idlName,idlObj);
    }
  HPP_RETHROW("Binding idl");
  /* Note: Using rebind() will overwrite any Object previously bound
   *       to /hpp/RobotConfig with localObj.
   *       Alternatively, bind() can be used, which will raise a
   *       CosNaming::NamingContext::AlreadyBound exception if the name
   *       supplied is already bound to an object. */
  /* Amendment: When using OrbixNames, it is necessary to first try bind
   * and then rebind, as rebind on it's own will throw a NotFoundexception if
   * the Name has not already been bound. [This is incorrect behaviour -
   * it should just bind]. */

  idlRef->_remove_ref(); /* Necessary after calling _this(). */
}
