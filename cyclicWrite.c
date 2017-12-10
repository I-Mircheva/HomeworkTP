
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <stdbool.h>

#include "cyclicBuf.h"
#include "gen.h"

int main() {
  int shmFd = shm_open("shared_buffer", O_CREAT | O_RDWR, S_IRWXU);//created new file descriptor referring to the shared memory object for writing
  if (shmFd == -1) {
    perror("Failed to open file");
    return -1;
  }

  if (ftruncate(shmFd, sizeof(struct cyclic_buffer)) == -1) {
    perror("Failed to truncate file");
    return -1;
  }

  struct cyclic_buffer* mem = mmap(NULL, sizeof(struct cyclic_buffer), PROT_READ | PROT_WRITE, MAP_SHARED, shmFd, 0); //a pointer to the shared mapped area for read and write
  if (mem == NULL) {
    perror("Failed to mmap");
    return -1;
  }

  mem->current_position = 0;
  uint32_t seed = 0;
  while (true) {
    generate((void*)mem-> arr[mem->current_position].arr, seed);
    seed++;
    mem->current_position++;
    mem->current_position %= 1000;
  }

  return 0;
}
