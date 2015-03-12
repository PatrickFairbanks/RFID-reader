/* Author: Sam Cristall
 * License: GPLv3
 */
#ifndef SKAA_COMMON_BASE_BAND_UNIT_H_
#define SKAA_COMMON_BASE_BAND_UNIT_H_

#include <BBU.h>

#include "SysClock.h"
#include "Clock.h"
#include "Utility.h"

static const uint16_t PREAMBLE = 0x5555;

INTERFACE(IBaseBandUnit) : public IClock<Concrete> {
public:
  void LockTiming() { STATIC_DISPATCH(LockTiming()); }
  void UnlockTiming() { STATIC_DISPATCH(UnlockTiming()); }
  bool ContainsPreamble() { return STATIC_DISPATCH(ContainsPreamble()); }
  uint16_t PeekWord() { return STATIC_DISPATCH(PeekWord()); }
  uint16_t GetWord() { return STATIC_DISPATCH(GetWord()); }
  void PushWord(uint16_t value) { return STATIC_DISPATCH(PushWord(value)); }
  void SetTransmitting(bool value) { STATIC_DISPATCH(SetTransmitting(value)); }
  uint16_t GetTime() const { return STATIC_DISPATCH_CONST(GetTime()); }
};

template <BBU_name name>
class BaseBandUnit : public IBaseBandUnit<BaseBandUnit<name>> {
  private:
    constexpr uint16_t GetBbuStreamBaudRate(SystemFrequency sf) const {
        return
            // (sf == SystemFrequency::_24_576_MHz ) ? 43691 : 
            (sf == SystemFrequency::_36_864_MHz ) ? 29127 : 
            (sf == SystemFrequency::_49_152_MHz ) ? 21845 : 
            (sf == SystemFrequency::_73_728_MHz ) ? 14563 : 
            // (sf == SystemFrequency::_98_304_MHz ) ? 10922 : 
               (constexpr_unreachable<uint16_t>()); // invalid system frequency
    }
  public:
    BaseBandUnit(SystemFrequency sf, uint16_t start_word) {
      bbu_setup(static_cast<BaudRate>(GetBbuStreamBaudRate(sf)), 
                BitStream, name, start_word);
    }
    void LockTiming() { bbu_lock_timing(name); }
    void UnlockTiming() { bbu_unlock_timing(name); }
    bool ContainsPreamble() { return bbu_contains_preamble(name); }
    uint16_t PeekWord() { return bbu_peek(name); }
    uint16_t GetWord() { return bbu_get_word(name); }
    void PushWord(uint16_t value) { bbu_push_word(value, name); }
    void SetTransmitting(bool value) {}
    uint16_t GetTime() const { return bbu_get_time(name);  }
  private:

};

#endif
