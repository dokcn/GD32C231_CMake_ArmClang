# ARMClang toolchain settings for GD32C231C8T6 (Arm Cortex-M23).

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR cortex-m23)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

set(CMAKE_C_COMPILER armclang.exe CACHE FILEPATH "ARMClang C compiler")
set(CMAKE_ASM_COMPILER armclang.exe CACHE FILEPATH "ARMClang assembler")
set(CMAKE_LINKER armlink.exe CACHE FILEPATH "ARM linker")

set(CMAKE_C_COMPILER_WORKS TRUE)
set(CMAKE_ASM_COMPILER_WORKS TRUE)

set(CMAKE_C_FLAGS_INIT
        "--target=arm-arm-none-eabi -mcpu=cortex-m23 -mfloat-abi=soft -ffunction-sections -fdata-sections -O1 -g"
        CACHE STRING "Initial ARMClang C flags")
set(CMAKE_ASM_FLAGS_INIT
        "--target=arm-arm-none-eabi -mcpu=cortex-m23 -mfloat-abi=soft -masm=auto -gdwarf-3"
        CACHE STRING "Initial ARMClang ASM flags")

set(CMAKE_C_LINK_EXECUTABLE
        "armlink.exe <CMAKE_C_LINK_FLAGS> <LINK_FLAGS> <OBJECTS> --output <TARGET> <LINK_LIBRARIES>")
