#ifndef INFLUX2_HELPERS_H
#define INFLUX2_HELPERS_H


#include <cstdint>

class Helpers {
public:
    static const int32_t MAX_AMPLITUDE = std::numeric_limits<int32_t>::max();
    static const int32_t MIN_AMPLITUDE = std::numeric_limits<int32_t>::min();

    // Define the number of CORDIC iterations for accuracy (adjust as needed)
    static const int32_t CORDIC_ITERATIONS = 15;

    // CORDIC scaling factor for cosine (fixed-point representation)
    static const int32_t CORDIC_K = 0.6072529350088812561694 * (1 << 30);
    static int32_t cordicSine(int32_t angle);

};


#endif //INFLUX2_HELPERS_H
