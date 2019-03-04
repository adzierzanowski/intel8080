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

void Intel8080Test::loadAndExecute(std::string filename)
{
  cpu->loadProgram(filename, 0x100);
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
  op_mov_test();
  op_stax_test();
  op_ldax_test();
  op_add_test();

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
  loadAndExecute("tests/cmc.bin");
  cpu->getFlag(Intel8080::Flag::C) ? fail("op_cmc") : pass("op_cmc");
}

void Intel8080Test::op_stc_test(void)
{
  loadAndExecute("tests/stc.bin");
  cpu->getFlag(Intel8080::Flag::C) ? pass("op_stc") : fail("op_stc");
}

void Intel8080Test::op_inr_test(void)
{
  std::string message = "op_inr";
  loadAndExecute("tests/inr.bin");

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

void Intel8080Test::op_dcr_test(void)
{
  std::string message = "op_dcr";
  loadAndExecute("tests/dcr.bin");

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
  loadAndExecute("tests/cma.bin");
  cpu->a == 0xae ? pass("op_cma") : fail("op_cma");
}

void Intel8080Test::op_mov_test(void)
{
  loadAndExecute("tests/mov.bin");
  cpu->memory[0xffff] == 0 ? pass("op_mov") : fail("op_mov");
}

void Intel8080Test::op_stax_test(void)
{
  loadAndExecute("tests/stax.bin");
  (cpu->memory[0x5555] == 0xaa && cpu->memory[0x6666] == 0xbb) ? pass("op_stax") : fail("op_stax");
}

void Intel8080Test::op_ldax_test(void)
{
  loadAndExecute("tests/ldax.bin");
  (cpu->a = 0xaa && cpu->b == 0xbb) ? pass("op_ldax") : fail("op_ldax");
}

void Intel8080Test::op_add_test(void)
{
  loadAndExecute("tests/add.bin");

  cpu->a == 0x07 ? pass("op_add") : fail("op_add");
}