
#ifndef _MY_STRUCT
#define _MY_STRUCT

#include <stdint.h>

struct cyclic_buffer {
  volatile uint64_t current_position;
  volatile int twoDarr[1000][4096];
};
#endif
