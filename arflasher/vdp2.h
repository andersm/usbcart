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

#ifndef VDP2_H_
#define VDP2_H_

#include <stdint.h>

#define VDP2_BASE           0x25e00000
#define VDP2_VRAM           VDP2_BASE
#define VDP2_CRAM           (VDP2_BASE+0x100000)
#define VDP2_REGISTER_BASE  (VDP2_BASE+0x180000)

#define TVMD    (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0x00))
#define DISP    (1<<15)
#define BDCLMD  (1<<8)
#define LSMD1   (1<<7)
#define LSMD0   (1<<6)
#define VRESO1  (1<<5)
#define VRESO0  (1<<4)
#define HRESO2  (1<<2)
#define HRESO1  (1<<1)
#define HRESO0  (1<<0)
/* Display modes. Only TV modes. */
#define NOINTERLACE     (0)
#define SINGLEINTERLACE (LSMD1)
#define DOUBLEINTERLACE (LSMD1|LSMD0)
#define VRES224         (0)
#define VRES240         (VRESO0)
/* 256 lines in PAL, 240 lines in NTSC */
#define VRES256         (VRESO1|VRESO0)
#define HRES320         (0)
#define HRES352         (HRESO0)
#define HRES640         (HRESO1)
#define HRES704         (HRESO1|HRESO0)

#define EXTEN   (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0x02))
#define EXLTEN  (1<<9)
#define EXSYEN  (1<<8)
#define DASEL   (1<<1)
#define EXBGEN  (1<<0)

#define TVSTAT  (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0x04))
#define EXLTFG  (1<<9)
#define EXSYFG  (1<<8)
#define VBLANK  (1<<3)
#define HBLANK  (1<<2)
#define ODD     (1<<1)
#define PAL     (1<<0)
/* Sync macros */
#define WaitForVBLANKIn()  while(!(TVSTAT&VBLANK));
#define WaitForVBLANKOut() while(TVSTAT&VBLANK);
#define WaitForHBLANKIn()  while(!(TVSTAT&HBLANK));
#define WaitForHBLANKOut() while(TVSTAT&HBLANK);

#define VRSIZE  (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0x06))
#define VRAMSZ  (1<<15)

#define HCNT    (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0x08))
#define VCNT    (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0x0a))
#define RAMCTL  (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0x0e))
#define CYCA0L  (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0x10))
#define CYCA0U  (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0x12))
#define CYCA1L  (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0x14))
#define CYCA1U  (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0x16))
#define CYCB0L  (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0x18))
#define CYCB0U  (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0x1a))
#define CYCB1L  (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0x1c))
#define CYCB1U  (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0x1e))
#define BGON    (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0x20))
#define R0TPON  (1<<12)
#define N3TPON  (1<<11)
#define N2TPON  (1<<10)
#define N1TPON  (1<<9)
#define N0TPON  (1<<8)
#define R1ON    (1<<5)
#define R0ON    (1<<4)
#define N3ON    (1<<3)
#define N2ON    (1<<2)
#define N1ON    (1<<1)
#define N0ON    (1<<0)

#define MZCTL   (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0x22))
#define SFSEL   (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0x24))
#define SFCODE  (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0x26))
#define CHCTLA  (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0x28))
#define CHCTLB  (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0x2a))
#define BMPNA   (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0x2c))
#define BMPNB   (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0x2e))
#define PNCN0   (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0x30))
#define PNCN1   (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0x32))
#define PNCN2   (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0x34))
#define PNCN3   (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0x36))
#define PNCR    (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0x38))
#define PLSZ    (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0x3a))
#define MPOFN   (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0x3c))
#define MPOFR   (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0x3e))
#define MPABN0  (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0x40))
#define MPCDN0  (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0x42))
#define MPABN1  (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0x44))
#define MPCDN1  (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0x46))
#define MPABN2  (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0x48))
#define MPCDN2  (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0x4a))
#define MPABN3  (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0x4c))
#define MPCDN3  (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0x4e))
#define MPABRA  (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0x50))
#define MPCDRA  (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0x52))
#define MPEFRA  (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0x54))
#define MPGHRA  (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0x56))
#define MPIJRA  (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0x58))
#define MPKLRA  (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0x5a))
#define MPMNRA  (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0x5c))
#define MPOPRA  (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0x5e))
#define MPABRB  (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0x60))
#define MPCDRB  (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0x62))
#define MPEFRB  (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0x64))
#define MPGHRB  (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0x66))
#define MPIJRB  (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0x68))
#define MPLKRB  (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0x6a))
#define MPMNRB  (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0x6c))
#define MPOPRB  (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0x6e))
#define SCXIN0  (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0x70))
#define SCXDN0  (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0x72))
#define SCYIN0  (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0x74))
#define SCYDN0  (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0x76))
#define ZMXIN0  (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0x78))
#define ZMXDN0  (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0x7a))
#define ZMYIN0  (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0x7c))
#define ZMYDN0  (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0x7e))
#define SCXIN1  (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0x80))
#define SCXDN1  (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0x82))
#define SCYIN1  (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0x84))
#define SCYDN1  (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0x86))
#define ZMXIN1  (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0x88))
#define ZMXDN1  (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0x8a))
#define ZMYIN1  (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0x8c))
#define ZMYDN1  (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0x8e))
#define SCXN2   (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0x90))
#define SCYN2   (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0x92))
#define SCXN3   (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0x94))
#define SCYN3   (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0x96))
#define ZMCTL   (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0x98))
#define SCRCTL  (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0x9a))
#define VCSTAU  (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0x9c))
#define VCSTAL  (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0x9e))
#define LSTA0U  (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0xa0))
#define LSTA0L  (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0xa2))
#define LSTA1U  (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0xa4))
#define LSTA1L  (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0xa6))
#define LCTAU   (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0xa8))
#define LCTAL   (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0xaa))
#define BKTAU   (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0xac))
#define BKTAL   (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0xae))
#define RPMD    (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0xb0))
#define RPRCTL  (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0xb2))
#define KTCTL   (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0xb4))
#define KTAOF   (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0xb6))
#define OVPNRA  (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0xb8))
#define OVPNRB  (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0xba))
#define RPTAU   (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0xbc))
#define RPTAL   (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0xbe))
#define WPSX0   (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0xc0))
#define WPSY0   (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0xc2))
#define WPEX0   (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0xc4))
#define WPEY0   (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0xc6))
#define WPSX1   (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0xc8))
#define WPSY1   (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0xca))
#define WPEX1   (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0xcc))
#define WPEY1   (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0xce))
#define WCTLA   (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0xd0))
#define WCTLB   (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0xd2))
#define WCTLC   (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0xd4))
#define WCTLD   (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0xd6))
#define LWTA0U  (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0xd8))
#define LWTA0L  (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0xda))
#define LWTA1U  (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0xdc))
#define LWTA1L  (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0xde))
#define SPCTL   (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0xe0))
#define SPCCCS1 (1<<13)
#define SPCCCS0 (1<<12)
#define SPCCN2  (1<<10)
#define SPCCN1  (1<<9)
#define SPCCN0  (1<<8)
#define SPCLMD  (1<<5)
#define SPWINEN (1<<4)
#define SPTYPE3 (1<<3)
#define SPTYPE2 (1<<2)
#define SPTYPE1 (1<<1)
#define SPTYPE0 (1<<0)
#define SPRITETYPE0 (0)
#define SPRITETYPE1 (SPTYPE0)
#define SPRITETYPE2 (SPTYPE1)
#define SPRITETYPE3 (SPTYPE1|SPTYPE0)
#define SPRITETYPE4 (SPTYPE2)
#define SPRITETYPE5 (SPTYPE2|SPTYPE0)
#define SPRITETYPE6 (SPTYPE2|SPTYPE1)
#define SPRITETYPE7 (SPTYPE2|SPTYPE1|SPTYPE0)
#define SPRITETYPE8 (SPTYPE3)
#define SPRITETYPE9 (SPTYPE3|SPTYPE0)
#define SPRITETYPEA (SPTYPE3|SPTYPE1)
#define SPRITETYPEB (SPTYPE3|SPTYPE1|SPTYPE0)
#define SPRITETYPEC (SPTYPE3|SPTYPE2)
#define SPRITETYPED (SPTYPE3|SPTYPE2|SPTYPE0)
#define SPRITETYPEE (SPTYPE3|SPTYPE2|SPTYPE1)
#define SPRITETYPEF (SPTYPE3|SPTYPE2|SPTYPE1|SPTYPE0)

#define SDCTL   (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0xe2))
#define CRAOFA  (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0xe4))
#define CRAOFB  (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0xe6))
#define LNCLEN  (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0xe8))
#define SFPRMD  (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0xea))
#define CCCTL   (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0xec))
#define SFCCMD  (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0xee))
#define PRISA   (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0xf0))
#define PRISB   (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0xf2))
#define PRISC   (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0xf4))
#define PRISD   (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0xf6))
#define PRINA   (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0xf8))
#define PRINB   (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0xfa))
#define PRIR    (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0xfc))
#define CCRSA   (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0x100))
#define CCRSB   (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0x102))
#define CCRSC   (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0x104))
#define CCRSD   (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0x106))
#define CCRNA   (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0x108))
#define CCRNB   (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0x10a))
#define CCRR    (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0x10c))
#define CCRLB   (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0x10e))
#define CLOFEN  (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0x110))
#define CLOFSL  (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0x112))
#define COAR    (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0x114))
#define COAG    (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0x116))
#define COAB    (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0x118))
#define COBR    (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0x11a))
#define COBG    (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0x11c))
#define COBB    (*(volatile uint16_t *)(VDP2_REGISTER_BASE+0x11e))

#endif /* VDP2_H_ */
