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

SET(BUILD_DIR ${CMAKE_BINARY_DIR}/doc)

SET(DOCDIR share/doc/${PROJECT_NAME})
SET(HTMLDIR ${DOCDIR}/doxygen-html)

# Add `doc' target.
ADD_CUSTOM_TARGET(
  doc ALL
  DEPENDS doxygen-html/index.html dg-coshell.1.gz)


# Generate Doxygen configuration file.
CONFIGURE_FILE(package.dox.cmake ${BUILD_DIR}/package.dox)

# Generating the Doxygen documentation
ADD_CUSTOM_COMMAND(
  OUTPUT doxygen-html/index.html
  COMMAND ${DOXYGEN_EXECUTABLE} ${BUILD_DIR}/package.dox
  COMMENT Generating Doxygen documentation
)

# Doxygen documentation installation rules.
INSTALL(DIRECTORY ${BUILD_DIR}/doxygen-html DESTINATION ${DOCDIR})
INSTALL(FILES ${BUILD_DIR}/${PROJECT_NAME}.doxytag DESTINATION ${HTMLDIR})
INSTALL(DIRECTORY pictures DESTINATION ${HTMLDIR})


# Man page generation.
FIND_PROGRAM(POD2MAN pod2man)
CONFIGURE_FILE(dg-coshell.pod.in ${BUILD_DIR}/dg-coshell.pod @ONLY)

ADD_CUSTOM_COMMAND(
  OUTPUT dg-coshell.1
  COMMAND ${POD2MAN} --section=1
                     --center="LOCAL USER COMMANDS"
                     --release ${PROJECT_NAME} dg-coshell.pod
          > dg-coshell.1
  DEPENDS ${BUILD_DIR}/dg-coshell.pod)

ADD_CUSTOM_COMMAND(
  OUTPUT dg-coshell.1.gz
  COMMAND gzip -c dg-coshell.1 > dg-coshell.1.gz
  DEPENDS ${BUILD_DIR}/dg-coshell.1)

INSTALL(FILES ${BUILD_DIR}/dg-coshell.1.gz DESTINATION share/man/man1)
