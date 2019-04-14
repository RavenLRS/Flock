CPU_FLAGS = -mcpu=cortex-m0
FLOAT-ABI =
DEVICE_SRC_DIRS = lib/STM32F0xx_HAL_Driver/Src
DEVICE_INCLUDE_DIRS = lib/STM32F0xx_HAL_Driver/Inc \
	lib/STM32F0xx_HAL_Driver/Inc/Legacy \
	lib/CMSIS/Device/ST/STM32F0xx/Include \
	lib/CMSIS/Include \
	lib/CMSIS/Include

C_DEFS += USE_FULL_LL_DRIVER USE_HAL_DRIVER STM32F030x6
