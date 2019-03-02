#ifndef INTEL8080TEST_H
#define INTEL8080TEST_H

#include <string>

#include "Intel8080.h"

class Intel8080Test
{
  Intel8080 *cpu;
  int failed = 0;
  int passed = 0;

  public:
    Intel8080Test(Intel8080 *cpu);

    void prepare(void);
    void loadAndExecute(std::vector<uint8_t> prog);
    void perform(void);
    void fail(std::string message);
    void pass(std::string message);

    void op_cmc_test(void);
    void op_stc_test(void);
    void op_inr_test(void);
    void op_dcr_test(void);
    void op_cma_test(void);
};

#endif
