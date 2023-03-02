/**************************** Author:  Yousef Ayman        **************************/
/**************************** Project: Advanced Stopwatch  **************************/
/**************************** File: main.c                 **************************/
/**************************** Version: V1.0                **************************/


/********************************************* Utils Files inclusions  ************************************/
#define F_CPU  8000000
#include <util/delay.h>
#include "STD_TYPES.h"

/******************************************** MCAL inclusions ********************************************/
#include "DIO_interface.h"
#include "Timers_interface.h"

/***************************************** HAL inclusions ***********************************************/
#include "Keypad_interface.h"
#include "LCD_interface.h"

/*************************************** Services *******************************************************/
#include "TimersDelay_interface.h"


/************************************** DEFINES and MACROS ********************************************/

/* LEDS */
#define YELLOW_LED   Pin6
#define BLUE_LED	 Pin5
#define GREEN_LED	 Pin4
#define RED_LED		 Pin7

/* LCD */
#define CURSOR_SET_LINE_POSITION      4
#define CURSOR_POSITION_HOUR_1_DIGIT  2  
#define CURSOR_POSITION_HOUR_2_DIGIT  1
#define CURSOR_POSITION_MIN_1_DIGIT   5
#define CURSOR_POSITION_MIN_2_DIGIT   4
#define CURSOR_POSITION_SEC_1_DIGIT   8
#define CURSOR_POSITION_SEC_2_DIGIT   7

/* Keypad Buttons configurations */
#define PAUSE_KEY 'c'
#define RESET_KEY '0'


/************************************ Global Vars ***************************************************/
s32 count = 0;
s32 resetcount = 0;
u8 Hour = 0;
u8 Min = 0;
u8 Seconds = 0;
u8 CountDownflag = 0;
u8 PausedFlag = 0;


/********************************* Fucntions Implementations **************************************/

void ConvertSeconds(u32 LocalCounts)
{
	if(LocalCounts == 0)
	{
		return;
	}
	Hour = (LocalCounts/3600);
	
	Min = (LocalCounts - ((u32)3600*Hour))/60;
	
	Seconds = (LocalCounts -((u32)3600*Hour)-((u32)Min*60));
}


void DisplayTime(void)
{
	if(Hour<=9)
	{
		if(Hour == 0)
		{
			LCD_SetCursor(CURSOR_SET_LINE_POSITION,CURSOR_POSITION_HOUR_1_DIGIT);
			LCD_Write_CHAR('0');
		}
		LCD_SetCursor(CURSOR_SET_LINE_POSITION,CURSOR_POSITION_HOUR_1_DIGIT);
		LCD_WriteNUM(Hour);
	}
	else
	{
		LCD_SetCursor(CURSOR_SET_LINE_POSITION,CURSOR_POSITION_HOUR_2_DIGIT);
		LCD_WriteNUM(Hour);
	}
	
	if(Min<=9)
	{
		if(Min == 0)
		{
			LCD_SetCursor(CURSOR_SET_LINE_POSITION,CURSOR_POSITION_MIN_1_DIGIT);
			LCD_Write_CHAR('0');
		}
		else
		{
			LCD_SetCursor(CURSOR_SET_LINE_POSITION,CURSOR_POSITION_MIN_1_DIGIT);
			LCD_WriteNUM(Min);
		}

	}
	else
	{
		LCD_SetCursor(CURSOR_SET_LINE_POSITION,CURSOR_POSITION_MIN_2_DIGIT);
		LCD_WriteNUM(Min);
	}
	
	if(Seconds<=9)
	{
		LCD_SetCursor(CURSOR_SET_LINE_POSITION,CURSOR_POSITION_SEC_2_DIGIT);
		LCD_Write_CHAR('0');
		
		if(Seconds == 0)
		{
			LCD_SetCursor(CURSOR_SET_LINE_POSITION,CURSOR_POSITION_SEC_1_DIGIT);
			LCD_Write_CHAR('0');
		}
		else
		{
			LCD_SetCursor(CURSOR_SET_LINE_POSITION,CURSOR_POSITION_SEC_1_DIGIT);
			LCD_WriteNUM(Seconds);
		}
	}
	else
	{
		LCD_SetCursor(CURSOR_SET_LINE_POSITION,CURSOR_POSITION_SEC_2_DIGIT);
		LCD_WriteNUM(Seconds);
	}
}

void StopWatch_Count (void)
{
	if(count == 0)
	{
		TIMER1_Stop();
		CountDownflag = 1;
	}
	count--;
}

void StopWatch_clear(void)
{
	LCD_SetCursor(4,1);
	LCD_WriteString("00:00:00");
}

void StopWatch_reset(void)
{
	Hour = 0;
	Min = 0;
	Seconds = 0;
	count = resetcount;
}

void StopWatch_Initializing(void)
{
	u8 key = NO_KEY;
	u16 Hour = 0;
	u16 Min = 0;
	u16 Sec = 0;
	u8 flag = 0;
	
	LCD_SetCursor(1,1);
	LCD_WriteString("Hour:");
	LCD_SetCursor(2,1);
	LCD_WriteString("Min:");
	LCD_SetCursor(3,1);
	LCD_WriteString("Sec:");
	
	
	
	DIO_SetPinValue(DIO_PORTA,BLUE_LED,HIGH);
	LCD_SetCursor(1,6);
	
	while(key != '+')
	{
		key = Keypad_GetValue();
		
		if(key >= '0' && key <= '9')
		{
			LCD_Write_CHAR(key);
			if(flag == 0)
			{
				/* Hour */
				Hour = Hour * 10 + (key - '0');
			}
			else if(flag == 1)
			{
				/* Min */
				Min = Min * 10 + (key - '0');
			}
			else if(flag == 2)
			{
				/* Sec */
				Sec = Sec * 10 + (key - '0');
			}
		}
		else if(key == 'c')
		{
			if(flag == 0)
			{
				LCD_SetCursor(2,5);
				
				/* Hour */
				count += (Hour * 3600);
				flag = 1;
			}
			else if(flag == 1)
			{
				LCD_SetCursor(3,5);
				/* Min */
				count += (Min * 60);
				flag = 2;
				
			}
			else if(flag == 2)
			{
				/* Sec */
				count += Sec;
				resetcount = count;
				break;
			}
		}
	}
	
	DIO_SetPinValue(DIO_PORTA,BLUE_LED,LOW);
}

int main(void)
{
	GIE();
	u8 key = NO_KEY;
	
	
	/* Initializing LEDS */
	DIO_SetPinDir(DIO_PORTA,GREEN_LED,OUTPUT);
	DIO_SetPinDir(DIO_PORTA,BLUE_LED,OUTPUT);
	DIO_SetPinDir(DIO_PORTA,YELLOW_LED,OUTPUT);
	DIO_SetPinDir(DIO_PORTB,RED_LED,OUTPUT);
	
	/* Initializing keypad and LCD*/
	Keypad_Init();
	LCD_init();
	
	/* Stop watch initialization */
	StopWatch_Initializing();
	StopWatch_clear();
	_delay_ms(100);
	ConvertSeconds(count);
	/* Blink off the cursor */
	LCD_Write_CMD(0x0C);  
	

	/* Initalizing the timer */
	TIMER1_Init(TIMER1_CTC_OCRA_TOP_MODE);
	TIMER1_Start(TIMER1_SCALER_8);
	Timer1_Delay_s(1,StopWatch_Count);
	
	
	
	while (1)
	{
		key = Keypad_GetValue();
		ConvertSeconds(count);
		DisplayTime();
		
		if(CountDownflag == 1)
		{
			DIO_SetPinValue(DIO_PORTB,RED_LED,HIGH);
			StopWatch_clear();
			CountDownflag = 0;
			count = 0;
			Hour = 0;
			Seconds = 0;
			Min = 0;
		}
		
		if(key == PAUSE_KEY)
		{
			/* Pause the stopwatch */
			if(PausedFlag == 0)
			{
				TIMER1_Stop();
				PausedFlag = 1;
				DIO_SetPinValue(DIO_PORTA,GREEN_LED,HIGH);
			}
			else
			{
				TIMER1_Start(TIMER1_SCALER_8);
				DIO_SetPinValue(DIO_PORTA,GREEN_LED,LOW);
				PausedFlag = 0;
			}
			
		}
		if(key == RESET_KEY)
		{
			/* Reset the stopwatch */
			StopWatch_clear();
			StopWatch_reset();
			DIO_SetPinValue(DIO_PORTB,RED_LED,LOW);
			PausedFlag = 1;
		}
	}
}





