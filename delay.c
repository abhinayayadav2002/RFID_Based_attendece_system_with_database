//delay.c
#include"types.h"
void delay_us(u32 dlyUS)
{
//XTAL=12MHZ,CCLK=60MHZ
dlyUS*=12;
while(dlyUS--);
}

void delay_ms(u32 dlyMS)
{
//XTAL=12MHZ,CCLK=60MHZ
dlyMS*=12000;
while(dlyMS--);
}

void delay_s(u32 dlyS)
{
//XTAL=12MHZ,CCLK=60MHZ
dlyS*=12000000;
while(dlyS--);
}
