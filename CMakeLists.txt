cmake_minimum_required(VERSION 3.17)
cmake_policy(SET CMP0123 NEW)

include(ARMClang_Cortex-M23.cmake)

project(GD32C231C8T6_Cmake C ASM)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(GD32C2X1_FIRMWARE_DIR
        "${CMAKE_SOURCE_DIR}/Firmware"
        CACHE PATH "Path to the extracted GD32C2x1_Firmware_Library package")

set(GD32C2X1_CMSIS_CORE_DIR
        "${GD32C2X1_FIRMWARE_DIR}/CMSIS")
set(GD32C2X1_DEVICE_DIR
        "${GD32C2X1_FIRMWARE_DIR}/CMSIS/GD/GD32C2x1")
set(GD32C2X1_STDPERIPH_DIR
        "${GD32C2X1_FIRMWARE_DIR}/GD32C2x1_standard_peripheral")

set(GD32C2X1_STARTUP_FILE
        "${GD32C2X1_DEVICE_DIR}/Source/ARM/startup_gd32c231_221.s")
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

# GD32 standard peripheral library removed - using minimal system init
# system_minimal.c replaces system_gd32c2x1.c to save Flash

# ---------- LVGL (selective source files for minimal Flash usage) ----------
set(LVGL_DIR "${CMAKE_SOURCE_DIR}/lvgl")
#set(LVGL_SOURCES
#        # Core
#        ${LVGL_DIR}/src/core/lv_disp.c
#        ${LVGL_DIR}/src/core/lv_event.c
#        ${LVGL_DIR}/src/core/lv_group.c
#        ${LVGL_DIR}/src/core/lv_indev.c
#        ${LVGL_DIR}/src/core/lv_indev_scroll.c
#        ${LVGL_DIR}/src/core/lv_obj.c
#        ${LVGL_DIR}/src/core/lv_obj_class.c
#        ${LVGL_DIR}/src/core/lv_obj_draw.c
#        ${LVGL_DIR}/src/core/lv_obj_pos.c
#        ${LVGL_DIR}/src/core/lv_obj_scroll.c
#        ${LVGL_DIR}/src/core/lv_obj_style.c
#        ${LVGL_DIR}/src/core/lv_obj_style_gen.c
#        ${LVGL_DIR}/src/core/lv_obj_tree.c
#        ${LVGL_DIR}/src/core/lv_refr.c
#        ${LVGL_DIR}/src/core/lv_theme.c
#        # HAL
#        ${LVGL_DIR}/src/hal/lv_hal_disp.c
#        ${LVGL_DIR}/src/hal/lv_hal_indev.c
#        ${LVGL_DIR}/src/hal/lv_hal_tick.c
#        # Draw
#        ${LVGL_DIR}/src/draw/lv_draw.c
#        ${LVGL_DIR}/src/draw/lv_draw_arc.c
#        ${LVGL_DIR}/src/draw/lv_draw_img.c
#        ${LVGL_DIR}/src/draw/lv_draw_label.c
#        ${LVGL_DIR}/src/draw/lv_draw_line.c
#        ${LVGL_DIR}/src/draw/lv_draw_rect.c
#        ${LVGL_DIR}/src/draw/lv_draw_transform.c
#        ${LVGL_DIR}/src/draw/lv_draw_layer.c
#        ${LVGL_DIR}/src/draw/lv_img_buf.c
#        ${LVGL_DIR}/src/draw/sw/lv_draw_sw.c
#        ${LVGL_DIR}/src/draw/sw/lv_draw_sw_arc.c
#        ${LVGL_DIR}/src/draw/sw/lv_draw_sw_blend.c
#        ${LVGL_DIR}/src/draw/sw/lv_draw_sw_img.c
#        ${LVGL_DIR}/src/draw/sw/lv_draw_sw_letter.c
#        ${LVGL_DIR}/src/draw/sw/lv_draw_sw_line.c
#        ${LVGL_DIR}/src/draw/sw/lv_draw_sw_polygon.c
#        ${LVGL_DIR}/src/draw/sw/lv_draw_sw_rect.c
#        ${LVGL_DIR}/src/draw/sw/lv_draw_sw_transform.c
#        # Font (only the one we use)
#        ${LVGL_DIR}/src/font/lv_font.c
#        ${LVGL_DIR}/src/font/lv_font_fmt_txt.c
#        ${LVGL_DIR}/src/font/lv_font_montserrat_10.c
#        # Misc
#        ${LVGL_DIR}/src/misc/lv_area.c
#        ${LVGL_DIR}/src/misc/lv_anim.c
#        ${LVGL_DIR}/src/misc/lv_async.c
#        ${LVGL_DIR}/src/misc/lv_color.c
#        ${LVGL_DIR}/src/misc/lv_fs.c
#        ${LVGL_DIR}/src/misc/lv_gc.c
#        ${LVGL_DIR}/src/misc/lv_ll.c
#        ${LVGL_DIR}/src/misc/lv_log.c
#        ${LVGL_DIR}/src/misc/lv_math.c
#        ${LVGL_DIR}/src/misc/lv_mem.c
#        ${LVGL_DIR}/src/misc/lv_printf.c
#        ${LVGL_DIR}/src/misc/lv_style.c
#        ${LVGL_DIR}/src/misc/lv_style_gen.c
#        ${LVGL_DIR}/src/misc/lv_timer.c
#        ${LVGL_DIR}/src/misc/lv_tlsf.c
#        ${LVGL_DIR}/src/misc/lv_txt.c
#        ${LVGL_DIR}/src/misc/lv_utils.c
#        # Widgets (only label and btn)
#        ${LVGL_DIR}/src/widgets/lv_btn.c
#        ${LVGL_DIR}/src/widgets/lv_label.c
#        ${LVGL_DIR}/src/widgets/lv_objx_templ.c
#        # Extra init (needed for lv_extra_init)
#        ${LVGL_DIR}/src/extra/lv_extra.c
#)
file(GLOB_RECURSE LVGL_SOURCES
        ${LVGL_DIR}/src/*.c
)

add_executable(${PROJECT_NAME}
        ${GD32C2X1_STARTUP_FILE}
        ${GD32C2X1_SYSTEM_FILE}
        ${LVGL_SOURCES}
        ${GD32C2X1_STDPERIPH_DIR}/Source/gd32c2x1_rcu.c
        ${GD32C2X1_STDPERIPH_DIR}/Source/gd32c2x1_gpio.c
        User/systick.c
        User/lcd_spi.c
        User/lv_port_disp.c
        main.c)

target_compile_definitions(${PROJECT_NAME} PRIVATE
        GD32C231
        LV_CONF_INCLUDE_SIMPLE)

target_include_directories(${PROJECT_NAME} PRIVATE
        "${GD32C2X1_CMSIS_CORE_DIR}"
        "${GD32C2X1_DEVICE_DIR}/Include"
        "${GD32C2X1_STDPERIPH_DIR}/Include"
        "${CMAKE_SOURCE_DIR}/User"
        "${CMAKE_SOURCE_DIR}"
        "${LVGL_DIR}"
        "${LVGL_DIR}/src")

target_link_options(${PROJECT_NAME} PRIVATE
        --cpu=Cortex-M23
        --strict
        --scatter "${CMAKE_SOURCE_DIR}/GD32.sct"
        --summary_stderr
        # --map
        --load_addr_map_info
        # --xref
        --callgraph
        # --symbols
        # --info summarysizes,sizes,totals,unused,veneers
        --info sizes,totals,veneers
)

# ---------- Post-build: generate .bin and .hex from .elf ----------
if(FROMELF_EXECUTABLE)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND "${FROMELF_EXECUTABLE}" --bin --output "$<TARGET_FILE_DIR:${PROJECT_NAME}>/${PROJECT_NAME}.bin" "$<TARGET_FILE:${PROJECT_NAME}>"
            COMMENT "Generating .bin file with fromelf")
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND "${FROMELF_EXECUTABLE}" --i32combined --output "$<TARGET_FILE_DIR:${PROJECT_NAME}>/${PROJECT_NAME}.hex" "$<TARGET_FILE:${PROJECT_NAME}>"
            COMMENT "Generating .hex file with fromelf")
else()
    message(WARNING "fromelf not found – .bin/.hex will not be generated.")
endif()
