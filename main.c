#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "StepMotor.h"
#include <string.h>

int main(void)
{
	OLED_Init();
	
	OLED_ShowString(1,1,"system loaded");

	StepMotor_Pulse_Init();
	
	while(1)
	{
		StepMotor_NPulse(1600,200,1);
		Delay_ms(1000);
		StepMotor_NPulse(1600,200,2);
		Delay_ms(1000);
		
	}
}
