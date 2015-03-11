/* Author: Sam Cristall
 * License: GPLv3
 */
#ifndef SKAA_COMMON_WIRELESS_H_
#define SKAA_COMMON_WIRELESS_H_

#include "SoftTimer.h"
#include "WInStream.h"
#include "WOutStream.h"
#include "HW_BBU.h"
#include "HW_LFSR.h"
#include "Ginseng.h"

//! Wireless class that offers creation of input and output streams to the XInC2
//! wireless base band
template <class BBUTy, class CRCTy, class FECTy> class Wireless {
public:
  using BBU = IBaseBandUnit<BBUTy>;
  using CRC = ILFSR<CRCTy>;
  using FEC = ILFSR<FECTy>;
  using WInTy = WInStream<BBUTy, CRCTy, FECTy>;
  using WOutTy = WOutStream<BBUTy, CRCTy>;

  //! Constructor
  constexpr Wireless(BBU &bbu, CRC &crc, FEC &fec, uint16_t start_word)
      : bbu_(bbu), crc_(crc), fec_(fec), start_word_(start_word) {}

  //! Try to acquire base band input stream
  template <typename ClockTy>
  WInTy TryAcquireInputStream(ClockTy timer) {
    ConfigureCrcDecoder(crc_);
    ConfigureFecDecoder(fec_);

    // By unlocking the timing, we enable lots of fun stuff in the BBU like
    // timing recovery and preamble detection!
    bbu_.UnlockTiming();
    
    do {
      while (!bbu_.ContainsPreamble()) {
        if (timer.HasElapsed())
          return WInTy(bbu_, crc_, fec_, false);
      }
      while (bbu_.ContainsPreamble()) {
        if (timer.HasElapsed())
          return WInTy(bbu_, crc_, fec_, false);
      }
    } while (bbu_.PeekWord() != start_word_);
    
    bbu_.LockTiming();

    return WInTy(bbu_, crc_, fec_, true);
  }

  //! Try to acquire base band output stream
  WOutTy TryAcquireOutputStream() { 
    ConfigureCrcEncoder(crc_);
    ConfigureFecEncoder(fec_);
    return WOutTy(bbu_, crc_);
  }


private:
  BBU &bbu_;
  CRC &crc_;
  FEC &fec_;
  uint16_t start_word_;
};

template <class B, class C, class F>
constexpr Wireless<B, C, F> MakeWireless(B &bbu, C &crc, F &fec,
                                         uint16_t start_word) {
  return Wireless<B, C, F>(bbu, crc, fec, start_word);
}

#endif
