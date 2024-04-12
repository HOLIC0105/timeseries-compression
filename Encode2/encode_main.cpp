#include "encode.hh"

#include <iostream>
#include <fstream>
#include <algorithm>
#include <cstring>

#include <map>

#include <getopt.h>

#define datatype double
#define timetype uint32_t

int P = 1;
int main(int argc, char * argv[]) {
  std::ios::sync_with_stdio(false);
  std::cin.tie(0); 
  std::cout.tie(0); 

  timetype t_begin = 0, t_interval = 1;
  datatype e_delta; //error band
  int period;
  std::string Filenamei, Filenameo;
  const char * optstring = "e:s:d:p:";
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
      case 'p':
        period = atoi(optarg);
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
  std::vector<datatype> ver;
  datatype x;
  while(in >> x) {
    ver.push_back(x);
  }
  
  for(int i = 1; i <= period; i ++) {
    std::vector<datatype> D;
    for(int j = i - 1; j < ver.size(); j += period) {
      D.push_back(ver[j]);
    //  std::cout << ver[j] << std::endl;
    }
    data.Init_(D, &tim, &val);
  }  
  /*
  for(int i = 0; i < tim.Src_.size(); i ++) {
    std::cout << tim.Src_[i] << " " << val.Src_[i] << std::endl;
  }
  */

  in.close();
  std::ofstream ou(Filenameo);

  Encode(tim, true);
  Encode(val, false);

  int num = tim.Src_.size();

  std::vector<datatype> ans;
  std::vector<bool> ret;

  if(tim.RleArrayNum_.size() * 1.8 < num) {
    for(auto &u : tim.RleArrayNum_) ans.push_back(u);
    for(auto &u : tim.RleArrayVal_) ans.push_back(u);
  } else {
    for(auto &u : tim.Delta_) ans.push_back(u);
  }
  if(val.RleArrayNum_.size() * 1.8 < num){
    for(auto &u : val.RleArrayNum_) ans.push_back(u);
    for(auto &u : val.RleArrayVal_) ans.push_back(u);
  } else {
    for(auto &u : val.Delta_) ans.push_back(u);
  }


 // ret.push_back(tim.RleArrayNum_.size() * 1.8 < num);
 // ret.push_back(val.RleArrayNum_.size() * 1.8 < num);

  std::vector<datatype> tmp;
  std::vector<std::pair<datatype,int>> sortarray;
  std::map<datatype,int> sum;

  for(auto &u : ans) sum[u] ++;

  int tmplength = sum.size();

  //ou.write((char *)&num, sizeof(num));
  //ou.write((char *)(&tmplength), sizeof(tmplength));

  for(auto &[x, y] : sum) {
    sortarray.push_back({x, y});
    if(y == 1) continue;
  }

  std::sort(sortarray.begin(), sortarray.end(), [&sortarray](std::pair<int,int> x, std::pair<int,int> y){
    return x.second > y.second;
  });

  for(int i = 0 ; i < sortarray.size(); i ++) {
    tmp.push_back(sortarray[i].first);
    sum[sortarray[i].first] = i;
  }

  for(auto &u : ans) {
    tmp.push_back(sum[u]);
  }
    Simple8bEncode(tmp, ret);

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