// Copyright 2017 ETH Zurich and University of Bologna.
// Copyright and related rights are licensed under the Solderpad Hardware
// License, Version 0.51 (the “License”); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
// http://solderpad.org/licenses/SHL-0.51. Unless required by applicable law
// or agreed to in writing, software, hardware and materials distributed under
// this License is distributed on an “AS IS” BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.

#define  L1_TEST_SUBMODULE
//#include "platform_common.h"
#include <gpio.h>
#include <stdio.h>
#include <sectdefs.h>
#include "uc_event.h"
#include "uc_int.h"
//#include "vsi.h"
//#include "scheduler_frame_for_gpio.h"
extern void scheduler_tick_notify(void);
void gpio_set_pin_function(int pinnumber, int function)
{
    volatile int old_function;

    old_function = *(volatile int *)(SOC_CTRL_PADFUN);
    old_function = old_function & (~(1 << pinnumber));
    old_function = old_function | (function << pinnumber);
    *(volatile int *)(SOC_CTRL_PADFUN) = old_function;
}

int gpio_get_pin_function(int pinnumber)
{
    volatile int old_function;
    old_function = *(volatile int *)(SOC_CTRL_PADFUN);
    old_function = (old_function >> pinnumber & 0x01);
    return old_function;
}

void gpio_set_pin_direction(int pinnumber, int direction)
{
    volatile int old_dir;
    old_dir = *(volatile int *)(GPIO_REG_PADDIR);
    if (direction == 0)
        old_dir &= ~(1 << pinnumber);
    else
        old_dir |= 1 << pinnumber;
    *(volatile int *)(GPIO_REG_PADDIR) = old_dir;
}

int gpio_get_pin_direction(int pinnumber)
{
    volatile int old_dir;
    old_dir = *(volatile int *)(GPIO_REG_PADDIR);
    old_dir = (old_dir >> (pinnumber)&0x01);
    return old_dir;
}

void gpio_set_pin_value(int pinnumber, int value)
{
    volatile int v;
    v = *(volatile int *)(GPIO_REG_PADOUT);
    if (value == 0)
        v &= ~(1 << pinnumber);
    else
        v |= 1 << pinnumber;
    *(volatile int *)(GPIO_REG_PADOUT) = v;
}

int gpio_get_pin_value(int pinnumber)
{
    volatile int v;
    v = *(volatile int *)(GPIO_REG_PADIN);
    v = (v >> pinnumber) & 0x01;
    return v;
}

void gpio_set_pin_value_reverse(int pinnumber)
{
    static int last_value = 0;
    if (last_value)
    {
        gpio_set_pin_value(pinnumber, 0);
        last_value = 0;
    }
    else
    {
        gpio_set_pin_value(pinnumber, 1);
        last_value = 1;
    }
}

void gpio_set_pin_irq_en(int pinnumber, int enable)
{
    int v;
    v = *(volatile int *)(GPIO_REG_INTEN);
    if (enable == 0)
        v &= ~(1 << pinnumber);
    else
        v |= 1 << pinnumber;
    *(volatile int *)(GPIO_REG_INTEN) = v;
}

void gpio_set_pin_irq_type(int pinnumber, int type)
{
    int type0;
    int type1;

    type0 = *(volatile int *)(GPIO_REG_INTTYPE0);
    type1 = *(volatile int *)(GPIO_REG_INTTYPE1);

    if ((type & 0x1) == 0)
        type0 &= ~(1 << pinnumber);
    else
        type0 |= 1 << pinnumber;

    if ((type & 0x2) == 0)
        type1 &= ~(1 << pinnumber);
    else
        type1 |= 1 << pinnumber;

    *(volatile int *)(GPIO_REG_INTTYPE0) = type0;
    *(volatile int *)(GPIO_REG_INTTYPE1) = type1;
}

void gpio_set_pin_mux(GPIO_CFG_TypeDef *GPIO_CFG, GPIO_PIN pin, GPIO_FUNCTION func)
{
    CHECK_PARAM(PARAM_GPIO_CFG(GPIO_CFG));
    CHECK_PARAM(PARAM_GPIO_PIN(pin));
    CHECK_PARAM(PARAM_GPIO_FUNC(func));

    //set pin mux
    if (func == GPIO_FUNC_1)
    {
        GPIO_CFG->PADMUX |= (1 << pin);
        GPIO_CFG->PADMUX1 &= ~(1 << pin);
    }
    else if (func == GPIO_FUNC_2)
    {
        GPIO_CFG->PADMUX &= ~(1 << pin);
        GPIO_CFG->PADMUX1 |= (1 << pin);
    }
    else
    {
        GPIO_CFG->PADMUX &= ~(1 << pin);
        GPIO_CFG->PADMUX1 &= ~(1 << pin);
    }
}

GPIO_FUNCTION gpio_get_pin_mux(GPIO_CFG_TypeDef *GPIO_CFG, GPIO_PIN pin)
{
    CHECK_PARAM(PARAM_GPIO_CFG(GPIO_CFG));
    CHECK_PARAM(PARAM_GPIO_PIN(pin));

    if ((GPIO_CFG->PADMUX & (1 << pin)) != 0)
    {
        return GPIO_FUNC_1;
    }
    else if ((GPIO_CFG->PADMUX1 & (1 << pin)) != 0)
    {
        return GPIO_FUNC_2;
    }
    else
    {
        return GPIO_FUNC_0;
    }
}

void gpio_set_pin_pupd(GPIO_CFG_TypeDef *GPIO_CFG, GPIO_PIN pin, GPIO_PUPD pupd)
{
    CHECK_PARAM(PARAM_GPIO_CFG(GPIO_CFG));
    CHECK_PARAM(PARAM_GPIO_PIN(pin));
    CHECK_PARAM(PARAM_GPIO_PUPD(pupd));

    //set pin pupd
    if(pupd == GPIO_PUPD_UP)
        GPIO_CFG->PADCFG |= (1 << pin);
    else
        GPIO_CFG->PADCFG &= ~(1 << pin);
}

void gpio_set_init(uint8_t pin_number, uint8_t en_func, uint8_t en_pullup)
{
    volatile GPIO_CFG_TypeDef *GPIO_CFG = (GPIO_CFG_TypeDef *)SOC_CTRL_BASE_ADDR;
    if (pin_number > 31)
    {
        return;
    }

    //set pin mux
    if (en_func)
        GPIO_CFG->PADMUX |= (1 << pin_number);
    else
        GPIO_CFG->PADMUX &= ~(1 << pin_number);

    //set pin status
    if (en_pullup)
        GPIO_CFG->PADCFG |= (1 << pin_number);
    else
        GPIO_CFG->PADCFG &= ~(1 << pin_number);
}

int gpio_get_irq_status()
{
    return *(volatile int *)(GPIO_REG_INTSTATUS);
}

#define PIN_NUMBER 11
void gpio_init(void)
{
    int_enable();
    gpio_set_pin_direction(PIN_NUMBER, GPIO_DIR_IN);
    gpio_set_pin_irq_type(PIN_NUMBER, GPIO_IT_RISE_EDGE);
    gpio_set_pin_irq_en(PIN_NUMBER, 1);

    enable_event_iqr(GPIO_INT_ID);
    return;
}

void gpio_deinit(void)
{
    int_disable();
    gpio_set_pin_irq_en(PIN_NUMBER, 0);
    disable_event_iqr(GPIO_INT_ID);
    return;
}

//unsigned int g_gpio_count = 0;
void ISR_GPIO(void)
{
    int temp = *(volatile int *)(GPIO_REG_INTSTATUS);

    if (0 != ((1 << PIN_NUMBER) & temp))
    {
        //        g_gpio_count = *(u32_t*)0x3b0014;
        scheduler_tick_notify();
    }

    ICP |= 1 << GPIO_INT_ID;
    //enable_event_iqr(GPIO_INT_ID);

    return;
}

/* set ldo to 3.3v */
void soc_hw_ldo_on(void)
{
    unsigned int ldo_re = 0xe00000;
    int a = (*((volatile unsigned int *)(0x1a10422c)));
    int b = (a | ldo_re);
    *(volatile unsigned int *)(0x1a10422c) = b;

    volatile unsigned int *pad_mux = (unsigned int *)0x1a107000;

    *pad_mux &= (~(0x1));

    gpio_set_pin_value(GPIO_PIN_26, 1);
    gpio_set_pin_value(GPIO_PIN_6, 1);
}

#define GPIO_8288_TO_8088_PIN_NUMBER 12

void gpoi_8088_to_8288_change_value()
{
    gpio_set_pin_value(GPIO_8288_TO_8088_PIN_NUMBER, 1);
    // rt_thread_mdelay(10);
    gpio_set_pin_value(GPIO_8288_TO_8088_PIN_NUMBER, 0);

    // u32_t count = os_getTimeStamp();
    // u32_t distance_count=0;
    // if(g_gpio_record_count < count)
    // {
    //    distance_count = count - g_gpio_record_count;
    // }
    // else
    // {
    //     distance_count = (~0 - g_gpio_record_count) + count;
    // }
    // g_gpio_record_count = count;
    // TRACE_PRINTF("<<<<<timerA dfe count [%u]\n",distance_count);
}
