/* Author: Thia Wyrod
 * License: GPLv3
 */
#include <IOConfig.h>
#include <Structs.h>
#include <SPI.h>

#include "time_funcs.h"
#include "buffered_button.h"
#include "struct_aliases.h"

static const GlobalPin compare_LED = LED4;
static const uint16_t kMsPerButtonPoll = 10; //main loop polls buttons every 10 ms
static const uint16_t kMsPerButtonClick = 50;
static BufferedButton write_switch = { SW3,
  kMsPerButtonClick/kMsPerButtonPoll, 0 };
static BufferedButton read_switch = { SW2,
  kMsPerButtonClick/kMsPerButtonPoll, 0 };

static GlobalPin const * const input_pins[] = { &write_switch.gl_pin,
  &read_switch.gl_pin
};
static const size_t kNumInputPins = sizeof(input_pins) / sizeof(*input_pins);


static void read_EEPROM(enum SPI_name spi)
{
  //while (condition) {
  //  uint16_t data = SPI_read(spi);
  //}
}

static void write_EEPROM(enum SPI_name spi)
{
  //while (condition) {
  //  SPI_write(data, spi);
  //}
}

int main(void)
{
  init_clock();
  SPI_set_config_optimal(kSystemF, SPI1);
  for (size_t i = 0; i < kNumInputPins; ++i) {
    io_set_config(DEFAULT_BUTTON_CFG, input_pins[i]->io_port);
    globalPin_set_dir(PinDir_Input, input_pins[i]);
  }
  io_set_config(DEFAULT_LED_CFG, compare_LED.io_port);
  globalPin_set_dir(PinDir_Output, &compare_LED);

  while(1) {
    if ( read_buf_button(&read_switch) )
      read_EEPROM(SPI1);
    else if ( read_buf_button(&write_switch) )
      write_EEPROM(SPI1);
    wait_ms(kMsPerButtonPoll);
  }
  return 0;
}
