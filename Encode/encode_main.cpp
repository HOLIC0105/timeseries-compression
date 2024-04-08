#include "encode.hh"

#include <iostream>
#include <fstream>
#include <cstring>

#include <getopt.h>

#define datatype int32_t
#define timetype uint32_t

int P = 1;
int main(int argc, char * argv[]) {
  std::ios::sync_with_stdio(false);
  std::cin.tie(0); 
  std::cout.tie(0); 

  timetype t_begin = 0, t_interval = 1;
  datatype e_delta; //error band
  std::string Filenamei, Filenameo;
  const char * optstring = "e:s:d:";
  int o;
  while((o = getopt(argc, argv, optstring)) != -1) {
    switch (o) {
      case 's':
        Filenamei = optarg;
        break;
      case 'd':
        Filenameo = optarg;
        break;
      case 'e':
        e_delta = atoi(optarg);
        break;
      case '?':
        printf("invalid option -- '%s'", optarg);
        exit(0);
        break;
    }
  }
  SdtDoor<timetype, datatype> data(t_begin, t_interval, e_delta);
  std::ifstream in(Filenamei);

  EncodeForm<timetype> tim;
  EncodeForm<datatype> val;
  data.Init_(in, &tim, &val); 
  in.close();

  Encode(tim, true);
  Encode(val, false);
  

  std::ofstream ou(Filenameo, std::ios::binary);

  int num = tim.Src_.size();
  ou.write((char *)&num, sizeof(num));

  std::vector<uint32_t> ans;
  std::vector<bool> ret;
  ret.push_back(tim.RleArrayNum_.size() * 1.8 < num);
  ret.push_back(val.RleArrayNum_.size() * 1.8 < num);
  if(ret[0]) {
    for(auto &u : tim.RleArrayNum_) ans.push_back(u);
    for(auto &u : tim.RleArrayVal_) ans.push_back(u);
  } else {
    for(auto &u : tim.Delta_) ans.push_back(u);
  }
  if(ret[1]){
    for(auto &u : val.RleArrayNum_) ans.push_back(u);
    for(auto &u : val.RleArrayVal_) ans.push_back(u);
  } else {
    for(auto &u : val.Delta_) ans.push_back(u);
  }

  Simple8bEncode(ans, ret);

  int len = (8 - ret.size() % 8) % 8;
  for(int i = 0; i < len; i ++) ret.push_back(0);

  for(size_t i = 0; i < ret.size(); i += 8) {
    char a, b;
    a = (ret[i] << 7) |  (ret[i + 1] << 6) |  (ret[i + 2] << 5) |  (ret[i + 3] << 4);
    b = (ret[i + 4] << 3) |  (ret[i + 5] << 2) |  (ret[i + 6] << 1) |  ret[i + 7];
    a |= b;
    ou.write(&a, sizeof(a));
  }

  return 0;
}