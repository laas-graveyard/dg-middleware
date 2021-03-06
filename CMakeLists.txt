# Copyright 2010, François Bleibel, JRL, CNRS/AIST
#
# This file is part of dg-middleware.
# dg-middleware is free software: you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public License
# as published by the Free Software Foundation, either version 3 of
# the License, or (at your option) any later version.
#
# dg-middleware is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Lesser Public License for more details.  You should have
# received a copy of the GNU Lesser General Public License along with
# dg-middleware. If not, see <http://www.gnu.org/licenses/>.

CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

INCLUDE(cmake/base.cmake)
INCLUDE(cmake/boost.cmake)
INCLUDE(cmake/pthread.cmake)
INCLUDE(cmake/cpack.cmake)
# Meta-information.
SET(PROJECT_NAME dg-middleware)
SET(PROJECT_DESCRIPTION "Dynamic graph distributed computation and control")
SET(PROJECT_URL "http://github.com/jrl-umi3218/dg-middleware")

SET(CUSTOM_HEADER_DIR "${PROJECT_NAME}")

# Disable -Werror on Unix for now.
SET(CXX_DISABLE_WERROR True)

SETUP_PROJECT()

SET(PKG_CONFIG_ADDITIONAL_VARIABLES
  ${PKG_CONFIG_ADDITIONAL_VARIABLES}
  plugindirname
  plugindir
  )

# Search for Boost.
SET(BOOST_COMPONENTS program_options thread)
SEARCH_FOR_BOOST()
ADD_DEFINITIONS(-DHAVE_LIBBOOST_THREAD)

# Search for Pthread.
SEARCH_FOR_PTHREAD()

# Search for dynamic-graph.
ADD_REQUIRED_DEPENDENCY("dynamic-graph >= 1.0")

# Search for jrl-mal
ADD_REQUIRED_DEPENDENCY("jrl-mal >= 1.0")

# Search for omniORB.
ADD_REQUIRED_DEPENDENCY("omniORB4 >= 4.1.1")

# Add extra keys to pkg-config file to export the location of the
# installed IDL file.
SET(IDLDIR share/idl/dg-middleware)
SET(PKG_CONFIG_EXTRA "idldir=${CMAKE_INSTALL_PREFIX}/${IDLDIR}")

ADD_SUBDIRECTORY(include)
ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(doc)
ADD_SUBDIRECTORY(unitTesting)

SETUP_PROJECT_FINALIZE()
SETUP_PROJECT_CPACK()