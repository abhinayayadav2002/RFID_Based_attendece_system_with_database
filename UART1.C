#include <LPC21xx.H>  /* LPC21xx definitions         */

#define UART_INT_ENABLE 1
void UART1_isr(void) __irq;

void InitUART1 (void) /* Initialize Serial Interface       */ 
{  
            		
  PINSEL0 |= (1<<16)|(1<<18); /* Enable RxD1 and TxD1              */
  U1LCR = 0x83;         /* 8 bits, no Parity, 1 Stop bit     */
  U1DLL = 97;           /* 9600 Baud Rate @ CCLK/4 VPB Clock  */
  U1LCR = 0x03;         /* DLAB = 0  */
  
  #if UART_INT_ENABLE > 0

  VICIntSelect = 0x00000000; // IRQ
  VICVectAddr0 = (unsigned)UART1_isr;
  VICVectCntl0 = 0x20 | 7; /* UART1 Interrupt */
  VICIntEnable = 1 << 7;   /* Enable UART1 Interrupt */
 
 // U1IIR = 0xc0;
 // U1FCR = 0xc7;
  U1IER = 0x03;       /* Enable UART1 RX and THRE Interrupts */   
             
  #endif
						
}

void UART1_Tx(char ch)  /* Write character to Serial Port    */  
{ 
  U1THR = ch; 
  while(((U1LSR>>6)&1)==0);
}

char UART1_Rx(void)    /* Read character from Serial Port   */
{                     
  while (((U1LSR>>0)&1)==0);
  return (U1RBR);
}

void UART1_Str(char *s)
{
   while(*s)
       UART1_Tx(*s++);
}

void UART1_Int(unsigned int n)
{
  unsigned char a[10];
  int i=0;
  if(n==0)
  {
    UART1_Tx('0');
	return;
  }
  else
  {
     while(n>0)
	 {
	   a[i++]=(n%10)+48;
	   n=n/10;
	 }
	 --i;
	 for(;i>=0;i--)
	 {
	   UART1_Tx(a[i]);
	 }
   }
}

/*void UART0_Float(float f)
{
  int x;
  float temp;
  x=f;
  UART0_Int(x);
  UART0_Tx('.');
  temp=(f-x)*100;
  x=temp;
  UART0_Int(x);
}*/


								
