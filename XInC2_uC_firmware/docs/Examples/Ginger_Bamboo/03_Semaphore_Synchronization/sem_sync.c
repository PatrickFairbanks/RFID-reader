/* Author: Thia Wyrod
 * License: GPLv3
 */
/* This sample firmware receives input on a single pin (imagined as a button
 * which can be physically pushed), counts the number of button pushes, and
 * drives a single output pin. It showcases the fundamentals of setting up
 * multiple threads and utilizing semaphores to share hardware and
 * variables across threads safely. It also showcases the use of the XPD.
 */
#include <stdlib.h>
#include <IOConfig.h>
#include <Semaphore.h>
#include <Thread.h>
#include <Structs.h>
#include <XPD.h>

#include "time_funcs.h"
#include "buffered_button.h"
#include "struct_aliases.h"

/* Constants related to the button */
static const size_t kMsPerPress = 1000; /* 1s of LED on per button click */
static const size_t kMsPerButtonPoll = 10; /* poll for button input every 10 ms */
static const size_t kMsPerButtonClick = 50; 

/* Constants related to the LED */
static const GlobalPin LED = LED4;
static const uint16_t kLedThrdNum = 1; /* LED logic will run on thread1 */
static const enum PinLogicState kLedInitState = OFF;
static const size_t kMsPerLedPoll = 20; /* 20ms reaction time to light LED */

/* declare this volatile, as it is shared across threads */
static volatile uint16_t pressesLeft = 0;
static const uint16_t kPressesSemNum = 0; /* use semaphore 0 for pressesLeft */

static void* led_thread(void* ptr)
{
  uint16_t curr_state = *(enum PinLogicState*)ptr;
  while(1) {
    sem_lock(kPressesSemNum); /* accessing pressesLeft, lock its sem */
    if (pressesLeft) {
      --pressesLeft;
      xpd_echo_int(pressesLeft, XPD_Flag_UnsignedDecimal);
      xpd_putc('\n');
      sem_unlock(kPressesSemNum); /* done with var for now, release sem ASAP */
      if (curr_state == OFF) { /* if LEDs are currently off, turn them on! */
        curr_state = ON;
        globalPin_write(ON, &LED);
      }
      wait_ms(kMsPerPress);
    }
    else {
      sem_unlock(kPressesSemNum); /* release the sem ASAP */
      if (curr_state == ON) { /* if LEDs are currently on, turn them off! */
        curr_state = OFF;
        globalPin_write(OFF, &LED);
      }
      wait_ms(kMsPerLedPoll);
    }
  }
  return NULL;
}

int main(void) /* keep in mind that main starts on thread0 */
{
  init_clock();

  io_set_config(DEFAULT_LED_CFG, LED.io_port);
  globalPin_set_dir(PinDir_Output, &LED);

  BufferedButton button = { SW3, kMsPerButtonClick/kMsPerButtonPoll, 0 };
  io_set_config(DEFAULT_BUTTON_CFG, button.gl_pin.io_port);
  globalPin_set_dir(PinDir_Input, &button.gl_pin);

  /* ensure that the LEDs begin in kLedInitState */
  globalPin_write(kLedInitState, &LED);

  /* note that because kLedInitState is a global var, it is accessible to all
   * threads without having to be passed in as an argument; this is strictly
   * for demonstration of thread_setup (although the 2nd argument can be NULL */
  thread_setup(led_thread, (void*)&kLedInitState, kLedThrdNum);
  thread_run(kLedThrdNum);

  /* main button polling loop */
  while(1) {
    if (read_buf_button(&button)) {
      sem_lock(kPressesSemNum); /* accessing pressesLeft, lock its sem */
      ++pressesLeft;
      xpd_echo_int(pressesLeft, XPD_Flag_UnsignedDecimal);
      xpd_putc('\n');
      sem_unlock(kPressesSemNum); /* release the sem ASAP */
    }
    wait_ms(kMsPerButtonPoll);
  }
  return 0;
}
