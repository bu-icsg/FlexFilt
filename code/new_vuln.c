#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include "varanus.h"
#include <sys/syscall.h>

int pkey_mprotect(void *ptr, size_t size, unsigned long orig_prot,
              unsigned long pkey) {
  return syscall(SYS_pkey_mprotect, ptr, size, orig_prot, pkey);
}

int pkey_alloc(int init_val) {
  return syscall(SYS_pkey_alloc, 0, init_val);
}

// This function is never called in the main function, yet we choose
// the inputs to strcpy in a way that the program jumps to this function
void not_called() {
  printf("Enjoy the execution of not called function!\n");

  // komodo_wr uses the extended WRPKR instruction, which writes a
  // protection key value to a specific memory protection key in the
  // PKR (protection key register file)
  // Here, you can safely ignore the details of WRPKR insrtuction and
  // just assume that it is an unsafe insrtuction that we want to
  // prevent its execution in the default domain (untrusted domain).
  // As every instruction page by default belongs to the default domain
  // (domain0), which is untrusted, we expect FlexFilt to prevent the
  // execution of this instruction.
  komodo_wr(0x1, 0x4);

  // We do not want this line to be executed
  printf("Successful WRPKR instruction execution in not_called function, which belongs to the untrusted domain (domain0)\n");
}

// The vulnerable_function which uses strcpy without checking the size
// of string input
void vulnerable_function(char* string) {
  char buffer[100];
  strcpy(buffer, string);
}

extern void* good_code1(void);

int main(int argc, char** argv) {
  
  printf("Main function!\n");
  char name[100];
  // The first argument here is the 32-bit instruction value and the
  // second argument is the mask (please refer to Figure 1 of the
  // paper).
  // The third argument is the number of FlexibleFilter which is
  // getting configured
  // This function defines a filter on the WRPKR custom instruction
  // (0x20c5b02b) in the deafult domain (domain0) and assign this
  // filter to the FlexibleFilter0
  komodo_filter_inst(0x20c5b02b, 0x00000000, 0x0);

  // The first argument here is the domain number and the second
  // argument is the filter number
  // This function activates the FlexibleFilter0 in domain0.
  komodo_filter_valid(0x0, 0x0);

  // call good_code, which is allowed to execute the WRPKR exctended
  // instruction
  void (*ptr) (void);
  ptr = &good_code1;
  ptr();
  
  // print the address of not_called
  size_t i;
  void (*ptr_to_not_called)() = not_called;
  for (i=0; i<sizeof ptr_to_not_called; i++)
    printf("%.2x", ((unsigned char *)&ptr_to_not_called)[i]);
  putchar('\n');
  //
  
  gets(name);

  // call the vulnerable function
  vulnerable_function(name);


  return 0;
}
