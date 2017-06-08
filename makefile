TARGET = BOOT

CROSS_COMPILE = arm-none-eabi

TOOL_CHAIN_ROOT = e:/Cross_Tool

TOOL_CHAIN_VER = 4.8.3

Suffix = .exe

CC      	= $(TOOL_CHAIN_ROOT)/bin/$(CROSS_COMPILE)-gcc$(Suffix)
LD      	= $(TOOL_CHAIN_ROOT)/bin/$(CROSS_COMPILE)-ld$(Suffix)
AR      	= $(TOOL_CHAIN_ROOT)/bin/$(CROSS_COMPILE)-ar$(Suffix)
OBJCOPY 	= $(TOOL_CHAIN_ROOT)/bin/$(CROSS_COMPILE)-objcopy$(Suffix)
OBJDUMP 	= $(TOOL_CHAIN_ROOT)/bin/$(CROSS_COMPILE)-objdump$(Suffix)
STRIP   	= $(TOOL_CHAIN_ROOT)/bin/$(CROSS_COMPILE)-strip$(Suffix)
READELF 	= $(TOOL_CHAIN_ROOT)/bin/$(CROSS_COMPILE)-readelf$(Suffix)

CORTEX_M = 3

ARCH_FLAGS = -mthumb -mcpu=cortex-m$(CORTEX_M)

CURDIR = .

TOP = $(CURDIR)

INC_FLAGS= -I $(TOP)/CORE	\
           -I $(TOP)/HARDWARE	\
           -I $(TOP)/STM32F10x_FWLib/inc	\
           -I $(TOP)/SYSTEM	\
           -I $(TOP)/USER

CFLAGS =  -W -Wall -g $(ARCH_FLAGS) -D STM32F10X_HD -D USE_STDPERIPH_DRIVER $(INC_FLAGS) -O0 -std=gnu11 -ffunction-sections -fdata-sections

C_SRC=$(wildcard ./USER/*.c) $(wildcard ./STM32F10x_FWLib/src/*.c)  $(wildcard ./SYSTEM/*.c) $(wildcard ./HARDWARE/*.c)

C_OBJ=$(C_SRC:%.c=%.o)

OBJS := $(C_OBJ)

LDSCRIPTS = -T $(TARGET).ld

LDFLAGS = -Wl,--start-group -lc -lm -Wl,--end-group -specs=nano.specs -specs=nosys.specs -static -Wl,-Map=$(TARGET).map -Wl,--gc-sections

.PHONY: all clean update

all:$(OBJS)
	$(CC) $(OBJS) $(LDSCRIPTS) -o $(TARGET).elf $(ARCH_FLAGS) $(LDFLAGS)
	$(OBJCOPY) $(TARGET).elf  $(TARGET).bin -Obinary 
	$(OBJCOPY) $(TARGET).elf  $(TARGET).hex -Oihex
	${OBJDUMP} -D -m arm $(TARGET).elf > $(TARGET).dump

$(C_OBJ):%.o:%.c
	$(CC) -c $(CFLAGS) -o $@ $<
	
clean:
	rm -f *.bin 
	rm -f *.elf 
	rm -f *.dump
	rm -f *.hex
	rm -f *.map
	rm -f ./USER/*.o
	rm -f ./STM32F10x_FWLib/src/*.o
	rm -f ./SYSTEM/*.o
	rm -f ./HARDWARE/*.o


 










