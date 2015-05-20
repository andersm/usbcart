/*

    Sega Saturn cartridge flash tool
    Copyright © 2012, 2015 Anders Montonen
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

#include <stdint.h>

#include "vdp2.h"
#include "smpc.h"

#include "vdp.h"
#include "conio.h"

#include "flash.h"

extern const uint8_t  RomImage;
extern const uint32_t RomLen;

#define PAD_START   (1<<11)
#define PAD_A       (1<<10)
#define PAD_C       (1<<9)
#define PAD_B       (1<<8)

static void InitPad(void)
{
    PDR1 = 0;
    DDR1 = 0x60;
    IOSEL = IOSEL1;
}

static uint16_t ReadPad(void)
{
    uint16_t bits;

    PDR1 = 0x60;
    bits = (PDR1 & 0x8) << 9;
    PDR1 = 0x40;
    bits |= (PDR1 & 0xf) << 8;
    PDR1 = 0x20;
    bits |= (PDR1 & 0xf) << 4;
    PDR1 = 0x00;
    bits |= (PDR1 & 0xf);

    return bits ^ 0x1FFF;
}

int main(void)
{
    uint16_t        vendorID;
    uint16_t        deviceID;
    uint32_t        page;
    FlashDevice_t   device;
    const uint16_t *pRom = (const uint16_t*)&RomImage;
    uint16_t        padData;

    vdp_init();
    conio_init();
    InitPad();

    TVMD = 0x8000;

    _printf(2, 2, 0xF0, "Action Replay Flasher");
    _printf(2, 3, 0xF0, "Revision 11");
    _printf(2, 4, 0xF0, "Press Start to begin");

    while (1)
    {
        padData = ReadPad();
        if (padData & PAD_START)
            break;
        WaitForVBLANKIn();
        WaitForVBLANKOut();
    }

    vendorID = FlashGetVendorID();
    deviceID = FlashGetDeviceID();
    FillFlashDeviceInfo(vendorID, deviceID, &device);

    _printf(2, 6, 0xF0, "Flash Vendor ID:");
    _printhexl(19, 6, 0xF0, vendorID);
    _printf(2, 7, 0xF0, "Flash Device ID:");
    _printhexl(19, 7, 0xF0, deviceID);
    _printf(2, 8, 0xF0, device.pVendorString);
    _printf(2, 9, 0xF0, device.pDeviceString);

    WaitForVBLANKIn();
    WaitForVBLANKOut();

    if (device.RomSize == 0 || device.PageSize == 0)
    {
        while(1)
        {
            WaitForVBLANKIn();
            WaitForVBLANKOut();
        }
    }

    _printf(2, 11, 0xF0, "Press A+B+C+Start to initiate write...");
    while (1)
    {
        padData = ReadPad();
        if ((padData & (PAD_START|PAD_A|PAD_C|PAD_B)) == (PAD_START|PAD_A|PAD_C|PAD_B))
            break;
        WaitForVBLANKIn();
        WaitForVBLANKOut();
    }

    _printf(2, 13, 0xF0, "Writing...");

    device.EraseFlash(&device);

    _printf(2, 14, 0xF0, "Page          of          ");

    _printhexl(23, 14, 0xF0, RomLen / device.PageSize / 2);

    for (page = 0; page < RomLen / device.PageSize / 2; page++)
    {
        _printhexl(7, 14, 0xF0, page + 1);
        device.WriteFlashPage(&device, page,
                              (uint16_t*)(pRom + (page * device.PageSize)));
    }

    _printf(2, 16, 0xF0, "Done!");

    while (1)
    {
        WaitForVBLANKIn();
        WaitForVBLANKOut();
    }
}
