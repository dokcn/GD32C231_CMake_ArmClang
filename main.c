#include "gd32c2x1.h"

static void delay(volatile uint32_t count)
{
    while (count-- != 0U) {
        __NOP();
    }
}

int main(void)
{
    while (1) {
        delay(48000U);
    }
}
