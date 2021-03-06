
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
//****************************************************************************
// @Macros
//****************************************************************************

//****************************************************************************
// @Defines
//****************************************************************************
#define DIGITAL_POINT 0x80
//****************************************************************************
// @Typedefs
//****************************************************************************
 
//****************************************************************************
// @Imported Global Variables
//****************************************************************************
static u8 Number[10]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f}; //0-9
const u8 initVal[] = {0x40,0x02};
//const u8 G1_OFF = 0x
typedef enum
{
	DECADE_REG = 0x68,
	DIGITAL_REG = 0x6A,
	POINT_REG = 0x6c,
}NUM_REG_E;
typedef enum 
{
	MACHINDE_MODE_REG =0x68,
	CHARGE_FINISH_REG =0x6c,
	OTHER_LED_REG = 0x6E
}LED_REG_E;

/*
typedef enum
{
	CHARGE_RED_LED = 0x01,
	V_LED = 0x02,
	A_LED = 0x04,
	AH_LED = 0x08,
	BAT_ERR_LED = 0x10,
	FIX_RED_LED = 0x20,
	BAT_REV_LED = 0x40,
	PERCENT_LED = 0x80,
}DISPLAY_LED_VAL_E;
*/

#define MACHINE_ERR_LED  0x80
#define CHARGE_FINISH_LED 0x80



typedef struct
{
	u8 G1;
	//u8 digital_regval;
	u8 G3;
	//u8 led_regval;
}REG_VAL_T;
//****************************************************************************
// @Global Variables
//****************************************************************************
REG_VAL_T regValBackup = {0x00,0x00};

//****************************************************************************
// @Prototypes Of Static Functions
//****************************************************************************
/******************************************************************************
 *  Function:
 *  Parameters: None
 *  Returns: None
 *  Description:   
 ******************************************************************************/

void display_default(void)
{
	TM1650_Set(DECADE_REG,initVal[0]);		
	TM1650_Set(DIGITAL_REG,initVal[0]);	
	TM1650_Set(POINT_REG,initVal[0]);	
	//TM1650_Set(OTHER_LED_REG,initVal[1]);
	display_led(V_LED_CMD,ON);
}
/******************************************************************************
 *  Function:
 *  Parameters: None
 *  Returns: None
 *  Description:  
 ******************************************************************************/
 

/******************************************************************************
 *  Function:
 *  Parameters: None
 *  Returns: None
 *  Description:   若电压小于10V，decade不显示，
 ******************************************************************************/
void display_num(u16 num)
{
	u8 decade=0,digit=0,point=0;
	if (num < 10) // only point
	{
		//decade=Number[0];
		decade = 0x80 & regValBackup.G1; // 不显示数字
		digit=Number[0];
		point=Number[num];
		digit |= DIGITAL_POINT;
		
	}else if (num<100)
	{
		decade=  0x80 & regValBackup.G1;// 不显示数字
		digit = Number[num/10];
		point = Number[num%10];
		digit |= DIGITAL_POINT;
	}
	else if (num<1000)
	{
		decade = Number[num/100];
		digit = Number[(num%100)/10];
		point = Number[(num%100)%10];	
		digit |= DIGITAL_POINT;
	}
	else if (num < 10000)
	{
		decade = Number[num/1000];
		digit = Number[(num%1000)/100];
		point = Number[(num%1000)%100];			
	}
	else
	{
		return;
	}

	TM1650_Set(DECADE_REG,decade);		
	TM1650_Set(DIGITAL_REG,digit);	
	TM1650_Set(POINT_REG,point);	
	regValBackup.G1 = decade;
	//regValBackup.digital_regval = digit;
	regValBackup.G3 = point;
}
/******************************************************************************
 *  Function:
 *  Parameters: None
 *  Returns: None
 *  Description:   
 ******************************************************************************/
void display_led(DISPLAY_LED_CMD_E ledCmd,u8 on_off)
{
	static u8 ledVal = 0;
	if (ledCmd >= MAX_LED_CMD)
	{
		return;
	}
	else if (ledCmd <LED_ALL_CLOSED_CMD)
	{
		if (on_off == ON)
		{
            ledVal |= 0x1<<ledCmd;
			//temp |= 0x1<<ledCmd;
			//ledVal |= temp;
		}else if (on_off == OFF)
		{
			//temp &= ~(0x1<<ledCmd);
			//ledVal &= temp;
            ledVal &= ~(0x1<<ledCmd);
		}	

	}else if (ledCmd == LED_ALL_CLOSED_CMD)
	{
		if (on_off == ON)
			ledVal = 0xff;
		else
			ledVal = 0x0;
	}
	TM1650_Set(OTHER_LED_REG,ledVal);	
	//regValBackup.led_regval = ledVal;
}
/******************************************************************************
 *  Function:
 *  Parameters: None
 *  Returns: None
 *  Description:   
 ******************************************************************************/
void display_machineMode_led(u8 on_off)
{
	u8 temp = 0;
	if (on_off == ON)
	{
		temp = MACHINE_ERR_LED |regValBackup.G1;
		TM1650_Set(MACHINDE_MODE_REG,temp);	
	}
	else
	{
		temp = (~MACHINE_ERR_LED)&regValBackup.G1;
		TM1650_Set(MACHINDE_MODE_REG,temp);	
	}
	regValBackup.G1 = temp;
}

/******************************************************************************
 *  Function:
 *  Parameters: None
 *  Returns: None
 *  Description:   
 ******************************************************************************/
void display_chargeFinish_led(u8 on_off)
{
	u8 temp = 0;
	
	if (on_off == ON)
	{
		temp = CHARGE_FINISH_LED | regValBackup.G3;
		TM1650_Set(CHARGE_FINISH_REG,temp); 
	}
	else
	{
		temp = (~CHARGE_FINISH_LED) & regValBackup.G3;
		TM1650_Set(CHARGE_FINISH_REG,temp); 
	}
	regValBackup.G3 = temp;
}

//****************************************************************************
// @Prototypes Of Global Functions
//****************************************************************************

/******************************************************************************
 *  Function:
 *  Parameters: None
 *  Returns: None
 *  Description:   
 ******************************************************************************/
 

//****************************************************************************
// @Interrupt Vectors
//****************************************************************************

/*
 *######################################################################
 *                           End of File
 *######################################################################
 */






