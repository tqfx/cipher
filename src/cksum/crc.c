/*!
 @file crc.c
 @brief Cyclic Redundancy Check
 @details https://en.wikipedia.org/wiki/Cyclic_redundancy_check
 @copyright Copyright (C) 2020-present tqfx, All rights reserved.
*/

#include "cksum/crc.h"

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic ignored "-Wconversion"
#endif /* __GNUC__ || __clang__ */

#undef CRC_LSB
#define CRC_LSB(bit)                                   \
    void crc##bit##_lsb(uint##bit##_t tab[CRC_TABSIZ], \
                        uint##bit##_t poly)            \
    {                                                  \
        for (unsigned int i = 0; i != 0x100; ++i)      \
        {                                              \
            uint##bit##_t crc = (uint##bit##_t)i;      \
            for (unsigned int j = 8; j; --j)           \
            {                                          \
                if (crc & 1)                           \
                {                                      \
                    crc >>= 1;                         \
                    crc ^= poly;                       \
                }                                      \
                else                                   \
                {                                      \
                    crc >>= 1;                         \
                }                                      \
            }                                          \
            tab[i] = crc;                              \
        }                                              \
    }
CRC_LSB(8)
CRC_LSB(16)
CRC_LSB(32)
CRC_LSB(64)
#undef CRC_LSB

#undef CRC_MSB
#define CRC_MSB(bit, msk)                                      \
    void crc##bit##_msb(uint##bit##_t tab[CRC_TABSIZ],         \
                        uint##bit##_t poly)                    \
    {                                                          \
        for (unsigned int i = 0; i != 0x100; ++i)              \
        {                                                      \
            uint##bit##_t crc = (uint##bit##_t)i << (bit - 8); \
            for (unsigned int j = 8; j; --j)                   \
            {                                                  \
                if (UINT##bit##_C(msk) & crc)                  \
                {                                              \
                    crc <<= 1;                                 \
                    crc ^= poly;                               \
                }                                              \
                else                                           \
                {                                              \
                    crc <<= 1;                                 \
                }                                              \
            }                                                  \
            tab[i] = crc;                                      \
        }                                                      \
    }
CRC_MSB(8, 0x80)
CRC_MSB(16, 0x8000)
CRC_MSB(32, 0x80000000)
CRC_MSB(64, 0x8000000000000000)
#undef CRC_MSB

uint8_t crc8(const uint8_t tab[CRC_TABSIZ],
             const void *pdata, size_t nbyte,
             uint8_t crc)
{
    const uint8_t *p = (const uint8_t *)pdata;
    const uint8_t *q = (const uint8_t *)pdata + nbyte;
    while (p != q)
    {
        crc = tab[crc ^ *p++];
    }
    return crc;
}

#undef CRCL
#define CRCL(bit)                                                  \
    uint##bit##_t crc##bit##l(const uint##bit##_t tab[CRC_TABSIZ], \
                              const void *pdata, size_t nbyte,     \
                              uint##bit##_t crc)                   \
    {                                                              \
        const uint8_t *p = (const uint8_t *)pdata;                 \
        const uint8_t *q = (const uint8_t *)pdata + nbyte;         \
        while (p != q)                                             \
        {                                                          \
            crc = (crc >> 8) ^ tab[(crc ^ *p++) & 0xFF];           \
        }                                                          \
        return crc;                                                \
    }
CRCL(16)
CRCL(32)
CRCL(64)
#undef CRCL

#undef CRCH
#define CRCH(bit)                                                       \
    uint##bit##_t crc##bit##h(const uint##bit##_t tab[CRC_TABSIZ],      \
                              const void *pdata, size_t nbyte,          \
                              uint##bit##_t crc)                        \
    {                                                                   \
        const uint8_t *p = (const uint8_t *)pdata;                      \
        const uint8_t *q = (const uint8_t *)pdata + nbyte;              \
        while (p != q)                                                  \
        {                                                               \
            crc = (crc << 8) ^ tab[((crc >> (bit - 8)) ^ *p++) & 0xFF]; \
        }                                                               \
        return crc;                                                     \
    }
CRCH(16)
CRCH(32)
CRCH(64)
#undef CRCH
