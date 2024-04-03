#ifndef ENCODE_FORM
#define ENCODE_FORM
#include <vector>
#include <fstream>
template<typename T>
struct DecodeForm{
  void Init_(std::ifstream &in) {
    char s;
    while(in.read(&s, sizeof(s))) {
      for(int i = 7; i >= 0; i --) {
        Src_.push_back((s >> i) & 1);
      }
    }
  }
  bool ZigZagFlag = 0;
  std::vector<bool> Src_;
  std::vector<T> Delta_;
  std::vector<int32_t> RleArrayNum_;
  std::vector<T> RleArrayVal_;
}; 
#endif // ENCODE_FORM