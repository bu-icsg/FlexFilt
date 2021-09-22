#include <stdio.h>
#include "varanus.h"

extern void* good_code1(void);
extern void* good_code2(void);

int main() {

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

  // call good_code1, which is in the trusted domain (domain1) when
  // executed by LD_PRELOAD and is allowed to execute the extended
  // WRPKR instruction. Here, we want to show that a trusted domain is
  // able to execute the target instruction.
  void (*ptr) (void);
  ptr = &good_code1;
  ptr();

  // call good_code2, which is in the trusted domain (domain1) when
  // executed by LD_PRELOAD and is allowed to execute the extended
  // WRPKR instruction. Here, we want to show that at rusted domain is
  // able to execute the target instruction.
  void (*ptr2) (void);
  ptr2 = &good_code2;
  ptr2();

  printf("Execute WRPKR extended instruction (0x20c5b02b) in the default domain (domain0)\n");
  // komodo_wr uses the extended WRPKR instruction, which writes a
  // protection key value to a specific memory protection key in the
  // PKR (protection key register file)
  // Here, you can safely ignore the details of WRPKR insrtuction and
  // just assume that it is an unsafe insrtuction that we want to
  // prevent its execution in the default domain (untrusted domain).
  // As every instruction page by default belongs to the default domain
  // (domain0), we expect FlexFilt to prevent the execution of this
  // instruction.
  komodo_wr(0x1, 0x4);

  // We do not want this line to be executed
  printf("Successful WRPKR instruction execution in untrusted domain (domain0)\n");

  return 0;
}
