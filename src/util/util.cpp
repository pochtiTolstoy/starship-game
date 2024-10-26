#include "util.h"

int eu_mod(int num, int mod) {
  int r = num % mod;
  if (r < 0)
    r += mod;
  return r;
}
