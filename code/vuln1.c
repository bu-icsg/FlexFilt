//$QEMU64 -g 1234 -L $RISCV/sysroot vuln1.exe "$(python -c 'print "A"*0x6c + "BBBB" + "\x1c\xdd\x02"')"

// 0x6c buffersize + 1 byte
// old SP addr BBBB
// not_called addr: 0x10138

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include "varanus.h"
#include <sys/syscall.h>
extern void* good_code(int*);


#define komodo_wr(pkey, perm) \
  ROCC_INSTRUCTION_0_R_R(XCUSTOM_KOMODO, ((uint64_t) pkey), ((uint64_t) perm), 16, 11, 12);

#define errExit(msg)    do { perror(msg); exit(EXIT_FAILURE); \
  } while (0)

int
pkey_set(unsigned long pkey, unsigned long rights, unsigned long flags)
{
  //unsigned int pkru = (rights << (2 * pkey));
  komodo_pkru_wr(pkey, rights);
  return 0;
}
int
pkey_mprotect(void *ptr, size_t size, unsigned long orig_prot,
              unsigned long pkey)
{
  return syscall(SYS_pkey_mprotect, ptr, size, orig_prot, pkey);
}
int
pkey_alloc(int init_val)
{
  return syscall(SYS_pkey_alloc, 0, init_val);
}
int
pkey_free(unsigned long pkey)
{
  return syscall(SYS_pkey_free, pkey);
}

extern void* good_code(int*);

void not_called() {
    printf("Enjoy your shell!\n");
    komodo_wr(0x1, 0x4);
}

void vulnerable_function(char* string) {
    char buffer[100];
    strcpy(buffer, string);
}

int main(int argc, char** argv) {
    printf("Main function!\n");
    char name[100];


    komodo_filter_inst(0x20c5b02b, 0x00000000, 0x0);
    komodo_filter_valid(0x0, 0x0);
  
    int *buffer;
    int status;
    buffer = mmap(NULL, 2*getpagesize(), PROT_READ | PROT_WRITE,
                  MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    if (buffer == MAP_FAILED)
      return -1;
    *buffer = __LINE__;
    printf("buffer contains: %d\n", *buffer);

    void (*ptr) (int*);
    ptr = &good_code;
    ptr(buffer);

    
    //print the address of not_called
    size_t i;
    void (*ptr_to_not_called)() = not_called;
    for (i=0; i<sizeof ptr_to_not_called; i++)
      printf("%.2x", ((unsigned char *)&ptr_to_not_called)[i]);
    putchar('\n');
    //////////
    gets(name);
    vulnerable_function(name);

    printf("write to domain in untrusted area\n");
    //komodo_wr(0x1, 0x4);
  
    *buffer = __LINE__;
    ptr(buffer);

    return 0;
}
