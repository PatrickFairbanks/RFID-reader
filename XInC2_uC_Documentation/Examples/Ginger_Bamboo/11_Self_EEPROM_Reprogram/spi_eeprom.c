/* Author: Thia Wyrod
 * License: GPLv3
 */
#include <IOConfig.h>
#include <Structs.h>
#include <SPI.h>
#include <EEPROM.h>
#include <XPD.h>

#include "time_funcs.h"
#include "buffered_button.h"
#include "struct_aliases.h"

static const uint16_t kMsPerButtonPoll = 10; //main loop polls buttons every 10 ms
static const uint16_t kMsPerButtonClick = 50;

static const uint16_t eeprom_var; //generates a variable in the EEPROM so that
//we reserve a "safe address" so that overwriting EEPROM here won't brick the firmware!
uint16_t const * const eeprom_addr = &eeprom_var-kInternalRAMOffset;

static BufferedButton buffered_switches[] = {
  { SW0, kMsPerButtonClick/kMsPerButtonPoll, 0 }, 
  { SW1, kMsPerButtonClick/kMsPerButtonPoll, 0 },
  { SW2, kMsPerButtonClick/kMsPerButtonPoll, 0 },
  { SW3, kMsPerButtonClick/kMsPerButtonPoll, 0 }
};
static const size_t kNumSwitches =
  sizeof(buffered_switches) / sizeof(*buffered_switches);

int main(void)
{
  init_clock();

  xpd_puts("\nValue in the EEPROM at boot: ");
  uint16_t eeprom_val = EEPROM_read_byte(SPI0, &kInternalChipSelect,
    (uint16_t*)eeprom_addr);
  xpd_echo_int(eeprom_val, XPD_Flag_Hex);

  SPI_set_config_optimal(kSystemF, SPI0);
  for (size_t i = 0; i < kNumSwitches; ++i) {
    io_set_config(DEFAULT_BUTTON_CFG, buffered_switches[i].gl_pin.io_port);
    globalPin_set_dir(PinDir_Input, &buffered_switches[i].gl_pin);
  }

  while(1) {
    for (size_t i = 0; i < kNumSwitches; ++i) {
      if ( read_buf_button(&buffered_switches[i]) ) {
        EEPROM_write_byte(SPI0, &kInternalChipSelect, (uint16_t*)eeprom_addr, i);
        xpd_puts("\nWrote ");
        xpd_echo_int(i, XPD_Flag_Hex);
        xpd_puts(" to the EEPROM");
      }
    }
    wait_ms(kMsPerButtonPoll);
  }
  return 0;
}
