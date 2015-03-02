#ifndef MACRO_INLINE_H_
#define MACRO_INLINE_H_

#ifdef __STDC_VERSION__
#  if __STDC_VERSION__ >= 199900L
#    define INLINE inline
#  else
#    define INLINE
#  endif
#else
#  define INLINE
#endif

#endif
