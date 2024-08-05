#include <stdio.h>
#include "fonts.h"

#include "driver_ili9488.h"
#include "driver_lcd.h"
#include "driver_fsmc.h"

#define LCD                 ((__IO _lcd*)(0x6C000000 + (1<<24) - 2))

/********** 全局变量定义 **********/
_lcddev     lcddev;
_lcd_color  lcd_color;


/********** LCD读写函数定义 **********/
/* 函数名：  void LCD_Write_Cmd(uint16_t _cmd)
 * 输入参数：_cmd->要写入的指令
 * 输出参数：无
 * 返回值：  无
 * 函数作用：往LCD写入指令
*/
void LCD_Write_Cmd(uint16_t _cmd)
{
	_cmd = _cmd;
    LCD->reg = _cmd;
}


/* 函数名：  void LCD_Write_Data(uint16_t _data)
 * 输入参数：_data->要写入的数据
 * 输出参数：无
 * 返回值：  无
 * 函数作用：往LCD写入数据
*/
void LCD_Write_Data(uint16_t _data)
{
	_data = _data;
    LCD->data = _data;
}


/* 函数名：  uint16_t LCD_Read_Data(void)
 * 输入参数：无
 * 输出参数：无
 * 返回值：  返回寄存器的值
 * 函数作用：返回寄存器的值
*/
uint16_t LCD_Read_Data(void)
{
    return LCD->data;
}

/* 函数名：  void LCD_SetColor(uint16_t _backcolor, uint16_t _textcolor)
 * 输入参数：_backcolor:背景色  _textcolor:字体颜色
 * 输出参数：无
 * 返回值：  无
 * 函数作用：设置背景颜色和字体颜色
*/
void LCD_SetColor(uint16_t _backcolor, uint16_t _textcolor)
{
	lcd_color.backcolor = _backcolor;
    lcd_color.textcolor = _textcolor;
}


/* 函数名：  LCD_Backlight_Enable(void)
 * 输入参数：无
 * 输出参数：无
 * 返回值：  无
 * 函数作用：LCD背光使能
*/
void LCD_Backlight_Enable(void)
{
	if (HAL_TIM_PWM_Start_IT(&hpwm, TIM_LCD_PWM_CHANNEL) != HAL_OK) // 启动LCD_PWM所在通道定时器PWM
	{
		Error_Handler();
	}
}


/* 函数名：  void LCD_Backlight_Disable(void)
 * 输入参数：无
 * 输出参数：无
 * 返回值：  无
 * 函数作用：LCD背光失能
*/
void LCD_Backlight_Disable(void)
{

	if (HAL_TIM_PWM_Stop_IT(&hpwm, TIM_LCD_PWM_CHANNEL) != HAL_OK) // 停止LCD_PWM所在通道定时器PWM
	{
		Error_Handler();
	}
}

/* 函数名：  void LCD_Brightness(uint8_t value)
 * 输入参数：value:背光亮度,0~100
 * 输出参数：无
 * 返回值：  无
 * 函数作用：LCD背光亮度
*/
void LCD_Brightness(uint8_t value)
{
	if (value > 100)
		value = 100;
	lcd_brt = value; // 背光亮度，范围0~100
}

/* 函数名：  void LCD_Rest(void)
 * 输入参数：无
 * 输出参数：无
 * 返回值：  无
 * 函数作用：LCD复位
*/
void LCD_Rest(void)
{
    LCD_RST(0);           // 拉低复位引脚，复位LCD
    lcd_delay_us(0xAFF);
    LCD_RST(1);           // 拉高复位引脚，恢复LCD
    lcd_delay_us(0xAFF);
}

/* 函数名：  void LCD_GetDevID(void)
 * 输入参数：无
 * 输出参数：无
 * 返回值：  无
 * 函数作用：获取LCD控制器的ID
*/
void LCD_GetDevID(void)
{
    uint16_t tmp = 0;
    
    LCD_Write_Cmd(0xD3);    // Read ID4
    tmp = LCD_Read_Data();  // 虚拟读取周期
    tmp = LCD_Read_Data();  // IC型号第1部分,值为x00
	tmp = tmp << 16;
    tmp = LCD_Read_Data();  // IC型号第2部分,值为x94
    tmp = tmp << 8;
    tmp |= LCD_Read_Data(); // IC型号第3部分,值为x88
//	tmp = tmp << 8;
//    tmp |= LCD_Read_Data(); // IC型号第3部分,值为x88
    
    lcddev.dev_id = tmp;
}

/* 函数名：  void LCD_Scan_Dir(uint8_t _dir)
 * 输入参数：_dir:方向选项0~7
 * 输出参数：无
 * 返回值：  无
 * 函数作用：设置LCD的方向
*/
void LCD_Scan_Dir(uint8_t _dir)
{   
	// bit[7] MY:Row Address Order
	// bit[6] MX:Column Address Order
	// bit[5] MV:Row/Column Exchange
	// bit[3] BGR:RGB-BGR Order
    _dir = (_dir<<5) | 0x08;  // 获取高3位值,RGB to BGR
	LCD_Write_Cmd(0x36);      // Memory Access Control
	LCD_Write_Data(_dir);     //根据scan_mode的值设置LCD方向，共0-7种模式    
}

/* 函数名：  void LCD_GRAM_Scan(uint8_t _opt)
 * 输入参数：_opt:扫描方式0~7
 * 输出参数：无
 * 返回值：  无
 * 函数作用：设置LCD的扫描方式
*/
void LCD_GRAM_Scan(uint8_t _opt)
{
    uint8_t tmp = _opt%2;
    if(_opt > 7)
        _opt = 0;
    lcddev.setx_cmd = 0x2A;     // Column Address Set
    lcddev.sety_cmd = 0x2B;     // Page Address Set
    lcddev.gram_cmd = 0x2C;     // Memory Write
    lcddev.scan_mode = _opt;    // 记录扫描模式以便之后判断屏幕方向
    
    if(tmp == 0)     // 0 2 4 6 竖屏
    {
		if(lcddev.dev_id == 0x9488)
        {
            lcddev.hor_res = 320;
            lcddev.ver_res = 480;
        }
    }
    else if(tmp == 1) // 1 3 5 7 横屏
    {
		if(lcddev.dev_id == 0x9488)
        {
            lcddev.hor_res = 480;
            lcddev.ver_res = 320;
        }
    }
    LCD_Scan_Dir(lcddev.scan_mode);
}

/* 函数名：  void LCD_SetCursor(uint16_t x, uint16_t y)
 * 输入参数：x:光标x坐标  y:光标y坐标
 * 输出参数：无
 * 返回值：  无
 * 函数作用：设置光标位置
*/
void LCD_SetCursor(uint16_t x, uint16_t y)
{
	LCD_Write_Cmd(lcddev.setx_cmd);          // 设置X坐标指令 
	LCD_Write_Data(x>>8);                    // 开始地址高8位
    LCD_Write_Data(x&0xFF);  	             // 开始地址低8位
	LCD_Write_Data((lcddev.hor_res-1)>>8);   // 结束地址高8位
    LCD_Write_Data((lcddev.hor_res-1)&0xFF); // 结束地址低8位                          
               
	LCD_Write_Cmd(lcddev.sety_cmd);          // 设置Y坐标指令 
	LCD_Write_Data(y>>8);                    // 开始地址高8位
    LCD_Write_Data(y&0xFF);                  // 开始地址低8位
	LCD_Write_Data((lcddev.ver_res-1)>>8);   // 结束地址高8位
    LCD_Write_Data((lcddev.ver_res-1)&0xFF); // 结束地址低8位   
}

/* 函数名：  void LCD_Clear(uint16_t _color)
 * 输入参数：_color:颜色
 * 输出参数：无
 * 返回值：  无
 * 函数作用：LCD清屏
*/
void LCD_Clear(uint16_t _color)
{
    uint32_t i = 0;
    uint32_t totalpoint = lcddev.hor_res * lcddev.ver_res; // 像素点总数
    
    lcd_color.backcolor = _color;  // 设置背景颜色
    LCD_SetCursor(0, 0);           // 设置显示位置
	
    LCD_Write_Cmd(lcddev.gram_cmd);// 写入GRAM的指令
    for(i=0;i<totalpoint;i++)      // 循环写像素颜色      
    {
        LCD_Write_Data(lcd_color.backcolor);
    }
}


/* 函数名：  LCD_Color_Fill(uint16_t x_start, uint16_t y_start, uint16_t x_end, uint16_t y_end, uint16_t *color)
 * 输入参数：(sx,sy),(ex,ey):填充矩形对角坐标,区域大小为:(ex-sx+1)*(ey-sy+1)  color:要填充的颜色
 * 输出参数：无
 * 返回值：  无
 * 函数作用：在指定区域内填充指定颜色块		
*/
void LCD_Color_Fill(uint16_t x_start, uint16_t y_start, uint16_t x_end, uint16_t y_end, uint16_t color)
{  
    uint16_t i,j;   
    uint16_t xlen = 0;    
       
    xlen = x_end - x_start + 1;       
    for(i=y_start; i<=y_end; i++)   
    {   
        LCD_SetCursor(x_start, i);                    
        LCD_Write_Cmd(lcddev.gram_cmd);                 
        for(j=0; j<xlen; j++)
        {
            LCD_Write_Data(color);
        }                
    }  
} 


/* 函数名：  void LCD_DrawPoint(uint16_t x, uint16_t y, uint16_t color)
 * 输入参数：x,y:坐标  color:颜色
 * 输出参数：无
 * 返回值：  无
 * 函数作用：LCD描点
*/
void LCD_DrawPoint(uint16_t x, uint16_t y, uint16_t color)
{	   
    LCD_Write_Cmd(lcddev.setx_cmd);           // 设置X坐标指令 
    LCD_Write_Data(x>>8);                     // 开始地址高8位
    LCD_Write_Data(x&0xFF);                   // 开始地址低8位
    LCD_Write_Data((lcddev.hor_res-1)>>8);    // 结束地址高8位
    LCD_Write_Data((lcddev.hor_res-1)&0xFF);  // 结束地址低8位   
	
    LCD_Write_Cmd(lcddev.sety_cmd);           // 设置Y坐标指令 
    LCD_Write_Data(y>>8);                     // 开始地址高8位
    LCD_Write_Data(y&0xFF);                   // 开始地址低8位
    LCD_Write_Data((lcddev.ver_res-1)>>8);    // 结束地址高8位
    LCD_Write_Data((lcddev.ver_res-1)&0xFF);  // 结束地址低8位
	
    LCD_Write_Cmd(lcddev.gram_cmd);           // 写入GRAM的指令  
    LCD_Write_Data(color);                    // 写入颜色数据 
}	

/* 函数名：  void LCD_ShowChar(uint16_t x, uint16_t  y, uint8_t num, uint8_t size, uint8_t mode)
 * 输入参数：x,y:起始坐标  num:要显示的字符(" "--->"~")  size:字体大小 12/16/24	 mode: 是否叠加(1叠加)  	
 * 输出参数：无
 * 返回值：  无
 * 函数作用：在指定位置显示一个字符
*/
void LCD_ShowChar(uint16_t x, uint16_t  y, uint8_t num, uint8_t size, uint8_t mode)
{  							  
    uint8_t data, t, t1;
	uint16_t y0 = y;
	uint8_t csize = (size/8+((size%8)?1:0))*(size/2); // 得到字体一个字符对应点阵集所占的字节数	
 	num=num-' '; // 得到偏移后的值（ASCII字库是从空格开始取模，所以-' '就是对应字符的字库）
	
	for(t=0; t<csize; t++)  
	{   
		if(size==12)
			data = asc2_1206[num][t];   //调用1206字体
		else if(size==16)
			data = asc2_1608[num][t];	//调用1608字体
		else if(size==24)
			data = asc2_2412[num][t];	//调用2412字体
		else 
			return;					  
		
		for(t1=0;t1<8;t1++)
		{			    
			if(data&0x80)             // 依次画最高位像素点
				LCD_DrawPoint(x, y, lcd_color.textcolor);
			else if(mode==0)
				LCD_DrawPoint(x, y, lcd_color.backcolor);
            
			data <<= 1;
			y++;
            
			if(y>lcddev.ver_res)      // 超出垂直显示区域
				return;		
			
			if((y-y0)==size)          // 一列画完    
			{
				y = y0;
				x++;
				if(x>lcddev.hor_res)  // 超出水平显示区域
					return;	 
				break;
			}
		}  	 
	}  	    	   	 	  
}   

/* 函数名：  void LCD_ShowString(uint16_t x, uint16_t y, uint8_t size, uint8_t *p)
 * 输入参数：x,y:起点坐标  size:字体大小  *p:字符串起始地址		
 * 输出参数：无
 * 返回值：  无
 * 函数作用：显示字符串
*/
void LCD_ShowString(uint16_t x, uint16_t y, uint8_t size, char *p)
{      
    while((*p<='~')&&(*p>=' '))   // 判断是不是非法字符
    {       
		LCD_ShowChar(x, y, *p, size, 0); // 显示该字符
		x+=size/2;                       // 字符位置水平偏移
		p++;                             // 指向下一个字符
		if(x>(lcddev.hor_res-size/2))    // 超出水平显示区域
		{
			x = 0;
			y+=size;
		}
		if(y>(lcddev.ver_res-size))     // 超出垂直显示区域
			break;
    }  
}


/* 函数名：  uint32_t LCD_Pow(uint8_t m, uint8_t n)
 * 输入参数：m:底数  n:次方
 * 输出参数：无
 * 返回值：  m^n次方
 * 函数作用：计算m^n
*/
uint32_t LCD_Pow(uint8_t m, uint8_t n)
{
	uint32_t result = 1;	 
	while(n--)result*=m;    
	return result;
}	

/* 函数名：  void LCD_ShowNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint8_t size)
 * 输入参数：x,y:起点坐标  num:数值(0~4294967295) len :数字的位数  size:字体大小	
 * 输出参数：无
 * 返回值：  无
 * 函数作用：显示数字
*/
void LCD_ShowNum(uint16_t x,uint16_t y,uint32_t num, uint8_t size)
{        
	uint8_t len = 0;
	uint8_t t = 0, temp = 0, enshow = 0;
	uint32_t num_t = num;
	
	while(num_t)  // 计算num的位数
	{
		len++;
        num_t/=10;
    }

	for(t=0; t<len; t++)
	{
		temp=(num/LCD_Pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				LCD_ShowChar(x+(size/2)*t, y, ' ', size, 0);
				continue;
			}
			else 
				enshow=1; 
		 	 
		}
	 	LCD_ShowChar(x+(size/2)*t, y, temp+'0', size, 0); 
	}
} 


/* 函数名：  void LCD_ShowNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint8_t size)
 * 输入参数：x1,y1:起点坐标  x2,y2:终点坐标 color:颜色
 * 输出参数：无
 * 返回值：  无
 * 函数作用：画线
*/
void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
	uint16_t t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 
	delta_x=x2-x1;              //计算坐标增量 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1;        //设置单步方向 
	else if(delta_x==0)incx=0;  //垂直线 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;  //水平线 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ ) //画线输出 
	{  
		LCD_DrawPoint(uRow, uCol, color);//画点 
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
} 

/* 函数名：  void LCD_Init(void)
 * 输入参数：无
 * 输出参数：无
 * 返回值：  无
 * 函数作用：LCD初始化(背光、LCD控制器)
*/
void LCD_Init(void)
{
	LCD_Backlight_Enable();     // 使能背光
	LCD_Brightness(90);         // 设置背光亮度0~100
	
    LCD_Rest();                 // LCD复位
	HAL_Delay(10);
    LCD_GetDevID();             // 获取LCD控制器ID
    
	if(lcddev.dev_id == 0x9488) // 如果LCD控制器是ILI9488
    {
        ILI9488_Init();         // 初始化LCD控制器
    }
}
