#ifndef DECODE
#define DECODE
#include "zigzag_decode.hh"
#include "delta_decode.hh"
#include "simple8b_decode.hh"
#include "run_length_decode.hh"
#include "sdtdoor_decode.hh"
#include "decode_form.hh"
#include <iostream>
#include <vector>

template <typename T>
static void VectorClear(std::vector<T> &v) {
  v.clear();
  v.shrink_to_fit();
}

template <typename T>
void Decode(DecodeForm<T> &data, bool IncreaseFlag, bool flag) {
  if(flag)RunLengthDecode(data.Delta_, data.RleArrayNum_, data.RleArrayVal_);
  if(IncreaseFlag) {
    DeltaDecode(data.Delta_);
  }
}

#endif // DECODE