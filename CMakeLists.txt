cmake_minimum_required(VERSION 3.17)

include(ARMClang_Cortex-M23.cmake)

project(GD32C231C8T6_Template C ASM)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(GD32C2X1_FIRMWARE_DIR
        "${CMAKE_SOURCE_DIR}/Firmware"
        CACHE PATH "Path to the extracted GD32C2x1_Firmware_Library package")

set(GD32C2X1_CMSIS_CORE_DIR
        "${GD32C2X1_FIRMWARE_DIR}/CMSIS/Core/Include")
set(GD32C2X1_DEVICE_DIR
        "${GD32C2X1_FIRMWARE_DIR}/CMSIS/GD/GD32C2x1")
set(GD32C2X1_STDPERIPH_DIR
        "${GD32C2X1_FIRMWARE_DIR}/GD32C2x1_standard_peripheral")

set(GD32C2X1_STARTUP_FILE
        "${GD32C2X1_DEVICE_DIR}/Source/ARM/startup_gd32c2x1.s")
set(GD32C2X1_SYSTEM_FILE
        "${GD32C2X1_DEVICE_DIR}/Source/system_gd32c2x1.c")

foreach(required_path
        "${GD32C2X1_CMSIS_CORE_DIR}"
        "${GD32C2X1_DEVICE_DIR}/Include"
        "${GD32C2X1_STDPERIPH_DIR}/Include"
        "${GD32C2X1_STDPERIPH_DIR}/Source"
        "${GD32C2X1_STARTUP_FILE}"
        "${GD32C2X1_SYSTEM_FILE}")
    if(NOT EXISTS "${required_path}")
        message(FATAL_ERROR
                "Missing GD32C2x1 firmware path: ${required_path}\n"
                "Download GD32C2x1_Firmware_Library_V1.3.0 from GigaDevice, "
                "then copy its Firmware directory to ${CMAKE_SOURCE_DIR}/Firmware.")
    endif()
endforeach()

file(GLOB_RECURSE GD32C2X1_STDPERIPH_SOURCES CONFIGURE_DEPENDS
        "${GD32C2X1_STDPERIPH_DIR}/Source/*.c")

add_executable(${PROJECT_NAME}
        ${GD32C2X1_STARTUP_FILE}
        ${GD32C2X1_SYSTEM_FILE}
        ${GD32C2X1_STDPERIPH_SOURCES}
        main.c)

target_compile_definitions(${PROJECT_NAME} PRIVATE
        GD32C231
        USE_STDPERIPH_DRIVER)

target_include_directories(${PROJECT_NAME} PRIVATE
        "${GD32C2X1_CMSIS_CORE_DIR}"
        "${GD32C2X1_DEVICE_DIR}/Include"
        "${GD32C2X1_STDPERIPH_DIR}/Include")

target_link_options(${PROJECT_NAME} PRIVATE
        --cpu=Cortex-M23
        --strict
        --scatter "${CMAKE_SOURCE_DIR}/GD32.sct"
        --summary_stderr
        --info summarysizes
        --map
        --load_addr_map_info
        --xref
        --callgraph
        --symbols
        --info sizes
        --info totals
        --info unused
        --info veneers)
