#include <LPC21xx.H>  /* LPC21xx definitions         */

#include <string.h>
#include <stdlib.h>
#include "types.h"
#include "delay.h"
#include "uart.h"
#include "lcd.h"
#include "lcd_defines.h"
#include "uart1.h"
#include "kpm.h"
#include "pin_connect_block.h"
#include "rtc.h"
#include "spi.h"
#include "spi_eeprom.h"



char buff[12],dummy,*status;

unsigned char i=0,ch,r_flag;
u8 flag,f;
u32 hh,mm,ss,d,m,y,day;
//s8 tadmin[10]="00328237";
s8 admin[10];


void UART1_isr(void) __irq

{
   		// CmdLCD(0X01);
       //StrLCD("in isr");

  if((U1IIR & 0x04)) //check if receive interrupt

  {

		ch = U1RBR;	/* Read to Clear Receive Interrupt */
			if(ch==0x02)
			r_flag=1;
		else if(ch != 0x03)
			buff[i++] = ch; 
		else
		{
			buff[i] = '\0';
			i=0;
			r_flag = 2;
		}

  }

  else

  {

      dummy=U1IIR; //Read to Clear transmit interrupt

  

  }

   VICVectAddr = 0; /* dummy write */

}

void eint1_isr(void)__irq
{
	flag=1;
	//clear eint1 status flag extint pheripheral
		EXTINT=1<<1;
	//clear eint1 status in vic pheripheral
	VICVectAddr=0;
}


void enable_eint1(void)
{
	//cfg p0.3 as eint1 input
	CfgPortPinFunc(0,3,3);
	//cfg vic for eint1 as irq type
	//VICIntSelect=0;//default all are irq type
	//cfg vic for enable eint1 interrupt
	 VICIntEnable=1<<15;
	//cfg vic for eint1 as v.irq highest priority (0)
	VICVectCntl1=(1<<5)|15;
	//load eint1_isr addres into v.irq reg
	VICVectAddr1=(u32)eint1_isr;
	//cfg external interrupt pheripheral and enable eint1
	//EXTINT=0;default enabled
  //cfg eint1 for edge triggering
     EXTMODE=1<<1;
   // cfg eint1 for falling edge
  //EXTPOLAR=0;
}
			
void edit(void)
{			u8 i,t;
            u8 choice;
            flag=0,f=0;
						//U0_TxStr("in edit");
						//U0_TxStr("\r\n");
			while(1)
			{
				CmdLCD(CLEAR_LCD);
				CmdLCD(GOTO_LINE1_POS0);
				StrLCD("1.ADMINCHANGE");
				CmdLCD(GOTO_LINE2_POS0);
				StrLCD("2.E");
				choice=Keyscan( );
				switch(choice)
				{
					case '1':  //U0_TxStr("1switchcase" );
                               CmdLCD(CLEAR_LCD);
						       CmdLCD(GOTO_LINE1_POS0);
				               StrLCD("CHANGE ADMIN");
							   CmdLCD(GOTO_LINE2_POS0);
							   StrLCD("PLACE CARD");
							   i=0;r_flag=0;
                               while(r_flag !=2);
							   for(i=0;buff[i];i++)
						       ByteWrite_25LC512(i,buff[i]);
							   ByteWrite_25LC512(i,0);
							    for(i=0;buff[i];i++)
	                               {
                                     t=ByteRead_25LC512(i);
		                              admin[i]=t;
	                               } 
								CmdLCD(CLEAR_LCD);
						       CmdLCD(GOTO_LINE1_POS0);
				               StrLCD(buff);
							   CmdLCD(GOTO_LINE2_POS0);
							   StrLCD("received");
					          break;
				   case '2': //U0_TxStr("2switchcase");
				             f=1;
				             break;
				   }
				   if(f)
				   break;
				   }
				   CmdLCD(CLEAR_LCD);
				   //U0_TxStr("after while");
					 //U0_TxStr("after edit");
					 //U0_TxStr("\r\n");
}
void set_time(char *time)
{
  CmdLCD(0X01);
   StrLCD("time");
   CmdLCD(0XC0);
   StrLCD(time);
   
  time=strtok(time,":");
  if(time);
  hh=atoi(time);
  time=strtok(NULL,":");
  if(time)
  mm=atoi(time);
  SetRTCTimeInfo(hh,mm,ss);
 }
 void set_date(char *date)
 {
   CmdLCD(0X01);
   StrLCD("date");
   CmdLCD(0XC0);
   StrLCD(date);
   date=strtok(date,"-");
   if(date)
   d=atoi(date);
   date=strtok(NULL,"-");
   if(date)
   m=atoi(date);
   date=strtok(NULL,"-");
   if(date)
   y=atoi(date);
   SetRTCDateInfo(d,m,y);
   }



void rtc_timeset(void)
{
  char *time,*date;
  time=U0_ReadStr();
  set_time(time);
  date=U0_ReadStr();
  set_date(date);
}
 void display_time(void)
 {
    GetRTCTimeInfo(&hh,&mm,&ss);
    DisplayRTCTime(hh,mm,ss);
	GetRTCDateInfo(&d,&m,&y);
	DisplayRTCDate(d,m,y);
 }
   


								
				                 


int main()

{
   u32 j,t;
   Initkpm();
   InitLCD();
   RTC_Init();
   Init_SPI0();
   InitUART1();
   Init_UART0();
   rtc_timeset();
   CmdLCD(0X01);
   StrLCD("RFID BASED ");
   CmdLCD(0XC0);
   StrLCD("ATTENDANCESYSTEM");
   delay_s(1);
   CmdLCD(0X01);
   display_time();

   //for(j=0;tadmin[j];j++)
    //ByteWrite_25LC512(j,tadmin[j]);
	//ByteWrite_25LC512(j,0);
	 //U0_TxStr("tadmin");
	 //U0_TxStr(tadmin);
	 //U0_TxStr("\r\n");

  for(j=0;j<10;j++)
	 {
        t=ByteRead_25LC512(j);
		 admin[j]=t;
		 if(admin[j])
		 continue;
		 else
		 break;
	  }
		 //U0_TxStr("admin");
	   //U0_TxStr(admin);
		 //U0_TxStr("\r\n");
	  CmdLCD(0XC0);
      StrLCD(admin);
												 
   	enable_eint1( );  

  while(1)

  {
        if(flag)
		    edit();
		i=0;r_flag=0;

		while(r_flag !=2)
		{
		   if(flag)
		      edit();
		}
		//U0_TxStr(buff);
		 //U0_TxStr("afterwhile");
		// U0_TxStr(buff);
		 if(strcmp(admin,buff)==0)
		 {
		    U0_TxByte('A');
		    U0_TxStr(buff);
			U0_TxByte('$');
		 }
		 else
		 {
			U0_TxByte('U');
		    U0_TxStr(buff);
			U0_TxByte('$');
		 }

	    	//U0_TxStr("\r\n");
			status=U0_ReadStr();
			//U0_TxStr(status);
			CmdLCD(0X01);
           StrLCD(status); 


		    CmdLCD(0XC0);
            StrLCD(buff); 

	    delay_s(1);	

  }

}

