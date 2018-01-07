
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
#ifndef __BSP_H_
#define __BSP_H_
//****************************************************************************
// @Project Includes
//****************************************************************************
#include "stm8s.h"
#include "stm8s_gpio.h"
#include "delay.h"
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
	KEY_PRESS=0,
	KEY_RELEASE=1,
}KEY_STATUS_E;

typedef enum
{
	BAT_NORMAL=0,
	BAT_REVERSE=1,
}BAT_CONNECT_DIR_STA_E;

typedef enum
{
	D24V_INVALID=0,
	D24V_VALID=1,
}D24V_CONNECT_STA_E;

typedef enum
{
	D5V_FROM_BAT=0,
	D5V_FROM_D24V=1,
}D5V_RESOURCE_E;
/*
typedef enum
{
	USB2A_LED_OFF = 0,
	USB2A_LED_ON = 1,
}USB2A_LED_E;

typedef enum
{
	USB1A_LED_OFF = 0,
	USB1A_LED_ON = 1,
}USB1A_LED_E;

typedef enum
{
	CHARGE_OFF=0,
	CHARGE_ON =1,
}CHARGE_CTRL_STA_E;
*/
typedef enum
{
	OFF = 0,
	ON = 1,	
}SW_E;


typedef struct
{
	u16 cnt;
	u8 period_10ms;
	u8 period_20ms;
	u8 period_50ms;
	u8 period_100ms;
	u8 period_200ms;
	u8 period_500ms;
	u8 period_1s;
}SYS_TICK_T;



typedef  void (*void_fptr) (void); 
typedef struct
{
	u8 timerType;
	u8 timRunningFlg;	
	u16 cntOvf;
	void_fptr pfnc;
}TIMER_T;

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
extern void sysInit(void);
extern KEY_STATUS_E getKeySwSta(void);
extern BAT_CONNECT_DIR_STA_E getBatConnectSta(void);
extern D24V_CONNECT_STA_E getD24Sta(void);
extern void set5VResource(D5V_RESOURCE_E res);
extern void setUSB2ALedSta(SW_E on_off);
extern void setUSB1ALedSta(SW_E on_off);
extern void setChargeCtrlSta(SW_E on_off);
// for display
extern 	void TM1650_Set(unsigned char add,unsigned char dat); // ˝¬Îπ‹œ‘ æ
extern void  TM1650_Init(void);
//adc
extern u16 adcSingleRead(ADC1_Channel_TypeDef channel);
extern u16 adcCcontinuousRead(ADC1_Channel_TypeDef channel);

extern void tim1_init(void);
extern void tim4_init(void);

extern void TIM4Start(u16 mstime,void_fptr fnc);

extern void iwdg_init(void);
extern void feedDog(void);
//****************************************************************************
// @Interrupt Vectors
//****************************************************************************

/*
 *######################################################################
 *                           End of File
 *######################################################################
 */
#endif
