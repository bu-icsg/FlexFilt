#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include "varanus.h"
#include <sys/syscall.h>

extern void* good_code(int*);
extern void* good_code2(int*);

#define komodo_wr(pkey, perm) \
  ROCC_INSTRUCTION_0_R_R(XCUSTOM_KOMODO, ((uint64_t) pkey), ((uint64_t) perm), 16, 11, 12);

#define errExit(msg)    do { perror(msg); exit(EXIT_FAILURE); \
  } while (0)


int main() {

  komodo_filter_inst(0x20c5b02b, 0x00000000, 0x0);
  komodo_filter_valid(0x0, 0x0);
  
  int *buffer;
  int status;
  buffer = mmap(NULL, 2*getpagesize(), PROT_READ | PROT_WRITE,
                MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
  if (buffer == MAP_FAILED)
    return -1;
  *buffer = __LINE__;

  void (*ptr) (int*);
  ptr = &good_code;
  ptr(buffer);

  void (*ptr2) (int*);
  ptr2 = &good_code2;
  ptr2(buffer);

  printf("About to write to domain in untrusted area\n");
  komodo_wr(0x1, 0x4);

  printf("Successful write in untrusted area\n");  
  
  return 0;
}
