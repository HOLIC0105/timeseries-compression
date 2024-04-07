#ifndef RUN_LENGTH_DECODE
#define RUN_LENGTH_DECODE
#include <stdint.h>
#include <stddef.h>
#include <vector>
template <typename T>
void RunLengthDecode(std::vector<T> &Data, std::vector<uint32_t> &Nums, std::vector<T> &Val) {
  for(size_t i = 0; i < Nums.size(); i ++) {
    for(int j = 0; j < Nums[i]; j ++) Data.push_back(Val[i]);
  }
} 
#endif //RUN_LENGTH_DECODE