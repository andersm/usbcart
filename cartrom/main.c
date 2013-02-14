/*

    Sega Saturn USB flash cart ROM
    Copyright © 2012, Anders Montonen
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
#include "scu.h"

#include "crc.h"

#define USB_FLAGS (*(volatile uint8_t*)(0x22200001))
#define USB_RXF     (1 << 0)
#define USB_TXE     (1 << 1)
#define USB_PWREN   (1 << 7)
#define USB_FIFO (*(volatile uint8_t*)(0x22100001))

#define RGB(r, g, b) ((((b)&0x1f)<<10)|(((g)&0x1f)<<5)|((r)&0x1f))

const uint16_t ColorTable[] =
{
    RGB(   0,    0, 0), /* black  */
    RGB(0x1f,    0, 0), /* red    */
    RGB(   0, 0x1f, 0), /* green  */
    RGB(0x1f, 0x13, 0)  /* orange */
};

typedef enum
{
    BLACK,
    RED,
    GREEN,
    ORANGE
} Color_e;

static void SetScreenColor(Color_e color)
{
    *(uint16_t*)VDP2_VRAM = ColorTable[color];
}

static void SignalError(void)
{
    /* Display a red screen for two seconds. */
    int delay = 60*2; /* Doesn't take 50Hz display into account.. */

    SetScreenColor(RED);
    while(delay > 0)
    {
        WaitForVBLANKIn();
        WaitForVBLANKOut();
        --delay;
    }
}

static uint8_t RecvByte(void)
{
    while ((USB_FLAGS & USB_RXF) != 0) ;
    return USB_FIFO;
}

static uint32_t RecvDword(void)
{
    uint32_t tmp = RecvByte();
    tmp = (tmp << 8) | RecvByte();
    tmp = (tmp << 8) | RecvByte();
    tmp = (tmp << 8) | RecvByte();

    return tmp;
}

static void SendByte(uint8_t byte)
{
    while ((USB_FLAGS & USB_TXE) != 0) ;
    USB_FIFO = byte;
}

static void DoDownload(void)
{
    uint8_t    *pData;
    uint32_t    len, ii;
    crc_t       checksum = crc_init();

    SetScreenColor(ORANGE);

    pData = (uint8_t*)RecvDword();

    len = RecvDword();

    for (ii = 0; ii < len; ++ii)
    {
        while ((USB_FLAGS & USB_TXE) != 0) ;
        USB_FIFO = pData[ii];
    }

    checksum = crc_update(checksum, pData, len);
    checksum = crc_finalize(checksum);

    SendByte(checksum);
}

static void DoUpload(void)
{
    uint8_t    *pData;
    uint32_t    len, ii;
    crc_t       readchecksum;
    crc_t       checksum = crc_init();

    SetScreenColor(ORANGE);

    pData = (uint8_t*)RecvDword();

    len = RecvDword();

    for (ii = 0; ii < len; ++ii)
    {
        // inlining is 20K/s faster
        while ((USB_FLAGS & USB_RXF) != 0) ;
        pData[ii] = USB_FIFO;
    }

    readchecksum = RecvByte();

    checksum = crc_update(checksum, pData, len);
    checksum = crc_finalize(checksum);

    if (checksum != readchecksum)
    {
        SendByte(0x1);
        SignalError();
    }
    else
    {
        SendByte(0);
    }
}

static void DoExecute(void)
{
    /* Read address, execute call. */
    void (*pFun)(void);

    pFun = (void(*)(void))RecvDword();
    (*pFun)();
}

/* Setup back screen and color entries, turns video on. */
static void InitVideo(void)
{
    TVMD = 0x8110;
    BGON  = 0;
    LCTAU = 0;
    LCTAL = 0;
    BKTAL = 0;
    BKTAU = 0;
    SetScreenColor(BLACK);
}

int main(void)
{
    uint8_t command;

    InitVideo();

    // Configure SCU A-bus settings
    // Disabling A-bus refresh adds ~20KB/s
    AREF = 0;
    // Removing wait states doubles speed
    ASR0 = 0;

    while(1)
    {
        SetScreenColor(GREEN);
        command = RecvByte();
        switch (command)
        {
        case 1:
            DoDownload();
            break;
        case 2:
            DoUpload();
            break;
        case 3:
            DoExecute();
            InitVideo();
            break;
        }

    }

    return 0;
}
