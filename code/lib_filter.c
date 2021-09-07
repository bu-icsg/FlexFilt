#include <stdio.h>
#include <stdlib.h>
#include "varanus.h"

#define komodo_wr(pkey, perm) \
  ROCC_INSTRUCTION_0_R_R(XCUSTOM_KOMODO, ((uint64_t) pkey), ((uint64_t) perm), 16, 11, 12);

void good_code(int * buffer) __attribute__ ((section ("separate")));

void good_code(int * buffer) {
  komodo_wr(0x1, 0x4);
  printf("wrote to domain 0x1 in good code1\n");
}

void good_code2(int * buffer) __attribute__ ((section ("separate")));

void good_code2(int * buffer) {
  komodo_wr(0x1, 0x4);
  printf("wrote to domain 0x1 in good code2\n");
}
