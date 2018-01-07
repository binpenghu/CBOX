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
#include "Logic.h"
//****************************************************************************
// @Macros
//****************************************************************************

//****************************************************************************
// @Defines
//****************************************************************************

//****************************************************************************
// @Typedefs
//****************************************************************************


typedef enum
{
	BAT_6V=0,
	BAT_12V,
	BAT_24V,
	BAT_36V,
	BAT_48V,
	BAT_MAX
}BAT_VOL_T;

typedef struct
{
	u16 batLowVolt;
	u16 batHighVolt;
	u16 batNorVolt;
	u16 batOffset;
	u16 batMaxErrVolt;  //=batHighVolt+0.1v*(6V基数)
}BAT_VOLTAGE_T;


typedef enum
{
	BAT_IS_NONE,
	BAT_IS_NORMAL,
	BAT_IS_REVERSE,
	BAT_IS_ERROR
}BAT_RTN_T;

typedef enum
{
	USB_NOT_CHARGE,
	USB_CUR_1A,
	USB_CUR_2A
}USB_RTN_T;

typedef enum
{
   IN_CHARGE,
   IN_FIX,
   IN_NONE,
   IN_AI_ERR
}IN_RTN_T;

typedef struct
{
	u8 batRtnSta;
	u8 inRtnSta;
	u8 usbRtnSta;
	u8 keyRtnSta;	
}CHARGE_DATA_T;

typedef struct
{
	u8 oldSta;
	u8 curSta;
}RTN_STA_T;

typedef struct
{
	KEY_VALUE_E oldKey;
	KEY_VALUE_E curKey;
}KEY_STA_T;

typedef struct
{
	u8 batType;
	u8 batChargeSta;	
	u16 batVoltage;
	u16 batCurrent;
	u16 batPercent;
	u16 batCapacity;
	u16 InVolt;
	u16 usbCurr;
}BAT_INFO_T;

//****************************************************************************
// @Imported Global Variables
//****************************************************************************
const BAT_VOLTAGE_T batTable[BAT_MAX]= // unit is 0.1v
{
	{48,72,55,5,73},		//6v = 60*0.1v
	{96,143,114,5,145},		//12v
	{192,286,228,5,290},	//24v
	{288,432,331,5,438},	//36v
	{440,576,441,5,584},	//48v
};
//****************************************************************************
// @Global Variables
//****************************************************************************
//CHARGE_DATA_T chargeSta;
BAT_INFO_T batInfo;

//****************************************************************************
// @Prototypes Of Static Functions
//****************************************************************************
static void finishChargeAction(void);

//****************************************************************************
// @Prototypes Of Global Functions
//****************************************************************************

/******************************************************************************
 *  Function:
 *  Parameters: None
 *  Returns: None
 *  Description:
 ******************************************************************************/
void logic_init()
{
	batInfo.batType = BAT_MAX;
	batInfo.batVoltage=0;
	batInfo.batCurrent=0;
	batInfo.batPercent=0;
	batInfo.batCapacity=0;
	batInfo.batChargeSta = 0;
	batInfo.InVolt=0;
	batInfo.usbCurr=0;
	setChargeCtrlSta(OFF); // relay1 default off
	set5VResource(D5V_FROM_BAT); // default by battery
}
/******************************************************************************
 *	Function:
 *	Parameters: None
 *	Returns:
 *	Description:   电瓶电压是否在范围内。
 ******************************************************************************/

u8 judgeBatType(u16 batval)
{
	u8 i=0;
	for (i=0;i<BAT_MAX;i++)
	{
		if (batval <= (batTable[i].batHighVolt+5) && batval >= batTable[i].batLowVolt)
		{
			return i;
		}
	}
	return 0xff;
}
/******************************************************************************
 *	Function:
 *	Parameters: None
 *	Returns:
 *	Description:   电瓶电压是否在范围内。
 ******************************************************************************/
 
u8 judgeBatStatus(u16 batval)
{
	//u16 batVal = 0;
	if (BAT_REVERSE == getBatConnectSta())
	{
		return BAT_IS_REVERSE;
	}
	if (batval >= 10)// >1V
	{
		if (batInfo.batType >= BAT_MAX)
		{
			batInfo.batType = judgeBatType(batval);
			if (batInfo.batType < BAT_MAX)
				return BAT_IS_NORMAL;
			//else
				//return BAT_IS_ERROR;
		}else 
		{
			if (batval <= batTable[batInfo.batType].batHighVolt && batval >= batTable[batInfo.batType].batLowVolt)
			{
				return BAT_IS_NORMAL;
			}
			//else 
			//{
				//return BAT_IS_ERROR;	
			//}
		}		
	}
	return BAT_IS_ERROR;	
	
}
/******************************************************************************
 *  Function:
 *  Parameters: None
 *  Returns: None
 *  Description:   判断是否进入充电模式
 ******************************************************************************/

u8 judgeInStatus(BAT_INFO_T* pbatInfo,u8 batStatus)
{
	u8 batType = pbatInfo->batType;
	u16 inval = pbatInfo->InVolt;
	u16 batVal = pbatInfo->batVoltage;
	
	if (batType >= BAT_MAX || batStatus != BAT_IS_NORMAL) // 无正常电瓶，不充电
		return IN_NONE;
	
	if (D24V_VALID ==  getD24Sta() && batStatus == BAT_IS_NORMAL)
	{
		if(inval <= batTable[batType].batMaxErrVolt &&
			inval >= batTable[batType].batNorVolt)
		{
			if (batVal < batTable[batType].batNorVolt)
				return IN_FIX;
			else			
				return IN_CHARGE;
		}
	}
	if (inval > batTable[batType].batMaxErrVolt)
	{
		return IN_AI_ERR;
	}
	//else
	//{
		return IN_NONE;
	//}
}
/******************************************************************************
 *  Function:
 *  Parameters: None
 *  Returns: None
 *  Description:
 ******************************************************************************/

u8 judgeUsbCurrent(u8 usbVal)
{
	if (usbVal > 0 && usbVal < 10)
	{
		return USB_CUR_1A;
	}
	else if(usbVal> 10)
	{
		return USB_CUR_2A;
	}
	else
	{
		return USB_NOT_CHARGE;
	}
}

/******************************************************************************
*  Function:
*  Parameters: None
*  Returns: None
*  Description:
******************************************************************************/

u16 getChargeBatVolt(BAT_INFO_T *bat)
{
	return bat->batVoltage;
}
	
u16 getChargeCurrent(BAT_INFO_T *bat)
{
	u16 curI = bat->batCurrent;	
    return curI;
}

u16 getChargePercent(BAT_INFO_T *bat)
{
	u16 curVolt = bat->batVoltage;
	u16 minVolt = batTable[bat->batType].batLowVolt;
	u16 maxVolt = batTable[bat->batType].batHighVolt;
	u16 percent = 0;
	if (curVolt>=minVolt && curVolt <= maxVolt)
	{
		percent = ((curVolt-minVolt)/(maxVolt-minVolt))*1000; // 0.1%
	}
	return percent;
}

u16 getChargeCapacity(BAT_INFO_T *bat)
{
	u16 curVolt = bat->batVoltage;
    return curVolt;
}

/******************************************************************************
*  Function:
*  Parameters: None
*  Returns: None
*  Description:
******************************************************************************/



void logicTask(void)
{
	static RTN_STA_T batRtn={0xff,0xff};
	static RTN_STA_T inRtn={0xff,0xff};
	static RTN_STA_T usbRtn={0xff,0xff};
	static KEY_STA_T keySta = {NON_KEY,V_KEY};
	
	batRtn.curSta = judgeBatStatus(batInfo.batVoltage);
	if (batRtn.curSta != batRtn.oldSta)
	{
		switch(batRtn.curSta)

		{
			case BAT_IS_REVERSE:
				display_led(BAT_REV_LED_CMD, ON);
				setChargeCtrlSta(OFF); // relay1 off
				break;
			case BAT_IS_NORMAL:
				display_led(BAT_ERR_LED_CMD,OFF); // bat_err_led off
				display_led(BAT_REV_LED_CMD,OFF); // reverse off
				display_num(getChargeBatVolt(&batInfo));
				break;
			case BAT_IS_ERROR:
				display_default();
				display_led(BAT_ERR_LED_CMD,ON); // bat_err_led on
				setChargeCtrlSta(OFF); // relay1 off
				break;
			default:
				display_default();
				setChargeCtrlSta(OFF); // relay1 off
				break;
		}
		batRtn.oldSta = batRtn.curSta;
	}
	
	inRtn.curSta = judgeInStatus(&batInfo,(u8)batRtn.curSta);
	if (inRtn.curSta != inRtn.oldSta)
	{
		switch (inRtn.curSta)
		{
			case IN_AI_ERR:
				display_machineMode_led(ON);
				display_led(FIX_RED_LED_CMD,OFF);
				display_led(CHARGE_RED_LED_CMD,OFF);
				setChargeCtrlSta(OFF); // relay1 off
				break;
			case IN_CHARGE:
				display_led(CHARGE_RED_LED_CMD,ON);
				display_led(FIX_RED_LED_CMD,OFF);
				setChargeCtrlSta(ON); // relay1 on
				break;
			case IN_FIX:
				display_led(FIX_RED_LED_CMD,ON);
				display_led(CHARGE_RED_LED_CMD,OFF);
				setChargeCtrlSta(ON); // relay1 on
				break;
			case IN_NONE:
				//break;
			default:
				display_led(FIX_RED_LED_CMD,OFF);
				display_led(CHARGE_RED_LED_CMD,OFF);
				setChargeCtrlSta(OFF); // relay1 off
				break;
		}
		inRtn.oldSta=inRtn.curSta;
	}
	
	usbRtn.curSta = judgeUsbCurrent(batInfo.usbCurr);
	if(usbRtn.curSta!=usbRtn.oldSta)
	{
		switch (usbRtn.curSta)
		{
			case USB_NOT_CHARGE:
				setUSB1ALedSta(OFF);
				setUSB2ALedSta(OFF);		
				break;
			case USB_CUR_1A:
				setUSB1ALedSta(ON);
				setUSB2ALedSta(OFF);
				break;
			case USB_CUR_2A:
				setUSB1ALedSta(OFF);
				setUSB2ALedSta(ON);					
				break;
			default:
				break;
		}
		usbRtn.oldSta=usbRtn.curSta;
	}
	
	//key status and show
	if (batRtn.curSta == BAT_IS_NORMAL)// && inRtn.curSta == IN_CHARGE)
	{
		key_task(&keySta.curKey);
		if (keySta.curKey == A_KEY && inRtn.curSta != IN_CHARGE)
		{
			keySta.curKey = AH_KEY;
		}
		if (keySta.curKey != keySta.oldKey)
		{
			switch(keySta.curKey)
			{
				case V_KEY:
					display_num(getChargeBatVolt(&batInfo));
					display_led(V_LED_CMD,ON);
					display_led(A_LED_CMD,OFF);
					display_led(AH_LED_CMD,OFF);
					display_led(PERCENT_LED_CMD,OFF);
					break;
				case A_KEY: 			
					display_num(getChargeCurrent(&batInfo));	
					display_led(V_LED_CMD,OFF);
					display_led(A_LED_CMD,ON);
					display_led(AH_LED_CMD,OFF);
					display_led(PERCENT_LED_CMD,OFF);															
					break;
				case AH_KEY:
					display_num(getChargeCapacity(&batInfo));
					display_led(V_LED_CMD,OFF);
					display_led(A_LED_CMD,OFF);
					display_led(AH_LED_CMD,ON);
					display_led(PERCENT_LED_CMD,OFF);
					break;
				case PER_KEY:
					display_num(getChargePercent(&batInfo));
					display_led(V_LED_CMD,OFF);
					display_led(A_LED_CMD,OFF);
					display_led(AH_LED_CMD,OFF);
					display_led(PERCENT_LED_CMD,ON);
					break;
				default:	
					break;
			}
			keySta.oldKey = keySta.curKey;
		}
	}
	// 5v source set
	if (batInfo.batVoltage>=360 || D24V_VALID == getD24Sta())
	{
		set5VResource(D5V_FROM_D24V);
	}
	if (usbRtn.curSta == USB_CUR_2A && batRtn.curSta == BAT_IS_NORMAL)
	{
		set5VResource(D5V_FROM_BAT);
	}
	
	//charge full
	if (batInfo.InVolt>= batTable[batInfo.batType].batHighVolt 
		&& batInfo.batChargeSta == 0)
	{
		TIM4Start(FINISH_DELAY,finishChargeAction);
		batInfo.batChargeSta = 1;
	}
}

void finishChargeAction(void)
{
	setChargeCtrlSta(OFF); // relay1 off
}

// 
void adDetectTask(void)
{
	static AD_STEP_T step =V_BACK_STEP;
	u16 temp=0,inVal=0;
	switch (step)
	{
		case V_BACK_STEP:
			temp = adcSingleRead(V_BACK_CHN);
			inVal = vBackConvert(temp);
			//adInputData.bat_adval = vBackConvert(temp);
			if (0 == buffer(vBatBuf,&vBatBufSta,inVal))
			{
				batInfo.batVoltage = vBatBufSta.avg;
			}
			step++;
			break;
		case V_IN_STEP:
			temp = adcSingleRead(V_IN_CHN); 
			//adInputData.in_adval = vInConvert(temp);
			batInfo.InVolt = vInConvert(temp);
			step++;
			break;
		case I_BACK_STEP:
			temp = adcSingleRead(I_BACK_CHN);
			//adInputData.bat_charge_adval = iBackConvert(temp);
			batInfo.batCurrent = iBackConvert(temp);
			step++;
			break;
		case I_USB_STEP:
			temp = adcSingleRead(I_USB_CHN);
			//adInputData.usb_adval = iUsbConvert(temp);
			batInfo.usbCurr = iUsbConvert(temp);
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






