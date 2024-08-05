/*  Copyright (s) 2019 深圳百问网科技有限公司
 *  All rights reserved
 * 
 * 文件名称：main.c
 * 摘要：
 *  
 * 修改历史     版本号       Author       修改内容
 *--------------------------------------------------
 * 2020.6.6      v01        百问科技      创建文件
 *--------------------------------------------------
*/
#include <stdio.h>

#include "main.h"
#include "stm32f1xx_clk.h"

#include "driver_usart.h"
#include "driver_timer.h"
#include "driver_fsmc.h"

#include "driver_lcd.h"
#include "OLED.h"


int main(void)
{    
    // 初始化HAL库函数必须要调用此函数
    HAL_Init();
	
    /*
     * 系统时钟即AHB/APB时钟配置    
	 * 使用外部高速时钟HSE（8MHz）配置系统时钟，经过PLL放大9倍，得到72MHz
     */
    SystemClock_Config();
	
	// 初始化USART1，设置波特率为115200 
    UsartInit(115200);
	
	
	printf("**********************************************\n\r");
    printf("-->百问科技：www.100ask.net\n\r");
    printf("-->LCD显示实验\n\r");
    printf("**********************************************\n\r");

//    TimerInit();              // 定时器初始化
//	TimerPWMInit();           // PWM初始化
//    FSMC_LCD_Init();          // FSMC接口初始化
//    LCD_Init();               // LCD初始化
	
//	LCD_Clear(WHITE);         // 清屏为白色 
//	
//	LCD_ShowChar(0, 0, 'A', 24, 0);          // 显示字符
//	
//	LCD_SetColor(BLACK, RED);              // 设置字符串颜色和背景色,显示字符串
//	LCD_ShowString(0, 24, 24, "www.100ask.net");
//	
//	LCD_SetColor(BLACK, RED);                // 设置字符串颜色和背景色,显示数字
//	LCD_ShowNum(0, 48, 123456789, 24); 
//	
//	LCD_DrawLine(160, 120, 70, 310, BLUE);   // 画线显示三角形
//	LCD_DrawLine(70, 310, 250, 310, BLUE);
//	LCD_DrawLine(250, 310, 160, 120, BLUE);
//	
//	LCD_Color_Fill(110,311, 210,411, GREEN); // 填充矩形
	
	OLED_Init();
	OLED_ShowString(1, 1, "LED0_RED");
    while(1);
}

/*
 *  函数名：void Error_Handler(void)
 *  输入参数：无
 *  输出参数：无
 *  返回值：无
*  函数作用：程序错误处理函数，此处暂时设为死循环，不做任何动作
*/
void Error_Handler(void)
{
    while(1)
    {
    }
}
