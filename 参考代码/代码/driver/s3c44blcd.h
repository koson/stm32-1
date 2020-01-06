

#ifndef __LCDLIB_H__
	#define __LCDLIB_H__
	#include "data.h"
U16  Setup_LCDInit(void); 
U16 LCD_READ_MEM(U32 off);
void LCD_WRITE_MEM( U32 off,U8 Data);
static void SetPixel(U16 x, U16 y, U8 c);
void LCD_DrawPixel  (U16 x, U16 y);
void LCD_DrawHLine  (U16 x0, U16 y0,  U16 x1);
void LCD_DrawVLine(U16 x0, U16 y0,  U16 y1);
void LCD_FillRect(U16 x0, U16 y0, U16 x1, U16 y1);
void LCD_Clear(U16 x0, U16 y0, U16 x1, U16 y1);
static  void _DrawPoint(U32 x0,U32 y0, U32 xoff, U32 yoff);
void LCD_DrawCircle(U32 x0, U32 y0, U32 r);
void LCD_FillCircle       (U16 x0, U16 y0, U16 r);
U16 Log2Phy(U32 Color);
U16 LCD_Log2Phy(U32 Color);
void Set_Color(U32 color);
void LCD_SetColor(U16 PhyColor);
void Set_BkColor(U32 color);
void LCD_SetBkColor(U16 PhyColor);
	
#endif /*__LCDLIB_H__*/
