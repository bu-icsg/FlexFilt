#define _GNU_SOURCE

#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/mman.h>
#include <stdio.h>
#include "varanus.h"

int
pkey_mprotect(void *ptr, size_t size, unsigned long orig_prot,
              unsigned long pkey)
{
  return syscall(SYS_pkey_mprotect, ptr, size, orig_prot, pkey);
}
int
pkey_alloc(int flags, int init_val)
{
  return syscall(SYS_pkey_alloc, flags, init_val);
}
int
pkey_free(unsigned long pkey)
{
  return syscall(SYS_pkey_free, pkey);
}

static int count = 0;
static int count2 = 0;

#define errExit(msg)    do { perror(msg); exit(EXIT_FAILURE); \
  } while (0)

void good_code(int * buffer) {
  void (*original_good_code) (int * buffer);
  printf("good code is preloading --- count: %d\n", count);
  original_good_code = dlsym(RTLD_NEXT,"good_code");

  if (count == 0) {

    int ipkey = pkey_alloc(0x1, 0x1);

    if (ipkey == -1)
      errExit("ipkey_alloc");
   
    unsigned long long addr = &(*original_good_code);
    addr = (addr >> 12) << 12;

    int status = pkey_mprotect((void *) addr, getpagesize(), PROT_EXEC | PROT_READ, ipkey);

    if (status == -1)
      errExit("pkey_mprotect");

  }
  original_good_code(buffer);
  count++;
}

void good_code2(int * buffer) {
  void (*original_good_code2) (int * buffer);
  printf("good code2 is preloading --- count: %d\n", count2);
  original_good_code2 = dlsym(RTLD_NEXT,"good_code2");

  if (count2 == 0) {
    unsigned long long addr = &(*original_good_code2);
    addr = (addr >> 12) << 12;

    int status = pkey_mprotect((void *) addr, getpagesize(), PROT_EXEC | PROT_READ, 0x1);
    if (status == -1)
      errExit("pkey_mprotect");

  }
  original_good_code2(buffer);
  count2++;
}
