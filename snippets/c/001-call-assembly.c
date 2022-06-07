#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static inline uint64_t r() {
  uint64_t r;

  // r = rax
  asm volatile("movq %%rax, %0" : "=r" (r));

  return r;
}

static inline uint64_t setrax(uint64_t *ptr) {
  uint64_t r;

  // rax = *ptr
  asm volatile("movq %0, %%rax" : : "r"(*ptr));
  // rax *= 3
  asm volatile("imulq $0x3, %rax");
  // rax += 1
  asm volatile("incq %rax");
  // r = rax
  asm volatile("movq %%rax, %0" : "=r" (r));

  return r;
}

int main(void) {
  uint64_t v = r();
  uint64_t h = 0x3;

  printf("%lu\n", v);
  v = setrax(&h);
  printf("%lu\n", v);

  return 0;
}
