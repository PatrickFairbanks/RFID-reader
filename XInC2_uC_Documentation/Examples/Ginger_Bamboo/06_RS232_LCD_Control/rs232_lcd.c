/* Author: Thia Wyrod
 * License: GPLv3
 */
#include <IOConfig.h>
#include <Structs.h>
#include <UART.h>

#include "time_funcs.h"
#include "buffered_button.h"
#include "struct_aliases.h"

typedef struct RS232_Pins {
  UART_Pin_Pair const D;
  GlobalPin const RTS;
  GlobalPin const CTS;
} RS232_Pins;

/* We have different ports for each board because active-high buttons CANNOT
 * share a port with active-low IO pins. There are also significant layout
 * differences between boards.
 */
#ifdef __BOARD_GINGER__
#define data_pins { { GPIO_C, io_PC0, 0x1, DEFAULT_POLARITY },\
                    { GPIO_C, io_PC1, 0x2, DEFAULT_POLARITY }, 10000 }
#define RTS_pin { GPIO_C, io_PC4, 0x10, DEFAULT_POLARITY }
#define CTS_pin { GPIO_C, io_PC5, 0x20, DEFAULT_POLARITY }

#else
#define data_pins { { GPIO_E, io_PE, 0x1, DEFAULT_POLARITY },\
                    { GPIO_E, io_PE, 0x2, DEFAULT_POLARITY }, 10000 }
#define RTS_pin { GPIO_E, io_PE, 0x4, DEFAULT_POLARITY }
#define CTS_pin { GPIO_E, io_PE, 0x8, DEFAULT_POLARITY }

#endif //__BOARD_GINGER__

static const RS232_Pins rs232_pins = { data_pins, RTS_pin, CTS_pin };

static const uint16_t kMsPerButtonPoll = 10; //main loop polls buttons every 10 ms
static const uint16_t kMsPerButtonClick = 50;
static BufferedButton clear_switch = { SW3,
  kMsPerButtonClick/kMsPerButtonPoll, 0 };
static BufferedButton display_switch = { SW2,
  kMsPerButtonClick/kMsPerButtonPoll, 0 };
static BufferedButton shift_switch = { SW1,
  kMsPerButtonClick/kMsPerButtonPoll, 0 };

static GlobalPin const * const button_pins[] = { &clear_switch.gl_pin,
  &display_switch.gl_pin, &shift_switch.gl_pin };
static const size_t kNumButtonPins = sizeof(button_pins) / sizeof(*button_pins);

static GlobalPin const * const input_pins[] = { &rs232_pins.D.rx,
  &rs232_pins.CTS };
static const size_t kNumInputPins = sizeof(input_pins) / sizeof(*input_pins);

static GlobalPin const * const output_pins[] = { &rs232_pins.D.tx,
  &rs232_pins.RTS };
static const size_t kNumOutputPins = sizeof(output_pins) / sizeof(*output_pins);


static void rs232_write_byte(uint16_t byte, RS232_Pins const * pins)
{
  globalPin_write(0, &pins->RTS); //send a request to send
  while(globalPin_read(&pins->CTS)); //wait until cleared to send
  uart_write_byte(byte, &pins->D);
  globalPin_write(1, &pins->RTS);
}

static uint16_t rs232_read_byte(RS232_Pins const * pins)
{
  while(globalPin_read(&pins->CTS)); //wait until the device requests to send
  globalPin_write(0, &pins->RTS); //notify the device that it is cleared to send
  uint16_t byte = uart_read_byte(&pins->D);
  globalPin_write(1, &pins->RTS);
  return byte;
}

static void clear_LCD(RS232_Pins const * pins)
{
  rs232_write_byte(0, pins);
}

static void display_LCD(RS232_Pins const * pins)
{
  //rs232_write_byte(//some_byte, pins);
}

static void shift_LCD(RS232_Pins const * pins)
{
  //rs232_write_byte(//some_byte, pins);
}

int main(void)
{
  init_clock();
  for (size_t i = 0; i < kNumInputPins; ++i) {
    io_set_config(DEFAULT_IO_CFG, input_pins[i]->io_port);
    globalPin_set_dir(PinDir_Input, input_pins[i]);
  }
  for (size_t i = 0; i < kNumOutputPins; ++i) {
    io_set_config(DEFAULT_IO_CFG, output_pins[i]->io_port);
    globalPin_set_dir(PinDir_Output, output_pins[i]);
  }
  for (size_t i = 0; i < kNumButtonPins; ++i) {
    io_set_config(DEFAULT_BUTTON_CFG, button_pins[i]->io_port);
    globalPin_set_dir(PinDir_Input, button_pins[i]);
  }

  while(1) {
    if ( read_buf_button(&clear_switch) )
      clear_LCD(&rs232_pins);
    else if ( read_buf_button(&display_switch) )
      display_LCD(&rs232_pins);
    else if ( read_buf_button(&shift_switch) )
      shift_LCD(&rs232_pins);
  wait_ms(kMsPerButtonPoll);
  }
  return 0;
}
