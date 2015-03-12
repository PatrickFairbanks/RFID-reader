/* Author: Thia Wyrod
 * License: GPLv3
 */
/* This sample firmware simply toggles a pin (linked to an LED) on a fixed time
 * interval. It showcases the fundamentals of setting up a GPIO port and using it.
 */
#include <IOConfig.h>
#include <Structs.h>

#include "time_funcs.h"
#include "struct_aliases.h"

/* Define port, io_port, and exact pin layout for the LED */
static const GlobalPin LED = LED4;

static const size_t kMsPerCycle = 1000; /* one second between LED toggles */

int main(void)
{
  /* Initialize the system clock to be able to use timers */
  init_clock();

  /* set drive strengths and other parameters for the specifc IO pins being used */
  io_set_config(DEFAULT_LED_CFG, LED.io_port);
  /* set LED port's output pins, all other pins retain their previous state */
  globalPin_set_dir(PinDir_Output, &LED);

  enum PinLogicState old_state = OFF;
  while(1) {
    enum PinLogicState new_state = !old_state; /* invert the current pin status */
    globalPin_write(new_state, &LED);
    old_state = new_state;
    wait_ms(kMsPerCycle);
  }
  return 0;
}
