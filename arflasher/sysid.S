!    Sega Saturn cartridge flash tool
!    Copyright © 2012, 2015 Anders Montonen
!    Original software by ExCyber
!    Graphics routines by Charles MacDonald
!    All rights reserved.
!
!    Redistribution and use in source and binary forms, with or without
!    modification, are permitted provided that the following conditions are met:
!
!    Redistributions of source code must retain the above copyright notice, this
!    list of conditions and the following disclaimer.
!    Redistributions in binary form must reproduce the above copyright notice,
!    this list of conditions and the following disclaimer in the documentation
!    and/or other materials provided with the distribution.
!
!    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
!    AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
!    IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
!    ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
!    LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
!    CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
!    SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
!    INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
!    CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
!    ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
!    POSSIBILITY OF SUCH DAMAGE.

! System ID block

.section .sysid

hwid:       .ascii "SEGA SEGASATURN "
makerid:    .ascii "ANDERS MONTONEN "
produm:     .ascii "ANTIME-002"
version:    .ascii "V1.000"
reldat:     .ascii "20120610"
devinfo:    .ascii "R  2    "
area:       .ascii "JTUE      "
            .ascii "      "
periph:     .ascii "J               "
title:      .ascii "                "
            .ascii "                "
            .ascii "                "
            .ascii "                "
            .ascii "                "
            .ascii "                "
            .ascii "                "
            .ascii "                "
ipsize:     .long  __ip_len
stackm:     .long 0
            .long 0
stacks:     .long 0
firstaddr:  .long 0
firstsize:  .long 0
            .long 0
            .long 0

            .incbin "sega/sys_sec.bin"
            .incbin "sega/sys_arej.bin"
            .incbin "sega/sys_aret.bin"
            .incbin "sega/sys_areu.bin"
            .incbin "sega/sys_aree.bin"
            .incbin "sega/sys_init.bin"
