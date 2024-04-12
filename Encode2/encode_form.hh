#ifndef ENCODE_FORM
#define ENCODE_FORM
#include <vector>
#include <fstream>
template<typename T>
struct EncodeForm{
  bool ZigZagFlag = 0;
  bool RleFlag = 0;
  std::vector<T> Src_;
  std::vector<T> Delta_;
  std::vector<uint32_t> RleArrayNum_;
  std::vector<T> RleArrayVal_;
 // std::vector<bool> RetNum_;
 // std::vector<bool> RetVal_;
}; 
#endif // ENCODE_FORM