/* Author: Thia Wyrod
 * License: GPLv3
 */
#include <stdlib.h>
#include <Structs.h>
#include <Thread.h>
#include <Semaphore.h>

#include "time_funcs.h"
#include "struct_aliases.h"

typedef struct Button_LED_Pair {
  GlobalPin const LED;
  GlobalPin const button;
} Button_LED_Pair;

Button_LED_Pair const button_led_pairs[] = {
  { LED1, SW0 }, { LED2, SW1 }, { LED3, SW2 }, { LED4, SW3 }
};
static const size_t kNumPairs = sizeof(button_led_pairs)/sizeof(*button_led_pairs);

static const uint16_t kLEDLockSemNum = 0;

static void* led_thread(void* pair_ptr)
{
  Button_LED_Pair const * const pair = (Button_LED_Pair const * const)(pair_ptr); 
  while(1) {
    if (globalPin_read(&pair->button) == ON) {
      sem_lock(kLEDLockSemNum);
      globalPin_write(ON, &pair->LED);
      while (globalPin_read(&pair->button) == ON) {}
      globalPin_write(OFF, &pair->LED);
      sem_unlock(kLEDLockSemNum);
    }
  }
  return NULL;
}

int main(void)
{
  init_clock();
  for (size_t i = 0; i < kNumPairs; ++i) {
    io_set_config(DEFAULT_LED_CFG, button_led_pairs[i].LED.io_port);
    io_set_config(DEFAULT_BUTTON_CFG, button_led_pairs[i].button.io_port);
    globalPin_set_dir(PinDir_Output, &button_led_pairs[i].LED);
    globalPin_set_dir(PinDir_Input, &button_led_pairs[i].button);
    globalPin_write(OFF, &button_led_pairs[i].LED);

    thread_setup(led_thread, (void *)&button_led_pairs[i], i+1);
    thread_run(i+1);
  }
  while(1) {}
  return 0;
}
