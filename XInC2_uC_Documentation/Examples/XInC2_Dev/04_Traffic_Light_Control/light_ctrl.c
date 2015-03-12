/* Author: Thia Wyrod
 * License: GPLv3
 */
#include <stdlib.h>
#include <IOConfig.h>
#include <Thread.h>
#include <Structs.h>

#include "time_funcs.h"
#include "buffered_button.h"
#include "struct_aliases.h"

enum LightCtrlMode {
  Auto,
  Manual
};

enum LightState {
  Red,
  Amber,
  Green,
  None
};

enum TrafficLightIndex {
  Red_Index = 0,
  Amber_Index = 1,
  Green_Index = 2,
  kLEDsPerTL = 3
};

typedef struct TrafficLight {
  enum LightState state;
  GlobalPin const leds[kLEDsPerTL];
} TrafficLight;


static void tl_change_state(TrafficLight* tl, enum LightState new_state)
{
  tl->state = new_state;
  size_t ind = kLEDsPerTL;
  switch(new_state) {
  case None:
    break;
  case Green:
    ind = Green_Index;
    break;
  case Red:
    ind = Red_Index;
    break;
  case Amber:
    ind = Amber_Index;
    break;
  }
  for (size_t i = 0; i < kLEDsPerTL; ++i) {
    if (i == ind)
      globalPin_write(ON, &tl->leds[i]);
    else
      globalPin_write(OFF, &tl->leds[i]);
  }
}


static TrafficLight lights[] = {
  { None, { LED2, LED3, LED4 } }, 
  { None, { LED5, LED6, LED7 } }
};
static const size_t kNumLights = sizeof(lights)/sizeof(*lights);


static const uint16_t kMsPerButtonPoll = 10; //main loop polls buttons every 10 ms
static const uint16_t kMsPerButtonClick = 50;
static BufferedButton mode_switch_button = { SW3,
  kMsPerButtonClick/kMsPerButtonPoll, 0 };
static BufferedButton manual_ctrl_button = { SW2,
  kMsPerButtonClick/kMsPerButtonPoll, 0 };

static BufferedButton * const buttons[] = { &mode_switch_button,
  &manual_ctrl_button };
static const size_t kNumButtons = sizeof(buttons) / sizeof(*buttons);


static const uint16_t kAutoThrdNum = 1;
static const size_t kMsGreen = 5000;
static const size_t kMsAmber = 1500;
static const size_t kMsAllRed = 2500;

static void* auto_ctrl_thrd(void* ptr)
{
  size_t curr_green_light = kNumLights;
  for (size_t i = 0; i < kNumLights; ++i) {
    if (lights[i].state == Green)
      curr_green_light = i;
    else
      tl_change_state(&lights[i], Red);
  }
  if (curr_green_light == kNumLights)
    curr_green_light = 0;
  while(1) {
    tl_change_state(&lights[curr_green_light], Green);
    wait_ms(kMsGreen);
    tl_change_state(&lights[curr_green_light], Amber);
    wait_ms(kMsAmber);
    tl_change_state(&lights[curr_green_light], Red);
    if (++curr_green_light == kNumLights)
      curr_green_light = 0;
    wait_ms(kMsAllRed);
  }
  return NULL;
}

int main(void)
{
  init_clock();

  // Configure buttons
  for (size_t i = 0; i < kNumButtons; ++i) {
    io_set_config(DEFAULT_BUTTON_CFG, buttons[i]->gl_pin.io_port);
    globalPin_set_dir(PinDir_Input, &buttons[i]->gl_pin);
  }

  // Configure lights, initialize to be all off
  for (size_t i = 0; i < kNumLights; ++i) {
    for (size_t j = 0; j < kLEDsPerTL; ++j) {
      io_set_config(DEFAULT_LED_CFG, lights[i].leds[j].io_port);
      globalPin_set_dir(PinDir_Output, &lights[i].leds[j]);
    }
    tl_change_state(&lights[i], None);
  }

  enum LightCtrlMode ctrl_mode = Manual;
  while(1) {
    if ( read_buf_button(&mode_switch_button) ) {
      if (ctrl_mode == Auto) {
        ctrl_mode = Manual;
        thread_stop(kAutoThrdNum);
      }
      else {
        ctrl_mode = Auto;
        thread_setup(auto_ctrl_thrd, NULL, kAutoThrdNum);
        thread_run(kAutoThrdNum);
      }
    }
    if (ctrl_mode == Manual) {
      if (read_buf_button(&manual_ctrl_button)) {
        if (lights[0].state != Green) {
          tl_change_state(&lights[0], Green);
          tl_change_state(&lights[1], Red);
        }
        else {
          tl_change_state(&lights[0], Red);
          tl_change_state(&lights[1], Green);
        }
      }
    }
    wait_ms(kMsPerButtonPoll);
  }
  return 0;
}
