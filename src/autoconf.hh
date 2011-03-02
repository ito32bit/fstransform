/*
 * autoconf.hh
 *
 * defines compiler features.
 * ideally, this file should be generated by a './configure' script from autotools
 *
 *  Created on: Feb 27, 2011
 *      Author: max
 */

#ifndef FSTRANSFORM_AUTOCONF_HH
#define FSTRANSFORM_AUTOCONF_HH

#include "check.hh"


/** define if compiler supports the types (long long) and (unsigned long long) */
#ifndef FT_HAVE_LONG_LONG
#  if defined(__GNUC__)
#    define FT_HAVE_LONG_LONG
#  endif
#endif /* FT_HAVE_LONG_LONG */



/** define to compiler's own version of 'inline' keyword */
#ifndef FT_INLINE
#  if defined(__cplusplus)
     /* inline is standard keyword in C++ */
#    define FT_INLINE inline
#  elif defined(__GNUC__)
     /* prefer __inline__ to inline, the former works even in gcc strict-ansi mode */
#    define FT_INLINE __inline__
#  else
#    define FT_INLINE
#  endif
#endif /* FT_INLINE */



/**
 * define if extern "C" { ... } is understood by the compiler and needed to get C linkage.
 * should be defined in C++, and undefined in C.
 */
#ifndef FT_HAVE_EXTERN_C
#  if defined(__cplusplus)
#    define FT_HAVE_EXTERN_C
#  endif
#endif /* FT_HAVE_EXTERN_C */


/**
 * define if namespace Foo { ... } is understood by the compiler.
 * should be defined in C++, and undefined in C.
 */
#ifndef FT_HAVE_NAMESPACE
#  if defined(__cplusplus)
#    define FT_HAVE_NAMESPACE
#  endif
#endif /* FT_HAVE_NAMESPACE */


/**
 * define if C++ compiler supports g++-style explicit template instantiation:
 * extern template ...<>; // declaration
 * template ...<>;        // instantiation
 */
#ifndef FT_HAVE_EXTERN_TEMPLATE
#  if defined(__cplusplus) && defined(__GNUC__)
#    define FT_HAVE_EXTERN_TEMPLATE
#  endif
#endif /* FT_HAVE_EXTERN_TEMPLATE */


#endif /* FSTRANSFORM_AUTOCONF_HH */
