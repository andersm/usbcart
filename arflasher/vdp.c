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

#include <stdint.h>

#include "vdp2.h"

void vdp_init(void)
{
    int ii;
    volatile uint16_t *vdp2_vram = (volatile uint16_t *)VDP2_VRAM;
    volatile uint16_t *vdp2_cram = (volatile uint16_t *)VDP2_CRAM;
    TVMD = 0x0000;

    RAMCTL = RAMCTL & (~0x3000);

    // Map Offset Register: Bitmap screen will be located at VRAM offset 0
    MPOFN = 0;

    // Character Control Register: 256 colors, enable NBG0 as a bitmap
    // |8 for 1024x256 bitmap
    CHCTLA = 0x0012|8;

    // Screen Scroll Value Registers: No scroll
    SCXIN0 = 0;
    SCXDN0 = 0;
    SCYIN0 = 0;

    // Screen Display Enable Register: Invalidate the transparency code for
    // NBG0 and display NBG0
    BGON = 0x0001;

    /* Clear VRAM */
    for (ii = 0; ii < 0x40000; ii++)
        vdp2_vram[ii] = 0x0000;

    /* Clear CRAM */
    for (ii = 0; ii < 0x0800; ii++)
        vdp2_cram[ii] = 0x0000;
}
