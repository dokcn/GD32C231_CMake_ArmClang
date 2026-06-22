#include "gd32c2x1.h"

static void delay(volatile uint32_t count)
{
    while (count-- != 0U) {
        __NOP();
    }
}

int main(void)
{
    rcu_periph_clock_enable(RCU_GPIOD);

    gpio_mode_set(GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_3);
    gpio_output_options_set(GPIOD, GPIO_OTYPE_PP, GPIO_OSPEED_LEVEL_1, GPIO_PIN_3);

    gpio_bit_set(GPIOD, GPIO_PIN_3);  /* 常亮 */

    while (1) {}
}
