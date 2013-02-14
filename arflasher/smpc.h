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

#ifndef SMPC_H_
#define SMPC_H_

#include <stdint.h>

#define SMPC_BASE   0x20100000

#define IREG0       (*(volatile uint8_t*)(SMPC_BASE+0x01))
#define IREG1       (*(volatile uint8_t*)(SMPC_BASE+0x03))
#define IREG2       (*(volatile uint8_t*)(SMPC_BASE+0x05))
#define IREG3       (*(volatile uint8_t*)(SMPC_BASE+0x07))
#define IREG4       (*(volatile uint8_t*)(SMPC_BASE+0x09))
#define IREG5       (*(volatile uint8_t*)(SMPC_BASE+0x0b))
#define IREG6       (*(volatile uint8_t*)(SMPC_BASE+0x0d))
#define COMREG      (*(volatile uint8_t*)(SMPC_BASE+0x1f))
#define OREG0       (*(volatile uint8_t*)(SMPC_BASE+0x21))
#define OREG1       (*(volatile uint8_t*)(SMPC_BASE+0x23))
#define OREG2       (*(volatile uint8_t*)(SMPC_BASE+0x25))
#define OREG3       (*(volatile uint8_t*)(SMPC_BASE+0x27))
#define OREG4       (*(volatile uint8_t*)(SMPC_BASE+0x29))
#define OREG5       (*(volatile uint8_t*)(SMPC_BASE+0x2b))
#define OREG6       (*(volatile uint8_t*)(SMPC_BASE+0x2d))
#define OREG7       (*(volatile uint8_t*)(SMPC_BASE+0x2f))
#define OREG8       (*(volatile uint8_t*)(SMPC_BASE+0x31))
#define OREG9       (*(volatile uint8_t*)(SMPC_BASE+0x33))
#define OREG10      (*(volatile uint8_t*)(SMPC_BASE+0x35))
#define OREG11      (*(volatile uint8_t*)(SMPC_BASE+0x37))
#define OREG12      (*(volatile uint8_t*)(SMPC_BASE+0x39))
#define OREG13      (*(volatile uint8_t*)(SMPC_BASE+0x3b))
#define OREG14      (*(volatile uint8_t*)(SMPC_BASE+0x3d))
#define OREG15      (*(volatile uint8_t*)(SMPC_BASE+0x3f))
#define OREG16      (*(volatile uint8_t*)(SMPC_BASE+0x41))
#define OREG17      (*(volatile uint8_t*)(SMPC_BASE+0x43))
#define OREG18      (*(volatile uint8_t*)(SMPC_BASE+0x45))
#define OREG19      (*(volatile uint8_t*)(SMPC_BASE+0x47))
#define OREG20      (*(volatile uint8_t*)(SMPC_BASE+0x49))
#define OREG21      (*(volatile uint8_t*)(SMPC_BASE+0x4b))
#define OREG22      (*(volatile uint8_t*)(SMPC_BASE+0x4d))
#define OREG23      (*(volatile uint8_t*)(SMPC_BASE+0x4f))
#define OREG24      (*(volatile uint8_t*)(SMPC_BASE+0x51))
#define OREG25      (*(volatile uint8_t*)(SMPC_BASE+0x53))
#define OREG26      (*(volatile uint8_t*)(SMPC_BASE+0x55))
#define OREG27      (*(volatile uint8_t*)(SMPC_BASE+0x57))
#define OREG28      (*(volatile uint8_t*)(SMPC_BASE+0x59))
#define OREG29      (*(volatile uint8_t*)(SMPC_BASE+0x5b))
#define OREG30      (*(volatile uint8_t*)(SMPC_BASE+0x5d))
#define OREG31      (*(volatile uint8_t*)(SMPC_BASE+0x5f))
#define SR          (*(volatile uint8_t*)(SMPC_BASE+0x61))
#define SF          (*(volatile uint8_t*)(SMPC_BASE+0x63))
#define PDR1        (*(volatile uint8_t*)(SMPC_BASE+0x75))
#define PDR2        (*(volatile uint8_t*)(SMPC_BASE+0x77))
#define DDR1        (*(volatile uint8_t*)(SMPC_BASE+0x79))
#define DDR2        (*(volatile uint8_t*)(SMPC_BASE+0x7b))
#define IOSEL       (*(volatile uint8_t*)(SMPC_BASE+0x7d))
#define IOSEL1      1
#define IOSEL2      2

#define EXLE        (*(volatile uint8_t*)(SMPC_BASE+0x7f))
#define EXLE1       1
#define EXLE2       2

/* SMPC commands */
#define MSHON       0x00
#define SSHON       0x02
#define SSHOFF      0x03
#define SNDON       0x06
#define SNDOFF      0x07
#define CDON        0x08
#define CDOFF       0x09
#define SYSRES      0x0d
#define CKCHG352    0x0e
#define CKCHG320    0x0f
#define INTBACK     0x10
#define SETTIME     0x16
#define SETSMEM     0x17
#define NMIREQ      0x18
#define RESENAB     0x19
#define RESDISA     0x1a

#endif /* SMPC_H_ */
