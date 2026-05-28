CC = arm-none-eabi-gcc
LD = arm-none-eabi-gcc
SIZE = arm-none-eabi-size
OBJCOPY = arm-none-eabi-size

RM = rm -rf

TARGET = flash
DIR_BUILD = build

MCU = -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb
CFLAGS = $(MCU) -std=gnu11 -DSTM32L432xx -Wall -O0 -ffunction-sections -fdata-sections
LDFLAGS = $(MCU) -Tdrivers/Device/STM32F411.ld --specs=nano.specs --specs=nosys.specs -Wl,--gc-sections

INCLUDES =	-Idrivers/Device/cmsis \
			-Idrivers/Inc

OBJS += $(DIR_BUILD)/main.o
OBJS += $(DIR_BUILD)/syscalls.o
OBJS += $(DIR_BUILD)/startup.o

PREREQ = $(DIR_BUILD)/

all: $(PREREQ) $(DIR_BUILD)/$(TARGET).elf

$(PREREQ):
	mkdir $(PREREQ)

$(DIR_BUILD)/%.o: app/Src/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $(DIR_BUILD)/$(*).o app/Src/$(*).c

$(DIR_BUILD)/%.o: drivers/Device/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $(DIR_BUILD)/$(*).o drivers/Device/$(*).c

clean:
	$(RM) $(DIR_BUILD)

.PHONY: clean flash