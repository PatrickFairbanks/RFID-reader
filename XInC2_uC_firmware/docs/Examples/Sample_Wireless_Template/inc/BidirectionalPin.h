/* Author: Sam Cristall
 * License: GPLv3
 */
#ifndef SKAA_COMMON_BIDIRECTIONAL_PIN_H_
#define SKAA_COMMON_BIDIRECTIONAL_PIN_H_

#include "InputPin.h"
#include "OutputPin.h"
#include "Utility.h"

enum class Direction {
  Input,
  Output
};

template <class Concrete>
class IBidirectionalPin : public IInputPin< Concrete>,
                          public IOutputPin<Concrete> {
public:
  void SetAsInput() { STATIC_DISPATCH(SetAsInputImpl()); }
  void SetAsOutput(bool value) { STATIC_DISPATCH(SetAsOutputImpl(value)); }
};

template <typename Port>
class BidirectionalPin : public IBidirectionalPin<BidirectionalPin<Port>> { 
private:
  
public:
  BidirectionalPin(Port& port, size_t number) 
    : port_(port), number_(number) {}

  void SetAsInputImpl() {
    port_.Configure(port_.Status() & ~(1 << (number_ + 8)));
  }

  void SetAsOutputImpl(bool init_state) {
    port_.Configure(port_.Status() | (1 << (number_ + 8)));
    this->Set(init_state);
  }

  Direction GetDirection() const {
    if (port_.Status() & (1 << number_))
      return Direction::Output;
    return Direction::Input;
  }
  
  bool GetImpl() { 
    return (port_.Read() & (1 << number_)) != 0;
  }
  
  void SetImpl(bool value) { 
    if (value) 
      port_.Write(port_.Status() | (1 << number_));
    else
      port_.Write(port_.Status() & ~(1 << number_));
  }

  uint16_t GetPinNumber() const { return number_; }
private:
  Port& port_;
  size_t number_;
};

template <typename P>
constexpr BidirectionalPin<P> MakeBidirectionalPin(P& port, size_t number) {
  return BidirectionalPin<P>(port, number);
} 
#endif
