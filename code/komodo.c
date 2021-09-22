#include "varanus.h"

void komodo_filter_inst(uint64_t inst, uint64_t mask, uint64_t index) {
  ROCC_INSTRUCTION_0_R_R(XCUSTOM_KOMODO, ((uint64_t) inst), ((uint64_t) index), 25, 11, 12);
  ROCC_INSTRUCTION_0_R_R(XCUSTOM_KOMODO, ((uint64_t) mask), ((uint64_t) index), 26, 11, 12);
}

void komodo_filter_valid(uint64_t index, uint64_t v_index) {
  ROCC_INSTRUCTION_0_R_R(XCUSTOM_KOMODO, ((uint64_t) index), ((uint64_t) v_index), 27, 11, 12);
}

