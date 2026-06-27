/**
 * @file main.c
 * @brief LVGL simple demo: label + circle
 */

#include "gd32c2x1.h"
#include "gd32c2x1_gpio.h"
#include "gd32c2x1_rcu.h"
#include "lcd_spi.h"
#include "lv_port_disp.h"
#include "lvgl/lvgl.h"
#include "systick.h"

static void led_init(void)
{
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_GPIOB);
    rcu_periph_clock_enable(RCU_GPIOD);

    gpio_mode_set(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_15);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_LEVEL_1, GPIO_PIN_15);

    gpio_mode_set(GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_1 | GPIO_PIN_3);
    gpio_output_options_set(GPIOD, GPIO_OTYPE_PP, GPIO_OSPEED_LEVEL_1, GPIO_PIN_1 | GPIO_PIN_3);

    gpio_mode_set(GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_4);
    gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_LEVEL_1, GPIO_PIN_4);

    gpio_bit_set(GPIOA, GPIO_PIN_15);
    gpio_bit_set(GPIOD, GPIO_PIN_1 | GPIO_PIN_3);
    gpio_bit_set(GPIOB, GPIO_PIN_4);
}

int main(void)
{
    systick_config();
    led_init();

    lv_init();
    lv_port_disp_init();

    lv_obj_set_style_bg_color(lv_scr_act(), lv_color_black(), LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t *label = lv_label_create(lv_scr_act());

    /* 2. 设置文本内容 */
    lv_label_set_text(label, "Hello！ LVGL!\nLabel Only Demo");

    /* 3. 设置对齐方式（居中） */
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);

    /* 4. (可选) 设置样式 - 字体颜色、大小等 */
    lv_obj_set_style_text_color(label, lv_color_hex(0xFF0000), LV_PART_MAIN);
    lv_obj_set_style_text_font(label, &lv_font_montserrat_10, LV_PART_MAIN);

    while (1) {
        lv_timer_handler();
        lcd_delay_ms(5);
    }
}
