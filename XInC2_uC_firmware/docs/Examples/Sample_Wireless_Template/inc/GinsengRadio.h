//
//  GinsengRadio.h
//  skaa
//
//  Created by Sam Cristall on 2014-07-22.
//
//
// License: GPLv3

#ifndef SKAA_COMMON_GINSENG_RADIO_H_
#define SKAA_COMMON_GINSENG_RADIO_H_


#include "RadioInterface.h"
#include "HW_SPI.h"
#include "HW_BBU.h"
#include "Port.h"
#include "OutputPin.h"
#include "make_array.h"
#include "GinsengRadioData.h"
#include "SoftTimer.h"

namespace Ginseng {

// Port E controls various radio GPIO, the following are all pins on Port E
static constexpr size_t ChipSelectPin = 0, RxOnPin = 1, RxNotOnPin = 2,
                        RxOnPaVaPc0Pin = 3, RxOnPaVaPc1Pin = 4, RxOnVpd0Pin = 5,
                        RxOnVpd1Pin = 6;

// Strobe commands are how the A7125 moves from mode to mode.  See the
// A7125 datasheet for more description.  We only really use PLL/Rx/Tx
static constexpr uint16_t StrobeSleep = 0x80, StrobeIdle = 0x90,
                          StrobeStandby = 0xA0, StrobePLL = 0xB0,
                          StrobeRx = 0xC0, StrobeTx = 0xD0, StrobeTFR = 0xE0,
                          StrobeRFR = 0xF0;

// GetGpioMask gets a configuration mask for the given pins.  It does not mask
// off the lower 8 bits.  The general control mechanism is to use pull downs
// when set as an input to set 0, and to drive high to set 1
static constexpr uint16_t GetGpioMask(uint16_t value) { return value; }
template <class First, class... Rest>
static constexpr uint16_t GetGpioMask(uint16_t value, First first_pin,
                                      Rest... rest) {
  return GetGpioMask(value | ((1 << first_pin) << PORT_CONFIG_OFFSET), rest...);
}

// To control the PA biasing, we use both programming and GPIO control
// the following is a mask for the GPIO control
// We would use pins, but the gpio controls need to be super fast
static constexpr uint16_t PA_GPIO_MASK = 0x78;

// Define some common masks using the PA_GPIO_MASK for common cases of biasing
// the Power Amplifier for the A7125
static constexpr uint16_t UPPER_POW_MASK =
    GetGpioMask(PA_GPIO_MASK, RxOnVpd1Pin, RxOnPaVaPc1Pin);
static constexpr uint16_t MID_POW_MASK =
    GetGpioMask(PA_GPIO_MASK, RxOnVpd0Pin, RxOnPaVaPc0Pin);
static constexpr uint16_t LOWER_POW_MASK =
    GetGpioMask(PA_GPIO_MASK, RxOnVpd0Pin);

// Initialize with all pins set as Outputs, driven low except RxOn
// and the chip select pin which are driven high
static constexpr uint16_t CTRL_PORT_INIT =
    GetGpioMask(0, RxOnVpd0Pin, RxOnVpd1Pin, RxOnPaVaPc0Pin, RxOnPaVaPc1Pin,
                RxOnPin, RxNotOnPin, ChipSelectPin) |
    (1 << RxOnPin) | (1 << ChipSelectPin);

// This is PH0, which switches in the receive or transmit BBU paths
static constexpr size_t BBU_PIN_NUMBER = 2;

// Group the PA mask entries into a lookup table
static constexpr auto PowerScalingGpioTable =
    make_array<uint16_t>(UPPER_POW_MASK, // 0
                         UPPER_POW_MASK, // 1
                         UPPER_POW_MASK, // 2
                         UPPER_POW_MASK, // 3
                         UPPER_POW_MASK, // 4
                         MID_POW_MASK,   // 5
                         MID_POW_MASK,   // 6
                         MID_POW_MASK,   // 7
                         MID_POW_MASK,   // 8
                         MID_POW_MASK,   // 9
                         MID_POW_MASK,   // 10
                         MID_POW_MASK,   // 11
                         LOWER_POW_MASK, // 12
                         LOWER_POW_MASK, // 13
                         LOWER_POW_MASK, // 14
                         LOWER_POW_MASK  // 15
                         );

static constexpr auto PowerScalingRegTable =
    make_array<uint16_t>(0x27, // 17.5 dBm
                         0x26, // 16.8
                         0x25, // 15.5
                         0x2C, // 14.8
                         0x24, // 13.7
                         0x2E, // 11.9
                         0x2D, // 10.4
                         0x24, // 8.6
                         0x23, // 7.4
                         0x22, // 5.1
                         0x30, // 3.0
                         0x20, // 0.0
                         0x25, // -3.0
                         0x24, // -7.5
                         0x2A, // -13.5
                         0x22  // -16.0
                         );

static constexpr size_t CALIBRATION_CHANNEL_LOW = 30;
static constexpr size_t CALIBRATION_CHANNEL_MID = 90;
static constexpr size_t CALIBRATION_CHANNEL_HIGH = 150;

static constexpr size_t INITIAL_PREAMBLE = 5;
static constexpr uint16_t PLL_TO_TX_TIME_IN_BBU_TICKS = 216;
static constexpr uint16_t RSSI_WAIT_TIME_IN_BBU_TICKS = 100;

template <class SPI, class BBUTy, class RadioPortTy, class BBUPinTy>
class GinsengRadio
    : public RadioInterface<GinsengRadio<SPI, BBUTy, RadioPortTy, BBUPinTy>> {
public:
  using BBU = IBaseBandUnit<BBUTy>;
  using RadioPort = IPort<RadioPortTy>;
  using BBUPin = IOutputPin<BBUPinTy>;

  GinsengRadio(SPI &spi, BBU &bbu, RadioPort &ctrl_port, BBUPin &bbu_pin)
      : spi_(spi), bbu_(bbu), ctrl_port_(ctrl_port), bbu_pin_(bbu_pin) {

    // Initialize the control port, set the BBU direction as an input
    ctrl_port_.Configure(CTRL_PORT_INIT);
    bbu_pin_.SetLow();

    // Configure/calibrate the radio
    ConfigureRadio();
  }

  //! Change the rf channel of the A7125
  void ChangeChannel(size_t channel) {
    assert(channel <= 49 && "Ginseng channel must be <= 49");
    bbu_.PushWord(0);
    bbu_.PushWord(0);
    spi_.Push(StrobePLL);
    // Use a simple scaling factor + offset to give good channel separation
    // Unsure how this was derived -- likely empirically a long long time ago
    WriteReg(A7125Reg::PLL1, (channel * 3) + 7);
  }

  //! Put the radio and the bbu into transmit mode
  //! \note Will send PREAMBLE for 16 * INITIAL_PREAMBLE bits
  //! \warning Transmission must begin immediately after running or timing may
  //!          break down
  void SwitchToTransmit(size_t power_level) {
    assert(power_level <= 15 && "Power level must be <= 15");

    // Put the radio into Tx Mode
    spi_.Push(StrobePLL);
    spi_.Push(StrobeTx);

    // Start a timer now, to start the countdown to poll cycle start
    auto timer = MakeSoftTimer(bbu_);
    timer.Start(PLL_TO_TX_TIME_IN_BBU_TICKS);

    // Get the power level and program it into the A7125
    WriteReg(A7125Reg::TXTEST, PowerScalingRegTable[power_level]);

    // Set the BBU direction pin to high, indicating output
    bbu_pin_.SetHigh();
    // Turn off RxOn, Turn on RxNotOn, and Or with the power scaling mask
    uint16_t new_port_state = ctrl_port_.Status();
    new_port_state &= (~(1 << RxOnPin) & ~(PA_GPIO_MASK << 8));
    new_port_state |= ((1 << RxNotOnPin) | PowerScalingGpioTable[power_level]);
    ctrl_port_.Configure(new_port_state);

    // Let the timer elapse so that we are sure the radio is on and ready to go
    timer.Wait();

    // When we first strobe to Tx, we need to send a bunch of preamble to cal
    // the radio into a good state. More preamble will result in easier lock-on
    bbu_.SetTransmitting(true);
    for (size_t i = 0; i < INITIAL_PREAMBLE; ++i)
      bbu_.PushWord(PREAMBLE);
  }

  //! Put the radio and the bbu into receive mode
  void SwitchToReceive() {
    // Set RxOn, Unset RxOff and set all of the PA control pins
    uint16_t new_port_state = ctrl_port_.Status();
    new_port_state &= ~(1 << RxOnPaVaPc0Pin) & ~(1 << RxOnPaVaPc1Pin) &
                      ~(1 << RxOnVpd0Pin)    & ~(1 << RxOnVpd1Pin) & 
                      ~(1 << RxNotOnPin);
    new_port_state |= ((1 << RxOnPaVaPc0Pin) | (1 << RxOnPaVaPc1Pin) |
                       (1 << RxOnVpd0Pin)    | (1 << RxOnVpd1Pin)) << 8;
    new_port_state |= (1 << RxOnPin);

    ctrl_port_.Configure(new_port_state);
    
    // Set the BBU direction pin to low, indicating input
    bbu_pin_.SetLow();
    spi_.Push(StrobePLL);
    spi_.Push(StrobeRx);
    bbu_.SetTransmitting(false);
  }

  //! Get an RSSI sample from the radio -- this will take >100 bbu ticks
  //! \note The radio will be in PLL mode after exiting
  uint16_t GetRSSI() {
    WriteReg(A7125Reg::MODECTRL, A7125_MODECTRL_MEASURE_ADC);
    WriteReg(A7125Reg::MODECTRL, A7125_MODECTRL_MEASURE_RSSI);
    spi_.Push(StrobeRx);
    while (ReadReg(A7125Reg::MODECTRL) & 1)
      continue;
    auto timer = MakeSoftTimer(bbu_);
    timer.Start(RSSI_WAIT_TIME_IN_BBU_TICKS);
    timer.Wait();
    WriteReg(A7125Reg::MODECTRL, A7125_MODECTRL_AUTO_IF_OFFSET);
    uint16_t result = ReadReg(A7125Reg::MODECTRL);
    spi_.Push(StrobePLL);
    return result;
  }

private:
  template <class...Args>
  void WriteReg(A7125Reg reg, Args... vals) {
    spi_.Push(static_cast<uint16_t>(reg), vals...);
  }

  uint16_t ReadReg(A7125Reg reg) {
    // To read a reg on the A7125, you set bit 6
    return spi_.Get(static_cast<uint16_t>(reg) | (1 << 6));
  }

  void ConfigureRadio() {
    // This will reset the radio, preparing it for configuration
    WriteReg(A7125Reg::MODE, A7125_MODE_INITIAL);

    WriteReg(A7125Reg::IDCODE, 0xAA, 0x55, 0x55, 0x55);

    // Loop through the config data, and if it doesn't equal DONT_CONFIG,
    // write the reg
    for (size_t i = 0; i < A7125_NUM_REGS; ++i) {
      uint16_t value = A7125Config[i];
      if (value != A7125_REG_DONT_CONFIG)
        WriteReg(static_cast<A7125Reg>(i), value);
    }

    // Write the calibration reg to do initial calibrations
    spi_.Push(StrobePLL);
    WriteReg(A7125Reg::CALIBRATION, A7125_CALIBRATION_INITAL);
    while (ReadReg(A7125Reg::CALIBRATION) & A7125_CALIBRATION_INITAL)
      continue;

    // We do a bit of repitition here -- we will set up the PLL to a channel,
    // then calibrate it, then assert it's okay
    // declare a lambda to reduce local repetition
    auto CHGroupCal = [&](size_t channel) {
      WriteReg(A7125Reg::PLL1, channel);
      WriteReg(A7125Reg::CALIBRATION, A7125_CALIBRATION_CHANNEL);
      while (ReadReg(A7125Reg::CALIBRATION) & A7125_CALIBRATION_CHANNEL)
        continue;

      assert(!(ReadReg(A7125Reg::VCOCAL1) & (1 << 3)) &&
             !(ReadReg(A7125Reg::VCOCCAL) & (1 << 4)) &&
             "Failed to cal A7125");
    };

    // Check the various channel groups
    CHGroupCal(CALIBRATION_CHANNEL_LOW);
    CHGroupCal(CALIBRATION_CHANNEL_MID);
    CHGroupCal(CALIBRATION_CHANNEL_HIGH);

    // Put the radio into standby
    spi_.Push(StrobeStandby);

    // This should never happen, but good for debugging broken hardware
    assert(!(ReadReg(A7125Reg::IFCAL1) & (1 << 4)) &&
           "Failed to cal A7125");

    // Read these out to clear their calibration registers
    // but discard the data
    // TODO: We should check if we need to assert these too
    ReadReg(A7125Reg::IFCAL2);
    ReadReg(A7125Reg::VCOCAL2);
  }

  SPI &spi_;
  BBU &bbu_;
  RadioPort &ctrl_port_;
  BBUPin &bbu_pin_;
};
}

template <class S, class B, class Po, class Pi>
using GinsengRadio = Ginseng::GinsengRadio<S, B, Po, Pi>;

template <class S, class B, class Port, class Pin>
constexpr GinsengRadio<S, B, Port, Pin> MakeGinsengRadio(S &spi, B &bbu, Port &ctrl_port,
                                               Pin &bbu_pin) {
  return GinsengRadio<S, B, Port, Pin>(spi, bbu, ctrl_port, bbu_pin);
}

#endif
