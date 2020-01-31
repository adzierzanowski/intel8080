#ifndef TEST_CPU_H
#define TEST_CPU_H

#include <memory>

#include <criterion/criterion.h>

#include "test.hh"
#include "cpu.hh"


std::unique_ptr<CPU> cpu;

void test_cpu_init(void);
void test_cpu_fini(void);

#endif
