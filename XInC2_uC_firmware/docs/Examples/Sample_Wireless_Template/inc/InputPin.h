/* Author: Sam Cristall
 * License: GPLv3
 */
#ifndef SKAA_COMMON_PIN_H
#define SKAA_COMMON_PIN_H

#include "Port.h"
#include "Utility.h"

INTERFACE(IInputPin) {
public:
  bool Get() { return STATIC_DISPATCH(GetImpl()); }
  bool IsHigh() { return Get(); }
  bool IsLow() { return !Get(); }
};

template <typename Port> class InputPin : public IInputPin<InputPin<Port>> {
public:
  InputPin(Port &port, size_t number) : port_(port), number_(number) {
    port_.Configure(port_.Status() & ~(1 << (number + 8)));
  }

  bool GetImpl() { return (port_.Read() & (1 << number_)) != 0; }

  uint16_t GetPinNumber() const { return number_; }

private:
  Port &port_;
  size_t number_;
};

class MockInputPin : public IInputPin<MockInputPin> {
public:
  MockInputPin() : value_(0) {}

  bool GetImpl() { return value_; }

  void SetValue(bool value) { value_ = value; }

private:
  bool value_;
};

template <typename Port>
constexpr InputPin<Port> MakeInputPin(Port &port, const size_t number) {
  return InputPin<Port>(port, number);
}

#endif
