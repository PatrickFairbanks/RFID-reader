/* Author: Thia Wyrod
 * License: GPLv3
 */
#include <IOConfig.h>
#include <Structs.h>
#include <I2C.h>

#include "time_funcs.h"
#include "buffered_button.h"
#include "struct_aliases.h"

/* We have different ports for each board because active-high buttons CANNOT
 * share a port with active-low IO pins. There are also significant layout
 * differences between boards.
 */
#ifdef __BOARD_GINGER__
#define i2c_pin_vals { GPIO_C, io_PC0, 0x1, DEFAULT_POLARITY },\
                     { GPIO_C, io_PC1, 0x2, DEFAULT_POLARITY }
#else
#define i2c_pin_vals { GPIO_E, io_PE, 0x1, DEFAULT_POLARITY },\
                     { GPIO_E, io_PE, 0x2, DEFAULT_POLARITY }
#endif //__BOARD_GINGER__
static const I2C_Pin_Pair i2c_pins = { i2c_pin_vals, 0xFF5E, 49152 };

static const uint16_t kMsPerButtonPoll = 10; //main loop polls buttons every 10 ms
static const uint16_t kMsPerButtonClick = 50;
static BufferedButton clear_switch = { SW3,
  kMsPerButtonClick/kMsPerButtonPoll, 0 };
static BufferedButton display_switch = { SW2,
  kMsPerButtonClick/kMsPerButtonPoll, 0 };
static BufferedButton shift_switch = { SW1,
  kMsPerButtonClick/kMsPerButtonPoll, 0 };

static GlobalPin const * const global_pins[] = { &clear_switch.gl_pin,
  &display_switch.gl_pin, &shift_switch.gl_pin, &i2c_pins.sda, &i2c_pins.scl };
static const size_t kNumGlobalPins = sizeof(global_pins) / sizeof(*global_pins);


static void clear_LCD(I2C_Pin_Pair const * pins)
{
  i2c_write_start(pins);
  //i2c_write_to_addr(//lcd_address, byte, pins);
  i2c_write_stop(pins);
}

static void display_LCD(I2C_Pin_Pair const * pins)
{
  i2c_write_start(pins);
  //i2c_write_to_addr(//lcd_address, byte, pins);
  i2c_write_stop(pins);
}

static void shift_LCD(I2C_Pin_Pair const * pins)
{
  i2c_write_start(pins);
  //i2c_write_to_addr(//lcd_address, byte, pins);
  i2c_write_stop(pins);
}

int main(void)
{
  init_clock();
  for (size_t i = 0; i < kNumGlobalPins; ++i) {
    io_set_config(DEFAULT_IO_CFG, global_pins[i]->io_port);
    globalPin_set_dir(PinDir_Input, global_pins[i]);
  }
  i2c_set_mode(I2C_Master, &i2c_pins); 

  while(1) {
    if ( read_buf_button(&clear_switch) )
      clear_LCD(&i2c_pins);
    else if ( read_buf_button(&display_switch) )
      display_LCD(&i2c_pins);
    else if ( read_buf_button(&shift_switch) )
      shift_LCD(&i2c_pins);
  }
  wait_ms(kMsPerButtonPoll);
  return 0;
}
