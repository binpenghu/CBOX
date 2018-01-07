
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

enum
{
	V_BUF=0,
	IN_BUF,
	I_BUF,
	I_SUB_BUF,
	BUF_MAX
};


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

//AD_IPNUT_T adInputData = {0};


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

/******************************************************************************
 *	Function:
 *	Parameters: None
 *	Returns: None
 *	Description:   
 ******************************************************************************/
#define BUF_LENTH  4
typedef struct
{
	u8 bufSta;
	u8 bufCnt;
	u16 total;
	u16 avg;
}BUF_STA_T;

u16 vBatBuf[BUF_LENTH];
u16 vInBuf[BUF_LENTH];
u16 iBatBuf[BUF_LENTH];
u16 iUsbBuf[BUF_LENTH];
BUF_STA_T vBatBufSta = {0,0,0,0};
BUF_STA_T vInBufSta = {0,0,0,0};
BUF_STA_T iBatBufSta = {0,0,0,0};
BUF_STA_T iUsbBufSta = {0,0,0,0};


u8 buffer(u16 *buf,BUF_STA_T *bufsta,u16 inVal)
{
	u8 j=0;
	buf[bufsta->bufCnt] = inVal; 
	if (bufsta->bufCnt==BUF_LENTH-1 || bufsta->bufSta == 1)
	{
		for (j=0;j++;j<BUF_LENTH)
		{
			bufsta->total += buf[j];
		}
		bufsta->avg = bufsta.total/BUF_LENTH;
		//bufsta->avg = bufsta.total<<2;
		bufsta->bufSta =1;
		return 0;
	}
	bufsta->bufCnt = bufsta->bufCnt<BUF_LENTH?bufsta->bufCnt++:0;
	return 1;
}
 

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
extern BAT_INFO_T batInfo;

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






