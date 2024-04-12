#ifndef SIMPLE8B_ENCODE
#define SIMPLE8B_ENCODE
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

constexpr uint64_t SelectorMaxVal[14] = {2, 4, 8, 16, 32, 64, 128, 256, 1024, 4096, 32768, 1048576, 1073741824, 1152921504606846976};
constexpr int8_t SelectorNums[14] = {60, 30, 20, 15, 12, 10, 8, 7, 6, 5, 4, 3, 2, 1};
constexpr int8_t SelectorBits[14] = {1, 2, 3, 4, 5, 6, 7, 8, 10, 12, 15, 20, 30, 60};

template <typename T>
static int GetSelector(const T &val) {
  int l = 0, r = 13, mid, ans = 0;
  while(l <= r ) {
    mid = (l + r) >> 1;
    if(val < SelectorMaxVal[mid]) ans = mid, r = mid - 1;
    else l = mid + 1;
  }
  return ans;
}

template <typename T>
static void WriteBits(std::vector<bool> &output, T val, int8_t bits) {
  std::vector<bool> a;
  while(val) {
    a.push_back(val & 1);
    val >>= 1;
  }
  for(int8_t i = a.size(); i < bits; i ++) {
    output.push_back(false);
  }
  for(size_t i = a.size() - 1; i < a.size(); i --) {
    output.push_back(a[i]);
  }
}

template <typename T>
void Simple8bEncode(std::vector<T> &input, std::vector<bool> &output)
{
  size_t ValuesRemaining(input.size());
  int L = 0, R = 0;
  int8_t MaxSelector = 0; 

  while(R < ValuesRemaining) {
    int TmpSelector = GetSelector(input[R ++]); //log(14);
    if(TmpSelector > MaxSelector) {
      MaxSelector ++;
      while(MaxSelector != TmpSelector) {
        while(R - L > SelectorNums[MaxSelector]) {
          WriteBits<int8_t>(output, MaxSelector, 4);
          int maxnL = L + SelectorNums[MaxSelector];
          int bits = SelectorBits[MaxSelector];
          while(L < maxnL) {
            WriteBits<T>(output, input[L ++], bits);
          }
        }
        MaxSelector ++;
      }
    } //[L, R - 1) 合并
    while(R - L >= SelectorNums[MaxSelector]) {
      WriteBits<T>(output, MaxSelector, 4);
      int maxnL = L + SelectorNums[MaxSelector];
      int bits = SelectorBits[MaxSelector];
      while(L < maxnL) {
        WriteBits<T>(output, input[L ++], bits);
      }
    } //[L, R) 合并
    if(L == R) MaxSelector = 0;
  }
  if(L != R) {
    WriteBits<T>(output, MaxSelector, 4);
    int maxnL = L + SelectorNums[MaxSelector];
    int bits = SelectorBits[MaxSelector];
    for(int i = L; i < R; i ++) WriteBits<T>(output, input[i], bits);
  }
}
#endif //SIMPLE8B_ENCODE