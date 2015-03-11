/* Author: Sam Cristall
 * License: GPLv3
 */
#ifndef SKAA_COMMON_W_OUT_STREAM_H_
#define SKAA_COMMON_W_OUT_STREAM_H_

#include "HW_BBU.h"
#include "HW_LFSR.h"

template <typename BBUTy, typename CRCTy> class WOutStream {
public:
  using BBU = IBaseBandUnit<BBUTy>;
  using CRC = ILFSR<CRCTy>;

  WOutStream(BBU &bbu, CRC &crc) : bbu_(bbu), crc_(crc) {}

  ~WOutStream() {
    crc_.Push(0);
    bbu_.PushWord(crc_.Get()); 
  }

  void PushWord(uint16_t value) { 
    bbu_.PushWord(value); 
    crc_.Push(value);
  }

  void PushWordWithoutCrc(uint16_t value) {
    bbu_.PushWord(value);
  }

  bool Valid() { return true; }
  operator bool() { return Valid(); }

private:
  BBU &bbu_;
  CRC &crc_;
};

#endif
