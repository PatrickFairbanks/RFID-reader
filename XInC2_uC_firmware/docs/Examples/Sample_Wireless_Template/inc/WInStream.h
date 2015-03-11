/* Author: Sam Cristall
 * License: GPLv3
 */
#ifndef SKAA_COMMON_W_IN_STREAM_H_
#define SKAA_COMMON_W_IN_STREAM_H_

#include "HW_BBU.h"
#include "HW_LFSR.h"

template <typename BBUTy, typename CRCTy, typename FECTy> class WInStream {
public:
  using BBU = IBaseBandUnit<BBUTy>;
  using CRC = ILFSR<CRCTy>;
  using FEC = ILFSR<FECTy>;
  
  WInStream(BBU &bbu, CRC &crc, FEC &fec, bool valid)
      : bbu_(bbu), crc_(crc), fec_(fec), valid_(valid) {}

  uint16_t GetWordWithNoCrcOrFec() {
    return GetWord(false, false);
  }

  uint16_t GetWordWithCrc() {
    return GetWord(true, false);
  }

  uint16_t GetWordWithCrcAndFec() {
    return GetWord(true, true);
  }

  void PushCrc(uint16_t word) {
    crc_.Push(word);
  }

  template <typename ...ExtraWords>
  bool CheckCrc(uint16_t first_word, ExtraWords... rest) {
    crc_.Push(first_word);
    return CheckCrc(rest...);
  }

  bool CheckCrc() {
    return crc_.Get() == 0;
  }

  bool Valid() { return valid_; }
  operator bool() { return Valid(); }

private:
  uint16_t GetWord(bool enable_crc, bool enable_fec) { 
    uint16_t word = bbu_.GetWord();
    if (enable_crc) crc_.Push(word);
    if (enable_fec) fec_.Push(word);
    return word;
  }

  BBU &bbu_;
  CRC &crc_;
  FEC &fec_;
  bool valid_;
};


#endif
