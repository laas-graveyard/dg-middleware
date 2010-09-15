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
