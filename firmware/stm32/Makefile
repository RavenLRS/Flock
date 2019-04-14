TARGET = flock
DEBUG ?= 1
ifeq ($(DEBUG),1)
OPT = -Og
else
OPT = -Os
endif
ifeq ($(LTO), 0)
LTO =
else
LTO = -flto -fuse-linker-plugin
endif
PLATFORM = flock1

CFLAGS 		?=
CPPFLAGS 	?=
SRC_DIRS 	?=
INCLUDE_DIRS	?=
C_DEFS		?=

PLATFORM_DIR = target/platforms/$(PLATFORM)
include $(PLATFORM_DIR)/platform.mk

DEVICE_DIR = target/devices/$(DEVICE)
include $(DEVICE_DIR)/device.mk

BUILD_DIR = build

FLOCK_LIB_DIRS = ../../src/libflock ../../src/libflockdevice

SRC_DIRS += $(DEVICE_DIR) $(DEVICE_SRC_DIRS)
SRC_DIRS += $(PLATFORM_DIR) $(PLATFORM_SRC_DIRS)
SRC_DIRS += $(FLOCK_LIB_DIRS)
SRC_DIRS += src

dir_c_sources	= $(wildcard $(dir)/*.c)
dir_s_sources	= $(wildcard $(dir)/*.s)

C_SOURCES       = $(foreach dir,$(SRC_DIRS),$(dir_c_sources))
ASM_SOURCES		= $(foreach dir,$(SRC_DIRS),$(dir_s_sources))

PREFIX = arm-none-eabi-
ifdef GCC_PATH
CC = $(GCC_PATH)/$(PREFIX)gcc
AS = $(GCC_PATH)/$(PREFIX)gcc -x assembler-with-cpp
CP = $(GCC_PATH)/$(PREFIX)objcopy
SZ = $(GCC_PATH)/$(PREFIX)size
else
CC = $(PREFIX)gcc
AS = $(PREFIX)gcc -x assembler-with-cpp
CP = $(PREFIX)objcopy
SZ = $(PREFIX)size
endif
HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S
 
MCU = $(CPU_FLAGS) -mthumb $(FPU) $(FLOAT-ABI)

AS_DEFS = 



# AS includes
AS_INCLUDES = 

# C includes
C_INCLUDES = src $(DEVICE_INCLUDE_DIRS) $(PLATFORM_INCLUDE_DIRS) $(FLOCK_LIB_DIRS)

# compile gcc flags
ASFLAGS += $(MCU) $(AS_DEFS) $(AS_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

CFLAGS += $(MCU)
CFLAGS += $(addprefix -D,$(C_DEFS))
CFLAGS += $(addprefix -I,$(C_INCLUDES))
CFLAGS += $(OPT)
CFLAGS += $(LTO)
CFLAGS += -Wall -fdata-sections -ffunction-sections

ifeq ($(DEBUG), 1)
CFLAGS += -g -gdwarf-2
endif


# Generate dependency information
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"


#######################################
# LDFLAGS
#######################################
# link script
LDSCRIPT = $(wildcard $(DEVICE_DIR)/*.ld)

# libraries
LIBS = -lc -lm -lnosys 
LIBDIR = 
LDFLAGS = $(MCU) -specs=nano.specs -T$(LDSCRIPT) $(LIBDIR) $(LIBS) $(OPT) $(LTO) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections -Wl,--print-memory-usage

# default action: build all
all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin


#######################################
# build the application
#######################################
# list of objects
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))
# list of ASM program objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR) 
	$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	$(AS) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) Makefile
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(SZ) $@

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(HEX) $< $@
	
$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(BIN) $< $@	
	
$(BUILD_DIR):
	mkdir $@

#######################################
# flash
#######################################

STFLASH                 = $(shell which st-flash)
OOCD                    = $(shell which openocd)
OOCD_FILE               = $(PLATFORM_DIR)/openocd.cfg

# With STLink
sflash-%: $(BUILD_DIR)/%.bin
	@printf "  FLASH  $<\n"
	st-flash write $< 0x8000000

sflash: sflash-$(TARGET)

# With OpenOCD
oflash-%: $(BUILD_DIR)/%.elf
	@printf "  FLASH   $<\n"
	(echo "halt; program $(realpath $<) verify reset" | nc -4 localhost 4444 2>/dev/null) || \
	$(OOCD) -f $(OOCD_FILE) \
	-c "program $< verify reset exit" \
	$(NULL)

oflash: oflash-$(TARGET)

#######################################
# clean up
#######################################
clean:
	-rm -fR $(BUILD_DIR)
  
#######################################
# dependencies
#######################################
-include $(wildcard $(BUILD_DIR)/*.d)

# *** EOF ***
