#ifndef TEST_ASSEMBLER_H
#define TEST_ASSEMBLER_H

#include <type_traits>
#include <memory>

#include <criterion/criterion.h>

#include "assembler.hh"


std::unique_ptr<Assembler> assembler;

void test_assembler_init(void);
void test_assembler_fini(void);

#endif