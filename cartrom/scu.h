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

#ifndef SCU_H_
#define SCU_H_

#include <stdint.h>

#define SCU_BASE 0x25FE0000

#define D0R     (*(volatile uint32_t*)(SCU_BASE+0x00))
#define D0W     (*(volatile uint32_t*)(SCU_BASE+0x04))
#define D0C     (*(volatile uint32_t*)(SCU_BASE+0x08))
#define D0AD    (*(volatile uint32_t*)(SCU_BASE+0x0c))
#define D0EN    (*(volatile uint32_t*)(SCU_BASE+0x10))
#define D0MD    (*(volatile uint32_t*)(SCU_BASE+0x14))

#define D1R     (*(volatile uint32_t*)(SCU_BASE+0x20))
#define D1W     (*(volatile uint32_t*)(SCU_BASE+0x24))
#define D1C     (*(volatile uint32_t*)(SCU_BASE+0x28))
#define D1AD    (*(volatile uint32_t*)(SCU_BASE+0x2c))
#define D1EN    (*(volatile uint32_t*)(SCU_BASE+0x30))
#define D1MD    (*(volatile uint32_t*)(SCU_BASE+0x34))

#define D2R     (*(volatile uint32_t*)(SCU_BASE+0x40))
#define D2W     (*(volatile uint32_t*)(SCU_BASE+0x44))
#define D2C     (*(volatile uint32_t*)(SCU_BASE+0x48))
#define D2AD    (*(volatile uint32_t*)(SCU_BASE+0x4c))
#define D2EN    (*(volatile uint32_t*)(SCU_BASE+0x50))
#define D2MD    (*(volatile uint32_t*)(SCU_BASE+0x54))

#define DSTP    (*(volatile uint32_t*)(SCU_BASE+0x60))
#define DSTA    (*(volatile uint32_t*)(SCU_BASE+0x7c))

#define PPAF    (*(volatile uint32_t*)(SCU_BASE+0x80))
#define PPD     (*(volatile uint32_t*)(SCU_BASE+0x84))
#define PDA     (*(volatile uint32_t*)(SCU_BASE+0x88))
#define PDD     (*(volatile uint32_t*)(SCU_BASE+0x8c))

#define T0C     (*(volatile uint32_t*)(SCU_BASE+0x90))
#define T1S     (*(volatile uint32_t*)(SCU_BASE+0x94))
#define T1MD    (*(volatile uint32_t*)(SCU_BASE+0x98))

#define IMS     (*(volatile uint32_t*)(SCU_BASE+0xa0))
#define IST     (*(volatile uint32_t*)(SCU_BASE+0xa4))

#define AIACK   (*(volatile uint32_t*)(SCU_BASE+0xa8))
#define ASR0    (*(volatile uint32_t*)(SCU_BASE+0xb0))
#define ASR1    (*(volatile uint32_t*)(SCU_BASE+0xb4))
#define AREF    (*(volatile uint32_t*)(SCU_BASE+0xb8))

#define RSEL    (*(volatile uint32_t*)(SCU_BASE+0xc4))

#define VER     (*(volatile uint32_t*)(SCU_BASE+0xc8))

#endif /* SCU_H_ */