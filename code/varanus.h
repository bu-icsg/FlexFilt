#ifndef __VARANUS_H__
#define __VARANUS_H__

#include <stdint.h>
#include <inttypes.h>

#include "../../../rocket-rocc-examples/rocc-software/src/xcustom.h"

#define XCUSTOM_KOMODO 1

#define komodo_wr(pkey, perm) \
  ROCC_INSTRUCTION_0_R_R(XCUSTOM_KOMODO, ((uint64_t) pkey), ((uint64_t) perm), 16, 11, 12);

#define errExit(msg)    do { perror(msg); exit(EXIT_FAILURE); \
  } while (0)

void komodo_filter_inst(uint64_t inst, uint64_t mask, uint64_t index);
void komodo_filter_valid(uint64_t index, uint64_t v_index);

#endif
