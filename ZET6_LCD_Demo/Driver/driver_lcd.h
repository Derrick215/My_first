#ifndef __LCD_FUNCTION_H
#define __LCD_FUNCTION_H

#include "main.h"
#include "driver_timer.h"

/*************** LCD延时函数重定义 ***************/
#define lcd_delay_ms(m)             HAL_Delay(m)
#define lcd_delay_us(u)             us_timer_delay(u)

/*************** LCD 常用颜色定义 ***************/
#define RGB888_TO_RGB565(color) ((((color) >> 19) & 0x1F) << 11) | \
                                ((((color) >> 10) & 0x3F) << 5) | \
                                (((color) >> 3) & 0x1F)
											
#define WHITE         	 (RGB888_TO_RGB565(0xFFFFFF))
#define BLACK         	 (RGB888_TO_RGB565(0x000000))	 
#define RED           	 (RGB888_TO_RGB565(0xFF0000))
#define GREEN         	 (RGB888_TO_RGB565(0x00FF00))
#define BLUE         	 (RGB888_TO_RGB565(0x0000FF)) 
#define YELLOW        	 (RGB888_TO_RGB565(0xFFFF00)) 


/*************** LCD 结构体定义 ***************/

/* LCD 指令结构体，用于读写LCD数据和寄存器 */
typedef struct
{
    __IO uint16_t reg;   // 寄存器值
    __IO uint16_t data;  // 数据值
}_lcd;   

/* LCD 设备信息 */
typedef struct
{
    uint16_t dev_id;    // 设备ID
    uint16_t hor_res;   // LCD水平显示长度
    uint16_t ver_res;   // LCD垂直显示长度
    uint16_t scan_mode; // 扫描模式
    
    uint16_t gram_cmd;  // 写入GRAM的指令
    uint16_t setx_cmd;  // 设置X坐标的指令
    uint16_t sety_cmd;  // 设置Y坐标的指令
}_lcddev;   

/* LCD 显示颜色结构体，用于设置背景颜色和笔迹颜色 */
typedef struct
{
    uint16_t backcolor; // 背景颜色值
    uint16_t textcolor; // 笔迹颜色值
}_lcd_color;    

/********** 全局变量声明 **********/
extern _lcddev    lcddev;
extern _lcd_color lcd_color;


/*************** 读写API声明 ***************/

/* 函数名：  void LCD_Write_Cmd(uint16_t _cmd)
 * 输入参数：_cmd->要写入的指令
 * 输出参数：无
 * 返回值：  无
 * 函数作用：往LCD写入指令
*/
extern void LCD_Write_Cmd(uint16_t _cmd);

/* 函数名：  void LCD_Write_Data(uint16_t _data)
 * 输入参数：_data->要写入的数据
 * 输出参数：无
 * 返回值：  无
 * 函数作用：往LCD写入数据
*/
extern void LCD_Write_Data(uint16_t _data);

/* 函数名：  uint16_t LCD_Read_Data(void)
 * 输入参数：无
 * 输出参数：无
 * 返回值：  返回寄存器的值
 * 函数作用：返回寄存器的值
*/
extern uint16_t LCD_Read_Data(void);

/*************** 颜色设置API声明 ***************/
extern void     LCD_SetColor(uint16_t _backcolor, uint16_t _textcolor);

/*************** LCD功能函数声明 ***************/
extern void     LCD_Scan_Dir(uint8_t _dir);
extern void     LCD_GRAM_Scan(uint8_t _opt);
extern void     LCD_SetCursor(uint16_t x, uint16_t y);
extern void     LCD_Clear(uint16_t _color);
extern void     LCD_Color_Fill(uint16_t x_start, uint16_t y_start, uint16_t x_end, uint16_t y_end, uint16_t color);

extern uint32_t LCD_Pow(uint8_t m, uint8_t n);
extern void     LCD_DrawPoint(uint16_t x, uint16_t y, uint16_t color);
extern void     LCD_ShowChar(uint16_t x, uint16_t  y, uint8_t num, uint8_t size, uint8_t mode);
extern void     LCD_ShowNum(uint16_t x,uint16_t y,uint32_t num, uint8_t size);
extern void     LCD_ShowString(uint16_t x, uint16_t y, uint8_t size, char *p);
extern void     LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);

/********** LCD初始化函数声明 **********/
extern void LCD_Init(void);

#endif /* __LCD_FUNCTION_H */
