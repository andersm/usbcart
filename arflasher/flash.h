/*

    Sega Saturn cartridge flash tool
    Copyright © 2012, Anders Montonen
    Original software by ExCyber
    Graphics routines by Charles MacDonald
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:

    Redistributions of source code must retain the above copyright notice, this
    list of conditions and the following disclaimer.
    Redistributions in binary form must reproduce the above copyright notice,
    this list of conditions and the following disclaimer in the documentation
    and/or other materials provided with the distribution.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
    AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
    ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
    LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
    CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
    SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
    INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
    CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
    ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
    POSSIBILITY OF SUCH DAMAGE.

*/

#ifndef FLASH_H_
#define FLASH_H_

#include <stdint.h>

typedef struct FlashDev
{
    const char *pVendorString;
    const char *pDeviceString;
    uint32_t    RomSize;
    uint32_t    PageSize;
    void        (*EraseFlash)(const struct FlashDev*);
    void        (*EraseFlashPage)(const struct FlashDev*, const uint32_t);
    void        (*WriteFlashPage)(const struct FlashDev*, const uint32_t, const uint16_t*);
} FlashDevice_t;

void FillFlashDeviceInfo(const uint16_t VendorID, const uint16_t DeviceID,
                         FlashDevice_t *pFlashDevice);

uint16_t FlashGetVendorID(void);
uint16_t FlashGetDeviceID(void);

#endif /* FLASH_H_ */
