
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
#include "gen.c"

int main() {
  int shmFd = shm_open("shared_buffer", O_CREAT | O_RDWR, S_IRWXU);//created new file descriptor referring to the shared memory object for reading and writing
  if (shmFd == -1) {
    perror("Failed to open file");
    return shmFd;
  }

  if (ftruncate(shmFd, sizeof(struct cyclic_buffer)) == -1) { //Set size of the shm obj
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
    generate((void*)mem-> twoDarr[mem->current_position], seed);
    seed++;
    mem->current_position++;
    mem->current_position %= 1000;
  }

  return 0;
}
  // Hints:
  // O_CREAT Create the shared memory object if it does not exist.  The
  //user and group ownership of the object are taken from the
  //corresponding process, and
  //the object's permission bits are set according to the low-
  //order 9 bits of mode, except that those bits set in the
  //process file mode creation mask are cleared
  //for the new object.  A set of macro constants which can be
  //used to define mode is listed in open(2).

  //A new shared memory object initially has zero length—the
  //size of the object can be set using ftruncate(2).  The
  //newly allocated bytes of a shared memory object are
  //automatically initialized to 0.
