#include <stdio.h>
#include "driver_ili9488.h"
#include "driver_lcd.h"

static void ILI9488_RegConfig(void);


/* ��������  void ILI9488_Init(void)
 * ���������_��
 * �����������
 * ����ֵ��  ��
 * �������ã�LCD��ʼ��(��������GRAM������)
*/
void ILI9488_Init(void)
{
    uint8_t status = 0;
    ILI9488_RegConfig();    // ����LCD��������
    
    LCD_GRAM_Scan(0);       // ����GRAMɨ�跽ʽ
    LCD_Clear(WHITE);
}

/* ��������  static void ILI9488_RegConfig(void)
 * �����������
 * �����������
 * ����ֵ��  ��
 * �������ã�ILI9488�Ĵ�����ʼ��
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
