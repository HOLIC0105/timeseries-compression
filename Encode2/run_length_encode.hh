#ifndef RUN_LENGTH_ENCODE
#define RUN_LENGTH_ENCODE
#include <stdint.h>
#include <stddef.h>
#include <vector>
template <typename T>
void RunLengthEncode(std::vector<T> &Data, std::vector<uint32_t> &Nums, std::vector<T> &Val) {
  Nums.push_back(0);
  Val.push_back(Data.front());
  for(auto &u: Data) {
    if(u == Val.back()) 
      Nums.back() ++; 
    else {
      Val.push_back(u);
      Nums.push_back(1);
    }
  }
} 
#endif //RUN_LENGTH_ENCODE