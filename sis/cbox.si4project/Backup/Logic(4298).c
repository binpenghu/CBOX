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




//****************************************************************************
// @Imported Global Variables
//****************************************************************************
const BAT_VOLTAGE_T batTable[BAT_MAX]= // unit is 0.1v
{
	{48,72,55,5,73},		//6v
	{96,143,114,5,145},		//12v
	{192,286,228,5,290},	//24v
	{288,432,331,5,438},	//36v
	{440,576,441,5,584},	//48v
};
typedef struct
{
	u8 batType;
	BAT_VOLTAGE_T batTypeVolt;
	u16 batVoltage;
	u16 batCurrent;
	u16 batPercent;
	u16 batCapacity;
}BAT_INFO_T;

static u8 batteryType = BAT_MAX;
//****************************************************************************
// @Global Variables
//****************************************************************************
//CHARGE_DATA_T chargeSta;
BAT_INFO_T batInfo;

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
void logic_init()
{
	batInfo.batType = BAT_MAX;
	batInfo.batVoltage=0;
	batInfo.batCurrent=0;
	batInfo.batPercent=0;
	batInfo.batCapacity=0;
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
		if (batval <= batTable[i].batHighVolt && batval >= batTable[i].batLowVolt)
		{
			return i;
		}
	}
	return 0xff;
}
 
u8 judgeBatStatus(u16 batval)
{
	//u16 batVal = 0;
	u8 i=0;
	if (BAT_REVERSE == getBatConnectSta())
	{
		display_led(BAT_REV_LED_CMD, ON);
		return BAT_IS_REVERSE;
	}
	if (batval >= 10)
	{
		if (batInfo.batType >= BAT_MAX)
		{
			batInfo.batType = judgeBatType(batval);
			if (batInfo.batType < BAT_MAX)
				return BAT_IS_NORMAL;
		}else 
		{
			if (batval <= batTable[batInfo.batType].batHighVolt && batval >= batTable[batInfo.batType].batLowVolt)
			{
				return BAT_IS_NORMAL;
			}
		}
	}
	else
	{
		return BAT_IS_NONE;
	}
	return BAT_IS_ERROR;
}
/******************************************************************************
 *  Function:
 *  Parameters: None
 *  Returns: None
 *  Description:   判断是否进入充电模式
 ******************************************************************************/
 typedef enum
 {
	IN_CHARGE,
	IN_FIX,
	IN_NONE,
	IN_ERR
 }IN_RTN_T;

u8 judgeChargeMode(u16 inval,u8 batType)
{
	if (batType >= BAT_MAX)
		return IN_ERR;

	if (D24V_VALID ==  getD24Sta())
	{
		if(inval <= batTable[batType].batMaxErrVolt &&
			inval >= batTable[batType].batNorVolt)
		{
			display_led(CHARGE_RED_LED_CMD,ON);
			display_led(FIX_RED_LED_CMD,OFF);
			return IN_CHARGE;
		}
		else if (inval > batTable[batType].batMaxErrVolt ||
			inval < batTable[batType].batNorVolt)
		{
			display_led(FIX_RED_LED_CMD,ON);
			display_led(CHARGE_RED_LED_CMD,OFF);
			return IN_FIX;
		}
	}else
	{
		return IN_NONE;
	}
}
/******************************************************************************
 *  Function:
 *  Parameters: None
 *  Returns: None
 *  Description:
 ******************************************************************************/
typedef enum
{
	USB_NOT_CHARGE,
	USB_CUR_1A,
	USB_CUR_2A
}USB_RTN_T;

u8 judgeUsbCurrent(u8 usbVal)
{
	if (usbVal > 0 && usbVal < 10)
	{
		setUSB1ALedSta(USB1A_LED_ON);
		setUSB2ALedSta(USB2A_LED_OFF);
		return USB_CUR_1A;
	}
	else if(usbVal> 10)
	{
		setUSB1ALedSta(USB1A_LED_OFF);
		setUSB2ALedSta(USB2A_LED_ON);
		return USB_CUR_2A;
	}
	else
	{
		setUSB1ALedSta(USB1A_LED_OFF);
		setUSB2ALedSta(USB2A_LED_OFF);
		return USB_NOT_CHARGE;
	}
}

/******************************************************************************
*  Function:
*  Parameters: None
*  Returns: None
*  Description:
******************************************************************************/
	
u16 getChargeCurrent(BAT_INFO_T *bat)
{
	u16 curVolt = bat->batVoltage;
}

u16 getChargePercent(BAT_INFO_T *bat)
{
	u16 curVolt = bat->batVoltage;
}

u16 getChargeCapacity(BAT_INFO_T *bat)
{
	u16 curVolt = bat->batVoltage;
}

/******************************************************************************
*  Function:
*  Parameters: None
*  Returns: None
*  Description:
******************************************************************************/



 
typedef struct
{
	u8 batRtnSta;
	u8 inRtnSta;
	u8 usbRtnSta;
	u8 keyRtnSta;
	
}CHARGE_DATA_T;


void logicTask(void)
{
	


}
//****************************************************************************
// @Interrupt Vectors
//****************************************************************************

/*
 *######################################################################
 *                           End of File
 *######################################################################
 */






