#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdbool.h>
#include <unistd.h>
#include "cyclicBuf.h"
#include "gen.h"
int main() {
  int shmFd = shm_open("shared_buffer", O_RDONLY, 0);
  if (shmFd == -1) {
    perror("Can't open file");
    return 1;
  }

  struct cyclic_buf* mem = mmap(NULL, sizeof(struct cyclic_buf), PROT_READ, MAP_SHARED, shmFd, 0);
  if (mem == NULL) {
    perror("Can't mmap");
    return -1;
  }

  uint64_t cur_pos = 0;
  printf("starting at %ld\n", cur_pos);
  while (true) {
    while (mem->pos == cur_pos) {
    };
    int64_t cur_seed = verify((void*)mem->BUFFER[cur_pos].arr);
    if (cur_seed == -1) {
      printf("Error!");
      break;
    }
    printf("Verified at %d with seed %d\n", cur_pos, cur_seed);
    cur_pos++;
    cur_pos %= 1000;
  }

  return 0;
}
