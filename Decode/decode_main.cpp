#include "decode.hh"

#include <iostream>
#include <map>
#include <fstream>
#include <unistd.h>

#include <getopt.h>

#define datatype double
#define timetype int32_t

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

  std::ifstream in1(Filenamei + "_mapping", std::ios::binary);
  std::ifstream in2(Filenamei + "_data", std::ios::binary);
  std::ofstream ou(Filenameo, std::ios::binary);

 
  int length;
  int flag1 = 0, flag2 = 0;
  int nums;
  std::vector<datatype> dst;
  std::vector<datatype> hash;
  datatype va;
  in2 >> nums;
  char c;
  for(int i = 0; i < nums; i ++) {
    in1.read((char *)(&va), sizeof(va));
    hash.push_back(va);
  }
  in2 >> length;
  in2 >> flag1;
  in2 >> flag2;
  length = hash[length];
  flag1 = hash[flag1];
  flag2 = hash[flag2];
  int x;
  while(in2 >> x) {
    dst.push_back(hash[x]);
  }
  in1.close();
  in2.close();
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

  SDTDecode(tim.Delta_, val.Delta_, ou);
  return 0;
}