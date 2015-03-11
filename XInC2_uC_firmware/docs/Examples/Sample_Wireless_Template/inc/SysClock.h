/*                  Copyright Eleven Engineering 2014                         */
/** \file SysClock.h
 *  \brief Hardware implementation of the generic Clock interface. Singleton
 *  class which is invoked to create software timers in different contexts.
 * Author: Thia Wyrod
 * License: GPLv3
 */
#ifndef SKAA_COMMON_SYS_CLOCK_H_
#define SKAA_COMMON_SYS_CLOCK_H_

#include <SystemClock.h>
#include "Clock.h"

enum class SystemFrequency : uint16_t {
  _36_864_MHz = _36_864_MHz,
  _49_152_MHz = _49_152_MHz,
  _73_728_MHz = _73_728_MHz,
};

class SystemClock : public IClock<SystemClock>
{
private:
  //Because we are limited to int16_t on SoftTimer number of ticks comparisons,
  //we must use a reasonably small unit, much less than a millisecond.
  static constexpr uint16_t genSysTicksPerMsSlice(enum SystemFrequency sf)
  {
    return (sf == SystemFrequency::_36_864_MHz) ? 36864/SlicesPerMs :
      (sf == SystemFrequency::_49_152_MHz) ? 49152/SlicesPerMs :
      (sf == SystemFrequency::_73_728_MHz) ? 73728/SlicesPerMs : 0;
  }
  SystemClock(enum SystemFrequency sf) :
    SysTicksPerMsSlice(genSysTicksPerMsSlice(sf))
  {
    sys_clock_init(crys_24_576_MHz, static_cast<enum sys_freq>(sf));
  }
  static SystemClock& CreateImpl(enum SystemFrequency sf)
  {
    static SystemClock SC(sf);
    return SC;
  }

public:
  static const uint16_t SlicesPerMs = kNumSlicesPerMs;
  const uint16_t SysTicksPerMsSlice;
  //delete the default constructors
  SystemClock(void) = delete;
  SystemClock(const SystemClock&) = delete;
  SystemClock(SystemClock&&) = delete;


  //Always call once in the program before ever calling CreateReference,
  //otherwise undefined behaviour occurs
  static SystemClock& Init(enum SystemFrequency sf)
  {
    return CreateImpl(sf);
  }
  static SystemClock& CreateRef(void)
  {
    return CreateImpl(SystemFrequency::_49_152_MHz);
  }
  uint16_t GetTime(void) const
  {
    return sys_clock_get_scu_time();
  }

};

#endif //SKAA_COMMON_SYS_CLOCK_H_
