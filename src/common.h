#ifndef _OTHELLO_COMMON_H_
#define _OTHELLO_COMMON_H_

#ifdef WIN32
#define inline
#endif
#ifdef __APPLE__
#define NULL 0
#define inline
#endif

#ifdef __c99
#  include <stdbool.h>
#  define Bool   _Bool
#else
#  define Bool   char
#  define false  (char) 0
#  define true   (char) 1
#endif

#define MIN(a,b)      ( (a) < (b) ? (a) : (b) )
#define MAX(a,b)      ( (a) > (b) ? (a) : (b) )

#define CLAMP(a,b,c)  ( (a) < (b) ? MIN (b,c) : (a) )

#define PI            (3.141592654)     /* (acos (-1.0))    */
#define RAD(a)	      ((a) * 0.01745329252)     /* ((a) * PI / 180) */

#endif                          /* _OTHELLO_COMMON_H_ */
