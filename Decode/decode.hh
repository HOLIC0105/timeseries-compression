#ifndef ENCODE
#define ENCODE
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

 void Print(std::vector<bool> &ret, std::ofstream &ou) {
    int len = (8 - ret.size() % 8) % 8;
    ret[0] = (len >> 2) & 1;
    ret[1] = (len >> 1) & 1;
    ret[2] = len & 1;
    for(int i = 0; i < len; i ++) ret.push_back(0);
    for(size_t i = 0; i < ret.size(); i += 8) {
      char a, b;
      a = (ret[i] << 7) |  (ret[i + 1] << 6) |  (ret[i + 2] << 5) |  (ret[i + 3] << 4);
      b = (ret[i + 4] << 3) |  (ret[i + 5] << 2) |  (ret[i + 6] << 1) |  ret[i + 7];
      a |= b;
      ou.write(&a, sizeof(a));
    }
  }

template <typename T>
void Encode(Form<T> *data, bool IncreaseFlag) {
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
    Simple8bEncode(data->RleArrayNum_, data->RetNum_);
    Simple8bEncode(data->RleArrayVal_, data->RetVal_);
    std::vector<bool> ver = data->RetNum_;
    /*
    for(auto u : ver) std::cout << u;
    std::cout << std::endl;
    */
    for(auto u : data->RetVal_) ver.push_back(u);
    std::ofstream ou("code.out", std::ios::binary);
    Print(ver, ou);
  };
  ApartCompress();
  //VectorClear(data->RleArrayNum_);

  //VectorClear(data->RleArrayVal_); 
}

#endif // ENCODE