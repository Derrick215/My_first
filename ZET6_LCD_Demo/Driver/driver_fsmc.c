/*  Copyright (s) 2019 深圳百问网科技有限公司
 *  All rights reserved
 * 
 * 文件名称：driver_fsmc.c
 * 摘要：
 *  
 * 修改历史     版本号        Author       修改内容
 *--------------------------------------------------
 * 2020.6.6      v01        百问科技      创建文件
 *--------------------------------------------------
*/

#include "driver_fsmc.h"

static FSMC_NORSRAM_TimingTypeDef   hfsmc_rw;
static SRAM_HandleTypeDef           hsram;

void FSMC_LCD_Init(void)
{
	hfsmc_rw.AddressSetupTime      = 0x00;   // 地址建立时间ADDSET  范围:0~15   (模式A需要设置)
//  hfsmc_rw.AddressHoldTime       = 0x01;   // 地址保持时间ADDHLD  范围:1~15   
    hfsmc_rw.DataSetupTime         = 0x02;   // 数据建立时间DATAST  范围:1~255  (模式A需要设置)
//  hfsmc_rw.BusTurnAroundDuration = 0x00;   // 总线恢复时间BUSTURN 范围:0~15   
//  hfsmc_rw.CLKDivision           = 0x00;   // 时钟分频因子CLKDIV  范围:2~16   
//  hfsmc_rw.DataLatency           = 0x00;   // 数据产生时间ACCMOD  范围:2~17   
    hfsmc_rw.AccessMode            = FSMC_ACCESS_MODE_A;  // 模式A
	
    hsram.Instance = FSMC_NORSRAM_DEVICE;                                   // 实例类型:NOR/SRAM设备
    hsram.Extended = FSMC_NORSRAM_EXTENDED_DEVICE;
    hsram.Init.NSBank               = FSMC_NORSRAM_BANK4;     				// 使用NE4,对应BANK1(NORSRAM)的区域4
    hsram.Init.DataAddressMux       = FSMC_DATA_ADDRESS_MUX_DISABLE; 	    // 地址/数据线不复用(nonmultiplexed)
    hsram.Init.MemoryType           = FSMC_MEMORY_TYPE_SRAM;   				// 存储器类型:SRAM
    hsram.Init.MemoryDataWidth      = FSMC_NORSRAM_MEM_BUS_WIDTH_16; 	    // 选择16位数据宽度(所接LCD为16位数据宽度)
//  hsram.Init.BurstAccessMode      = FSMC_BURST_ACCESS_MODE_DISABLE;       // 是否使能突发访问,仅对同步突发存储器有效,此处未用到
//  hsram.Init.WaitSignalPolarity   = FSMC_WAIT_SIGNAL_POLARITY_LOW;        // 等待信号的极性,适用于突发模式访问,此处未用到
//  hsram.Init.WaitSignalActive     = FSMC_WAIT_TIMING_BEFORE_WS;   	    // 存储器是在等待周期之前的一个时钟周期还是等待周期期间使能NWAIT,适用于突发模式访问,此处未用到
    hsram.Init.WriteOperation       = FSMC_WRITE_OPERATION_ENABLE;    	    // 写使能
//  hsram.Init.WaitSignal           = FSMC_WAIT_SIGNAL_DISABLE;           	// 等待使能位,适用于突发模式访问,此处未用到
    hsram.Init.ExtendedMode         = FSMC_EXTENDED_MODE_ENABLE;        	// 启用扩展模式,使用模式A
//  hsram.Init.AsynchronousWait     = FSMC_ASYNCHRONOUS_WAIT_DISABLE;	    // 是否使能异步传输模式下的等待信号,此处未用到
//  hsram.Init.WriteBurst           = FSMC_WRITE_BURST_DISABLE;           	// 禁止突发写,适用于突发模式访问,此处未用到
    
    if(HAL_SRAM_Init(&hsram, &hfsmc_rw, &hfsmc_rw) != HAL_OK)
    {
        Error_Handler();
    }    
}

void HAL_SRAM_MspInit(SRAM_HandleTypeDef *hsram)
{
    GPIO_InitTypeDef    GPIO_InitStruct = {0};
 
    __HAL_RCC_FSMC_CLK_ENABLE();                     // 使能FSMC时钟
	__HAL_RCC_GPIOC_CLK_ENABLE();                    // 使能GPIO端口C时钟
	__HAL_RCC_GPIOD_CLK_ENABLE();                    // 使能GPIO端口D时钟
	__HAL_RCC_GPIOE_CLK_ENABLE();                    // 使能GPIO端口E时钟
	__HAL_RCC_GPIOF_CLK_ENABLE();                    // 使能GPIO端口F时钟
	__HAL_RCC_GPIOG_CLK_ENABLE();                    // 使能GPIO端口G时钟

    GPIO_InitStruct.Mode      = GPIO_MODE_OUTPUT_PP; // 配置为推挽输出功能
    GPIO_InitStruct.Pull      = GPIO_PULLUP;         // 上拉
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;// 引脚翻转速率快
    
    GPIO_InitStruct.Pin = LCD_RST_PIN;               
    HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);          // 按上面参数配置LCD_RST引脚(PF11)

    GPIO_InitStruct.Pin = LCD_CS_PIN;
    HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);          // 按上面参数配置LCD_CS引脚(PG12) 
    
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;     // 配置为复用推挽功能
    GPIO_InitStruct.Pull      = GPIO_NOPULL;         // 不上拉
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;// 引脚翻转速率快

    // PDx
    GPIO_InitStruct.Pin = LCD_GPIOD_PIN;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);          // 按上面参数配置D组涉及的GPIO
    
    // PEx
    GPIO_InitStruct.Pin = LCD_GPIOE_PIN;           
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);          // 按上面参数配置E组涉及的GPIO
}
