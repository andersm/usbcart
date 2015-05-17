#   Sega Saturn USB flash cart ROM
#   Copyright © 2012, 2015 Anders Montonen
#   All rights reserved.
#
#   Redistribution and use in source and binary forms, with or without
#   modification, are permitted provided that the following conditions are met:
#
#   Redistributions of source code must retain the above copyright notice, this
#   list of conditions and the following disclaimer.
#   Redistributions in binary form must reproduce the above copyright notice,
#   this list of conditions and the following disclaimer in the documentation
#   and/or other materials provided with the distribution.
#
#   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
#   AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
#   IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
#   ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
#   LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
#   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
#   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
#   INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
#   CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
#   ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
#   POSSIBILITY OF SUCH DAMAGE.

CC = sh-elf-gcc
AS = sh-elf-as
OBJCOPY = sh-elf-objcopy
LDFLAGS = -nostartfiles -nostdlib -T $(LDSCRIPT)
CFLAGS  = -Wall -Werror -m2 -Os -fomit-frame-pointer -std=c99

LIBS = -lgcc

EXE = romimage.bin

RAMEXE = ramimage.bin
RAMEXEOBJ = ramimage.elf

OBJ = obj/crt0.o \
	obj/main.o   \
	obj/crc.o    \
	obj/sysid.o

RAMOBJ = obj/crt0.o \
	obj/main.o   \
	obj/crc.o

all : $(EXE)

ram : $(RAMEXE)

$(EXE) : LDSCRIPT = ldscript
$(EXE) : makedir $(OBJ)
	$(CC) $(LDFLAGS) $(OBJ) $(LIBS) -o $(EXE)

$(RAMEXE) : LDSCRIPT = ramldscript
$(RAMEXE) : makedir $(RAMOBJ)
	$(CC) $(LDFLAGS) $(RAMOBJ) $(LIBS) -o $(RAMEXEOBJ)
	$(OBJCOPY) -O binary $(RAMEXEOBJ) $(RAMEXE)

obj/%.o : %.c
	$(CC) -c $< -o $@ $(CFLAGS)

obj/%.o : %.S
	$(AS) $< -o $@

clean :
	rm -f obj/*.o
	rm -f $(EXE)
	rm -f $(RAMEXE)
	rm -f $(RAMEXEOBJ)

makedir :
	mkdir -p obj

#
# end of makefile
#
