
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdbool.h>
#include <unistd.h>

#include "cyclicBuf.h"
#include "gen.h"

int main() {
  int shmFd = shm_open("shared_buffer", O_RDONLY, 0); //file descriptor referring to the shared memory object only for reading
  if (shmFd == -1) {
    perror("Failed to open file");
    return -1;
  }

  struct cyclic_buffer* mem = mmap(NULL, sizeof(struct cyclic_buffer), PROT_READ, MAP_SHARED, shmFd, 0); //a pointer to the shared mapped area
  if (mem == MAP_FAILED) {
    perror("Failed to mmap");
    return -1;
  }

  uint64_t cur_pos = 0;
  printf("Starting at %lu\n", cur_pos);
  while (1) {
    if (mem-> current_position == cur_pos)
      continue;
    int64_t cur_seed = verify((void*)mem-> arr[cur_pos].arr);
    if (cur_seed == -1) {
      printf("Failed to verify");
      break;
    }
    printf("Verified at %lu with seed %lu\n", cur_pos, cur_seed);
    cur_pos++;
    cur_pos %= 1000;
  }

  return 0;
}
