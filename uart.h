#include "types.h"
void Init_UART0(void);
void U0_TxByte(u8);
u8 U0_RxByte(void);
void U0_TxStr(s8* );
s8 *U0_ReadStr(void);
void U0_TxStr(s8* s );
void U0_TxS32(s32 n);
void U0_TxF32(f32 fnum,u8 ndp);
void U0_TxU32(u32 n);
