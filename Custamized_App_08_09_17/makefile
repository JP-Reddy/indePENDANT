
CROSS := mips-elf-
AS := $(CROSS)as
CC := $(CROSS)gcc
CPP := ${CC} -E
C++ := $(CROSS)g++ 
AR := $(CROSS)ar
OBJCOPY := $(CROSS)objcopy
OBJDUMP := $(CROSS)objdump
NM := $(CROSS)nm
LD := $(CROSS)ld -EL


CP := cp -f
RM := rm -fr
ECHO := echo 
MKDIR := mkdir

.PHONY : all USSAGE app __envbuildall__ __envbuildapp__

BDIR := $(SOFT_WORKDIR)/build
LDIR := $(SOFT_WORKDIR)/lib
CHK := $(SOFT_WORKDIR)/tool/g510chk.exe

SYS_EXT :=

CCFLAG := -EL \
		  -mips16 \
		  -std=gnu99 \
		  -Wno-pointer-sign \
		  -G0 \
		  -c \
		  -Wshadow \
		  -O3 \
		  -mlong-calls \
		  -msoft-float \
		  -Wall \
		  -Wundef \
		  -Wunused-function \
		  -Wuninitialized \
		  -minterlink-mips16 \
		  -fno-strict-aliasing \
		  -fno-inline-small-functions \
		  -fno-inline-functions \
		  -fno-align-functions \
		  -fno-align-jumps \
		  -fno-align-loops \
		  -fno-align-labels \
		  -fno-builtin-iswspace \
		  -ffixed-t3 \
		  -ffixed-t4 \
		  -ffixed-t5 \
		  -ffixed-t6 \
		  -ffixed-t7 \
		  -ffixed-s2 \
		  -ffixed-s3 \
		  -ffixed-s4 \
		  -ffixed-s5 \
		  -ffixed-s6 \
		  -ffixed-s7 \
		  -ffixed-fp \
		  -mexplicit-relocs \
		  -fweb \
		  -frename-registers \
		  -mmemcpy \
		  -mmips-tfile \
		  -nostartfiles \
		  -nostdlib \
		  -nostdinc \
		  -nodefaultlibs \
		  -fwide-exec-charset=UTF-16LE \
		  -fshort-wchar \
		  -pipe


CCINCLUDE := -I ./inc

USSAGE:
	@$(ECHO) "USAGE :" make all/app

-include $(SOFT_WORKDIR)/sys_src/makefile
include $(SOFT_WORKDIR)/app/makefile

ifndef SYS_HEADER

SYS_HEADER := $(SOFT_WORKDIR)/lib/sys_init.lib
SYS_EXT += -lsys_ext -lgcc

else

include $(SOFT_WORKDIR)/sys_ext/makefile

all: __envbuildall__ $(SYS_HEADER) $(OBJS_APP) $(SYS_EXT)
	@$(LD) $(OBJS_APP_NAME) --oformat=elf32-littlemips -L $(SOFT_WORKDIR)/lib -L $(SOFT_WORKDIR)/tool $(SYS_EXT) -nostdlib $(sys_ext) --whole-archive $(SYS_HEADER) --script rda_app.lds -Map $(BDIR)/image.map -o $(BDIR)/image.out
	@${OBJCOPY} -S -O binary $(BDIR)/image.out $(BDIR)/app.bin
	@$(OBJDUMP) -D -s -t -b binary -m mips $(BDIR)/app.bin > $(BDIR)/app.dis
	@$(ECHO) "build appchk.bin"
	@$(CHK) $(BDIR)/app.bin $(BDIR)/appchk.bin

endif

%.o : %.c
	@$(ECHO) "CC " $<
	@$(CC) $(CCFLAG) $< -o $(BDIR)/$@ $(CCINCLUDE)

__envbuildall__:
	@$(RM) $(BDIR)
	@$(RM) $(LDIR)
	@$(MKDIR) $(BDIR)
	@$(MKDIR) $(LDIR)
	
app: __envbuildapp__ $(OBJS_APP)
	@$(LD) $(OBJS_APP_NAME) --oformat=elf32-littlemips -L $(SOFT_WORKDIR)/lib -L $(SOFT_WORKDIR)/tool $(SYS_EXT) -nostdlib --whole-archive $(SYS_HEADER) --script rda_app.lds -Map $(BDIR)/image.map -o $(BDIR)/image.out
	@${OBJCOPY} -S -O binary $(BDIR)/image.out $(BDIR)/app.bin
	@$(OBJDUMP) -D -s -t -b binary -m mips $(BDIR)/app.bin > $(BDIR)/app.dis
	@$(CHK) $(BDIR)/app.bin $(BDIR)/appchk.bin
	
__envbuildapp__:
	@$(RM) $(BDIR)
	@$(MKDIR) $(BDIR)