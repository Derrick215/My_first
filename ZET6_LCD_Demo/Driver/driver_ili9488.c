#include <stdio.h>
#include "driver_ili9488.h"
#include "driver_lcd.h"

static void ILI9488_RegConfig(void);


/* 函数名：  void ILI9488_Init(void)
 * 输入参数：_无
 * 输出参数：无
 * 返回值：  无
 * 函数作用：LCD初始化(控制器、GRAM、清屏)
*/
void ILI9488_Init(void)
{
    uint8_t status = 0;
    ILI9488_RegConfig();    // 配置LCD基本参数
    
    LCD_GRAM_Scan(0);       // 配置GRAM扫描方式
    LCD_Clear(WHITE);
}

/* 函数名：  static void ILI9488_RegConfig(void)
 * 输入参数：无
 * 输出参数：无
 * 返回值：  无
 * 函数作用：ILI9488寄存器初始化
*/
static void ILI9488_RegConfig(void)
{  
	LCD_Write_Cmd(0x3A);   // Interface Pixel Format  
	LCD_Write_Data(0x05);  // DBI-16bit     
	   
	LCD_Write_Cmd(0xB6);   // Display Function Control 
	LCD_Write_Data(0x02);  // RM:System interface
	LCD_Write_Data(0x22);  // SS:S960->S1 GS:G1->G480
	LCD_Write_Data(0x3B);  // NL
 
	LCD_Write_Cmd(0x11);   // Sleep OUT
	LCD_Write_Data(0x00);  // No parameter 
	
	LCD_Write_Cmd(0x29);   // Display ON
	LCD_Write_Data(0x00);  // No parameter 
}
