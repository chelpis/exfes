#include "my_memory.h"
#include <stdlib.h>

void *exfes_calloc(size_t num, size_t size, size_t num_max_retries) {
  void *p = calloc(num, size);
  if (p)
    return p;
  if (num_max_retries == 0)
    return 0;
  return exfes_calloc(num, size, num_max_retries - 1);
}
