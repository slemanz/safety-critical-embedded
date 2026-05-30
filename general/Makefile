CC = arm-none-eabi-gcc
LD = arm-none-eabi-gcc
SIZE = arm-none-eabi-size
OBJCOPY = arm-none-eabi-objcopy

JLINK = jlink

RM = rm -rf

TARGET = flash
DIR_BUILD = build

MCU = -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb
CFLAGS = -c $(MCU) -std=gnu11 -DSTM32F411xE -Wall -O0 -g3 -ffunction-sections -fdata-sections
LDFLAGS = $(MCU) -Tdrivers/Device/STM32F411.ld --specs=nano.specs --specs=nosys.specs -Wl,--gc-sections

INCLUDES =	-Idrivers/Device/cmsis \
			-Idrivers/Inc

OBJS += $(DIR_BUILD)/main.o
OBJS += $(DIR_BUILD)/syscalls.o
OBJS += $(DIR_BUILD)/startup.o

DRIVERS += $(DIR_BUILD)/fpu.o
DRIVERS += $(DIR_BUILD)/uart.o
DRIVERS += $(DIR_BUILD)/timebase.o
DRIVERS += $(DIR_BUILD)/bsp.o

PREREQ = $(DIR_BUILD)/

all: $(PREREQ) $(DIR_BUILD)/$(TARGET).elf

$(PREREQ):
	mkdir $(PREREQ)

$(DIR_BUILD)/%.o: app/Src/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $(DIR_BUILD)/$(*).o app/Src/$(*).c

$(DIR_BUILD)/%.o: drivers/Device/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $(DIR_BUILD)/$(*).o drivers/Device/$(*).c

$(DIR_BUILD)/%.o: drivers/Src/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $(DIR_BUILD)/$(*).o drivers/Src/$(*).c

$(DIR_BUILD)/$(TARGET).elf: $(OBJS) $(DRIVERS)
	$(CC) $(LDFLAGS) -o $@ $^
	$(OBJCOPY) -O binary $(DIR_BUILD)/$(TARGET).elf $(DIR_BUILD)/$(TARGET).bin
	$(SIZE) $(DIR_BUILD)/$(TARGET).elf

flash:
	@$(JLINK) -CommanderScript flash.jlink >/dev/null 2>&1 && echo "Flash Finished"

clean:
	$(RM) $(DIR_BUILD)

.PHONY: clean flash