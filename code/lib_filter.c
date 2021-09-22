#include <stdio.h>
#include "varanus.h"

void good_code1(void) __attribute__ ((section ("separate")));

void good_code1(void) {
  // good_code1 has been assigned to domain1; hence, it is allowed to
  // execute the WRPKR custom instruction which is used in komodo_wr.
  // komodo_wr updates the permission bits of PKR register but here
  // the deatils are not important. We just assume this is an unsafe
  // instruction that FlexFilt has to allow its execution in trusted
  // domain1.
  komodo_wr(0x1, 0x4);
  printf("Executed the WRPKR extended instruction (0x20c5b02b) in good code1, which belongs to the trusted domain1\n");
}

void good_code2(void) __attribute__ ((section ("separate")));

void good_code2(void) {
  // good_code2 has been assigned to domain1; hence, it is allowed to
  // execute the WRPKR custom instruction which is used in komodo_wr.
  // komodo_wr updates the permission bits of PKR register but here
  // the deatils are not important. We just assume this is an unsafe
  // instruction that FlexFilt has to allow its execution in trusted
  // domain1.
  // We have two separate functions here in  the same domain, just to
  // show more than one function can simply belong to the same
  // trusted domain.
  komodo_wr(0x1, 0x4);
  printf("Executed the WRPKR extended instruction (0x20c5b02b) in good code2, which belongs to the trusted domain1\n");
}
