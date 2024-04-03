
#ifndef ZIGZAG_ENCODE
#define ZIGZAG_ENCODE
#include <vector>
#include <stdint.h>
template <typename T>
void ZigZagEncode(std::vector<T> &delta)
{
    int8_t shift = (sizeof(T) * 8) - 1; // sizeof * 8 = # of bits
    for(auto &val : delta) {
      val = (val << 1) ^ (val >> shift); 
    }
    return;
}
#endif //ZIGZAG_ENCODE