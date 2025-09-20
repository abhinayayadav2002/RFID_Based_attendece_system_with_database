
//kpm.c
#include"types.h"
#include<LPC21XX.h>
#include"kpm_defines.h"
u8 kpmLUT[4][4]={
	                      {'1','2','3','*'},
												{'4','5','6','/'},
												{'7','8','9','+'},
												{'%','0','=','-'}
											};
													
void Initkpm(void)
{
	//cfg row lines as gpio out
	IODIR1|=15<<ROW0;
	//default all col lines are anyway high;
}
u32 Colscan(void)
{
	u32 t;
	t=(((IOPIN1>>COL0)&15)<15)?0:1;
	return t;
}
u32 Rowcheck(void)
{
	u32 rNO;
	for(rNO=0;rNO<=3;rNO++)
	{
		//ground every row at a time starting with 0th row
		IOPIN1=((IOPIN1&~(15<<ROW0))|((~(1<<rNO))<<ROW0));
		if(Colscan()==0)
			break;
	}
	//re-initialize all rows grounded
	IOCLR1=15<<ROW0;
	return rNO;
}			
u32 Colcheck(void)
{
	u32 cNO;
	for(cNO=0;cNO<=3;cNO++)
	{
		if(((IOPIN1>>(COL0+cNO))&1)==0)
			break;
	}
	return cNO;
}
u32 Keyscan(void)
{
	u32 rNO,cNO,keyV;
	//wait for any key press
	while(Colscan());
	//proceed to identify row in which key is pressed
	rNO=Rowcheck();
	//proceed to identify col in which key is pressed
	cNO=Colcheck();
	//extract key value from lut for pressed key
	keyV=kpmLUT[rNO][cNO];
	while(Colscan()==0);
  return keyV;
}
