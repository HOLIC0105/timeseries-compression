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
void Decode(DecodeForm<T> *data) {
  if(IncreaseFlag) {
    DeltaEncode(data->Src_, data->Delta_);
  } else {
    //DeltaEncode(data->Src_, data->Delta_);
    DeltaOfDeltaEncode(data->Src_, data->Delta_);
    /*
     for(auto u : data->Src_) std::cout << u << " "; std::cout << std::endl;
     for(auto u : data->Delta_) std::cout << u << " "; std::cout << std::endl;
      for(auto u : data->Delta_) std::cout << u << " "; std::cout << std::endl;
    */
    ZigZagEncode(data->Delta_);
    data->ZigZagFlag = true;
  }
  //VectorClear(data->Src_);
  RunLengthEncode(data->Delta_, data->RleArrayNum_, data->RleArrayVal_);

 // VectorClear(data->Delta_);

  /*
  for(auto u : data->RleArrayNum_) std::cout << u << " "; std::cout << std::endl;
  for(auto u : data->RleArrayVal_) std::cout << u << " " ;std::cout << std::endl;
  */

  auto ApartCompress = [&data]()->void{
    int l, r; //need find ....




    Simple8bDecode(data->Src_, data->RleArrayNum_, l, r);
    Simple8bDecode(data->Src_, data->RleArrayVal_, l, r);
    
    std::ofstream ou("code.out", std::ios::binary);
    Print(ver, ou);
  };
  ApartCompress();
  //VectorClear(data->RleArrayNum_);

  //VectorClear(data->RleArrayVal_); 
}

#endif // DECODE