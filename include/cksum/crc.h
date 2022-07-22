/*!
 @file crc.h
 @brief Cyclic Redundancy Check
 @details https://en.wikipedia.org/wiki/Cyclic_redundancy_check
 @copyright Copyright (C) 2020-present tqfx, All rights reserved.
*/

#ifndef __CKSUM_CRC_H__
#define __CKSUM_CRC_H__

#include <stddef.h>
#include <stdint.h>

#define CRC_TABSIZ 0x100

#define CRC8_POLY UINT8_C(0x31)
#define CRC8_INIT UINT8_C(0x00)

#define CRC16_POLY UINT16_C(0xA001)
#define CRC16_INIT UINT16_C(0x0000)

#define CRC32_POLY UINT32_C(0xEDB88320)
#define CRC32_INIT UINT32_C(0xFFFFFFFF)

#define CRC64_POLY UINT64_C(0x42F0E1EBA9EA3693)
#define CRC64_INIT UINT64_C(0xFFFFFFFFFFFFFFFF)

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

void crc8_lsb(uint8_t tab[CRC_TABSIZ], uint8_t poly);
void crc8_msb(uint8_t tab[CRC_TABSIZ], uint8_t poly);
uint8_t crc8(const uint8_t tab[CRC_TABSIZ], const void *pdata, size_t nbyte, uint8_t crc);

void crc16_lsb(uint16_t tab[CRC_TABSIZ], uint16_t poly);
void crc16_msb(uint16_t tab[CRC_TABSIZ], uint16_t poly);
uint16_t crc16l(const uint16_t tab[CRC_TABSIZ], const void *pdata, size_t nbyte, uint16_t crc);
uint16_t crc16h(const uint16_t tab[CRC_TABSIZ], const void *pdata, size_t nbyte, uint16_t crc);

void crc32_lsb(uint32_t tab[CRC_TABSIZ], uint32_t poly);
void crc32_msb(uint32_t tab[CRC_TABSIZ], uint32_t poly);
uint32_t crc32l(const uint32_t tab[CRC_TABSIZ], const void *pdata, size_t nbyte, uint32_t crc);
uint32_t crc32h(const uint32_t tab[CRC_TABSIZ], const void *pdata, size_t nbyte, uint32_t crc);

void crc64_lsb(uint64_t tab[CRC_TABSIZ], uint64_t poly);
void crc64_msb(uint64_t tab[CRC_TABSIZ], uint64_t poly);
uint64_t crc64l(const uint64_t tab[CRC_TABSIZ], const void *pdata, size_t nbyte, uint64_t crc);
uint64_t crc64h(const uint64_t tab[CRC_TABSIZ], const void *pdata, size_t nbyte, uint64_t crc);

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* __CKSUM_CRC_H__ */
