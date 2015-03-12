/* Author: Sam Cristall
 * License: GPLv3
 */
#ifndef SKAA_COMMON_PORT_H_
#define SKAA_COMMON_PORT_H_

#include <GPIO.h>

#include "Utility.h"

static constexpr size_t PORT_CONFIG_OFFSET = 8;
static constexpr bool PORT_CONFIG_OUTPUT = true;
static constexpr bool PORT_CONFIG_INPUT = false;
static constexpr bool PORT_VALUE_SET = true;
static constexpr bool PORT_VALUE_UNSET = false;

INTERFACE(IPort) {
public:
  void Configure(uint16_t cfg) { STATIC_DISPATCH( Configure(cfg) ); }
  uint16_t Status() const { return STATIC_DISPATCH_CONST( Status() ); }
  void Write(uint16_t value) { STATIC_DISPATCH( Write(value) ); }
  uint16_t Read() const { return STATIC_DISPATCH_CONST( Read() ); }
};

template <enum GPIO_port_name CfgPort>
class Port : public IPort<Port<CfgPort>> {
public:
  void Configure(uint16_t cfg) {
    gpio_set_config(cfg, CfgPort);
  }
  uint16_t Status() const {
    return gpio_get_config(CfgPort);
  }
  void Write(uint16_t value) {
    gpio_write(value, CfgPort);
  }
  uint16_t Read() const {
    return gpio_read(CfgPort);
  }
};

typedef Port<GPIO_A> PortA;
typedef Port<GPIO_B> PortB;
typedef Port<GPIO_C> PortC;
typedef Port<GPIO_D> PortD;
typedef Port<GPIO_E> PortE;
typedef Port<GPIO_F> PortF;
typedef Port<GPIO_G> PortG;
typedef Port<GPIO_H> PortH;
typedef Port<GPIO_I> PortI;
typedef Port<GPIO_J> PortJ;


#endif
