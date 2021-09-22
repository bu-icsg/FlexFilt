#define _GNU_SOURCE

#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/mman.h>
#include "varanus.h"

int pkey_mprotect(void *ptr, size_t size, unsigned long orig_prot,
              unsigned long pkey) {
  return syscall(SYS_pkey_mprotect, ptr, size, orig_prot, pkey);
}

int pkey_alloc(int flags, int init_val) {
  return syscall(SYS_pkey_alloc, flags, init_val);
}

static int count = 0;
static int count2 = 0;

void good_code1(void) {
  void (*original_good_code1) (void);

  printf("good code1 is preloading --- count: %d\n", count);
  original_good_code1 = dlsym(RTLD_NEXT,"good_code1");

  if (count == 0) {
    // If this is the first time executing good_code1, allocate an
    // instruction protection key.
    int ipkey = pkey_alloc(0x1, 0x1);

    if (ipkey == -1)
      errExit("ipkey_alloc");

    // Extract the address of the good_code1 function
    unsigned long long addr = &(*original_good_code1);
    addr = (addr >> 12) << 12;

    // Assign the instruction protection key to the corresponding page
    // of the good_code1
    int status = pkey_mprotect((void *) addr, getpagesize(), PROT_EXEC | PROT_READ, ipkey);

    if (status == -1)
      errExit("pkey_mprotect");

  }
  count++;
  // Run the original good code1
  original_good_code1();
}

void good_code2(void) {
  void (*original_good_code2) (void);
  printf("good code2 is preloading --- count: %d\n", count2);
  original_good_code2 = dlsym(RTLD_NEXT,"good_code2");

  if (count2 == 0) {
    // For simplicity, in this simple program we assume good_code1
    // always executes before good_coode2;  hence, the instruction
    // protection key has already been allocated. No need to allocate
    // a new instruction protection key.
    // As both good_code1 and good_code2 are trusted functions, we
    // assign the same instruction protection key to both of them.
    // Here, to make the test much simpler, we assume that the
    // instruction protection key allocated by good_code1 is 1. 
    unsigned long long addr = &(*original_good_code2);
    addr = (addr >> 12) << 12;

    // Assign the instruction protection key to the corresponding page
    // of the good_code
    int status = pkey_mprotect((void *) addr, getpagesize(), PROT_EXEC | PROT_READ, 0x1);
    if (status == -1)
      errExit("pkey_mprotect");

  }
  count2++;

  // Run the original good code2
  original_good_code2();
}
