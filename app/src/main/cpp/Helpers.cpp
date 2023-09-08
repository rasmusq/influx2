#include <valarray>
#include <limits>
#include "Helpers.h"

int32_t Helpers::cordicSine(int32_t angle) {
    int32_t x = 1 << 30;  // Initial x value (cosine of 0)
    int32_t y = 0;       // Initial y value (sine of 0)
    int32_t z = angle;

    for (int i = 0; i < CORDIC_ITERATIONS; ++i) {
        int32_t d = (z >= 0) ? 1 : -1;
        int32_t x_new = x - (y >> i) * d;
        int32_t y_new = y + (x >> i) * d;
        int32_t z_new = z - d * (atan(1.0 / (1 << i)) * (1 << 30));

        x = x_new;
        y = y_new;
        z = z_new;
    }

    return x;
}
