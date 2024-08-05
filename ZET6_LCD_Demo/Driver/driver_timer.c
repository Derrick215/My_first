/*  Copyright (s) 2019 深圳百问网科技有限公司
 *  All rights reserved
 * 
 * 文件名称：driver_timer.c
 * 摘要：
 *  
 * 修改历史     版本号       Author       修改内容
 *--------------------------------------------------
 * 2020.6.6      v01        百问科技      创建文件
 *--------------------------------------------------
*/
#include "main.h"
#include "driver_timer.h"

/*
 * 定义全局变量
*/
TIM_HandleTypeDef htim;
TIM_HandleTypeDef hpwm;
uint8_t lcd_brt;


/*
 *  函数名：void TimerInit(void)
 *  输入参数：
 *  输出参数：无
 *  返回值：无
 *  函数作用：初始化定时器
*/
void TimerInit(void)
{
    TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  
	// 定时器基本功能配置
    htim.Instance               = TIM2;                             // 使用定时器2
    htim.Init.Prescaler         = 72-1;                             // 预分频系数PSC=72-1(范围：0~0xFFFF)
	// 72MHz经过72分频后,定时器时钟为1MHz,即定时器计数1次的时间，刚好为1us
	htim.Init.CounterMode       = TIM_COUNTERMODE_UP;               // 向上计数
    htim.Init.Period            = 0;                                // 自动装载器ARR的值(范围：0~0xFFFF)
	htim.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;           // 时钟分频(与输入采样相关)
	//htim.Init.RepetitionCounter = 0;                              // 重复计数器值,仅存在于高级定时器
    htim.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;   // 不自动重新装载
    
    if (HAL_TIM_Base_Init(&htim) != HAL_OK)    
    {
        Error_Handler();
    }
	
	// 定时器时钟源选择
    sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;    // 选用内部时钟作为定时器时钟源
    if (HAL_TIM_ConfigClockSource(&htim, &sClockSourceConfig) != HAL_OK)
    {
        Error_Handler();
    }
}
/*
 *  函数名：void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
 *  输入参数：htim-TIM句柄
 *  输出参数：无
 *  返回值：无
 *  函数作用：使能TIM时钟
*/
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
	if(htim->Instance==TIM2) 
    {
		__HAL_RCC_TIM2_CLK_ENABLE();  // 使能TIM2的时钟
	}
}

/*
 *  函数名：void us_timer_delay(uint16_t t)
 *  输入参数：t-延时时间us 范围-0~65535us
 *  输出参数：无
 *  返回值：无
 *  函数作用：定时器实现的延时函数，延时时间为t us，为了缩短时间，函数体使用寄存器操作
*/
void us_timer_delay(uint16_t t)
{
    uint16_t counter = 0;
	__HAL_TIM_SET_AUTORELOAD(&htim, t);		     // 设置定时器自动加载值
	__HAL_TIM_SET_COUNTER(&htim, counter);       // 设置定时器初始值
	HAL_TIM_Base_Start(&htim);                   // 启动定时器
	while(counter != t)                          // 直到定时器计数从0计数到t结束循环,刚好t us
	{
		counter = __HAL_TIM_GET_COUNTER(&htim);  // 获取定时器当前计数
	}
	HAL_TIM_Base_Stop(&htim);                    // 停止定时器
}

/*
 *  函数名：void ms_timer_delay(uint16_t t)
 *  输入参数：t-延时时间ms 范围-0~65535ms
 *  输出参数：无
 *  返回值：无
 *  函数作用：定时器实现的延时函数，延时时间为t ms
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
 *  函数名：void TimerPWMInit(void)
 *  输入参数：无
 *  输出参数：无
 *  返回值：无
 *  函数作用：初始化定时器，输出频率1Hz，占空比50%的PWMN波
*/
void TimerPWMInit(void)
{
    TIM_ClockConfigTypeDef sClockSourceConfig;
    TIM_OC_InitTypeDef sConfig;
    
	// 定时器基本功能配置
    hpwm.Instance               = TIMx;                           // 指定定时器TIM3
    hpwm.Init.Prescaler         = TIM_PRESCALER;                  // 预分频系数PSC=360-1
	hpwm.Init.CounterMode       = TIM_COUNTERMODE_UP;             // 向上计数
    hpwm.Init.Period            = TIM_PERIOD;                     // 自动装载器ARR的值,ARR=2000-1
    // 72MHz经过360分频后,定时器时钟为200KHz,即计数器每间隔5us计数一次,从0计数到ARR,经历10ms
                                             
    hpwm.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;         // 定时器时钟不从HCLK分频
    //hpwm.Init.RepetitionCounter = 0;                            // 重复计数器值,仅存在于高级定时器
    hpwm.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE; // 不自动重新装载
    
    // 将TIM3按PWM模式初始化
    if (HAL_TIM_PWM_Init(&hpwm) != HAL_OK)
    {
        Error_Handler();
    }
	
	// 定时器时钟源选择
    sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL; // 选用内部时钟作为定时器时钟源
    HAL_TIM_ConfigClockSource(&hpwm, &sClockSourceConfig);
    
	// 配置PWM的输出通道参数
    sConfig.OCMode       = TIM_OCMODE_PWM1;               // PWM输出的两种模式:PWM1 当极性为低,CCR<CNT,输出低电平,反之高电平
    sConfig.OCPolarity   = TIM_OCPOLARITY_LOW;            // 设置极性为低
	sConfig.OCFastMode   = TIM_OCFAST_DISABLE;            // 输出比较快速使能禁止(仅在PWM1和PWM2可设置)
	sConfig.Pulse = LCD_BRT;                              // 在PWM1模式下,通道1(LCD_PWM)占空比
    if (HAL_TIM_PWM_ConfigChannel(&hpwm, &sConfig, TIM_LCD_PWM_CHANNEL) != HAL_OK)
    {
        Error_Handler();
    }
}

/*
 *  函数名：void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
 *  输入参数：htim-TIM句柄
 *  输出参数：无
 *  返回值：无
 *  函数作用：HAL_TIM_PWM_Init回调硬件初始化
*/
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
    GPIO_InitTypeDef   GPIO_InitStruct;
    
    TIM_PWM_CLK_EN();                                    // PWM所涉及的TIM3时钟使能
    TIM_PWM_GPIO_CLK_EN();                               // PWM所涉及的GPIOC时钟使能
    __HAL_RCC_AFIO_CLK_ENABLE();                         // 重映射涉及时钟使能
    //__HAL_AFIO_REMAP_TIM3_PARTIAL();                   // 启用TIM3重映射    
	__HAL_AFIO_REMAP_TIM3_ENABLE();	
    
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;              // 复用推挽输出
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    
    GPIO_InitStruct.Pin = TIM_LCD_PWM_PIN;
    HAL_GPIO_Init(TIM_LCD_PWM_PORT, &GPIO_InitStruct);   // 初始化LCD_PWM引脚

    HAL_NVIC_SetPriority(TIMx_IRQn, 0, 0);               // 配置定时器中断优先级
    HAL_NVIC_EnableIRQ(TIMx_IRQn);                       // 使能TIM3中断
}

/*
 *  函数名：void TIM3_IRQHandler(void)
 *  输入参数：无
 *  输出参数：无
 *  返回值：无
 *  函数作用：TIM3中断的中断处理函数
*/
void TIM3_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&hpwm);
}

/*
 *  函数名：void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
 *  输入参数：htim-TIM句柄
 *  输出参数：无
 *  返回值：无
 *  函数作用：TIM中断回调周期更新函数
*/
void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
    if(htim->Instance == TIMx)
    {
        __HAL_TIM_SET_COMPARE(&hpwm, TIM_LCD_PWM_CHANNEL, lcd_brt *20);   
    }
}
