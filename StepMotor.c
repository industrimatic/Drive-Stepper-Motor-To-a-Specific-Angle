//PB1 = STEP =TIM3_CH4
//PB0 = DIR
#include "stm32f10x.h"                  // Device header
#include "Delay.h"
void StepMotor_PWM_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;		
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_WriteBit(GPIOB,GPIO_Pin_0,Bit_SET);
	
	TIM_InternalClockConfig(TIM3);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 1000 - 1;		//ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler = 36 - 1;		//PSC
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);
	
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 500;		//CCR
	TIM_OC4Init(TIM3, &TIM_OCInitStructure);
	
	TIM_Cmd(TIM3, ENABLE);
	
}

void StepMotor_Pulse_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_WriteBit(GPIOB,GPIO_Pin_0,Bit_RESET);
	GPIO_WriteBit(GPIOB,GPIO_Pin_1,Bit_RESET);
}

void StepMotor_Pulse(void) //发送一次脉冲
{
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)==Bit_SET) //要求脉冲的前沿必须是低电平
	{
		GPIO_WriteBit(GPIOB,GPIO_Pin_1,Bit_RESET);
	}
	else
	{
		GPIO_WriteBit(GPIOB,GPIO_Pin_1,Bit_SET);
		Delay_us(200);	//这里的delay决定一次脉冲的通频带时间，不要太小，也不要太高，依据你自己的电机而定 
		GPIO_WriteBit(GPIOB,GPIO_Pin_1,Bit_RESET);
		Delay_us(200);
	}
}

void StepMotor_NPulse(uint32_t N,uint32_t time,uint8_t direction)//两次有效边沿间隔的时间为三次us的delay加起来
	//   转速=步距角/(两次有效边沿间隔的时间*60) 单位:RPM,°,s
	//	 例：本人的步进电机步距角1.8°，四分，因此这里应该用0.45°代入
{
	if(direction == 1)
	{
		GPIO_WriteBit(GPIOB,GPIO_Pin_0,Bit_RESET);
	}
	else if(direction == 2)
	{
		GPIO_WriteBit(GPIOB,GPIO_Pin_0,Bit_SET);
	}
	for(uint32_t i = 0 ; i < N ; i++)
	{
		StepMotor_Pulse();
		Delay_us(time);
	}
}
