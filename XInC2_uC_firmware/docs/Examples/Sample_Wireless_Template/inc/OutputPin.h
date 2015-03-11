/* Author: Sam Cristall
 * License: GPLv3
 */
#ifndef SKAA_COMMON_OUTPUT_PIN_H
#define SKAA_COMMON_OUTPUT_PIN_H

#include "Port.h"
#include "Utility.h"

INTERFACE(IOutputPin) {
public:
  void Set(bool value) {STATIC_DISPATCH(SetImpl(value));}
  void SetHigh() { Set(true); }
  void SetLow() { Set(false); }
};

template <typename Port> class OutputPin : public IOutputPin<OutputPin<Port>> {
public:
  OutputPin(Port &port, size_t number, bool init_state)
      : port_(port), number_(number) {
    port_.Configure(port_.Status() | (1 << (number + 8)));
    this->Set(init_state);
  }

  void SetImpl(bool value) {
    if (value)
      port_.Write(port_.Status() | (1 << number_));
    else
      port_.Write(port_.Status() & ~(1 << number_));
  }

  uint16_t GetPinNumber() const { return number_; }

private:
  Port &port_;
  size_t number_;
};

template <typename Port>
constexpr OutputPin<Port> MakeOutputPin(Port &port, size_t number, bool init_state) {
  return OutputPin<Port>(port, number, init_state);
}

#endif
