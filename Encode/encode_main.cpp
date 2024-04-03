#include "encode.hh"

#include <iostream>
#include <fstream>

#define datatype int32_t
#define timetype uint32_t

int P = 1;
int main(int argc, char * argv[]) {
  std::ios::sync_with_stdio(false);
  std::cin.tie(0); 
  std::cout.tie(0); 

  timetype t_begin = 1, t_interval = 1;
  datatype e_delta; //error band
  std::string filename;
  int decimal_digits;

  std::cin >> filename >> e_delta >> decimal_digits ;
  for(int i = 0; i < decimal_digits; i ++) P = P * 10;

  SdtDoor<timetype, datatype> data(t_begin, t_interval, e_delta);
  std::ifstream in(filename);
  
  EncodeForm<timetype> tim;
  EncodeForm<datatype> val;
  data.Init_(in, &tim, &val); 
  in.close();
  /*
  for(auto &u : tim.Src_) std::cout << u << " ";std::cout << std::endl;
  for(auto &u : val.Src_) std::cout << u << " ";std::cout << std::endl;
  */
  Encode(&tim, true);
  //std::cout << "!!!!!!!" << std::endl;
  Encode(&val, false);
  return 0;
}