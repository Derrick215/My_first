/*  Copyright (s) 2019 深圳百问网科技有限公司
 *  All rights reserved
 * 
 * 文件名称：driver_timer.h
 * 摘要：
 *  
 * 修改历史     版本号       Author       修改内容
 *--------------------------------------------------
 * 2020.6.6      v01        百问科技      创建文件
 *--------------------------------------------------
*/
#ifndef __DRIVER_TIMER_H
#define __DRIVER_TIMER_H

#include "stm32f1xx_hal.h"


#define TIMx                     TIM3
#define TIMx_IRQn                TIM3_IRQn
#define TIMx_IRQHandler          TIM3_IRQHandler
#define TIM_PRESCALER            ((36*10)-1)      // 频率200kHz
#define TIM_PERIOD               (2000-1)         // 周期2000/f_tim = 10ms
#define LCD_BRT                  (0)              // 占空比

#define TIM_LCD_PWM_PIN          GPIO_PIN_6
#define TIM_LCD_PWM_PORT         GPIOC
#define TIM_LCD_PWM_CHANNEL      TIM_CHANNEL_1

#define TIM_PWM_CLK_EN()        __HAL_RCC_TIM3_CLK_ENABLE()
#define TIM_PWM_GPIO_CLK_EN()   __HAL_RCC_GPIOC_CLK_ENABLE()

/*
 *  函数名：void TimerInit(void)
 *  输入参数：
 *  输出参数：无
 *  返回值：无
 *  函数作用：初始化定时器
*/
extern void TimerInit(void);

/*
 *  函数名：void us_timer_delay(uint16_t t)
 *  输入参数：t-延时时间us
 *  输出参数：无
 *  返回值：无
 *  函数作用：定时器实现的延时函数，延时时间为t us，为了缩短时间，函数体使用寄存器操作，用户可对照手册查看每个寄存器每一位的意义
*/
extern void us_timer_delay(uint16_t t);

/*
 *  函数名：void ms_timer_delay(uint16_t t)
 *  输入参数：t-延时时间ms
 *  输出参数：无
 *  返回值：无
 *  函数作用：定时器实现的延时函数，延时时间为t ms
*/
extern void ms_timer_delay(uint16_t t);


extern TIM_HandleTypeDef hpwm;
extern uint8_t lcd_brt;
/*
 *  函数名：void TimerPWMInit(void)
 *  输入参数：无
 *  输出参数：无
 *  返回值：无
 *  函数作用：初始化定时器
*/
extern void TimerPWMInit(void);
extern void LCD_Backlight_Enable(void);

#endif
