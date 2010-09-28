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

/**
\mainpage

\section intro_dg-middleware Introduction

The dynamic-graph middleware package contains all 'middleware' components that go
on top of dynamic-graph and allow distant access to entities.

This includes:
- The ServerCommand entity (load both server-abstract.so and server-command.so)
  This entity allows Corba connections into it, and executions of commands through
  the commandLine (see server-command.h for the list of supported functions)
- The DistantShell entity (distant-shell.so) is a server-side entity that executes
  files remotely.
- The coshell program (installs in bin/sot) can be used to connect to the server remotely.


\section How to use this package
If you wish to expose a corba server to your potential clients, simply load and create the
ServerCommand entity in your library.

Programs who wish to connect and execute commands (using the default
scripting language) to your server can use the coshell executable (installed in $(PREFIX)/bin)

If you want to customize the ServerCommand behaviour, define a new entity that
 derives from the server-abstract entity.

*/
