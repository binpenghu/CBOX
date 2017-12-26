#include "stm8s_gpio.h"
#include "TM1650.h"
//#include "main.h"



//#define TM_STB_L   GPIO_WriteLow(GPIOC,GPIO_PIN_5)
//#define TM_STB_H   GPIO_WriteHigh(GPIOC,GPIO_PIN_5)

//#define TM_DAT_command 0x40
//#define TM_DIS_control 0x80
//#define TM_ADD_command 0xc0


//extern u8 KEY[5];

void TM_Start(void)	//��ʼ�ź�
{
	TM_DIO_OUT;
	TM_CLK_H;
	TM_DIO_H;
	nop();nop();nop();nop();nop();
	TM_DIO_L;
}

/*
void TM_ask(void) //ACK�ź�
{
	unsigned char  timeout = 1;
	BitStatus DIO;
	//TM_DIO_IN;
	TM_CLK_H;
	nop();nop();nop();nop();nop();
	TM_CLK_L;
//	DIO = TM_DIO_READ;
	while((DIO)&&(timeout<=100))
	{
//		DIO = TM_DIO_READ;
	  timeout++;	
	}
	nop();nop();nop();nop();nop();
	TM_CLK_L;
}
*/

void TM_Stop(void) //ֹͣ�ź�
{
	TM_DIO_OUT;
	TM_CLK_H;
	TM_DIO_L;
	nop();nop();nop();nop();nop();
	TM_DIO_H;
}

//д�뵥�ֽں���
void TM_w_BYTE(unsigned char TM_w_data) //дһ���ֽڸ�λ��ǰ����λ�ں�
{
    unsigned char i,temp1;
		TM_DIO_OUT;
    temp1 = TM_w_data;                //��ֵ
		TM_CLK_L;
    for(i = 0;i < 8;i++)             //ѭ��д��ÿһλ
    { 
			TM_CLK_L;
        if(temp1 & 0x80)               //д���һλ
            TM_DIO_H;
        else
            TM_DIO_L; 
				//TM_CLK_L;		
				nop();nop();nop();nop();nop();
				TM_CLK_H;
				nop();nop();nop();nop();nop();
				TM_CLK_L;
        temp1 <<= 1;                   //��ѭ��һλ
    }
		nop();nop();nop();nop();nop();
		TM_CLK_H;
		nop();nop();nop();nop();nop();
		TM_CLK_L;
		nop();nop();nop();nop();nop();
}

//������ȡ����
unsigned char TM_r_key(void)
{
    unsigned char i,rekey;
		BitStatus DIO;
		TM_Start();
    TM_w_BYTE(0x49);                    //����ɨ������
		//TM_ask();
		TM_DIO_IN;				//����Ϊ����ģʽ
			for(i=0;i<8;i++)
			{
				TM_CLK_H;
				rekey = rekey<<1;
				DIO = TM_DIO_READ;
				if(DIO) {rekey++;} 
				nop();nop();nop();nop();nop();
				TM_CLK_L;          //ʱ������
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
	
void TM1650_Set(unsigned char add,unsigned char dat) //�������ʾ
{
	//д�Դ����Ӹߵ�ַ��ʼд
	TM_Start();
	TM_w_BYTE(add); //��һ���Դ��ַ
	//TM_ask();
	TM_w_BYTE(dat);
	//TM_ask();
	TM_Stop();
}



