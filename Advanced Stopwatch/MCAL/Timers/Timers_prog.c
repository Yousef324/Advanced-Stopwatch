#include "STD_TYPES.h"
#include "BIT_MATH.h"
#include "Timers_private.h"
#include "Timers_interface.h"


/***************************************************************************************************/
/******************************************** TIMER 0 *********************************************/
/***************************************************************************************************/


void Timer0_Init(Timer0Mode_type Mode)
{	
	/* Selecting the mode */
	switch(Mode)
	{
		case TIMER0_NORMAL_MODE:
		CLR_BIT(TCCR0,WGM00);
		CLR_BIT(TCCR0,WGM01);
		break;
		case TIMER0_PHASECORRECT_MODE:
		SET_BIT(TCCR0,WGM00);
		CLR_BIT(TCCR0,WGM01);
		break;
		case TIMER0_CTC_MODE:
		CLR_BIT(TCCR0,WGM00);
		SET_BIT(TCCR0,WGM01);
		break;
		case TIMER0_FASTPWM_MODE:
		SET_BIT(TCCR0,WGM00);
		SET_BIT(TCCR0,WGM01);
		break;
	}
} 

void TIMER0_OC_Mode(OC0Mode_type Mode)
{
	switch(Mode)
	{
		case OC0_DISCONNECTED:
		CLR_BIT(TCCR0,COM00);
		CLR_BIT(TCCR0,COM01);
		break;
		case OC0_TOGGLE:
		SET_BIT(TCCR0,COM00);
		CLR_BIT(TCCR0,COM01);
		break;
		case OC0_NON_INVERTING:
		CLR_BIT(TCCR0,COM00);
		SET_BIT(TCCR0,COM01);
		break;
		case OC0_INVERTING:
		SET_BIT(TCCR0,COM00);
		SET_BIT(TCCR0,COM01);
		break;
	}
}

void Timer0_Start(Timer0Scaler_type Scaler)
{
	/* Initializing the Timer0 prescaler */
	TCCR0 = TCCR0 & 0xF8;
	TCCR0 = TCCR0 | Scaler;
}

void Timer0_Stop(void)
{
	/* Selecting the Timer0 prescaler to zero */
	CLR_BIT(TCCR0,CS00);
	CLR_BIT(TCCR0,CS01);
	CLR_BIT(TCCR0,CS02);
}

void Timer0_Set(u8 value)
{
	TCNT0 = value;
}

void TIMER0_OV_InterruptEnable(void)
{
	SET_BIT(TIMSK,TOIE0);
}

void TIMER0_OV_InterruptDisable(void)
{
	CLR_BIT(TIMSK,TOIE0);
}

void TIMER0_OC_InterruptEnable(void)
{
	SET_BIT(TIMSK,OCIE0);
}

void TIMER0_OC_InterruptDisable(void)
{
	CLR_BIT(TIMSK,OCIE0);
}

static void (*GlobalFptr)(void) = NULL_PTR;
void TIMER0_SetCallBackTime(Timer0InterruptSource_type type,void (*LocalFptr) (void))
{
	switch (type)
	{
		case OVF0_INT:
		GlobalFptr = LocalFptr;
		break;
		
		case OC0_INT:
		GlobalFptr = LocalFptr;
		break;
	}
	
}

ISR(TIMER0_OVF_vect)
{
	if(GlobalFptr != NULL_PTR)
	{
		GlobalFptr();
	}
}

ISR(TIMER0_OC_vect)
{
	if(GlobalFptr != NULL_PTR)
	{
		GlobalFptr();
	}
}



/***************************************************************************************************/
/******************************************** TIMER 1 **********************************************/
/***************************************************************************************************/

void TIMER1_Init(Timer1Mode_type mode)
{
	//Select Mode
	switch(mode)
	{
		case TIMER1_NORMAL_MODE:
		CLR_BIT(TCCR1A,WGM10);
		CLR_BIT(TCCR1A,WGM11);
		CLR_BIT(TCCR1B,WGM12);
		CLR_BIT(TCCR1B,WGM13);
		break;
		case TIMER1_CTC_ICR_TOP_MODE:
		CLR_BIT(TCCR1A,WGM10);
		CLR_BIT(TCCR1A,WGM11);
		SET_BIT(TCCR1B,WGM12);
		SET_BIT(TCCR1B,WGM13);
		break;
		case TIMER1_CTC_OCRA_TOP_MODE:
		CLR_BIT(TCCR1A,WGM10);
		CLR_BIT(TCCR1A,WGM11);
		SET_BIT(TCCR1B,WGM12);
		CLR_BIT(TCCR1B,WGM13);
		break;
		case TIMER1_PHASECORRECT_ICR_TOP_MODE:
		CLR_BIT(TCCR1A,WGM10);
		SET_BIT(TCCR1A,WGM11);
		CLR_BIT(TCCR1B,WGM12);
		SET_BIT(TCCR1B,WGM13);
		break;
		case TIMER1_PHASECORRECT_OCRA_TOP_MODE:
		SET_BIT(TCCR1A,WGM10);
		SET_BIT(TCCR1A,WGM11);
		CLR_BIT(TCCR1B,WGM12);
		SET_BIT(TCCR1B,WGM13);
		break;
		case TIMER1_FASTPWM_ICR_TOP_MODE:
		CLR_BIT(TCCR1A,WGM10);
		SET_BIT(TCCR1A,WGM11);
		SET_BIT(TCCR1B,WGM12);
		SET_BIT(TCCR1B,WGM13);
		break;
		case TIMER1_FASTPWM_OCRA_TOP_MODE:
		SET_BIT(TCCR1A,WGM10);
		SET_BIT(TCCR1A,WGM11);
		SET_BIT(TCCR1B,WGM12);
		SET_BIT(TCCR1B,WGM13);
		break;
	}

	
}

void TIMER1_Start(Timer1Scaler_type scaler)
{
	//prescaler (clock)
	TCCR1B = TCCR1B  & 0xf8  ; //11111000
	scaler = scaler  & 0x07  ; //00000111
	TCCR1B = TCCR1B  | scaler;
}

void TIMER1_Stop(void)
{
	CLR_BIT(TCCR1B,CS10);
	CLR_BIT(TCCR1B,CS11);
	CLR_BIT(TCCR1B,CS12);
	
	CLR_BIT(TCCR1A,CS10);
	CLR_BIT(TCCR1A,CS11);
	CLR_BIT(TCCR1A,CS12);
}

void TIMER1_OCRA1_Mode(OC1A_Mode_type oc1a_mode)
{
	switch(oc1a_mode)
	{
		case OCRA_DISCONNECTED:
		CLR_BIT(TCCR1A,COM1A0);
		CLR_BIT(TCCR1A,COM1A1);
		break;
		case OCRA_TOGGLE:
		SET_BIT(TCCR1A,COM1A0);
		CLR_BIT(TCCR1A,COM1A1);
		break;
		case OCRA_NON_INVERTING:
		CLR_BIT(TCCR1A,COM1A0);
		SET_BIT(TCCR1A,COM1A1);
		break;
		case OCRA_INVERTING:
		SET_BIT(TCCR1A,COM1A0);
		SET_BIT(TCCR1A,COM1A1);
		break;
	}
}

void TIMER1_OCRB1_Mode(OC1B_Mode_type oc1b_mode)
{
	switch(oc1b_mode)
	{
		case OCRB_DISCONNECTED:
		CLR_BIT(TCCR1A,COM1B0);
		CLR_BIT(TCCR1A,COM1B1);
		break;
		case OCRB_TOGGLE:
		SET_BIT(TCCR1A,COM1B0);
		CLR_BIT(TCCR1A,COM1B1);
		break;
		case OCRB_NON_INVERTING:
		CLR_BIT(TCCR1A,COM1B0);
		SET_BIT(TCCR1A,COM1B1);
		break;
		case OCRB_INVERTING:
		SET_BIT(TCCR1A,COM1B0);
		SET_BIT(TCCR1A,COM1B1);
		break;
	}
}

void TIMER1_InputCaptureEdge(ICU_Edge_type edge)
{
	switch(edge)
	{
		case RISING:
		SET_BIT(TCCR1B,ICES1);
		break;
		case FALLING:
		CLR_BIT(TCCR1B,ICES1);
		break;
	}
}

/*************************** Enable/Disable Functions **************************/
void TIMER1_ICU_InterruptEnable(void)
{
	SET_BIT(TIMSK,TICIE1);
}
void TIMER1_ICU_InterruptDisable(void)
{
	CLR_BIT(TIMSK,TICIE1);
}

void TIMER1_OVF_InterruptEnable(void)
{
	SET_BIT(TIMSK,TOIE1);
}
void TIMER1_OVF_InterruptDisable(void)
{
	CLR_BIT(TIMSK,TOIE1);
}

void TIMER1_OCA_InterruptEnable(void)
{
	SET_BIT(TIMSK,OCIE1A);
}
void TIMER1_OCA_InterruptDisable(void)
{
	CLR_BIT(TIMSK,OCIE1A);
}

void TIMER1_OCB_InterruptEnable(void)
{
	SET_BIT(TIMSK,OCIE1B);
}
void TIMER1_OCB_InterruptDisable(void)
{
	CLR_BIT(TIMSK,OCIE1B);
}

/*************************** Read/Write Functions **************************/
void Timer1_Set(u16 value)
{
	TCNT1  = value;
}

u16 Timer1_OVF_Read(void)
{
	return TCNT1 ;
}

void Timer1_OCA_Set(u16 value)
{
	OCR1A  = value;
}

u16 Timer1_OCA_Read(void)
{
	return OCR1A;
}

void Timer1_OCB_Set(u16 value)
{
	OCR1B  = value;
}

u16 Timer1_OCB_Read(void)
{
	return OCR1B ;
}


void Timer1_ICR1_Set(u16 value)
{
	ICR1   = value;
}
u16 Timer1_ICR1_Read(void)
{
	return ICR1 ;
}


/*************************** Pointer to Functions to be assigned to ISR **************************/
//we make it static to not be edited by any one in another files
//concept of setter and getter
static void (*Fptr_OVF1_INT)(void) = NULL_PTR;
static void (*Fptr_OCA_INT)(void) = NULL_PTR;
static void (*Fptr_OCB_INT)(void) = NULL_PTR;
static void (*Fptr_ICU1_INT)(void) = NULL_PTR;
/**************************************** Call Back Functions ************************************/
void TIMER1_SetCallBack(Timer1InterruptSource_type Interrupt,void(*Local_fptr)(void))
{
	switch(Interrupt)
	{
		case OVF1_INT:
		Fptr_OVF1_INT=Local_fptr;
		break;
		case OCA_INT:
		Fptr_OCA_INT=Local_fptr;
		break;
		case OCB_INT:
		Fptr_OCB_INT=Local_fptr;
		break;
		case ICU1_INT:
		Fptr_ICU1_INT=Local_fptr;
		break;
	}
}
/********************************************** ISR **********************************************/
ISR(TIMER1_OVF_vect)
{
	if(Fptr_OVF1_INT != NULL_PTR)
	{
		Fptr_OVF1_INT();
	}
}
ISR(TIMER1_OCA_vect)
{
	if(Fptr_OCA_INT != NULL_PTR)
	{
		Fptr_OCA_INT();
	}
}
ISR(TIMER1_OCB_vect)
{
	if(Fptr_OCB_INT != NULL_PTR)
	{
		Fptr_OCB_INT();
	}
}
ISR(TIMER1_ICU_vect)
{
	if(Fptr_ICU1_INT != NULL_PTR)
	{
		Fptr_ICU1_INT();
	}
}