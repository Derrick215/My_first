/*  Copyright (s) 2019 ���ڰ������Ƽ����޹�˾
 *  All rights reserved
 * 
 * �ļ����ƣ�driver_timer.c
 * ժҪ��
 *  
 * �޸���ʷ     �汾��       Author       �޸�����
 *--------------------------------------------------
 * 2020.6.6      v01        ���ʿƼ�      �����ļ�
 *--------------------------------------------------
*/
#include "main.h"
#include "driver_timer.h"

/*
 * ����ȫ�ֱ���
*/
TIM_HandleTypeDef htim;
TIM_HandleTypeDef hpwm;
uint8_t lcd_brt;


/*
 *  ��������void TimerInit(void)
 *  ���������
 *  �����������
 *  ����ֵ����
 *  �������ã���ʼ����ʱ��
*/
void TimerInit(void)
{
    TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  
	// ��ʱ��������������
    htim.Instance               = TIM2;                             // ʹ�ö�ʱ��2
    htim.Init.Prescaler         = 72-1;                             // Ԥ��Ƶϵ��PSC=72-1(��Χ��0~0xFFFF)
	// 72MHz����72��Ƶ��,��ʱ��ʱ��Ϊ1MHz,����ʱ������1�ε�ʱ�䣬�պ�Ϊ1us
	htim.Init.CounterMode       = TIM_COUNTERMODE_UP;               // ���ϼ���
    htim.Init.Period            = 0;                                // �Զ�װ����ARR��ֵ(��Χ��0~0xFFFF)
	htim.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;           // ʱ�ӷ�Ƶ(������������)
	//htim.Init.RepetitionCounter = 0;                              // �ظ�������ֵ,�������ڸ߼���ʱ��
    htim.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;   // ���Զ�����װ��
    
    if (HAL_TIM_Base_Init(&htim) != HAL_OK)    
    {
        Error_Handler();
    }
	
	// ��ʱ��ʱ��Դѡ��
    sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;    // ѡ���ڲ�ʱ����Ϊ��ʱ��ʱ��Դ
    if (HAL_TIM_ConfigClockSource(&htim, &sClockSourceConfig) != HAL_OK)
    {
        Error_Handler();
    }
}
/*
 *  ��������void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
 *  ���������htim-TIM���
 *  �����������
 *  ����ֵ����
 *  �������ã�ʹ��TIMʱ��
*/
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
	if(htim->Instance==TIM2) 
    {
		__HAL_RCC_TIM2_CLK_ENABLE();  // ʹ��TIM2��ʱ��
	}
}

/*
 *  ��������void us_timer_delay(uint16_t t)
 *  ���������t-��ʱʱ��us ��Χ-0~65535us
 *  �����������
 *  ����ֵ����
 *  �������ã���ʱ��ʵ�ֵ���ʱ��������ʱʱ��Ϊt us��Ϊ������ʱ�䣬������ʹ�üĴ�������
*/
void us_timer_delay(uint16_t t)
{
    uint16_t counter = 0;
	__HAL_TIM_SET_AUTORELOAD(&htim, t);		     // ���ö�ʱ���Զ�����ֵ
	__HAL_TIM_SET_COUNTER(&htim, counter);       // ���ö�ʱ����ʼֵ
	HAL_TIM_Base_Start(&htim);                   // ������ʱ��
	while(counter != t)                          // ֱ����ʱ��������0������t����ѭ��,�պ�t us
	{
		counter = __HAL_TIM_GET_COUNTER(&htim);  // ��ȡ��ʱ����ǰ����
	}
	HAL_TIM_Base_Stop(&htim);                    // ֹͣ��ʱ��
}

/*
 *  ��������void ms_timer_delay(uint16_t t)
 *  ���������t-��ʱʱ��ms ��Χ-0~65535ms
 *  �����������
 *  ����ֵ����
 *  �������ã���ʱ��ʵ�ֵ���ʱ��������ʱʱ��Ϊt ms
*/
void ms_timer_delay(uint16_t t)
{
	int i = 0;
	
	for(; i<t; i++)
	{
		us_timer_delay(1000);
	}
}

/*
 *  ��������void TimerPWMInit(void)
 *  �����������
 *  �����������
 *  ����ֵ����
 *  �������ã���ʼ����ʱ�������Ƶ��1Hz��ռ�ձ�50%��PWMN��
*/
void TimerPWMInit(void)
{
    TIM_ClockConfigTypeDef sClockSourceConfig;
    TIM_OC_InitTypeDef sConfig;
    
	// ��ʱ��������������
    hpwm.Instance               = TIMx;                           // ָ����ʱ��TIM3
    hpwm.Init.Prescaler         = TIM_PRESCALER;                  // Ԥ��Ƶϵ��PSC=360-1
	hpwm.Init.CounterMode       = TIM_COUNTERMODE_UP;             // ���ϼ���
    hpwm.Init.Period            = TIM_PERIOD;                     // �Զ�װ����ARR��ֵ,ARR=2000-1
    // 72MHz����360��Ƶ��,��ʱ��ʱ��Ϊ200KHz,��������ÿ���5us����һ��,��0������ARR,����10ms
                                             
    hpwm.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;         // ��ʱ��ʱ�Ӳ���HCLK��Ƶ
    //hpwm.Init.RepetitionCounter = 0;                            // �ظ�������ֵ,�������ڸ߼���ʱ��
    hpwm.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE; // ���Զ�����װ��
    
    // ��TIM3��PWMģʽ��ʼ��
    if (HAL_TIM_PWM_Init(&hpwm) != HAL_OK)
    {
        Error_Handler();
    }
	
	// ��ʱ��ʱ��Դѡ��
    sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL; // ѡ���ڲ�ʱ����Ϊ��ʱ��ʱ��Դ
    HAL_TIM_ConfigClockSource(&hpwm, &sClockSourceConfig);
    
	// ����PWM�����ͨ������
    sConfig.OCMode       = TIM_OCMODE_PWM1;               // PWM���������ģʽ:PWM1 ������Ϊ��,CCR<CNT,����͵�ƽ,��֮�ߵ�ƽ
    sConfig.OCPolarity   = TIM_OCPOLARITY_LOW;            // ���ü���Ϊ��
	sConfig.OCFastMode   = TIM_OCFAST_DISABLE;            // ����ȽϿ���ʹ�ܽ�ֹ(����PWM1��PWM2������)
	sConfig.Pulse = LCD_BRT;                              // ��PWM1ģʽ��,ͨ��1(LCD_PWM)ռ�ձ�
    if (HAL_TIM_PWM_ConfigChannel(&hpwm, &sConfig, TIM_LCD_PWM_CHANNEL) != HAL_OK)
    {
        Error_Handler();
    }
}

/*
 *  ��������void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
 *  ���������htim-TIM���
 *  �����������
 *  ����ֵ����
 *  �������ã�HAL_TIM_PWM_Init�ص�Ӳ����ʼ��
*/
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
    GPIO_InitTypeDef   GPIO_InitStruct;
    
    TIM_PWM_CLK_EN();                                    // PWM���漰��TIM3ʱ��ʹ��
    TIM_PWM_GPIO_CLK_EN();                               // PWM���漰��GPIOCʱ��ʹ��
    __HAL_RCC_AFIO_CLK_ENABLE();                         // ��ӳ���漰ʱ��ʹ��
    //__HAL_AFIO_REMAP_TIM3_PARTIAL();                   // ����TIM3��ӳ��    
	__HAL_AFIO_REMAP_TIM3_ENABLE();	
    
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;              // �����������
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    
    GPIO_InitStruct.Pin = TIM_LCD_PWM_PIN;
    HAL_GPIO_Init(TIM_LCD_PWM_PORT, &GPIO_InitStruct);   // ��ʼ��LCD_PWM����

    HAL_NVIC_SetPriority(TIMx_IRQn, 0, 0);               // ���ö�ʱ���ж����ȼ�
    HAL_NVIC_EnableIRQ(TIMx_IRQn);                       // ʹ��TIM3�ж�
}

/*
 *  ��������void TIM3_IRQHandler(void)
 *  �����������
 *  �����������
 *  ����ֵ����
 *  �������ã�TIM3�жϵ��жϴ�����
*/
void TIM3_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&hpwm);
}

/*
 *  ��������void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
 *  ���������htim-TIM���
 *  �����������
 *  ����ֵ����
 *  �������ã�TIM�жϻص����ڸ��º���
*/
void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
    if(htim->Instance == TIMx)
    {
        __HAL_TIM_SET_COMPARE(&hpwm, TIM_LCD_PWM_CHANNEL, lcd_brt *20);   
    }
}
