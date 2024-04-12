#ifndef DELTA_ENCODE
#define DELTA_ENCODE
#include <stdint.h>
#include <stddef.h>
#include <vector>
#include <iostream>

template<typename T>
void DeltaEncode(std::vector<T> &src, std::vector<T> &delta) {
  delta.resize(src.size());
  delta[0] = src.front();
  for(size_t i = 1; i < src.size(); i ++) {
    delta[i] = (src[i] - src[i - 1]);
  }
}

template<typename T>
void DeltaOfDeltaEncode(std::vector<T> &src, std::vector<T> &delta) {
  std::vector<T> mid;
  delta.resize(src.size());
  mid.resize(src.size());
  delta[0] = src.front();
  mid[0] = 0;
  for(size_t i = 1; i < src.size(); i ++) {
    mid[i] = (src[i] - src[i - 1]);
    //std::cout << mid[i] << ",";
  }
  //std::cout << std::endl;
  for(size_t i = 1; i < src.size(); i ++) {
    delta[i] = (mid[i] - mid[i - 1]);
  }
} 
#endif //DELTA_ENCODE