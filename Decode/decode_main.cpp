#include "decode.hh"

#include <iostream>
#include <fstream>

#include <getopt.h>

#define datatype int32_t
#define timetype uint32_t

int P = 1;
int main(int argc, char * argv[]) {
  std::ios::sync_with_stdio(false);
  std::cin.tie(0); 
  std::cout.tie(0); 

  std::string Filenamei = "", Filenameo = "";
  const char * optstring = "s:d:";
  int o;
  while((o = getopt(argc, argv, optstring)) != -1) {
    switch (o) {
      case 's':
        Filenamei = optarg;
        break;
      case 'd':
        Filenameo = optarg;
        break;
      case '?':
        printf("invalid option -- '%s'", optarg);
        exit(0);
        break;
    }
  }

  std::ifstream in(Filenamei, std::ios::binary);

  char s;
  int length;

  std::vector<bool> src;

  in.read((char *)(&length), sizeof(length));

  while(in.read(&s, sizeof(s))) {
    for(int i = 7; i >= 0; i --) {
      src.push_back((s >> i) & 1);
    }
  }

  in.close();
  
  std::vector<uint32_t> dst;

  Simple8bDecode(src, dst);

  DecodeForm<timetype> tim;
  DecodeForm<datatype> val;

  int l = 0, r = dst.size(), sum = 0, num = 0;

  while(sum != length) {
    tim.RleArrayNum_.push_back(dst[l]);
    sum += dst[l];
    num ++, l ++;
  }
  while(num --) {
    tim.RleArrayVal_.push_back(dst[l ++]);
  }

  while(sum) {
    val.RleArrayNum_.push_back(dst[l]);
    sum -= dst[l];
    num ++, l ++;
  }
  while(num --) {
    val.RleArrayVal_.push_back(dst[l ++]);
  }

  Decode(tim, true);
  Decode(val, false);

  std::ofstream ou(Filenameo);

  SDTDecode(tim.Delta_, val.Delta_, ou);
  return 0;
}