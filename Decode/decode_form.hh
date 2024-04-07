#ifndef ENCODE_FORM
#define ENCODE_FORM
#include <vector>
#include <fstream>
template<typename T>
struct DecodeForm{
  std::vector<T> Delta_;
  std::vector<uint32_t> RleArrayNum_;
  std::vector<T> RleArrayVal_;
}; 
#endif // ENCODE_FORM