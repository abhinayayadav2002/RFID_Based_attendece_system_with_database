//pin_connect_block.c
#include"types.h"
#include<LPC214X.h>
void CfgPortPinFunc(u32 portno,u32 pinno,u32 pinfunc)
{
if(portno==0)
{
if(pinno<16)
{
PINSEL0=((PINSEL0&~(3<<(pinno*2)))|(pinfunc<<(pinno*2)));
}
else if((pinno>=16)&&(pinno<32))
{
PINSEL1=((PINSEL1&~(3<<(pinno%16)))|(pinfunc<<((pinno%16))*2));
}
}
}
