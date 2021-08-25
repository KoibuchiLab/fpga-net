/* simgrid/config.h - Results of the configure made visible to user code.   */

/* Copyright (c) 2009-2018. The SimGrid Team. All rights reserved.          */

/* This program is free software; you can redistribute it and/or modify it
 * under the terms of the license (GNU LGPL) which comes with this package. */

#ifndef SIMGRID_PUBLIC_CONFIG_H
#define SIMGRID_PUBLIC_CONFIG_H
#include <xbt/base.h>

/** Define the version numbers of the used header files.
  See sg_version_get() to retrieve the version of the dynamic library. */
#define SIMGRID_VERSION_MAJOR 3
#define SIMGRID_VERSION_MINOR 21
#define SIMGRID_VERSION_PATCH 0
#define SIMGRID_GIT_VERSION   "e011a997e0"
#define SIMGRID_INSTALL_PREFIX "/home/aaa10078nj/fpga_network/SimGrid-3.21/build"

SG_BEGIN_DECL()
/** Retrieves the version numbers of the used dynamic library (so, DLL or dynlib), while
    SIMGRID_VERSION_MAJOR and friends give the version numbers of the used header files */
XBT_PUBLIC void sg_version_get(int* major, int* minor, int* patch);

/** Display the version information and some additional blurb. */
XBT_PUBLIC void sg_version();
SG_END_DECL()


/* Version as a single integer. v3.4 is 30400, v3.16.2 is 31602, v42 will be 420000, and so on. */
#define SIMGRID_VERSION (100UL * (100UL * (SIMGRID_VERSION_MAJOR) + (SIMGRID_VERSION_MINOR)) + (SIMGRID_VERSION_PATCH))

#define SIMGRID_VERSION_STRING "SimGrid version 3.21"

/* Was Jedule compiled in?  */
#define SIMGRID_HAVE_JEDULE 0
/* Was the Lua support compiled in? */
#define SIMGRID_HAVE_LUA 0
/* Were mallocators (object pools) compiled in? */
#define SIMGRID_HAVE_MALLOCATOR 1
/* Was the model-checking compiled in? */
#define SIMGRID_HAVE_MC 0
/* Was the NS3 support compiled in? */
#define SIMGRID_HAVE_NS3 0

#endif /* SIMGRID_PUBLIC_CONFIG_H */