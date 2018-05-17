#include <stdlib.h>

void *mycalloc(size_t num, size_t size, int retry) {
  //if (rand() % 100 == 0)
    //return NULL;
  void *ret;
  while (retry > 0) {
    ret = calloc(num, size);
    if (ret == NULL)
      retry -= 1;
    else
      retry = 0;
  }
  return ret;
}
