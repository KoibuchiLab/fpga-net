/* internal_config.h -- characteristics of the platform, as probed by Cmake */

/* Warning: The file internal_config.h is AUTOMATICALLY GENERATED by Cmake.
 * Edit the template instead: tools/cmake/src/internal_config.h.in          */

/* Copyright (c) 2004-2018. The SimGrid Team. All rights reserved.          */

/* This program is free software; you can redistribute it and/or modify it
 * under the terms of the license (GNU LGPL) which comes with this package. */

#include "simgrid/config.h"       /* what was compiled in? */

/* Non-standard header files */
/* <execinfo.h> */
#define HAVE_EXECINFO_H 1
/* <linux/futex.h> */
#define HAVE_FUTEX_H 1
/* <signal.h> */
#define HAVE_SIGNAL_H 1
/* <unistd.h> */
#define HAVE_UNISTD_H 1
/* <ucontext.h> */
#define HAVE_UCONTEXT_H 1
/* <valgrind/valgrind.h> */
#define HAVE_VALGRIND_H 1

/* Address Sanitizer */
#define HAVE_SANITIZER_THREAD 0
#define HAVE_SANITIZER_ADDRESS 0
#define HAVE_SANITIZER_ADDRESS_FIBER_SUPPORT 0

/* Time portability */
/* Function gettimeofday */
#define HAVE_GETTIMEOFDAY 1
/* Function clock_gettime */
#define HAVE_POSIX_GETTIME 1
/* Function nanosleep */
#define HAVE_NANOSLEEP 1

/* The usable context factories */
#define HAVE_BOOST_CONTEXTS 1
#define HAVE_RAW_CONTEXTS 1
#define HAVE_THREAD_CONTEXTS 1
#define HAVE_UCONTEXT_CONTEXTS 1

/* Variables for the thread contexts (and parallel mode of raw contexts) */
/* Define to 1 if threads are usable . */
#define HAVE_PTHREAD 0
/* Does not seems defined on Mac nor Windows */
#define HAVE_PTHREAD_SETAFFINITY 0

/* Variables for the raw contexts (to select the right assembly code) */
#define SIMGRID_PROCESSOR_i686 0
#define SIMGRID_PROCESSOR_x86_64 1

/* Variables for the SysV contexts */
#define sg_makecontext_stack_addr(skaddr) ((skaddr))
#define sg_makecontext_stack_size(sksize) ((sksize))

/* Variable for SysV, raw and Boost contexts */
/* Does the stack growth upward, or downward? */
#define PTH_STACKGROWTH -1

/* MC variables */
/* getdtablesize: get descriptor table size */
#define HAVE_GETDTABLESIZE 1
/* Did we compile mmalloc in? */
#define HAVE_MMALLOC 0
/* process_vm_readv: transfer data between process address spaces */
#define HAVE_PROCESS_VM_READV 1
/* Set to true if enable_model-checking is true and the dependencies available */
#define SIMGRID_HAVE_MC 0

/* SMPI variables */
/* SMPI enabled */
#define HAVE_SMPI 1
/* Fortran language is available for SMPI, and which one */
#define SMPI_FORTRAN 1
#define SMPI_GFORTRAN 1
#define SMPI_FLANG 0
#define SMPI_IFORT 0
/* We have mmap and objdump to handle privatization */
#define HAVE_PRIVATIZATION 1
/* We have PAPI to fine-grain trace execution time */
#define HAVE_PAPI 0
/* We have sendfile to efficiently copy files for dl-open privatization */
#define HAVE_SENDFILE 1

/* Other function checks */
/* Function backtrace */
#define HAVE_BACKTRACE 1
/* Function dlfunc */
#define HAVE_DLFUNC 0
/* Function mmap */
#define HAVE_MMAP 1
/* Function mremap */
#define HAVE_MREMAP 1
/* Function sem_init (part of XPG6 standard only) */
#define HAVE_SEM_INIT 1
/* Function popen */
#define HAVE_POPEN 1
/* Function sysconf */
#define HAVE_SYSCONF 1
/* Function vasprintf */
#define HAVE_VASPRINTF 1

/* Other checks */
/* Path to the addr2line tool */
#define   ADDR2LINE "/usr/bin/addr2line"
/* The graphviz library */
#define HAVE_GRAPHVIZ 0
/* The lib unwind library (for MC and backtrace display) */
#define HAVE_LIBUNWIND 0
