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

  timetype t_begin = 1, t_interval = 1;
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
  std::ofstream ou(Filenameo, std::ios::binary);
  Encode(&tim, true);
  Encode(&val, false);
  int num = tim.
  return 0;
}