#ifndef DELTA_DECODE
#define DELTA_DECODE
#include <stdint.h>
#include <stddef.h>
#include <vector>
#include <iostream>

template<typename T>
void DeltaEncode(std::vector<T> &delta) {
  for(size_t i = 1; i < delta_.size(); i ++) {
    delta_[i] += delta_[i - 1];
  }
}

template<typename T>
void DeltaOfDeltaDecode(std::vector<T> &delta) {
 for(size_t i = 2; i < delta.size(); i ++) {
    delta_[i] += delta_[i - 1];
  }
  for(size_t i = 1; i < delta.size(); i ++) {
    delta_[i] += delta_[i - 1];
  }
} 
#endif //DELTA_DECODE