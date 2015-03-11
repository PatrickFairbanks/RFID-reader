/* Author: Sam Cristall
 * License: GPLv3
 */
#ifndef SKAA_COMMON_SOFT_TIMER_H_
#define SKAA_COMMON_SOFT_TIMER_H_

#include <SXC.h>

#include "Clock.h"

//! SoftTimer Class
/*!
 * \brief Object for managing timed events and stalls
 */
template<typename ClockTy>
class SoftTimer {
public:
  using Clock = IClock<ClockTy>;
  
  SoftTimer(const Clock& ref_clock) : ref_clock_(ref_clock) {}

  void Start(uint16_t time) {
    end_time_ = ref_clock_.GetTime() + time;
  }

  void StartWithRef(uint16_t ref, uint16_t time) {
    end_time_ = ref + time;
  }

  void Tick(uint16_t time) {
    end_time_ += time;
  }

  bool HasElapsed() const {
    int16_t delta = (int16_t)(ref_clock_.GetTime() - end_time_);
    return delta >= 0;
  }
  
  void Wait() const {
    while (HasElapsed() == false)
      continue;
  }
  
private:
  const Clock& ref_clock_;
  uint16_t end_time_;
};

template <typename T>
constexpr SoftTimer<T> MakeSoftTimer(IClock<T>& clock) {
  return SoftTimer<T>(clock);
}

#endif
