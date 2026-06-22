# ARMClang toolchain settings for GD32C231C8T6 (Arm Cortex-M23).

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR cortex-m23)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

find_program(ARMCLANG_EXECUTABLE
        NAMES armclang.exe armclang
        HINTS
            "C:/Program Files/ArmCompilerforEmbedded6.24/bin"
            "C:/Program Files/ArmCompilerforEmbedded6/bin")
find_program(ARMLINK_EXECUTABLE
        NAMES armlink.exe armlink
        HINTS
            "C:/Program Files/ArmCompilerforEmbedded6.24/bin"
            "C:/Program Files/ArmCompilerforEmbedded6/bin")
find_program(FROMELF_EXECUTABLE
        NAMES fromelf.exe fromelf
        HINTS
            "C:/Program Files/ArmCompilerforEmbedded6.24/bin"
            "C:/Program Files/ArmCompilerforEmbedded6/bin")

if(NOT ARMCLANG_EXECUTABLE)
    message(FATAL_ERROR "armclang was not found. Add Arm Compiler for Embedded bin directory to PATH.")
endif()

if(NOT ARMLINK_EXECUTABLE)
    message(FATAL_ERROR "armlink was not found. Add Arm Compiler for Embedded bin directory to PATH.")
endif()

set(CMAKE_C_COMPILER "${ARMCLANG_EXECUTABLE}" CACHE FILEPATH "ARMClang C compiler")
set(CMAKE_ASM_COMPILER "${ARMCLANG_EXECUTABLE}" CACHE FILEPATH "ARMClang assembler")
set(CMAKE_LINKER "${ARMLINK_EXECUTABLE}" CACHE FILEPATH "ARM linker")

set(CMAKE_C_COMPILER_WORKS TRUE)
set(CMAKE_ASM_COMPILER_WORKS TRUE)

set(CMAKE_C_FLAGS_INIT
        "--target=arm-arm-none-eabi -mcpu=cortex-m23 -mfloat-abi=soft -ffunction-sections -fdata-sections -O1 -g"
        CACHE STRING "Initial ARMClang C flags")
set(CMAKE_ASM_FLAGS_INIT
        "--target=arm-arm-none-eabi -mcpu=cortex-m23 -mfloat-abi=soft -masm=auto -gdwarf-3"
        CACHE STRING "Initial ARMClang ASM flags")

set(CMAKE_C_LINK_EXECUTABLE
        "\"${ARMLINK_EXECUTABLE}\" <CMAKE_C_LINK_FLAGS> <LINK_FLAGS> <OBJECTS> --output <TARGET> <LINK_LIBRARIES>")
