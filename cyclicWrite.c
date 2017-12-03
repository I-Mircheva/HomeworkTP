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
  int shmFd = shm_open("shared_buffer", O_CREAT | O_RDWR, S_IRWXU);
  if (shmFd == -1) {
    perror("Can't open file");
    return 1;
  }

  int res;
  res = ftruncate(shmFd, sizeof(struct cyclic_buf));
  if (res == -1) {
    perror("Can't truncate file");
    return res;
  }

  struct cyclic_buf* mem = mmap(NULL, sizeof(struct cyclic_buf), PROT_READ | PROT_WRITE, MAP_SHARED, shmFd, 0);
  if (mem == NULL) {
    perror("Can't mmap");
    return -1;
  }

  mem->pos = 0;
  uint32_t seed = 0;
  while (true) {
    generate((void*)mem->BUFFER[mem->pos].arr, seed);
    seed++;
    mem->pos++;
    mem->pos %= 1000;
  }

  return 0;
}
