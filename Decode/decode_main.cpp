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
  std::string s1 = "unzip " + Filenamei + ".final";
  std::string s2 = "rm mapping data";
  if(system(s1.c_str()) == 0) { 
    std::ifstream in1("mapping", std::ios::binary);
    std::ifstream in2("data", std::ios::binary);
    std::ofstream ou(Filenameo, std::ios::binary);
    int length, nums, flag1, flag2;
    std::vector<datatype>  hash;
    std::vector<datatype> dst;
    datatype va;
    in2 >> nums;
    for(int i = 0; i < nums; i ++) {
      in1.read((char *)(&va), sizeof(va));
      hash.push_back(va);
    }
    in2 >> length >> flag1 >> flag2;
    length = hash[length];
    flag1 = hash[flag1];
    flag2 = hash[flag2];
    while(in2 >> o) dst.push_back(hash[o]);
    in1.close();
    in2.close();
    system(s2.c_str());
    DecodeForm<timetype> tim;
    DecodeForm<datatype> val;
    int l = 0, r = dst.size(), num = 0, sum = 0;
    if(flag1) {
      while(sum != length) {
        tim.RleArrayNum_.push_back(dst[l]);
        sum += dst[l ++];
        num ++;
      }
      while(num --) tim.RleArrayVal_.push_back(dst[l ++]);
    } else {
      int r = l + length;
      while(l < r) tim.Delta_.push_back(dst[l ++]);
    }
    num = sum = 0;
    if(flag2) {
      while(sum != length) {
        val.RleArrayNum_.push_back(dst[l]);
        sum += dst[l ++];
        num ++;
      }
      while(num --) val.RleArrayVal_.push_back(dst[l ++]);
    } else {
      int r = l + length;
      while(l < r) val.Delta_.push_back(dst[l ++]);
    }
    Decode(tim, true, flag1);
    Decode(val, false, flag2);
    SDTDecode(tim.Delta_, val.Delta_, ou);
  } else std::cout << "NO THE FINAL FILE : " << Filenamei << std::endl;
  return 0;
}