#include "STD_TYPES.h"
#include "Timers_interface.h"

static u32 NoOfOverflow_0 = 0;
void (*GlobalFptr)(void) = NULL_PTR;
static void Timer0_Func(void);

/***************************************************** USING TIMER 0 ********************************************/
void Timer0_Delay_S(u8 seconds, void (*LocalFptr) (void))
{
	Timer0_Init(TIMER0_NORMAL_MODE);
	NoOfOverflow_0 = ((u32)seconds * 1000000)/250;
	Timer0_Start(TIMER0_SCALER_8);
	Timer0_Set(6);
	GlobalFptr = LocalFptr;
	TIMER0_SetCallBackTime(OVF0_INT,Timer0_Func);
	TIMER0_OV_InterruptEnable();
}

static void Timer0_Func(void)
{
	static u32 count = 0;
	count++;
	if(count == NoOfOverflow_0)
	{
		GlobalFptr();
		count = 0;
		Timer0_Set(6);
	}
}

/****************************************** USING TIMER 1  ****************************************/

static u32 NoOfOverflow_1 = 0;
void (*Fptr_Timer_s)(void) = NULL_PTR;
static void Timer1_Func(void);

void Timer1_Delay_s(u16 Time, void (*localFptr)(void))
{
	Timer1_OCA_Set(999);
	NoOfOverflow_1 = Time * 1000;
	Fptr_Timer_s = localFptr;
	TIMER1_SetCallBack(OCA_INT,Timer1_Func);
	TIMER1_OCA_InterruptEnable();
}

static void Timer1_Func(void)
{
	static u32 count = 0;
	count++;
	if(count == NoOfOverflow_1)
	{
		count = 0;
		Fptr_Timer_s();
	}
}