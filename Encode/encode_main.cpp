#include "encode.hh"

#include <iostream>
#include <fstream>
#include <algorithm>
#include <cstring>

#include <map>

#include <getopt.h>

#define datatype double
#define timetype int32_t

int P = 1;
int main(int argc, char * argv[]) {
  std::ios::sync_with_stdio(false);
  std::cin.tie(0); 
  std::cout.tie(0); 

  timetype t_begin = 0, t_interval = 1;
  datatype e_delta; 
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
        e_delta = std::stod(optarg);
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

  std::ifstream in(Filenamei);
  std::ofstream ou1("mapping", std::ios::binary);
  std::ofstream ou2("data", std::ios::binary);

  EncodeForm<timetype> tim;
  EncodeForm<datatype> val;
  std::vector<datatype> ver;
  std::vector<int64_t> tmp;
  std::vector<std::pair<datatype,int>> sortarray;
  std::map<datatype,int> sum;

  datatype x;

  while(in >> x) ver.push_back(x);
  SdtDoor<timetype, datatype> data(t_begin, t_interval, e_delta);
  for(int i = 1; i <= period; i ++) {
    std::vector<datatype> D;
    for(int j = i - 1; j < ver.size(); j += period) D.push_back(ver[j]);
    data.Init_(D, &tim, &val);
  }  

  in.close();

  Encode(tim, true);
  Encode(val, false);

  int num = tim.Src_.size();
  std::vector<datatype> ans;
  ans.push_back(num);
  ans.push_back(tim.RleArrayNum_.size() * 1.5 < num);
  ans.push_back(val.RleArrayNum_.size() * 1.5 < num);
  
  if(ans[1]) {
    for(auto &u : tim.RleArrayNum_) ans.push_back(u);
    for(auto &u : tim.RleArrayVal_) ans.push_back(u);
  } else for(auto &u : tim.Delta_) ans.push_back(u);

  if(ans[2]){
    for(auto &u : val.RleArrayNum_) ans.push_back(u);
    for(auto &u : val.RleArrayVal_) ans.push_back(u);
  } else for(auto &u : val.Delta_) ans.push_back(u);

  for(auto &u : ans) sum[u] ++;
  int tmplength = sum.size();

  for(auto &[x, y] : sum) {
    sortarray.push_back({x, y});
    if(y == 1) continue;
  }

  std::sort(sortarray.begin(), sortarray.end(), [&sortarray](std::pair<int,int> x, std::pair<int,int> y){
    return x.second > y.second;
  });
  
  ou2 << sortarray.size() <<" ";
  for(int i = 0 ; i < sortarray.size(); i ++) {
    ou1.write((char *)(&sortarray[i].first), sizeof(datatype));
    sum[sortarray[i].first] = i;
  }
  for(auto &u : ans) ou2 << sum[u] << " ";
  ou1.close();
  ou2.close();
  std::string s1 = "zip " + Filenameo + ".final " + "mapping " + "data";
  std::string s2 = "rm mapping data";
  system(s1.c_str());
  system(s2.c_str());
  return 0;
}