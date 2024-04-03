#include "decode.hh"

#include <iostream>
#include <fstream>

#define datatype int32_t
#define timetype uint32_t

int P = 1;
int main(int argc, char * argv[]) {
  std::ios::sync_with_stdio(false);
  std::cin.tie(0); 
  std::cout.tie(0); 

  std::string filename;

  std::cin >> filename;

  std::ifstream in(filename, std::ios::binary);

  DecodeForm<datatype> data;
  data.Init_(in);
  in.close();
  Decode(data);
  return 0;
}