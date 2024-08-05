/*  Copyright (s) 2019 ���ڰ������Ƽ����޹�˾
 *  All rights reserved
 * 
 * �ļ����ƣ�driver_timer.h
 * ժҪ��
 *  
 * �޸���ʷ     �汾��       Author       �޸�����
 *--------------------------------------------------
 * 2020.6.6      v01        ���ʿƼ�      �����ļ�
 *--------------------------------------------------
*/
#ifndef __DRIVER_TIMER_H
#define __DRIVER_TIMER_H

#include "stm32f1xx_hal.h"


#define TIMx                     TIM3
#define TIMx_IRQn                TIM3_IRQn
#define TIMx_IRQHandler          TIM3_IRQHandler
#define TIM_PRESCALER            ((36*10)-1)      // Ƶ��200kHz
#define TIM_PERIOD               (2000-1)         // ����2000/f_tim = 10ms
#define LCD_BRT                  (0)              // ռ�ձ�

#define TIM_LCD_PWM_PIN          GPIO_PIN_6
#define TIM_LCD_PWM_PORT         GPIOC
#define TIM_LCD_PWM_CHANNEL      TIM_CHANNEL_1

#define TIM_PWM_CLK_EN()        __HAL_RCC_TIM3_CLK_ENABLE()
#define TIM_PWM_GPIO_CLK_EN()   __HAL_RCC_GPIOC_CLK_ENABLE()

/*
 *  ��������void TimerInit(void)
 *  ���������
 *  �����������
 *  ����ֵ����
 *  �������ã���ʼ����ʱ��
*/
extern void TimerInit(void);

/*
 *  ��������void us_timer_delay(uint16_t t)
 *  ���������t-��ʱʱ��us
 *  �����������
 *  ����ֵ����
 *  �������ã���ʱ��ʵ�ֵ���ʱ��������ʱʱ��Ϊt us��Ϊ������ʱ�䣬������ʹ�üĴ����������û��ɶ����ֲ�鿴ÿ���Ĵ���ÿһλ������
*/
extern void us_timer_delay(uint16_t t);

/*
 *  ��������void ms_timer_delay(uint16_t t)
 *  ���������t-��ʱʱ��ms
 *  �����������
 *  ����ֵ����
 *  �������ã���ʱ��ʵ�ֵ���ʱ��������ʱʱ��Ϊt ms
*/
extern void ms_timer_delay(uint16_t t);


extern TIM_HandleTypeDef hpwm;
extern uint8_t lcd_brt;
/*
 *  ��������void TimerPWMInit(void)
 *  �����������
 *  �����������
 *  ����ֵ����
 *  �������ã���ʼ����ʱ��
*/
extern void TimerPWMInit(void);
extern void LCD_Backlight_Enable(void);

#endif
