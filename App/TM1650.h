#ifndef __TM1628_H__
#define __TM1628_H__

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

#define nop()                {_asm("nop\n");}    /* No Operation */

void TM_w_BYTE(unsigned char TM_w_data); 
//void TM_w_command(unsigned char TM_w_data);
//void TM_w_data(unsigned char TM_w_addr,unsigned char TM_w_data);

unsigned int TM_r_BYTE(void);
unsigned char TM_r_key(void);

BitStatus I2C_CLK;
BitStatus I2C_DIO;

//void TM_init(void);
void TM_Start(void);
//void TM_ask(void);
void TM_Stop(void);
void TM1650_Set(unsigned char add,unsigned char dat);

FlagStatus COM_busy=0;
void IC_w_BYTE(unsigned char TM_w_data) //дһ���ֽڸ�λ��ǰ����λ�ں�
{
    unsigned char i,temp1,timeout;
		TM_DIO_OUT;
		TM_CLK_OUT;
    temp1 = TM_w_data;                //��ֵ
		TM_CLK_H;nop();nop();nop();nop();nop();
    for(i = 0;i < 8;i++)             //ѭ��д��ÿһλ
    { 
		TM_CLK_OUT;
				TM_CLK_H;
			//	nop();nop();nop();nop();nop();
        if(temp1 & 0x80)               //д���һλ
            TM_DIO_H;
        else
            TM_DIO_L; 
				//TM_CLK_L;		
				TM_CLK_L;
        temp1 <<= 1;                   //��ѭ��һλ
      //  nop();nop();nop();nop();nop();
			TM_CLK_IN;
			nop();nop();
			I2C_CLK=TM_CLK_READ;
		while(I2C_CLK!=0)
		{I2C_CLK=TM_CLK_READ;}
    }
	//	TM_CLK_H;
		
		
	}

u8 IC_r_BYTE(void) //дһ���ֽڸ�λ��ǰ����λ�ں�
{
    unsigned char i,j,rekey,timeout;
		TM_DIO_IN;
		TM_CLK_IN;
		I2C_DIO = TM_DIO_READ;
		I2C_CLK = TM_CLK_READ;
		rekey=0;
		for(i=0;i<8;i++)
			{
				rekey = rekey<<1;
				TM_CLK_IN;
				I2C_CLK = TM_CLK_READ;
				while(I2C_CLK!=0)
				{
					I2C_CLK = TM_CLK_READ;
				}		
				I2C_DIO = TM_DIO_READ;
				if(I2C_DIO) {rekey++;} 
				TM_CLK_OUT;
				TM_CLK_L;
			}
		return(rekey);
	//	TM_CLK_OUT;
	//	TM_CLK_L;
}

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
		TM_DIO_IN;	
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
		DIO = TM_DIO_READ;
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

void TM_Start(void)	//��ʼ�ź�
{
	TM_DIO_OUT;
	TM_CLK_H;
	TM_DIO_H;
	nop();nop();nop();nop();nop();
	TM_DIO_L;
}

#endif