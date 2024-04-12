#ifndef ENCODE
#define ENCODE
#include "zigzag_encode.hh"
#include "delta_encode.hh"
#include "simple8b_encode.hh"
#include "run_length_encode.hh"
#include "sdtdoor_encode.hh"
#include "encode_form.hh"
#include <iostream>
#include <vector>

template <typename T>
static void VectorClear(std::vector<T> &v) {
  v.clear();
  v.shrink_to_fit();
}

template <typename T>
void Encode(EncodeForm<T> &data, bool IncreaseFlag) {
  if(IncreaseFlag) {
    DeltaEncode(data.Src_, data.Delta_);
  } else {
    DeltaOfDeltaEncode(data.Src_, data.Delta_);
    //ZigZagEncode(data.Delta_);
  }
  
  RunLengthEncode(data.Delta_, data.RleArrayNum_, data.RleArrayVal_);

}

#endif // ENCODE