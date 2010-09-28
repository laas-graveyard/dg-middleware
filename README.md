dg-middleware
=============

This software instruments dynamic-graph with CORBA to allow
distributed computation of the data-flow.

It provides a dynamic graph plug-in which can be loaded to start the
CORBA service inside a stack of task instance and a remote shell which
allows to control the data-flow remotely.


Setup
-----

To compile this package, it is recommended to create a separate build
directory:

    mkdir _build
    cd _build
    cmake [OPTIONS] ..
    make install

Please note that CMake produces a `CMakeCache.txt` file which should
be deleted to reconfigure a package from scratch.
