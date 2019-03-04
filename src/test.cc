#include "Intel8080.h"
#include "Intel8080Test.h"

Intel8080 cpu;
Intel8080Test test(&cpu);

int main(void)
{
  test.perform();
}
