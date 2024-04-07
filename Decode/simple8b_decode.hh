#ifndef SIMPLE8B_DECODE
#define SIMPLE8B_DECODE
#include <stdint.h>
#include <stddef.h>
#include <vector>

//
// ┌──────────────┬─────────────────────────────────────────────────────────────┐
// │   Selector   │      0   1   2   3   4   5   6   7   8   9   10  11  12  13 │
// ├──────────────┼─────────────────────────────────────────────────────────────┤
// │     Bits     │      1   2   3   4   5   6   7   8   10  12  15  20  30  60 │
// ├──────────────┼─────────────────────────────────────────────────────────────┤
// │      N       │      60  30  20  15  12  10  8   7   6   5   4   3   2   1  │
// └──────────────┴─────────────────────────────────────────────────────────────┘
//

//constexpr uint64_t SelectorMaxVal[14] = {2, 4, 8, 16, 32, 64, 128, 256, 1024, 4096, 32768, 1048576, 1073741824, 1152921504606846976};
constexpr int8_t SelectorNums[14] = {60, 30, 20, 15, 12, 10, 8, 7, 6, 5, 4, 3, 2, 1};
constexpr int8_t SelectorBits[14] = {1, 2, 3, 4, 5, 6, 7, 8, 10, 12, 15, 20, 30, 60};

template <typename T>
void Simple8bDecode(std::vector<bool> &src, std::vector<T> &dst) {
  int l = 0, r = src.size();
  while(l < r) {
    int SelectorId =  (src[l] << 3) | (src[l + 1] << 2) | (src[l + 2] << 1) | src[l + 3];
    l += 4;
    size_t maxn = std::min(l + SelectorNums[SelectorId] * SelectorBits[SelectorId], r);
    while(l < maxn) {
      T val = 0;
      int R = l + SelectorBits[SelectorId];
      while(l < R) val = (val << 1) | src[l ++];
      dst.push_back(val); 
    }
  }
} 
#endif //SIMPLE8B_DECODE