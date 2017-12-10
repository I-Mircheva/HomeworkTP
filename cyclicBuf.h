
#ifndef _MY_STRUCT
#define _MY_STRUCT

#include <stdint.h>
struct chunk_t {
  volatile int arr[4096];
};

struct cyclic_buffer {
  volatile uint64_t current_position;
  volatile struct chunk_t arr[1000];
};
#endif
