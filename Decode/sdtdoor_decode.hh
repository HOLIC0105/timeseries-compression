#ifndef SDTDOOR
#define SDTDOOR

#include "decode_form.hh"
#include <fstream>
#include <iomanip>

template<typename T, typename D>
void SDTDecode(std::vector<T> &tim, std::vector<D> &val, std::ofstream &ou) {
  for(int i = 0; i + 1 < tim.size(); i ++) {
    D y_delta = val[i + 1] - val[i];
    T x_delta = tim[i + 1] - tim[i];
    double Y = 0;
    for(T j = tim[i]; j < tim[i + 1]; j ++) {
      ou << std::setprecision(8) << D(val[i] + Y / x_delta) << "\n";
      Y += y_delta;
    }
  }
  ou << std::setprecision(8) << val.back()<< "\n";
}
#endif //SDTDOOR