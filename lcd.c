//lcd.c
#include<LPC21XX.h>
#include"types.h"
#include"delay.h"
#include"LCD_defines.h"
#include"LCD.h"
void WriteLCD(u8 byte)
{
	//select write opertion
	IOCLR0=1<<LCD_RW;
	//write any byte on to D0 to D7
	IOPIN1=((IOPIN1&~((u32)0XFF<<LCD_DATA))|((u32)byte<<LCD_DATA));
	//apply high to low on enable
	IOSET0=1<<LCD_EN;
	delay_us(1);
	IOCLR0=1<<LCD_EN;
	delay_ms(2);
}
void CmdLCD(u8 cmd)
{
	//select cmd reg
	IOCLR0=1<<LCD_RS;
	//write to cmd reg
	WriteLCD(cmd);
}
	
void CharLCD(u8 ASCII)
{
	//select data reg
	IOSET0=1<<LCD_RS;
	//write to ddram via data reg
	WriteLCD(ASCII);
}
void InitLCD(void)
{
	//cfg p0.8(D0) to p0.15(D7),rs(p0.16),rw(p0.17),en(p0.18) as gpio out
	IODIR1|=((u32)0XFF<<LCD_DATA);
  IODIR0|=(1<<LCD_RS)|(1<<LCD_RW)|(1<<LCD_EN);
	delay_ms(15);
	CmdLCD(0X30);
	CmdLCD(0X30);
	CmdLCD(0X30);
	CmdLCD(MODE_8BIT_2LINE);
	CmdLCD(DSP_ON_CUR_OFF);
	CmdLCD(CLEAR_LCD);
	CmdLCD(SHIFT_CUR_RIGHT);
}
void StrLCD(s8 *s)
{
	while(*s)
		CharLCD(*s++);
}
void U32LCD(u32 n)
{
	s32 i=0;
	u8 digit[10];
	if(n==0)
	{
		CharLCD('0');
	}
	else
	{
		while(n>0)
		{
		    digit[i]=(n%10)+48;
			  n/=10;
			  i++;
		}
		for(--i;i>=0;i--)
		CharLCD(digit[i]);
	}
}
void S32LCD(s32 n)
{
	if(n<0)
	{
		CharLCD('-');
		n=-n;
	}
	U32LCD(n);
}
void F32LCD(f32 fnum,u8 ndp)
{
	s32 i;
	u32 n;
	if(fnum<0)
	{
		CharLCD('-');
		fnum=-fnum;
	}
	n=fnum;
	U32LCD(n);
	CharLCD('.');
	for(i=0;i<ndp;i++)
	{
		fnum=(fnum-n)*10;
		n=fnum;
		CharLCD(n+48);
	}
}
	
//void BuildCGRAM(s8 *,u8);
