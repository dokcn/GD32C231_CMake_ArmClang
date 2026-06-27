/**
 * @file lcd_spi.h
 * @brief SPI LCD driver for GD32C231 (ILI9341, 240x320, 2.2 inch)
 *
 * Pin Assignment (SPI1):
 *   PB13 - SPI1_SCK  (SCL)
 *   PB14 - SPI1_MISO (not used, TX only)
 *   PB15 - SPI1_MOSI (SDA)
 *   PA8  - GPIO CS   (Chip Select)
 *   PC7  - GPIO DC   (Data/Command)
 *   PC6  - GPIO RST  (Reset)
 *   Backlight connected to VDD directly, no pin control.
 */

#ifndef LCD_SPI_H
#define LCD_SPI_H

#include <stdint.h>

/* LCD resolution */
#define LCD_WIDTH   240
#define LCD_HEIGHT  320

/* Pin definitions */
#define LCD_SPI_PERIPH      SPI1
#define LCD_SPI_CLK_RCU     RCU_SPI1

/* CS pin - PA8 */
#define LCD_CS_PORT         GPIOA
#define LCD_CS_PIN          GPIO_PIN_8
#define LCD_CS_RCU          RCU_GPIOA

/* DC pin - PC7 */
#define LCD_DC_PORT         GPIOC
#define LCD_DC_PIN          GPIO_PIN_7
#define LCD_DC_RCU          RCU_GPIOC

/* RST pin - PC6 */
#define LCD_RST_PORT        GPIOC
#define LCD_RST_PIN         GPIO_PIN_6
#define LCD_RST_RCU         RCU_GPIOC

/* No backlight pin - directly connected to VDD */

/* SPI GPIO - PB13(SCK), PB14(MISO), PB15(MOSI) */
#define LCD_SPI_GPIO_PORT   GPIOB
#define LCD_SPI_SCK_PIN     GPIO_PIN_13
#define LCD_SPI_MISO_PIN    GPIO_PIN_14
#define LCD_SPI_MOSI_PIN    GPIO_PIN_15
#define LCD_SPI_GPIO_RCU    RCU_GPIOB
#define LCD_SPI_GPIO_AF     GPIO_AF_0

/* CS control macros */
#define LCD_CS_LOW()        gpio_bit_reset(LCD_CS_PORT, LCD_CS_PIN)
#define LCD_CS_HIGH()       gpio_bit_set(LCD_CS_PORT, LCD_CS_PIN)

/* DC control macros */
#define LCD_DC_CMD()        gpio_bit_reset(LCD_DC_PORT, LCD_DC_PIN)
#define LCD_DC_DATA()       gpio_bit_set(LCD_DC_PORT, LCD_DC_PIN)

/* RST control macros */
#define LCD_RST_LOW()       gpio_bit_reset(LCD_RST_PORT, LCD_RST_PIN)
#define LCD_RST_HIGH()      gpio_bit_set(LCD_RST_PORT, LCD_RST_PIN)

/* Backlight: connected to VDD, no pin control needed */

/* Function declarations */
void lcd_init(void);
void lcd_reset(void);
void lcd_write_cmd(uint8_t cmd);
void lcd_write_data(uint8_t data);
void lcd_write_data_buf(const uint8_t *buf, uint32_t len);
void lcd_set_window(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
void lcd_fill_color(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
void lcd_fill_pixel(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint8_t *buf);
void lcd_clear(uint16_t color);
void lcd_blit_area(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1,
                   const uint8_t *buf, uint32_t len);
void lcd_delay_ms(uint32_t ms);

#endif /* LCD_SPI_H */
