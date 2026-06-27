/**
 * @file lcd_spi.c
 * @brief ILI9341 driver using software SPI (most reliable on GD32C231)
 *
 * PB13 - SCK, PB15 - MOSI (GPIO bit-bang, SPI mode 0)
 * PA8  - CS, PC7 - DC, PC6 - RST
 */

#include "lcd_spi.h"
#include "gd32c2x1.h"
#include "gd32c2x1_gpio.h"
#include "gd32c2x1_rcu.h"
#include "gd32c2x1_spi.h"

#include "systick.h"

#include <stddef.h>

void lcd_delay_ms(uint32_t ms)
{
    /*for (uint32_t m = 0U; m < ms; m++) {
        for (volatile uint32_t i = 0U; i < 4000U; i++) {
            __NOP();
        }
    }*/
    delay_1ms(ms);
}

static void soft_spi_write_byte(uint8_t data)
{
    for (int8_t bit = 7; bit >= 0; bit--) {
        gpio_bit_reset(LCD_SPI_GPIO_PORT, LCD_SPI_SCK_PIN);
        if ((data & (1U << (uint32_t) bit)) != 0U) {
            gpio_bit_set(LCD_SPI_GPIO_PORT, LCD_SPI_MOSI_PIN);
        } else {
            gpio_bit_reset(LCD_SPI_GPIO_PORT, LCD_SPI_MOSI_PIN);
        }
        gpio_bit_set(LCD_SPI_GPIO_PORT, LCD_SPI_SCK_PIN);
    }
    gpio_bit_reset(LCD_SPI_GPIO_PORT, LCD_SPI_SCK_PIN);
}

static void lcd_gpio_init(void)
{
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_GPIOB);
    rcu_periph_clock_enable(RCU_GPIOC);
    // rcu_periph_clock_enable(RCU_SPI1);

    gpio_mode_set(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LCD_CS_PIN);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_LEVEL_1, LCD_CS_PIN);

    gpio_mode_set(GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LCD_SPI_SCK_PIN | LCD_SPI_MOSI_PIN);
    gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_LEVEL_1, LCD_SPI_SCK_PIN | LCD_SPI_MOSI_PIN);
    gpio_bit_reset(GPIOB, LCD_SPI_SCK_PIN);
    gpio_bit_reset(GPIOB, LCD_SPI_MOSI_PIN);

    gpio_mode_set(GPIOC, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LCD_RST_PIN | LCD_DC_PIN);
    gpio_output_options_set(GPIOC, GPIO_OTYPE_PP, GPIO_OSPEED_LEVEL_1, LCD_RST_PIN | LCD_DC_PIN);

    /*spi_parameter_struct spi_init_struct;
    spi_init_struct.trans_mode           = SPI_TRANSMODE_FULLDUPLEX;
    spi_init_struct.device_mode          = SPI_MASTER;
    spi_init_struct.frame_size           = SPI_FRAMESIZE_8BIT;
    spi_init_struct.clock_polarity_phase = SPI_CK_PL_HIGH_PH_2EDGE;
    spi_init_struct.nss                  = SPI_NSS_SOFT;
    spi_init_struct.prescale             = SPI_PSC_2;
    spi_init_struct.endian               = SPI_ENDIAN_MSB;
    spi_init(SPI1, &spi_init_struct);
    spi_enable(SPI1);*/

    LCD_CS_HIGH();
    LCD_RST_HIGH();
    LCD_DC_DATA();
}

void lcd_reset(void)
{
    LCD_RST_HIGH();
    lcd_delay_ms(5);
    LCD_RST_LOW();
    lcd_delay_ms(2);
    LCD_RST_HIGH();
    lcd_delay_ms(120);
}

static void lcd_cmd_with_data(uint8_t cmd, const uint8_t *data, uint32_t len)
{
    LCD_CS_LOW();
    LCD_DC_CMD();
    soft_spi_write_byte(cmd);
    if ((data != NULL) && (len > 0U)) {
        LCD_DC_DATA();
        for (uint32_t i = 0U; i < len; i++) {
            soft_spi_write_byte(data[i]);
        }
    }
    LCD_CS_HIGH();
}

void lcd_write_cmd(uint8_t cmd)
{
    lcd_cmd_with_data(cmd, NULL, 0U);
}

void lcd_write_data(uint8_t data)
{
    LCD_CS_LOW();
    LCD_DC_DATA();
    soft_spi_write_byte(data);
    LCD_CS_HIGH();
}

void lcd_write_data_buf(const uint8_t *buf, uint32_t len)
{
    LCD_CS_LOW();
    LCD_DC_DATA();
    for (uint32_t i = 0U; i < len; i++) {
        soft_spi_write_byte(buf[i]);
    }
    LCD_CS_HIGH();
}

static void lcd_window_begin(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
    const uint8_t col[4] = {
        (uint8_t) (x0 >> 8), (uint8_t) (x0 & 0xFFU),
        (uint8_t) (x1 >> 8), (uint8_t) (x1 & 0xFFU)};
    const uint8_t row[4] = {
        (uint8_t) (y0 >> 8), (uint8_t) (y0 & 0xFFU),
        (uint8_t) (y1 >> 8), (uint8_t) (y1 & 0xFFU)};

    LCD_CS_LOW();

    LCD_DC_CMD();
    soft_spi_write_byte(0x2A);
    LCD_DC_DATA();
    for (uint32_t i = 0U; i < 4U; i++) {
        soft_spi_write_byte(col[i]);
    }

    LCD_DC_CMD();
    soft_spi_write_byte(0x2B);
    LCD_DC_DATA();
    for (uint32_t i = 0U; i < 4U; i++) {
        soft_spi_write_byte(row[i]);
    }

    LCD_DC_CMD();
    soft_spi_write_byte(0x2C);
    LCD_DC_DATA();
}

static void lcd_window_end(void)
{
    LCD_CS_HIGH();
}

void lcd_set_window(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
    lcd_window_begin(x0, y0, x1, y1);
    lcd_window_end();
}

void lcd_fill_color(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color)
{
    uint32_t pixels = (uint32_t) w * (uint32_t) h;
    uint8_t hi = (uint8_t) (color >> 8);
    uint8_t lo = (uint8_t) (color & 0xFFU);

    lcd_window_begin(x, y, (uint16_t) (x + w - 1U), (uint16_t) (y + h - 1U));
    for (uint32_t i = 0U; i < pixels; i++) {
        soft_spi_write_byte(hi);
        soft_spi_write_byte(lo);
    }
    lcd_window_end();
}

void lcd_fill_pixel(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint8_t *buf)
{
    uint32_t len = (uint32_t) w * (uint32_t) h * 2U;

    lcd_window_begin(x, y, (uint16_t) (x + w - 1U), (uint16_t) (y + h - 1U));
    for (uint32_t i = 0U; i < len; i++) {
        soft_spi_write_byte(buf[i]);
    }
    lcd_window_end();
}

void lcd_clear(uint16_t color)
{
    lcd_fill_color(0U, 0U, LCD_WIDTH, LCD_HEIGHT, color);
}

void lcd_blit_area(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1,
                   const uint8_t *buf, uint32_t len)
{
    lcd_window_begin(x0, y0, x1, y1);
    for (uint32_t i = 0U; i < len; i++) {
        soft_spi_write_byte(buf[i]);
    }
    lcd_window_end();
}

static void ili9341_init0(void)
{
    static const uint8_t pixfmt[] = {0x55};
    static const uint8_t madctl[] = {0x08};

    lcd_reset();

    lcd_write_cmd(0x01);
    lcd_delay_ms(10);

    lcd_cmd_with_data(0x36, madctl, 1U);
    lcd_cmd_with_data(0x3A, pixfmt, 1U);

    lcd_write_cmd(0x11);
    lcd_delay_ms(120);

    lcd_write_cmd(0x29);
    lcd_delay_ms(50);
}

static void ili9341_init(void)
{
    lcd_reset();

    // 软复位
    lcd_write_cmd(0x01);
    lcd_delay_ms(10);

    // ================== 官方初始化序列开始 ==================
    // 以下为扩展寄存器、电源、VCOM及Gamma等配置

    static const uint8_t dt_cb[] = {0x39, 0x2C, 0x00, 0x34, 0x02};
    lcd_cmd_with_data(0xCB, dt_cb, 5);

    static const uint8_t dt_cf[] = {0x00, 0xC1, 0x30};
    lcd_cmd_with_data(0xCF, dt_cf, 3);

    static const uint8_t dt_e8[] = {0x85, 0x00, 0x78};
    lcd_cmd_with_data(0xE8, dt_e8, 3);

    static const uint8_t dt_ea[] = {0x00, 0x00};
    lcd_cmd_with_data(0xEA, dt_ea, 2);

    static const uint8_t dt_ed[] = {0x64, 0x03, 0x12, 0x81};
    lcd_cmd_with_data(0xED, dt_ed, 4);

    static const uint8_t dt_f7[] = {0x20};
    lcd_cmd_with_data(0xF7, dt_f7, 1);

    static const uint8_t dt_c0[] = {0x23}; // Power control VRH[5:0]
    lcd_cmd_with_data(0xC0, dt_c0, 1);

    static const uint8_t dt_c1[] = {0x10}; // Power control SAP[2:0];BT[3:0]
    lcd_cmd_with_data(0xC1, dt_c1, 1);

    static const uint8_t dt_c5[] = {0x3E, 0x28}; // VCM control 1
    lcd_cmd_with_data(0xC5, dt_c5, 2);

    static const uint8_t dt_c7[] = {0x86}; // VCM control 2
    lcd_cmd_with_data(0xC7, dt_c7, 1);

    static const uint8_t dt_36[] = {0x88}; // Memory Access Control
    lcd_cmd_with_data(0x36, dt_36, 1);

    static const uint8_t dt_3a[] = {0x55}; // Pixel Format Set (16-bit/pixel)
    lcd_cmd_with_data(0x3A, dt_3a, 1);

    static const uint8_t dt_b1[] = {0x00, 0x18}; // Frame Rate Control
    lcd_cmd_with_data(0xB1, dt_b1, 2);

    static const uint8_t dt_b6[] = {0x08, 0x82, 0x27}; // Display Function Control
    lcd_cmd_with_data(0xB6, dt_b6, 3);

    static const uint8_t dt_f2[] = {0x00}; // 3Gamma Function Disable
    lcd_cmd_with_data(0xF2, dt_f2, 1);

    static const uint8_t dt_26[] = {0x01}; // Gamma curve selected
    lcd_cmd_with_data(0x26, dt_26, 1);

    static const uint8_t dt_e0[] = {0x0F, 0x31, 0x2B, 0x0C, 0x0E, 0x08, 0x4E, 0xF1, 0x37, 0x07, 0x10, 0x03, 0x0E, 0x09, 0x00};
    lcd_cmd_with_data(0xE0, dt_e0, 15); // Positive Gamma Correction

    static const uint8_t dt_e1[] = {0x00, 0x0E, 0x14, 0x03, 0x11, 0x07, 0x31, 0xC1, 0x48, 0x08, 0x0F, 0x0C, 0x31, 0x36, 0x0F};
    lcd_cmd_with_data(0xE1, dt_e1, 15); // Negative Gamma Correction

    // ================== 官方初始化序列结束 ==================

    // 退出睡眠模式
    lcd_write_cmd(0x11);
    lcd_delay_ms(120); // 必须等待 120ms 供电源电路稳定

    // 开启显示
    lcd_write_cmd(0x29);

    // 发送 0x2C 指令，告诉芯片接下来准备接收像素画面数据
    lcd_write_cmd(0x2C);
}

void lcd_init(void)
{
    lcd_gpio_init();
    ili9341_init();
}
