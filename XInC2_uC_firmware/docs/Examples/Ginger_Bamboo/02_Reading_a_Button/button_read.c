/* Author: Thia Wyrod & Sam Cristall
 * License: GPLv3
 */
/* This sample firmware simply drives an LED while a button is held, and shuts
 * it off while the button is released.
 */
#include <SystemClock.h>
#include <IOConfig.h>
#include <Structs.h>

#include "time_funcs.h"
#include "struct_aliases.h"

static const GlobalPin LED = LED4;
static const GlobalPin button = SW3;

int main(void)
{
  sys_clock_init(kCrysF, kSystemF);

  io_set_config(DEFAULT_LED_CFG, LED.io_port);
  globalPin_set_dir(PinDir_Output, &LED);
  io_set_config(DEFAULT_BUTTON_CFG, button.io_port);
  globalPin_set_dir(PinDir_Input, &button);

  while(1) {
    /* read the value of the button in, then set the LED to a matching result */
    if (globalPin_read(&button) == ON)
      globalPin_write(ON, &LED);
    else
      globalPin_write(OFF, &LED);
  }
  return 0;
}
