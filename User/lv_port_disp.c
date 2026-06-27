/**
 * @file lv_port_disp.c
 * @brief LVGL display driver port implementation for GD32C231 SPI LCD
 */

#include "lv_port_disp.h"
#include "lcd_spi.h"
#include <string.h>
/* Partial buffer: 240 x 2 rows */
#define DISP_BUF_LINES  2

static lv_disp_drv_t disp_drv;
static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf1[LCD_WIDTH * DISP_BUF_LINES];

/**
 * @brief Flush callback - send rendered buffer to LCD via SPI
 */
static void disp_flush(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p)
{
    int32_t x0 = area->x1;
    int32_t y0 = area->y1;
    int32_t x1 = area->x2;
    int32_t y1 = area->y2;

    int32_t w = (x1 - x0 + 1);
    int32_t h = (y1 - y0 + 1);
    int32_t size = w * h;

    lcd_blit_area((uint16_t)x0, (uint16_t)y0, (uint16_t)x1, (uint16_t)y1,
                  (const uint8_t *)color_p, (uint32_t)size * 2U);

    /* Tell LVGL flushing is done */
    lv_disp_flush_ready(disp_drv);
}

/**
 * @brief Initialize the LVGL display driver
 */
void lv_port_disp_init(void)
{
    /* Initialize LCD hardware */
    lcd_init();

    /* Initialize draw buffer */
    lv_disp_draw_buf_init(&draw_buf, buf1, NULL, LCD_WIDTH * DISP_BUF_LINES);

    /* Initialize display driver */
    lv_disp_drv_init(&disp_drv);

    disp_drv.draw_buf = &draw_buf;
    disp_drv.flush_cb = disp_flush;
    disp_drv.hor_res = LCD_WIDTH;
    disp_drv.ver_res = LCD_HEIGHT;

    /* Register the display driver */
    lv_disp_drv_register(&disp_drv);

}
