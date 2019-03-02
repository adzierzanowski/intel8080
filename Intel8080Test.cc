#include "Intel8080Test.h"

Intel8080Test::Intel8080Test(Intel8080* cpu) : cpu(cpu)
{
}

void Intel8080Test::prepare(void)
{
  cpu->setProgramCounter(0x100);
  //cpu->debugOutput = false;
}

void Intel8080Test::loadAndExecute(std::vector<uint8_t> prog)
{
  cpu->loadProgram(prog, 0x100);
  cpu->setProgramCounter(0x100);
  cpu->execute();
}

void Intel8080Test::perform(void)
{
  prepare();

  op_cmc_test();
  op_stc_test();
  op_inr_test(); // TODO: test flags
  op_dcr_test(); // TODO: test flags
  op_cma_test();
  // daa - not implemented
  // nop - passed, lol


  printf("%d test(s) passed, %d test(s) failed\n", passed, failed);
}

void Intel8080Test::fail(std::string message)
{
  failed++;
  printf("\033[1m\033[38;5;9mFAIL\033[0m: %s\n", message.c_str());
}

void Intel8080Test::pass(std::string message)
{
  passed++;
  printf("\033[1m\033[38;5;10mPASS\033[0m: %s\n", message.c_str());
}

void Intel8080Test::op_cmc_test(void)
{
  std::vector<uint8_t> prog = {
    0x37, // set carry flag
    0x3f, // compliment carry flag

    // terminate
    0x0e, // mvi c, 0x00
    0x00,
    0xcd, // call 0x0005
    0x05,
    0x00
  };

  loadAndExecute(prog);

  cpu->getFlag(Intel8080::Flag::C) ? fail("op_cmc") : pass("op_cmc");
}

void Intel8080Test::op_stc_test(void)
{
  std::vector<uint8_t> prog = {
    0x37, // set carry flag

    // terminate
    0x0e, // mvi c, 0x00
    0x00,
    0xcd, // call 0x0005
    0x05,
    0x00
  };

  loadAndExecute(prog);

  cpu->getFlag(Intel8080::Flag::C) ? pass("op_stc") : fail("op_stc");
}

void Intel8080Test::op_inr_test(void)
{

  std::string message = "op_inr";

  std::vector<uint8_t> prog = {
    0x06, // mvi b, 0x00
    0x00,
    0x04, // inr b

    0x16, // mvi d, 0x02
    0x02,
    0x14, // inr d

    0x1e, // mvi e, 0x03
    0x03,
    0x1c, // inr e

    0x26, // mvi h, 0x04
    0x04,
    0x24, // inr h

    0x2e, // mvi l, 0x05
    0x05,
    0x2c, // inr l

    0x36, // mvi m, 0x06
    0x06,
    0x34, // inr m

    0x3e, // mvi a, 0x07
    0x07,
    0x3c, // inr a

    // terminate
    0x0e, // mvi c, 0xff
    0xff,
    0x0c, // inr c
    0x00,
    0xcd, // call 0x0005
    0x05,
    0x00
  };

  loadAndExecute(prog);

  if (cpu->b != 0x01) { fail(message + " b=" + std::to_string(cpu->b)); return; }
  if (cpu->c != 0x00) { fail(message + " c=" + std::to_string(cpu->c)); return; }
  if (cpu->d != 0x03) { fail(message + " d=" + std::to_string(cpu->d)); return; }
  if (cpu->e != 0x04) { fail(message + " e=" + std::to_string(cpu->e)); return; }
  if (cpu->h != 0x05) { fail(message + " h=" + std::to_string(cpu->h)); return; }
  if (cpu->l != 0x06) { fail(message + " l=" + std::to_string(cpu->l)); return; }
  if (cpu->memory[cpu->getRegisterPairValue(Intel8080::RegisterPair::HL)] != 0x07)
  {
    fail(message + " m=" + std::to_string(cpu->memory[cpu->getRegisterPairValue(Intel8080::RegisterPair::HL)]));
    return;
  }

  pass(message);

  loadAndExecute(prog);
}

void Intel8080Test::op_dcr_test(void)
{

  std::string message = "op_dcr";

  std::vector<uint8_t> prog = {
    0x06, // mvi b, 0x02
    0x02,
    0x05, // dcr b

    0x16, // mvi d, 0x04
    0x04,
    0x15, // dcr d

    0x1e, // mvi e, 0x05
    0x05,
    0x1d, // dcr e

    0x26, // mvi h, 0x06
    0x06,
    0x25, // dcr h

    0x2e, // mvi l, 0x07
    0x07,
    0x2d, // dcr l

    0x36, // mvi m, 0x08
    0x08,
    0x35, // dcr m

    0x3e, // mvi a, 0x09
    0x09,
    0x3d, // dcr a

    // terminate
    0x0e, // mvi c, 0x01
    0x01,
    0x0d, // dcr c
    0x00,
    0xcd, // call 0x0005
    0x05,
    0x00
  };

  loadAndExecute(prog);

  if (cpu->b != 0x01) { fail(message + " b=" + std::to_string(cpu->b)); return; }
  if (cpu->c != 0x00) { fail(message + " c=" + std::to_string(cpu->c)); return; }
  if (cpu->d != 0x03) { fail(message + " d=" + std::to_string(cpu->d)); return; }
  if (cpu->e != 0x04) { fail(message + " e=" + std::to_string(cpu->e)); return; }
  if (cpu->h != 0x05) { fail(message + " h=" + std::to_string(cpu->h)); return; }
  if (cpu->l != 0x06) { fail(message + " l=" + std::to_string(cpu->l)); return; }
  if (cpu->memory[cpu->getRegisterPairValue(Intel8080::RegisterPair::HL)] != 0x07)
  {
    fail(message + " m=" + std::to_string(cpu->memory[cpu->getRegisterPairValue(Intel8080::RegisterPair::HL)]));
    return;
  }

  if (cpu->a != 0x08) { fail(message + " a=" + std::to_string(cpu->a)); return; }

  pass(message);
}

void Intel8080Test::op_cma_test(void)
{

  std::vector<uint8_t> prog = {
    0x3e, // mvi a, 0x51,
    0x51,
    0x2f, // cma

    // terminate
    0x0e, // mvi c, 0x01
    0x01,
    0x0d, // dcr c
    0x00,
    0xcd, // call 0x0005
    0x05,
    0x00
  };

  loadAndExecute(prog);

  cpu->a == 0xae ? pass("op_cma") : fail("op_cma");
}
