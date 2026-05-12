CC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
OBJDUMP = arm-none-eabi-objdump
SIZE = arm-none-eabi-size

CFLAGS = -mcpu=cortex-m0plus -mthumb -O2 -Wall -g
LDFLAGS = -T psoc4_linker.ld -nostartfiles -Wl,-Map=firmware.map,--gc-sections --specs=nano.specs --specs=rdimon.specs
LDLIBS = -lc -lrdimon

SRCS = main.c i2c_bitbang.c ds1307.c startup.c
OBJS = $(SRCS:.c=.o)

TARGET = firmware

all: $(TARGET).elf $(TARGET).hex $(TARGET).bin size

$(TARGET).elf: $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(OBJS) $(LDLIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET).hex: $(TARGET).elf
	$(OBJCOPY) -O ihex $< $@

$(TARGET).bin: $(TARGET).elf
	$(OBJCOPY) -O binary $< $@

size: $(TARGET).elf
	$(SIZE) $<

dump: $(TARGET).elf
	$(OBJDUMP) -d $< > $(TARGET).asm

flash: $(TARGET).hex
	openocd -s "C:/OpenOCD/share/openocd/scripts" -f interface/cmsis-dap.cfg -f target/psoc4.cfg -c "program $(TARGET).hex verify reset exit"

clean:
	rm -f *.o *.elf *.hex *.bin *.map *.asm

.PHONY: all clean flash dump size
