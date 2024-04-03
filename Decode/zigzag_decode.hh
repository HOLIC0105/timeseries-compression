
#ifndef ZIGZAG_DECODE
#define ZIGZAG_DECODE
#include <vector>
#include <stdint.h>
template <typename T>
void ZigZagEncode(std::vector<T> &delta)
{
  for(auto &val : delta) {
    val = (val >> 1) ^ -(val & 1); 
  }
  return;
}
#endif //ZIGZAG_DECODE