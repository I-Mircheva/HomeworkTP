#ifndef _DUMB_SHM_STRUCT
#define _DUMB_SHM_STRUCT

#include <stdint.h>
struct chunk_t {
  volatile int arr[4096];
};
struct cyclic_buf {
  volatile uint64_t pos;
  volatile struct chunk_t BUFFER[1000];
};
#endif
