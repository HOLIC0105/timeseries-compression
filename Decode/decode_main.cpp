#include "decode.hh"

#include <iostream>
#include <fstream>
#include <unistd.h>

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
  bool flag1 = 0, flag2 = 0;
  std::vector<bool> src;

  in.read((char *)(&length), sizeof(length));
  if(in.read(&s, sizeof(s))) {
    flag1 = (s >> 7) & 1;
    flag2 = (s >> 6) & 1;
    for(int i = 5; i >= 0; i --)
      src.push_back((s >> i) & 1);
    while(in.read(&s, sizeof(s))) {
      for(int i = 7; i >= 0; i --) {
        src.push_back((s >> i) & 1);
      }
    }
  }
  in.close();
  
  std::vector<uint32_t> dst;

  Simple8bDecode(src, dst);
  DecodeForm<timetype> tim;
  DecodeForm<datatype> val;

  int l = 0, r = dst.size(), sum = 0, num = 0;
  if(flag1) {
    while(sum != length) {
      tim.RleArrayNum_.push_back(dst[l]);
      sum += dst[l];
      num ++, l ++;
    }
    while(num --) {
      tim.RleArrayVal_.push_back(dst[l ++]);
    }
  } else {
    int r = l + length;
    while(l < r) {
      tim.Delta_.push_back(dst[l]);
      l ++;
    }
  }
  num = sum = 0;
  if(flag2) {
    while(sum != length) {
      val.RleArrayNum_.push_back(dst[l]);
      sum += dst[l];
      num ++, l ++;
    }
    while(num --) {
      val.RleArrayVal_.push_back(dst[l ++]);
    }
  } else {
    int r = l + length;
    while(l < r) {
      val.Delta_.push_back(dst[l]);
      l ++;
    }
  }

  Decode(tim, true, flag1);
  Decode(val, false, flag2);
  
  std::ofstream ou(Filenameo);

  SDTDecode(tim.Delta_, val.Delta_, ou);
  return 0;
}