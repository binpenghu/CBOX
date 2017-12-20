
include "stm8s.h"



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
void adc_init()
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
	FlagStatus flag_status;
	u16 u16_adc1_value;
	ADC1_ConversionConfig(ADC1_CONVERSIONMODE_SINGLE, channel, ADC1_ALIGN_RIGHT);		
	ADC1_StartConversion();
	flag_status = ADC1_GetFlagStatus(ADC1_FLAG_EOC);
	delay_us(10);
	if (flag_status == SET) // SET or RESET
	{
		u16_adc1_value = ADC1_GetConversionValue(); 
		ADC1_ClearFlag(ADC1_FLAG_EOC);
		return u16_adc1_value;
	};
}

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



