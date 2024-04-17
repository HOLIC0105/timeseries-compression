#ifndef SDTDOOR
#define SDTDOOR

#include "encode_form.hh"
#include <fstream>
#include <cmath>

#define MAX_DOUBLE 1.79769e+308
const double eps = 1e-8;
template<typename T, typename D>
struct Point{
  T x;
  D y;
};
template<typename T, typename D>
class SdtDoor{
  public:
    SdtDoor(const T &tim, const T &t, const double & e, const int a) : 
              t_begin_(tim), t_interval_(t), e_delta_(e), absolute_(a){} 
  void Init_(std::vector<D> in, EncodeForm<T> *x, EncodeForm<D> *y) {
    T tim = t_begin_;
    auto val = in.begin(); 
    double delta = e_delta_, now_up, now_down, up_gate = -MAX_DOUBLE, down_gate = MAX_DOUBLE;
    Point<T, D> last_data;
    x->Src_.push_back(tim);
    y->Src_.push_back(*val);
    if(!absolute_)delta = std::fabs(*val) * e_delta_ / 100.0;
    tim += t_interval_;
    val ++;
    while(val != in.end()) {
      T x_delta = tim - x->Src_.back();
      D y_delta = *val - y->Src_.back();
      now_up = double(y_delta - delta) / x_delta;
      up_gate = now_up > up_gate ? now_up : up_gate;
      now_down = double(y_delta + delta) / x_delta;
      down_gate = now_down < down_gate ? now_down : down_gate;
      if(up_gate > down_gate + eps) {
        x->Src_.push_back(last_data.x);
        y->Src_.push_back(last_data.y);
        x_delta = tim - last_data.x;
        y_delta = *val - last_data.y;
        if(!absolute_) delta =  std::min(std::fabs(*val), std::fabs(last_data.y)) * e_delta_ / 100.0;
        up_gate = double(y_delta - delta) / x_delta;
        down_gate = double(y_delta + delta) / x_delta;
      }
      last_data = {tim, *val};
      if(!absolute_) delta = std::min(delta,1.0 * std::fabs(*val) * e_delta_ / 100.0);
      val ++;
      tim += t_interval_;
    }
    x->Src_.push_back(last_data.x);
    y->Src_.push_back(last_data.y);
    t_begin_ = tim;
  }
  private:
    T t_begin_;
    const T t_interval_;
    const double e_delta_;
    const int absolute_;
};
#endif //SDTDOOR