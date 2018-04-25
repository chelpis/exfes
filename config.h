#ifndef FES_CONFIG_H
#define FES_CONFIG_H

#define HAVE_SSE2
#if __GNUC__
#if __x86_64__ || __ppc64__
#define HAVE_64_BITS
#endif
#endif

#endif 
