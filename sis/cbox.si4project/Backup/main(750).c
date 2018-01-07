/**
  ******************************************************************************
  * @file    Project/main.c 
  * @author  MCD Application Team
  * @version V2.2.0
  * @date    30-September-2014
  * @brief   Main program body
   ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */ 


/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"
#include "bsp.h"

#include "led.h"
#include "delay.h"
#include "display.h"
#include "key.h"
#include "logic.h"
#include "adConvert.h"
/* Private defines -----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


extern void test_key_task();
extern  void test_displayAd(void);
u16 adval=0;
SYS_TICK_T sysTick={0,0,0,0,0,0};

void main(void)
{
  disableInterrupts();
  sysInit();
  TM1650_Init();
  tim1_init();
  tim4_init();
 // LED_Init();
  logic_init();
  enableInterrupts();
  //M4Start(500, LED_TRI);// for test
  /* Infinite loop */
  while (1)
  {
  	if (sysTick.period_10ms)
  	{  		
  		sysTick.period_10ms=0;
  	}
  	if (sysTick.period_20ms)
  	{
  		sysTick.period_20ms=0;
		adDetectTask();
  	}
  	if (sysTick.period_100ms)
  	{
  		sysTick.period_100ms=0;
  		// add user task
  		logicTask();
  	}
  	if (sysTick.period_1s)
  	{
  		sysTick.period_1s=0;
  		//LED_TRI();
  	}
  }
}


#define TIM4_PERIOD 100
void TIM4_Config(void)
{
  /* TIM4 configuration:
   - TIM4CLK is set to 16 MHz, the TIM4 Prescaler is equal to 128 so the TIM1 counter
   clock used is 16 MHz / 128 = 125 000 Hz
  - With 125 000 Hz we can generate time base:
      max time base is 2.048 ms if TIM4_PERIOD = 255 --> (255 + 1) / 125000 = 2.048 ms
      min time base is 0.016 ms if TIM4_PERIOD = 1   --> (  1 + 1) / 125000 = 0.016 ms
  - In this example we need to generate a time base equal to 1 ms
   so TIM4_PERIOD = (0.001 * 125000 - 1) = 124 */

  /* Time base configuration */
  TIM4_TimeBaseInit(TIM4_PRESCALER_128, TIM4_PERIOD);
  /* Clear TIM4 update flag */
  TIM4_ClearFlag(TIM4_FLAG_UPDATE);
  /* Enable update interrupt */
  TIM4_ITConfig(TIM4_IT_UPDATE, ENABLE);
  
  /* enable interrupts */
  enableInterrupts();

  /* Enable TIM4 */
  TIM4_Cmd(ENABLE);	
}
#ifdef USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param file: pointer to the source file name
  * @param line: assert_param error line source number
  * @retval : None
  */
void assert_failed(u8* file, u32 line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif
//****************************************************************************
// @Interrupt Vectors
//****************************************************************************
void tim1_vector(void)
{
	TIM1_ClearITPendingBit(TIM1_IT_UPDATE);
	sysTick.cnt++;
	if(sysTick.cnt%10==0)
	{
		sysTick.period_10ms=1;
	}
	if (sysTick.cnt%20==0)
	{
		sysTick.period_20ms=1;
	}
	if (sysTick.cnt%50==0)
	{
		sysTick.period_50ms=1;
	}
	if (sysTick.cnt%100==0)
	{
		sysTick.period_100ms=1;
	}
	if (sysTick.cnt%200==0)
	{
		sysTick.period_200ms=1;
	}
	if (sysTick.cnt%500==0)
	{
		sysTick.period_500ms=1;
	}
	if (sysTick.cnt%1000==0)
	{
		sysTick.period_1s=1;
		sysTick.cnt=0;
	}
	/*
	if(sysTick.cnt>=65535)
	{
		sysTick.cnt++;
		//temp = sysTick.cnt;		
	}*/
	//sysTick.cnt = sysTick.cnt>=65535?0: sysTick.cnt++;
}



/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
