/**
 * @file systick.c
 * @brief SysTick configuration using direct register access
 */

#include "systick.h"
#include "gd32c2x1.h"

volatile static uint32_t delay;
volatile static uint32_t sys_tick_ms = 0;

/* SystemCoreClock is defined in system_gd32c2x1.c */
extern uint32_t SystemCoreClock;

uint32_t get_tick_ms(void)
{
    return sys_tick_ms;
}

void systick_config(void)
{
    /* Configure SysTick for 1ms interrupts */
    if (SysTick_Config(SystemCoreClock / 1000U)) {
        while (1) {
        }
    }
    NVIC_SetPriority(SysTick_IRQn, 0x00U);
}

void delay_1ms(uint32_t count)
{
    delay = count;
    while (delay != 0U) {
    }
}

void delay_decrement(void)
{
    if (delay != 0U) {
        delay--;
    }
}

/* SysTick_Handler */
void SysTick_Handler(void)
{
    delay_decrement();
    sys_tick_ms++;
}
