/* Author: Sam Cristall
 * License: GPLv3
 * Edited by: Thia Wyrod
 */
#include "XPD.h"
#include "CRC.h"
#include "FEC.h"
#include "GinsengRadio.h"
#include "Wireless.h"

/* System consts for Ginseng, do not modify */
const enum crystal_freq kCrysFreq = crys_24_576_MHz;
const enum sys_freq kSystemF = _49_152_MHz;
const SystemFrequency kSysFreq = SystemFrequency::_49_152_MHz;

/* Parameters for the wireless setup - customize to suit your protocol */
static const size_t kPacketSize = 8;
static const size_t kNumPacketsInBuffer = 16;
static const size_t kNumReceivers = 1;
static const uint16_t kTransmitTimeInBbuTicks = 1000;
static const uint16_t kPollCycleTime = 
                          kTransmitTimeInBbuTicks * (1 + kNumReceivers);

using Packet = std::array<uint16_t, kPacketSize>;

/* Implementation of a simple circular buffer: please note that it is NOT
 * thread-safe as-is, unless a maximum of one thread writes to it, and a maximum
 * of one thread reads from it */
template <typename T, size_t Len>
class CircularBuffer {
  public:
    CircularBuffer() : read_(0), write_(0) {}

    const T& Peek() {
      return buffer_[read_];
    }

    void Pop() {
      if (read_ != write_)
        read_++;
      if (read_ == Len)
        read_ = 0;
    }

    void Push(Packet& packet) {
      size_t end = read_ == 0 ? Len - 1 : read_ - 1;
      if (write_ != end) {
        buffer_[write_] = packet;
        write_++;
      }
    }

    bool HasData() const {
      return write_ != read_;
    }
    
  private:
    std::array<T, Len> buffer_;
    size_t read_, write_;
};

/* Convenience struct for packet-storing buffers */
struct WirelessBuffers {
  CircularBuffer<Packet, kNumPacketsInBuffer> tx_buffer;
  CircularBuffer<Packet, kNumPacketsInBuffer> rx_buffer;
};

/* This function defines the thread that will be responsible for filling and
 * emptying the rx and tx buffers respectively by listening on the radio/BBU and
 * transmitting data. Only modify this if you are feeling brave!
 */
void* wireless_thread(void* arg) {
  auto* buffers = reinterpret_cast<WirelessBuffers*>(arg);
  auto& tx_buffer = buffers->tx_buffer;
  auto& rx_buffer = buffers->rx_buffer;

  RadioPort radio_port;
  RadioBbuPort bbu_port;
  RadioSpi spi(kSysFreq);
  RadioBbu bbu(kSysFreq, kRadioStartWord);
  Crc16Lfsr crc16;
  FecLfsr fec;

  auto chip_select =
     MakeOutputPin(radio_port, kRadioChipSelectPinNumber, true);
  auto bbu_pin = MakeOutputPin(bbu_port, kBbuDirectionPinNumber, true);
  auto spi_with_cs = MakeSpiWithChipSelect(spi, chip_select); 
  auto radio = MakeGinsengRadio(spi_with_cs, bbu, radio_port, bbu_pin);
  auto wireless = MakeWireless(bbu, crc16, fec, kRadioStartWord);
  auto poll_cycle_clock = MakeSoftTimer(bbu);
  uint16_t ack_vector = 0, rx_acks = 0;

  auto GetReceiveClock = [&](size_t rx) {
    auto timer = MakeSoftTimer(bbu);
    timer.Start((rx + 1) + kTransmitTimeInBbuTicks);
    return timer;
  };

  while (true) {
    radio.ChangeChannel(0);
    radio.SwitchToTransmit(0);

    if (auto wout = wireless.TryAcquireOutputStream()) {
      wout.PushWord(ack_vector);
      ack_vector = 0;
      if (tx_buffer.HasData()) {
        for (auto element : tx_buffer.Peek())
          wout.PushWord(element);
      }
    }

    radio.SwitchToReceive();

    for (size_t i = 0; i < kNumReceivers; ++i) {
      if (auto win = wireless.TryAcquireInputStream(GetReceiveClock(i))) {
        bool ack_;
        ack_ = win.GetWordWithCrc();
        Packet rx_temp_buffer;
        for (auto& element : rx_temp_buffer)
          element = win.GetWordWithCrc();

        if (win.CheckCrc()) {
          if (ack_)
            rx_acks |= (1 << i);
          rx_buffer.Push(rx_temp_buffer);
          ack_vector |= (1 << i);
        }
      }
    }

    if (rx_acks == ((1 << kNumReceivers) - 1)) {
      rx_acks = 0;
      tx_buffer.Pop();
    }

    poll_cycle_clock.Tick(kPollCycleTime);
  }
}

/* This function defines the thread that will be responsible for filling and
 * emptying the tx and rx buffers respectively by executing your
 * program/protocol logic.
 */
void* worker_thread(void* arg)
{
  auto* buffers = reinterpret_cast<WirelessBuffers*>(arg);
  auto& tx_buffer = buffers->tx_buffer;
  auto& rx_buffer = buffers->rx_buffer;
  while (true) {
    if (rx_buffer.HasData()) {
      Packet pkt = rx_buffer.Peek();
      rx_buffer.Pop();
      //do something with this packet!
      //to potentially transmit data, push data to the tx_buffer
    }
  }
  return NULL;
}

static WirelessBuffers data_buffers;
void* (*const thrd_funcs[])(void*) = { wireless_thread, worker_thread };

/* Set up and run all secondary threads, then sleep */
int main() {
  sys_clock_init(kCrysFreq, kSystemF);
  size_t curr_thrd_num = 1;
  for (auto& thrd_func : thrd_funcs) {
    thread_setup(thrd_func, &data_buffers, curr_thrd_num);
    thread_run(curr_thrd_num);
    ++curr_thrd_num;
  }
  return 0;
}

