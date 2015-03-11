/* Author: Thia Wyrod
 * License: GPLv3
 */
#include <IOConfig.h>
#include <Structs.h>
#include <ADC.h>
#include <XPD.h>

#include "time_funcs.h"
#include "buffered_button.h"
#include "struct_aliases.h"

static const uint16_t kMsPerButtonPoll = 10; //main loop polls buttons every 10 ms
static const uint16_t kMsPerButtonClick = 50;
static BufferedButton read_switch = { SW3,
  kMsPerButtonClick/kMsPerButtonPoll, 0 };

static uint16_t volt_to_temper(uint16_t val)
{
  //insert the appropriate scaling factor and other operations as needed to
  //convert the ADC voltage 10-bit value to a temperature
  return val;
}

static const uint16_t kADCcfg = ADC_CLK_RATE_DIV_2|ADC_CONT_SAMPLING|
  ADC_ENABLE_FLOW_CONTROL|ADC_THREE_CHANNEL_MODE;
static const uint16_t kChannelNums[] = {1, 1, 1};

int main(void)
{
  init_clock();
  ADC_set_config(kADCcfg);
  ADC_set_channel_nums(kChannelNums);
  while (1) {
    if (read_buf_button(&read_switch)) {
      ADC_power_on();
      //because we are using flow control, we don't bother checking the busy flag
      xpd_echo_int(volt_to_temper(ADC_read()), XPD_Flag_UnsignedDecimal);
      xpd_putc('\n');
    }
    wait_ms(kMsPerButtonPoll);
  }
  ADC_power_off();
  return 0;
}
