#include "my_memory.h"
#include <stdlib.h>

void *exfes_calloc(size_t num, size_t size, size_t max_num_retries)
{
    while (1) {
        void *p = calloc(num, size);
        if (p)
            return p;
        if (max_num_retries == 0)
            return 0;
        max_num_retries -= 1;
    }
}
