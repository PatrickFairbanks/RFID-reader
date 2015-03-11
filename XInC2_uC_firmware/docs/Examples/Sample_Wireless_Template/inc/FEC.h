/* Author: Sam Cristall
 * License: GPLv3
 */
#ifndef SKAA_COMMON_FEC_H_
#define SKAA_COMMON_FEC_H_

#include <array>
#include "HW_LFSR.h"

static const std::array<uint16_t, 512> FEC_Syndrome_Table_9Bit {{
0x0000,
0x0006,0x0007,0x0189,0x0008,0x010B,0x018A,0x00E9,0x0009,0x006B,0x010C,0x0112,0x018B,0x013C,0x00EA,0x008D,0x000A,
0x000F,0x006C,0x01FA,0x010D,0x01CC,0x0113,0x00BE,0x018C,0x00B3,0x013D,0x01EE,0x00EB,0x0094,0x008E,0x0080,0x000B,
0x0002,0x01F1,0x001E,0x006D,0x00CC,0x01FB,0x0016,0x010E,0x01C6,0x01CD,0x0035,0x0114,0x0150,0x00BF,0x007C,0x018D,
0x01F5,0x00B4,0x0172,0x013E,0x017C,0x01EF,0x0026,0x00EC,0x0020,0x0095,0x0157,0x008F,0x0040,0x0081,0x014E,0x000C,
0x00B0,0x0003,0x0068,0x01F2,0x003D,0x001F,0x01C3,0x006E,0x01A6,0x00CD,0x01A3,0x01FC,0x00D9,0x0017,0x0196,0x010F,
0x0051,0x01C7,0x0177,0x01CE,0x00F4,0x0036,0x01E4,0x0115,0x01A9,0x0151,0x0142,0x00A0,0x0163,0x007D,0x00FE,0x018E,
0x001A,0x01F6,0x0185,0x00B5,0x0181,0x0173,0x0064,0x013F,0x0199,0x017D,0x00AE,0x01D0,0x01DB,0x0027,0x004E,0x00ED,
0x01FF,0x0021,0x00B9,0x0096,0x0056,0x0158,0x00D2,0x0070,0x00DC,0x0041,0x0148,0x0082,0x01B8,0x014F,0x0047,0x000D,
0x01CA,0x00B1,0x0092,0x0004,0x0109,0x0069,0x013A,0x01F3,0x017A,0x003E,0x005E,0x0000,0x00CA,0x01C4,0x016E,0x006F,
0x00F2,0x01A7,0x0161,0x00CE,0x003B,0x01A4,0x00D7,0x01FD,0x0054,0x00DA,0x01B6,0x0018,0x019F,0x0197,0x01D9,0x00F0,
0x0039,0x0052,0x019D,0x01C8,0x0107,0x0178,0x00C8,0x01CF,0x01E7,0x00F5,0x002E,0x0037,0x0105,0x01E5,0x0103,0x0116,
0x01B1,0x01AA,0x0123,0x0152,0x01E9,0x0143,0x015D,0x00A1,0x00F7,0x0164,0x011B,0x007E,0x0010,0x00FF,0x00C2,0x018F,
0x0154,0x001B,0x0032,0x01F7,0x01EB,0x0186,0x012F,0x00B6,0x0145,0x0182,0x00AB,0x0174,0x015F,0x0065,0x01A0,0x0120,
0x0118,0x019A,0x002B,0x017E,0x01B3,0x00AF,0x005B,0x01D1,0x01AC,0x01DC,0x0128,0x0028,0x0125,0x004F,0x00A8,0x00EE,
0x0060,0x01E0,0x01DF,0x0022,0x0012,0x00BA,0x00E5,0x0097,0x00E1,0x0057,0x012B,0x0159,0x00C4,0x00D3,0x0136,0x0071,
0x00A3,0x00DD,0x01D4,0x0042,0x00F9,0x0149,0x009B,0x0083,0x0166,0x01B9,0x01AF,0x0130,0x011D,0x0048,0x0076,0x000E,
0x01F9,0x01CB,0x00BD,0x00B2,0x01ED,0x0093,0x009F,0x0005,0x0188,0x010A,0x00E8,0x006A,0x0111,0x013B,0x008C,0x01F4,
0x0171,0x017B,0x0025,0x003F,0x0156,0x005F,0x014D,0x0001,0x001D,0x00CB,0x0015,0x01C5,0x0034,0x016F,0x007B,0x0050,
0x0176,0x00F3,0x01E3,0x01A8,0x0141,0x0162,0x00FD,0x00CF,0x0067,0x003C,0x01C2,0x01A5,0x01A2,0x00D8,0x0195,0x01FE,
0x00B8,0x0055,0x00D1,0x00DB,0x0147,0x01B7,0x0046,0x0019,0x0184,0x0180,0x0063,0x0198,0x00AD,0x01DA,0x004D,0x00F1,
0x0160,0x003A,0x00D6,0x0053,0x01B5,0x019E,0x01D8,0x01C9,0x0091,0x0108,0x0139,0x0179,0x005D,0x00C9,0x016D,0x01B0,
0x0122,0x01E8,0x015C,0x00F6,0x011A,0x002F,0x00C1,0x0038,0x019C,0x0106,0x00C7,0x01E6,0x002D,0x0104,0x0102,0x0117,
0x002A,0x01B2,0x005A,0x01AB,0x0127,0x0124,0x00A7,0x0153,0x0031,0x01EA,0x012E,0x0144,0x00AA,0x015E,0x01BF,0x00A2,
0x01D3,0x00F8,0x009A,0x0165,0x01AE,0x011C,0x0075,0x007F,0x01DE,0x0011,0x00E4,0x00E0,0x012A,0x00C3,0x0135,0x0170,
0x0024,0x0155,0x014C,0x001C,0x0014,0x0033,0x007A,0x01F8,0x00BC,0x01EC,0x009E,0x0187,0x00E7,0x0110,0x008B,0x00B7,
0x00D0,0x0146,0x0045,0x0183,0x0062,0x00AC,0x004C,0x0175,0x01E2,0x0140,0x00FC,0x0066,0x01C1,0x01A1,0x0194,0x0121,
0x015B,0x0119,0x00C0,0x019B,0x00C6,0x002C,0x0101,0x017F,0x00D5,0x01B4,0x01D7,0x0090,0x0138,0x005C,0x016C,0x01D2,
0x0099,0x01AD,0x0074,0x01DD,0x00E3,0x0129,0x0134,0x0029,0x0059,0x0126,0x00A6,0x0030,0x012D,0x00A9,0x01BE,0x00EF,
0x0044,0x0061,0x004B,0x01E1,0x00FB,0x01C0,0x0193,0x0023,0x014B,0x0013,0x0079,0x00BB,0x009D,0x00E6,0x008A,0x0098,
0x0073,0x00E2,0x0133,0x0058,0x00A5,0x012C,0x01BD,0x015A,0x00DF,0x00C5,0x0100,0x00D4,0x01D6,0x0137,0x016B,0x0072,
0x0132,0x00A4,0x01BC,0x00DE,0x011F,0x01D5,0x016A,0x0043,0x004A,0x00FA,0x0192,0x014A,0x0078,0x009C,0x0089,0x0084,
0x0085,0x0167,0x0086,0x01BA,0x0168,0x0190,0x0087,0x0131,0x01BB,0x011E,0x0169,0x0049,0x0191,0x0077,0x0088
}};

static const uint16_t kFecPolynomial = 0x11;
static const uint16_t kFecKeyLength = 0x9;
static const uint16_t kFecInit = 0;
static const uint16_t kFecKeyLeftAdjust = kWordLength - kFecKeyLength;
static const uint16_t* const FecSyndromeLookup = &FEC_Syndrome_Table_9Bit[0];
static const uint16_t kFecSyndromeMask = 0x1FF; // 9 bit mask for 9-bit FEC
static const uint16_t kFecBitLength = 512;

using FecLfsr = LFSR<LFSR_NAME_1>;

// FEC uses DECODE config to encode, a little confusing!
template <class LFSR>
void ConfigureFecEncoder(LFSR& lfsr) {
  lfsr.Configure(LFSR_FBMODE_DECODE, kFecKeyLength, kWordLength, kFecPolynomial);
  lfsr.Set(kFecInit);
}

template <class LFSR>
void ConfigureFecDecoder(LFSR& lfsr) {
  lfsr.Configure(LFSR_FBMODE_ENCODE, kFecKeyLength, kWordLength, kFecPolynomial);
  lfsr.Set(kFecInit);
}

template <class FEC>
void FinalizeFecDecode(FEC& fec, uint16_t keyword) {
  fec.Configure(LFSR_FBMODE_ENCODE, kFecKeyLength, kFecKeyLength, kFecPolynomial);
  fec.Push(keyword);
}

template <class CRC, size_t N>
bool AttemptFec(CRC& crc, uint16_t crc_result, uint16_t syndrome, std::array<uint16_t, N>& buffer) {
  static_assert(N * kWordLength < 512, "FEC only works up to 512 bits!");
  syndrome &= kFecSyndromeMask;
  uint16_t lookup = FecSyndromeLookup[syndrome];
  size_t bit_index = lookup & 0xF;

  // the syndrome lookup assumes a 512 bitlength -- we need to add
  // the difference in word index as it will assume 
  // we padded with 512 - N*16 zeroes.  We also need to add +1 for the CRC
  size_t word_offset = (N - (512 / kWordLength)) + 1;
  size_t word_index = ((lookup >> 4) & 0x1F) + word_offset;

  if (static_cast<int16_t>(word_index) < 0 || word_index > N)
    return false;

  // The CRC is protected by FEC too, so if we get N as the result, flip a bit
  // in the CRC instead
  if (word_index == N) {
    crc_result ^= (1 << bit_index);
  } else {
    buffer[word_index] ^= (1 << bit_index);
  }
  
  ConfigureCrcDecoder(crc);
  for (auto word : buffer)
    crc.Push(word);
  crc.Push(crc_result);

  return crc.Get() == 0;
}


#endif
