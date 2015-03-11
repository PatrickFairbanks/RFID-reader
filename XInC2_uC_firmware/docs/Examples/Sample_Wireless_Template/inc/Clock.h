//
//  Clock.h
//  skaa
//
//  Created by Sam Cristall on 2014-07-23.
//
//
// License: GPLv3

#ifndef SKAA_COMMON_CLOCK_H_
#define SKAA_COMMON_CLOCK_H_

#include "Utility.h"

INTERFACE(IClock) {
public:
  uint16_t GetTime() const { return STATIC_DISPATCH_CONST(GetTime()); }
};


#endif
