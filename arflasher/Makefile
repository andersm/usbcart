#
# Sega Saturn cartridge flash tool
# by Anders Montonen, 2012
#
# Original software by ExCyber
# Graphics routines by Charles MacDonald
#
# Creative Commons Attribution-ShareAlike 3.0 Unported (CC BY-SA 3.0)
#

CC	=	sh-elf-gcc
AS	=	sh-elf-as
LDFLAGS =	-m2 -nostartfiles
FLAGS	=	-Wall -Werror -m2 -mb -Os -fomit-frame-pointer -nostdlib -std=c99

EXE =	flasher.bin
IMG =	flasher.iso

OBJ =	obj/crt0.o	\
		obj/main.o	\
		obj/flash.o \
		obj/conio.o \
		obj/vdp.o \
		obj/romwrap.o

CDOBJ = $(OBJ) \
		obj/sysid.o

all :	$(EXE)

cd	:	$(IMG)

$(EXE)	:	$(OBJ)
		$(CC) $(LDFLAGS) $(OBJ) -o $(EXE) -T ldscript_bin $(LIBS)

$(IMG)	:	$(CDOBJ)
		$(CC) $(LDFLAGS) $(CDOBJ) -o $(EXE) -T ldscript_cd $(LIBS)
		mkisofs -sysid "SEGA SATURN" -volid "CARTFLASH" -volset "CARTFLASH" -publisher "SEGA ENTERPRISES, LTD." -preparer "SEGA ENTERPRISES, LTD." -appid "CARTFLASH" -generic-boot $(EXE) -full-iso9660-filenames -o $(IMG) hello.txt

obj/%.o :	%.c
		$(CC) -c $< -o $@ $(FLAGS)

obj/%.o :	%.S
		$(AS) $< -o $@

clean	:
		rm -f obj/*.o
		rm -f $(EXE)
		rm -f $(IMG)

makedir :
		mkdir obj

#
# end of makefile
#
