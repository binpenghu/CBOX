

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

#include "bsp.h"

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
TIMER_T tim;

//****************************************************************************
// @Prototypes Of Static Functions
//****************************************************************************
static void adc_init(void);
//****************************************************************************
// @Prototypes Of Global Functions
//****************************************************************************

/*******************************************************************************
 *  Function:
 *  Parameters: None
 *  Returns: None
 *  Description:   
 *******************************************************************************/

void sysInit(void)
{
  //时钟
    //默认启动为HSI RC 8分频时钟，即16MHZ/1=16MHZ 
  //CLK_DeInit();
  CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
  while(!(CLK->ICKR & CLK_ICKR_HSIRDY));
 
  /*CLK_SYSCLKConfig(CLK_PRESCALER_HSIDIV1);
  ErrorStatus clk_return_status;
  clk_return_status = CLK_ClockSwitchConfig(CLK_SWITCHMODE_AUTO, CLK_SOURCE_HSI, DISABLE, CLK_CURRENTCLOCKSTATE_DISABLE);
  if (clk_return_status == SUCCESS)  //SUCCESS or ERROR
  {};*/
  
  //中断
   
  //I/0
    //input
  GPIO_Init(GPIOA, GPIO_PIN_3, GPIO_MODE_IN_FL_NO_IT);
  GPIO_Init(GPIOC, GPIO_PIN_5, GPIO_MODE_IN_FL_NO_IT);
  GPIO_Init(GPIOC, GPIO_PIN_6, GPIO_MODE_IN_FL_NO_IT);
    //output
  GPIO_Init(GPIOD, GPIO_PIN_4, GPIO_MODE_OUT_OD_HIZ_SLOW); //USB2A LED 控制
  GPIO_Init(GPIOC, GPIO_PIN_7, GPIO_MODE_OUT_OD_HIZ_SLOW);
  GPIO_Init(GPIOD, GPIO_PIN_6, GPIO_MODE_OUT_PP_LOW_SLOW);
  GPIO_Init(GPIOC, GPIO_PIN_3, GPIO_MODE_OUT_PP_LOW_SLOW);
    //模拟IIC
  GPIO_Init(GPIOB, GPIO_PIN_4, GPIO_MODE_OUT_PP_LOW_FAST);//SDA
  GPIO_Init(GPIOB, GPIO_PIN_5, GPIO_MODE_OUT_PP_LOW_FAST);//SCL
  
  //外设-ADC
  
    //复用输入配置
  GPIO_Init(GPIOD, GPIO_PIN_5, GPIO_MODE_IN_FL_NO_IT);
  GPIO_Init(GPIOC, GPIO_PIN_4, GPIO_MODE_IN_FL_NO_IT);
  GPIO_Init(GPIOD, GPIO_PIN_2, GPIO_MODE_IN_FL_NO_IT);
  GPIO_Init(GPIOD, GPIO_PIN_3, GPIO_MODE_IN_FL_NO_IT);
    //复用外设配置
  //ADC1_DeInit();
  //ADC1_Init(ADC1_CONVERSIONMODE_SINGLE, ADC1_CHANNEL_2, ADC1_PRESSEL_FCPU_D2, ADC1_EXTTRIG_TIM, DISABLE, ADC1_ALIGN_LEFT, ADC1_SCHMITTTRIG_CHANNEL0, DISABLE);
  //ADC1_Init(ADC1_CONVERSIONMODE_SINGLE, ADC1_CHANNEL_3, ADC1_PRESSEL_FCPU_D2, ADC1_EXTTRIG_TIM, DISABLE, ADC1_ALIGN_LEFT, ADC1_SCHMITTTRIG_CHANNEL0, DISABLE);
  //ADC1_Init(ADC1_CONVERSIONMODE_SINGLE, ADC1_CHANNEL_4, ADC1_PRESSEL_FCPU_D2, ADC1_EXTTRIG_TIM, DISABLE, ADC1_ALIGN_LEFT, ADC1_SCHMITTTRIG_CHANNEL0, DISABLE);
  //ADC1_Init(ADC1_CONVERSIONMODE_SINGLE, ADC1_CHANNEL_5, ADC1_PRESSEL_FCPU_D2, ADC1_EXTTRIG_TIM, DISABLE, ADC1_ALIGN_LEFT, ADC1_SCHMITTTRIG_CHANNEL0, DISABLE);
  //ADC1_Cmd(ENABLE);
  adc_init();

}

/*******************************************************************************
 *  Function:
 *  Parameters: None
 *  Returns: None
 *  Description:   
 *******************************************************************************/
KEY_STATUS_E getKeySwSta(void)
{
	//BitStatus bit_status;
	 u8 temp = 0;
	 //bit_status = GPIO_ReadInputPin(GPIOA, GPIO_PIN_3);
	//temp = bit_status & GPIO_PIN_3;
	 temp = GPIOA->IDR & GPIO_PIN_3;
	 //if (temp  != RESET)  //SET or RESET
     if (temp)
	 {
		return KEY_RELEASE;
		
	 }else
	 {
		return KEY_PRESS;
	 }
}
/*******************************************************************************
 *	Function:
 *	Parameters: None
 *	Returns: None
 *	Description:   
 *******************************************************************************/
	
BAT_CONNECT_DIR_STA_E getBatConnectSta(void)
{
	//BitStatus bit_status;
	u8 temp = 0;
	//bit_status = GPIO_ReadInputPin(GPIOC, GPIO_PIN_5);
	//temp = bit_status & GPIO_PIN_5;
	temp = GPIOC->IDR & GPIO_PIN_5;
	//if (bit_status != RESET)	//SET or RESET
	if (temp != RESET)
	{
	   return BAT_NORMAL;
	}else
	{
	   return BAT_REVERSE;
	}
}

/*******************************************************************************
 *	Function:
 *	Parameters: None
 *	Returns: None
 *	Description:   
 *******************************************************************************/
D24V_CONNECT_STA_E getD24Sta(void)
{
	//BitStatus bit_status;
	u8 temp = 0;
	//bit_status = GPIO_ReadInputPin(GPIOC, GPIO_PIN_6);
	temp = GPIOC->IDR & GPIO_PIN_6;
	//temp = bit_status & GPIO_PIN_6;
	//if (bit_status != RESET)	//SET or RESET
	if (temp != RESET)
	{
	   return D24V_INVALID;
	}else
	{
	   return D24V_VALID;
	}
}
/*******************************************************************************
 *	Function:
 *	Parameters: None
 *	Returns: None
 *	Description:   
 *******************************************************************************/

void set5VResource(D5V_RESOURCE_E res)
{
	if(res == D5V_FROM_D24V)
		GPIO_WriteHigh(GPIOD, GPIO_PIN_6);
	else
		GPIO_WriteLow(GPIOD, GPIO_PIN_6);
}

/*******************************************************************************
 *	Function:
 *	Parameters: None
 *	Returns: None
 *	Description:   
 *******************************************************************************/
void setUSB2ALedSta(SW_E on_off)
{
	if (on_off == ON)
		GPIO_WriteLow(GPIOD, GPIO_PIN_4);
	else
		GPIO_WriteHigh(GPIOD, GPIO_PIN_4);
}


/*******************************************************************************
 *	Function:
 *	Parameters: None
 *	Returns: None
 *	Description:   
 *******************************************************************************/
void setUSB1ALedSta(SW_E on_off)
{
	if (on_off == ON)
		GPIO_WriteLow(GPIOC, GPIO_PIN_7);
	else
		GPIO_WriteHigh(GPIOC, GPIO_PIN_7);
}
	

/*******************************************************************************
 *	Function:
 *	Parameters: None
 *	Returns: None
 *	Description:   
 *******************************************************************************/
void setChargeCtrlSta(SW_E on_off)
{
	if(on_off == ON)
	{
		GPIO_WriteHigh(GPIOC, GPIO_PIN_3);
	}else
	{
		GPIO_WriteLow(GPIOC, GPIO_PIN_3);
	}
}
/*
fnc:
des: adc channel init
5 channels;2-5
v_in for charge voltage
v_back for bat voltage
i_usb for usb charge current
i_back for bat charge currente
backup thinking: 
1-同一时间只能转换一个通道？完成转化标志是什么。
2-连续转换导致通道占用？如何控制连续转换，读取值？

3-中断配置，模拟看门狗中断与转换结束中断。
*/
void adc_init(void)
{
	ADC1_DeInit(); // 寄存器回复默认值
	ADC1_PrescalerConfig(ADC1_PRESSEL_FCPU_D2);// 分频 fadc= fmaster/2 = 1/2/4MHZ
	//选择触发源
	//ADC采样速率，最大4MHZ,转换速率为3.5us
	//IO配置，必须为浮空输入,关闭Smit触发器 - ADC_CSR
	ADC1_SchmittTriggerConfig(ADC1_SCHMITTTRIG_CHANNEL2, DISABLE);
	ADC1_SchmittTriggerConfig(ADC1_SCHMITTTRIG_CHANNEL3, DISABLE);
	ADC1_SchmittTriggerConfig(ADC1_SCHMITTTRIG_CHANNEL4, DISABLE);
	ADC1_SchmittTriggerConfig(ADC1_SCHMITTTRIG_CHANNEL5, DISABLE);	
	//对齐方式
	ADC1_Cmd(ENABLE);
}

/*
fnc:
des: adc read 
*/

u16 adcSingleRead(ADC1_Channel_TypeDef channel)
{
	u8 flag_status = 0;
	//u8 temp = 0;
	u16 u16_adc1_value;
	ADC1_ConversionConfig(ADC1_CONVERSIONMODE_SINGLE, channel, ADC1_ALIGN_RIGHT);		
	ADC1_StartConversion();	
	flag_status = ADC1_GetFlagStatus(ADC1_FLAG_EOC);
	delay_us(10);
	//temp = (u8)flag_status & channel;
	if (flag_status&0x80) // SET or RESET
	//if (temp)
	{
		u16_adc1_value = ADC1_GetConversionValue(); 
		ADC1_ClearFlag(ADC1_FLAG_EOC);
		return u16_adc1_value;
	};
    return 0;
}
/*
fnc:
des: adc read 
*/

u16 adcCcontinuousRead(ADC1_Channel_TypeDef channel)
{
	FlagStatus flag_status = RESET;
	u16 timeout=0;
	u8 ichn;
	ADC1_ConversionConfig(ADC1_CONVERSIONMODE_CONTINUOUS, channel, ADC1_ALIGN_LEFT);
	ADC1_DataBufferCmd(ENABLE);
	ADC1_StartConversion();
	do
	{
		flag_status = ADC1_GetFlagStatus(ADC1_FLAG_EOC);
		timeout++;
	}while(SET==flag_status || timeout<=10000);
	
	u16 u16_adc1_value;
	do
	{
		u16_adc1_value += ADC1_GetBufferValue(ichn);	
	}
	while (ichn++<9); //10次
	
	return u16_adc1_value/10;
}

/*******************************************************************************
 *	Function: 
 *	Parameters: None
 *	Returns: None
 *	Description:   IIC
 *******************************************************************************/

#define TM_DIO_L   GPIO_WriteLow(GPIOB,GPIO_PIN_5)
#define TM_DIO_H   GPIO_WriteHigh(GPIOB,GPIO_PIN_5)
	
#define TM_DIO_OUT  GPIO_Init(GPIOB, GPIO_PIN_5, GPIO_MODE_OUT_PP_LOW_FAST)
#define TM_DIO_IN   GPIO_Init(GPIOB,GPIO_PIN_5,GPIO_MODE_IN_PU_NO_IT)
#define TM_DIO_READ  GPIO_ReadInputPin(GPIOB,GPIO_PIN_5)
	
#define TM_CLK_L   GPIO_WriteLow(GPIOB,GPIO_PIN_4)
#define TM_CLK_H   GPIO_WriteHigh(GPIOB,GPIO_PIN_4)
	
#define TM_CLK_OUT  GPIO_Init(GPIOB, GPIO_PIN_4, GPIO_MODE_OUT_PP_HIGH_FAST)
#define TM_CLK_IN   GPIO_Init(GPIOB,GPIO_PIN_4,GPIO_MODE_IN_PU_NO_IT)
#define TM_CLK_READ  GPIO_ReadInputPin(GPIOB,GPIO_PIN_4)

void TM_Start(void)	//开始信号
{
	TM_DIO_OUT;
	TM_CLK_H;
	TM_DIO_H;
	nop();nop();nop();nop();nop();
	TM_DIO_L;
}
void TM_Stop(void) //停止信号
{
	TM_DIO_OUT;
	TM_CLK_H;
	TM_DIO_L;
	nop();nop();nop();nop();nop();
	TM_DIO_H;
}
//写入单字节函数
void TM_w_BYTE(unsigned char TM_w_data) //写一个字节高位在前，低位在后
{
    unsigned char i,temp1;
		TM_DIO_OUT;
    temp1 = TM_w_data;                //赋值
		TM_CLK_L;
    for(i = 0;i < 8;i++)             //循环写入每一位
    { 
			TM_CLK_L;
        if(temp1 & 0x80)               //写入第一位
            TM_DIO_H;
        else
            TM_DIO_L; 
				//TM_CLK_L;		
				nop();nop();nop();nop();nop();
				TM_CLK_H;
				nop();nop();nop();nop();nop();
				TM_CLK_L;
        temp1 <<= 1;                   //左循环一位
    }
		nop();nop();nop();nop();nop();
		TM_CLK_H;
		nop();nop();nop();nop();nop();
		TM_DIO_IN;	
		TM_CLK_L;
		nop();nop();nop();nop();nop();
}

//按键读取函数
unsigned char TM_r_key(void)
{
    unsigned char i,rekey;
		BitStatus DIO;
		TM_Start();
    TM_w_BYTE(0x49);                    //读键扫描命令
		//TM_ask();
		TM_DIO_IN;				//设置为输入模式
		DIO = TM_DIO_READ;
			for(i=0;i<8;i++)
			{
				TM_CLK_H;
				rekey = rekey<<1;
				DIO = TM_DIO_READ;
				if(DIO) {rekey++;} 
				nop();nop();nop();nop();nop();
				TM_CLK_L;          //时钟拉低
				nop();nop();nop();nop();nop();
			}
			TM_CLK_H;
		nop();nop();nop();nop();nop();
		TM_CLK_L;
		nop();nop();nop();nop();nop();
	//TM_ask();
	TM_Stop();
	return(rekey);
}

/*

void iic_start(void)
{
	
}

void iic_stop()
{

}

void iic_byts()
{

}
*/

/*******************************************************************************
 *	Function:
 *	Parameters: None
 *	Returns: None
 *	Description:   
 *******************************************************************************/
	void TM1650_Set(unsigned char add,unsigned char dat) //数码管显示
	{
		//写显存必须从高地址开始写
		TM_Start();
		TM_w_BYTE(add); //第一个显存地址
		//TM_ask();
		TM_w_BYTE(dat);
		//TM_ask();
		TM_Stop();
	}

/*******************************************************************************
 *	Function:
 *	Parameters: None
 *	Returns: None
 *	Description:   
 *******************************************************************************/
//u8 CODE[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};
void TM1650_Init(void)
{

	TM1650_Set(0x48,0x11); 

	TM1650_Set(0x68,0x3f);

	TM1650_Set(0x6A,0x3f);

	TM1650_Set(0x6C,0x3f);

	TM1650_Set(0x6E,0x00);

}
/*******************************************************************************
 *	Function:
 *	Parameters: None
 *	Returns: None
 *	Description:   sysclk = 16mhz ,  timerclk =16/16000 = 1khz.
 fCK_CNT = fCK_PSC / (PSCR[15:0] + 1)
 PSCR +1 = 16000=> PSCR = 15999;
 *******************************************************************************/
void tim1_init(void)
{
	TIM1_TimeBaseInit(128, TIM1_COUNTERMODE_UP, 4, 24);
    TIM1_ITConfig(TIM1_IT_UPDATE, ENABLE);
    TIM1_ARRPreloadConfig(ENABLE);
    TIM1_SetCounter(0);
	TIM1_Cmd(ENABLE);
}
/*******************************************************************************
 *	Function:
 *	Parameters: None
 *	Returns: None
 *	Description:   
 *******************************************************************************/

void tim4_init(void)
{
	TIM4_TimeBaseInit(TIM4_PRESCALER_128, 125-1);  //定时1ms (16M/128/125 = 1000)
	TIM4_ClearFlag(TIM4_FLAG_UPDATE);			   //清除标志位
	TIM4_ITConfig(TIM4_IT_UPDATE, ENABLE);		   //使能更新UPDATE中断  
    TIM4_SetCounter(0);                            //计数值归零
    TIM4_Cmd(DISABLE);  
    
}
/*******************************************************************************
 *	Function:
 *	Parameters: None
 *	Returns: None
 *	Description:   
 *******************************************************************************/

void TIM4Start(u16 mstime,void_fptr fnc)
{
  if (tim.timRunningFlg == 0)
  {
  	tim.timRunningFlg = 1;
  	tim.cntOvf = mstime;
  	tim.pfnc = fnc;
    TIM4_SetCounter(0);                            //计数值归零
    TIM4_Cmd(ENABLE);                              //启动定时器
  }
}
/*******************************************************************************
 *	Function:
 *	Parameters: None
 *	Returns: None
 *	Description:   
 *******************************************************************************/

void TIM4Stop(void)
{
  if (tim.timRunningFlg==1)
  {
	  TIM4_SetCounter(0);
	  tim.timRunningFlg=0;
	  TIM4_Cmd(DISABLE);							 //关闭定时器
  }
}
/*******************************************************************************
 *	Function:
 *	Parameters: None
 *	Returns: None
 *	Description:   
 *******************************************************************************/
void iwdg_init(void)
{
	IWDG_Enable();
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	IWDG_SetPrescaler(IWDG_Prescaler_32);
	IWDG_SetReload(250);
	IWDG_ReloadCounter();
}
 /*******************************************************************************
 *	Function:
 *	Parameters: None
 *	Returns: None
 *	Description:   
 *******************************************************************************/
void feedDog(void)
{
	IWDG_ReloadCounter();
}
//****************************************************************************
// @Interrupt Vectors
//****************************************************************************
extern void LED_TRI(void);
/*******************************************************************************
 *	Function:
 *	Parameters: None
 *	Returns: None
 *	Description:   
 *******************************************************************************/

void tim4_vector(void)
{
	static u16 tick=0;
	static u16 secCnt=0;
	TIM4_ClearITPendingBit(TIM4_IT_UPDATE); 	   //清除中断标志
	tick++;
	if (tick %1000 == 0)
	{
		tick = 0;
		secCnt++;
		if (secCnt>=tim.cntOvf)
		{
			tim.pfnc();
			TIM4Stop();
			secCnt=0;
		}
	}
	/*
	if (tick >= tim.cntOvf)
	{
		//LED_TRI();
		//TIM4Start(500,LED_TRI);
	}*/
}



/*
 *######################################################################
 *                           End of File
 *######################################################################
 */
