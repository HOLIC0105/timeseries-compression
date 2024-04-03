#ifndef SDTDOOR
#define SDTDOOR

#include "decode_form.hh"
#include <fstream>

#define MAX_DOUBLE 1.79769e+308
template<typename T, typename D>
struct Point{
  T x;
  D y;
};
template<typename T, typename D>
class SdtDoor{
  public:
    SdtDoor(const T &tim, const T &t, const D &e) : 
              t_begin_(tim), t_interval_(t), e_delta_(e){} 
  void Init_(std::ifstream &in, Form<T> *x, Form<D> *y) {
    T tim = t_begin_;
    double val; 
    double now_up, now_down, up_gate = -MAX_DOUBLE, down_gate = MAX_DOUBLE;
    Point<T, D> last_data;
    in >> val;
    val *= 1000000;
    x->Src_.push_back(tim);
    y->Src_.push_back(val);
    while(in >> val) {
      val *= 1000000;
      tim += t_interval_;
      T x_delta = tim - x->Src_.back();
      D y_delta = val - x->Src_.back();
      now_up = double(y_delta - e_delta_) / x_delta;
      up_gate = now_up > up_gate ? now_up : up_gate;
      now_down = double(y_delta + e_delta_) / x_delta;
      down_gate = now_down < down_gate ? now_down : down_gate;
      if(up_gate > down_gate) {
        x->Src_.push_back(last_data.x);
        y->Src_.push_back(last_data.y);
        x_delta = tim - x->Src_.back();
        y_delta = val - x->Src_.back();
        up_gate = double(y_delta - e_delta_) / x_delta;
        down_gate = double(y_delta + e_delta_) / x_delta;
      }
      last_data = {tim, val};
    }
    x->Src_.push_back(last_data.x);
    y->Src_.push_back(last_data.y);
  }
  private:
    const T t_interval_;
    const T t_begin_;
    const D e_delta_;
};
#endif //SDTDOOR