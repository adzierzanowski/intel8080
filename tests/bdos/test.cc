#include <stdio.h>
#include <stdint.h>

uint8_t a = 0x89;
uint16_t b = a & 0xff;
uint16_t c = a;

int main(int argc, char *argv[])
{
  c &= 0xff;
  printf("%d %d\n", b, c);

  return 0;
}
