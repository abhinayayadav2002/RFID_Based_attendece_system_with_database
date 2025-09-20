#include <LPC214X.h>
#include "uart_defines.h"
#include "types.h"

void Init_UART0(void)
{
	//cfg p0.0 as TXD0 pin
	PINSEL0|=1<<(0*2);
	//CFG p0.1 as RXD0 PIN FUNC
	PINSEL0|=1<<(1*2);
	//or write directly PINSEL0=0X05;
	//CFG UART FRAME SETTINGS
	U0LCR=(1<<DLAB_BIT)|3;//3 HERE INDICATES 8 DATABITS,DLAB_BIT =1 MEANS ENABLE BAUD SETTINGS
//CFG FOR REQUIRED BAUD RATE	
	U0DLM=DIVISOR>>8;//MSB BYTE OF DIVISOR
  U0DLL=DIVISOR;	//LSB BYTE OF DIVISOR
	//CLEAR DLAB_BIT FOR TRANSMISSION OR RECEPTION
	U0LCR&=~(1<<DLAB_BIT);
}
	
void U0_TxByte(u8 byte)
{
	//PLACE BYTE INTO TXBUFF AND START TRANSMISSION
	U0THR=byte;
	//wait until transmission completed
	while(((U0LSR>>THRE)&1)==0);
}
u8 U0_RxByte(void)
{
	//WAIT UNTIL RECEPTION
	while(((U0LSR>>DRB)&1)==0);
	//READ AND RETURN RECEIVED BYTE
	return U0RBR;
}
	

void U0_TxStr(s8* s )
{
	while(*s)
	U0_TxByte(*s++);
}
void U0_TxU32(u32 n)
{
	s32 i=0;
	u8 digit[10];
	if(n==0)
	{
		U0_TxByte('0');
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
		U0_TxByte(digit[i]);
		
	}
}

void U0_TxS32(s32 n)
{
	if(n<0)
	{
		U0_TxByte('-');
		n=-n;
	}
	U0_TxU32(n);
}
void U0_TxF32(f32 fnum,u8 ndp)
{
	s32 i;
	u32 n;
	if(fnum<0)
	{
		U0_TxByte('-');
		fnum=-fnum;
	}
	n=fnum;
	U0_TxU32(n);
	U0_TxByte('.');
	for(i=0;i<ndp;i++)
	{
		fnum=(fnum-n)*10;
		n=fnum;
		U0_TxByte(n+48);
		
	}
}


s8* U0_ReadStr(void)
{
	static s8 rbuf[100];
	u32 i=0;
	while(1)
	{
		rbuf[i]=U0_RxByte( );
		//U0_TxByte(rbuf[i]);
		if((rbuf[i]=='\n')||(rbuf[i]=='\r'))
		{
			rbuf[i]='\0';
			break;
		}
		i++;
	}
	return rbuf;
}
