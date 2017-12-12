
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdbool.h>
#include <unistd.h>

#include "cyclicBuf.h"
#include "gen.h"
#include "gen.c"

int main() {
  int shmFd = shm_open("shared_buffer", O_RDONLY, 0); //file descriptor referring to the shared memory object only for reading
  if (shmFd == -1) {
    perror("Failed to open file");
    return shmFd;
  }

  struct cyclic_buffer* mem = mmap(NULL, sizeof(struct cyclic_buffer), PROT_READ, MAP_SHARED, shmFd, 0); //a pointer to the shared mapped area
  if (mem == MAP_FAILED) {
    perror("Failed to mmap");
    return -1;
  }

  uint64_t cur_pos = 0 , prev_seed = -1; // so it can be 0 when compering to verify
  printf("Starting at %lu\n", cur_pos);
  while (1) {
    if (mem-> current_position == cur_pos){
      sleep(1);//waits for the other p2
      continue;
    }
    if(mem-> current_position >= cur_pos + 1000){
      printf("Error: Position is %d", mem-> current_position);
      break; //in case of p2 in front of p1
    }
    int64_t cur_seed = verify((void*)mem-> twoDarr[cur_pos]);
    if (cur_seed == -1 || prev_seed + 1 != cur_seed /*or || (prev_seed != 0 && prev_seed + 1 != cur_seed)*/) {
      printf("Failed to verify");
      break;
    }
    prev_seed = cur_seed;
    printf("Verified at %lu with seed %lu\n", cur_pos, cur_seed);
    cur_pos++;
    // cur_pos %= 1000; //Will use cur_pos for tracking the position even after overflow
  }

  return 0;
}
