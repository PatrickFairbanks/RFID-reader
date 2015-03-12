//
//  RadioPresets.h
//  skaa
//
//  Created by Sam Cristall on 2014-07-18.
//
//
// License: GPLv3

#ifndef SKAA_COMMON_RADIO_PRESETS_H_
#define SKAA_COMMON_RADIO_PRESETS_H_

#include "Utility.h"

INTERFACE(RadioInterface) {
public:
  void SwitchToTransmit(size_t power_level) {
    STATIC_DISPATCH(SwitchToTransmit(power_level));
  }
  void SwitchToReceive() {
    STATIC_DISPATCH(SwitchToReceive());
  }
  void SwitchToIdle() {
    STATIC_DISPATCH(SwitchToIdle());
  }
  void ChangeChannel(size_t channel) {
    STATIC_DISPATCH(ChangeChannel(channel));
  }
  uint16_t GetRSSI() {
    return STATIC_DISPATCH(GetRSSI());
  }

private:
};

#endif
