#ifndef TIMERSDELAY_INTERFACE_H_
#define TIMERSDELAY_INTERFACE_H_


void Timer0_Delay_S(u8 seconds, void (*LocalFptr) (void));

void Timer1_Delay_s(u16 Time, void (*localFptr)(void));


#endif /* TIMERSDELAY_INTERFACE_H_ */