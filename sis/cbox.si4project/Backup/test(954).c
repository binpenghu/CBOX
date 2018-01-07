
/****************************************************************************
// Copyright (c) 2017
//
// @Module        
// @Filename      
// @Project       
// @Author        HBP
//----------------------------------------------------------------------------
// @Controller    STM8S
//
// @Compiler      IAR
//
// @Description   
//
//----------------------------------------------------------------------------
// @Date          2017
// @Version       V1.0
****************************************************************************/

//****************************************************************************
// @Project Includes
//****************************************************************************
#include "display.h"
#include "key.h"
//****************************************************************************
// @Macros
//****************************************************************************

//****************************************************************************
// @Defines
//****************************************************************************

//****************************************************************************
// @Typedefs
//****************************************************************************
 
//****************************************************************************
// @Imported Global Variables
//****************************************************************************

//****************************************************************************
// @Global Variables
//****************************************************************************

//****************************************************************************
// @Prototypes Of Static Functions
//****************************************************************************

//****************************************************************************
// @Prototypes Of Global Functions
//****************************************************************************
u8 keyValTest=0;
u8 test_part = 2;
/******************************************************************************
 *  Function:
 *  Parameters: None
 *  Returns: None
 *  Description:   
 ******************************************************************************/
 void test_key_task(void)
{
	static u8 temp = 0;
	temp = keyValTest;
	if (temp <=0)
		return;
	if (temp <=10)
	{
		test_part=1;
		dispaly_num(temp);
	}else if (temp <=12)
	{
		dispaly_num(100+temp);
	}else if (temp <= 13)
	{
		display_machineMode_led(ON);
		//display_chargeFinish_led(OFF);
	}else if (temp <= 14)
	{
		//display_machineMode_led(OFF);
		display_chargeFinish_led(ON);
	}else if (temp <= 22)
	{
		display_led((temp-15),ON);
	}else
	{
		display_machineMode_led(OFF);
		display_chargeFinish_led(OFF);
		display_led(LED_ALL_CLOSED_CMD,OFF);
		keyValTest = 0;
		test_part = 2;
		
	}
}

 /******************************************************************************
  *  Function:
  *  Parameters: None
  *  Returns: None
  *  Description:	
  ******************************************************************************/
 void test_displayAd(void)
 {
	u16 adval=0;
	u16 bat_val = 0;
	if (test_part == 2)
	{
		adval = adcSingleRead(ADC1_CHANNEL_5);
		bat_val = adval*0.7;
		dispaly_num(bat_val);
        delay_ms(400);
	}

 }

//****************************************************************************
// @Interrupt Vectors
//****************************************************************************

/*
 *######################################################################
 *                           End of File
 *######################################################################
 */





