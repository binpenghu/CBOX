
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
1-ͬһʱ��ֻ��ת��һ��ͨ�������ת����־��ʲô��
2-����ת������ͨ��ռ�ã���ο�������ת������ȡֵ��
3-�ж����ã�ģ�⿴�Ź��ж���ת�������жϡ�
*/
void adc_init()
{
	ADC1_DeInit(); // �Ĵ����ظ�Ĭ��ֵ
	ADC1_PrescalerConfig(ADC1_PRESSEL_FCPU_D2);// ��Ƶ fadc= fmaster/2 = 1/2/4MHZ
	//ѡ�񴥷�Դ
	//ADC�������ʣ����4MHZ,ת������Ϊ3.5us
	//IO���ã�����Ϊ��������,�ر�Smit������ - ADC_CSR
ADC1_SchmittTriggerConfig(ADC1_SCHMITTTRIG_CHANNEL2, DISABLE);
ADC1_SchmittTriggerConfig(ADC1_SCHMITTTRIG_CHANNEL3, DISABLE);
ADC1_SchmittTriggerConfig(ADC1_SCHMITTTRIG_CHANNEL4, DISABLE);
ADC1_SchmittTriggerConfig(ADC1_SCHMITTTRIG_CHANNEL5, DISABLE);	
	//���뷽ʽ
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
	while (ichn++<9); //10��
	
	return u16_adc1_value/10;
}



