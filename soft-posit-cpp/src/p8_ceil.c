#include "internals.h"
#include "platform.h"
#include <math.h>

posit8_t p8_ceil(posit8_t pA) {
  uint8_t mask = 0x20;
  uint8_t scale = 0;

  union ui8_p8 uA;
  uA.p = pA;
  uint8_t ui_a = uA.ui;

  // Check if input is NaR (0x80)
  if (ui_a == 0x80) {
    return pA;
  }

  bool sign = ui_a > 0x80;

  // If negative, take two's complement
  if (sign) {
    ui_a = -ui_a;
  }

  uint8_t result;

  if (ui_a <= 0x40) {
    // 0 <= |pA| < 1
    // If negative and not exactly -1, ceil to zero
    // If positive or exactly -1, ceil to 1
    if (sign && (ui_a != 0x40)) {
      result = 0x0;
    } else {
      result = 0x40;
    }
  } else if (ui_a <= 0x60) {
    // 1 <= |pA| < 2
    // If negative and not exactly -2, ceil to -1
    // If positive or exactly -2, ceil to 2
    if (sign && (ui_a != 0x60)) {
      result = 0x40;
    } else {
      result = 0x60;
    }
  } else if (ui_a <= 0x68) {
    // 2 <= |pA| < 3
    // If negative and not exactly -3, ceil to -2
    // If positive or exactly -3, ceil to 3
    if (sign && (ui_a != 0x68)) {
      result = 0x60;
    } else {
      result = 0x68;
    }
  } else if (ui_a >= 0x78) {
    // If |A| is 8 or greater, leave it unchanged
    return pA; // This also handles NaR case (0x80)
  } else {
    // For other values, find the regime and handle rounding
    while ((mask & ui_a) != 0) {
      scale += 1;
      mask >>= 1;
    }

    mask >>= scale;

    mask >>= 1;
    uint8_t tmp = ui_a & mask;
    uint8_t bit_n_plus_one = tmp;
    ui_a ^= tmp;
    tmp = ui_a & (mask - 1); // bits_more
    ui_a ^= tmp;

    // For positive numbers, round up if there are any bits set in positions we
    // cleared
    if (!sign && ((bit_n_plus_one | tmp) != 0)) {
      ui_a += mask << 1;
    }

    result = ui_a;
  }

  // Apply sign to the result
  if (sign) {
    result = -result;
  }

  union ui8_p8 uZ;
  uZ.ui = result;
  return uZ.p;
}