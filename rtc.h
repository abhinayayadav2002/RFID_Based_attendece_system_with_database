#ifndef RTC_H
#define RTC_H

void RTC_Init(void);
void GetRTCTimeInfo(u32 *,u32 *,u32 *);
void DisplayRTCTime(u32,u32,u32);
void GetRTCDateInfo(u32 *,u32 *,u32 *);
void DisplayRTCDate(u32,u32,u32);

void SetRTCTimeInfo(u32,u32,u32);
void SetRTCDateInfo(u32,u32,u32);

void GetRTCDay(u32 *);
void DisplayRTCDay(u32);
void SetRTCDay(u32);

void SetRTCHour(u32);
void SetRTCMin(u32);
void SetRTCSec(u32);
void SetRTCDate(u32);
void SetRTCMonth(u32);
void SetRTCYear(u32);
void SetRTCDay(u32);

void GetRTCHour(s32 *);
void GetRTCMin(s32 *);
void GetRTCSec(s32 *);
void GetRTCDate(s32 *);
void GetRTCMonth(s32 *);
void GetRTCYear(s32 *);
void GetDay(void);


#endif
