
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
#include "adConvert.h"
//****************************************************************************
// @Macros
//****************************************************************************

//****************************************************************************
// @Defines
//****************************************************************************
#define V_BACK_CHN   ADC1_CHANNEL_5
#define V_IN_CHN 	 ADC1_CHANNEL_2
#define I_BACK_CHN	 ADC1_CHANNEL_3
#define I_USB_CHN    ADC1_CHANNEL_4
//****************************************************************************
// @Typedefs
//****************************************************************************


typedef enum
{
	V_BACK_STEP=0,
	V_IN_STEP,
	I_BACK_STEP,
	I_USB_STEP,
	MAX_STEP
}AD_STEP_T;




//****************************************************************************
// @Imported Global Variables
//****************************************************************************

//****************************************************************************
// @Global Variables
//****************************************************************************
const float chargeBatCurOffset = 0.175;   //unit 0.1A
const float chargeUsbCurOffset = 0.025;   //unit 0.1A
const float batterVoltOffset = 0.7; 	  //unit 0.1V
const float inVoltOffset = 0.7; 	  	  //unit 0.1V

AD_IPNUT_T adInputData = {0};


//****************************************************************************
// @Prototypes Of Static Functions
//****************************************************************************

//****************************************************************************
// @Prototypes Of Global Functions
//****************************************************************************
/******************************************************************************
 *  Function:
 *  Parameters: None
 *  Returns: None
 *  Description:   
 ******************************************************************************/
u16 vBackConvert(u16 adval)
{
	u16 temp=0;
	temp = (u16)(adval*batterVoltOffset); //0.1v
	return temp;
}

u16 vInConvert(u16 adval)
{
	u16 temp=0;
	temp = (u16)(adval*inVoltOffset);//0.1v
	return temp;

}

u16 iBackConvert(u16 adval)
{
	u16 temp=0;
	temp = (u16)(adval *chargeBatCurOffset); // 0.1A
	return temp;
}

u16 iUsbConvert(u16 adval)
{
	u16 temp=0;
	temp = (u16)(adval *chargeUsbCurOffset); // 0.1A
	return temp;
}

/******************************************************************************
 *  Function:
 *  Parameters: None
 *  Returns: None
 *  Description:  four channels ad convert. each 25ms period, 
 
 ******************************************************************************/
void adDetectTask(void)
{
	static AD_STEP_T step =V_BACK_STEP;
	u16 temp=0;
	switch (step)
	{
		case V_BACK_STEP:
			temp = adcSingleRead(V_BACK_CHN); 
			adInputData.bat_adval = vBackConvert(temp);
			step++;
			break;
		case V_IN_STEP:
			temp = adcSingleRead(V_IN_CHN); 
			adInputData.in_adval = vInConvert(temp);
			step++;
			break;
		case I_BACK_STEP:
			temp = adcSingleRead(I_BACK_CHN);
			adInputData.bat_charge_adval = iBackConvert(temp);
			step++;
			break;
		case I_USB_STEP:
			temp = adcSingleRead(I_USB_CHN);
			adInputData.usb_adval = iUsbConvert(temp);
			step=V_BACK_STEP;
			break;
		default:
			step=V_BACK_STEP;
			break;
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






