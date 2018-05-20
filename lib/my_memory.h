// my_memory.h provides
//
// function declarations:
//      exfes_calloc

#ifndef MY_MEMORY_H_INCLUDED
#define MY_MEMORY_H_INCLUDED

#include <stdlib.h>  // size_t

void *exfes_calloc(size_t num, size_t size, size_t max_num_retries);

#endif  // MY_MEMORY_H_INCLUDED
