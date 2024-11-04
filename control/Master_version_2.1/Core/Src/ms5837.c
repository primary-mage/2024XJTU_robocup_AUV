#include"ms5837.h"

#define MS5837 0xEC//�궨���豸��ַ
#define IIC_MS5837 hi2c1

uint8_t MS_RESET = 0x1E;//��λ����

uint8_t ADC_READ = 0x00;//ADC��ȡ��ʼ����


/*
C1 ѹ�������� SENS|T1
C2  ѹ������  OFF|T1
C3	�¶�ѹ��������ϵ�� TCS
C4	�¶�ϵ����ѹ������ TCO
C5	�ο��¶� T|REF
C6 	�¶�ϵ�����¶� TEMPSENS
*/

uint32_t  Cal_C[7];	        //���ڴ��PROM�е�6������1-6

double OFF_;
float Aux;
/*
dT ʵ�ʺͲο��¶�֮��Ĳ���
TEMP ʵ���¶�	
*/
uint64_t dT,TEMP;
/*
OFF ʵ���¶Ȳ���
SENS ʵ���¶�������
*/
uint64_t SENS;
uint32_t D1_Pres,D2_Temp=0;	// ����ѹ��ֵ,�����¶�ֵ
uint32_t TEMP2,T2,OFF2,SENS2;	//�¶�У��ֵ

uint32_t Pressure;				//��ѹ
uint32_t Atmdsphere_Pressure;//����ѹ
uint32_t Depth;

/*******************************************************************************
  * @��������	MS583730BA_RESET
  * @����˵��   ��λMS5611
  * @�������   ��
  * @�������   ��
  * @���ز���   ��
*******************************************************************************/
void MS583703BA_RESET(void)
{
		HAL_I2C_Master_Transmit (&IIC_MS5837,MS5837,&MS_RESET,1, HAL_MAX_DELAY);
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:unsigned long MS561101BA_getConversion(void)
*��������:    ��ȡ MS5837 ��ת����� 
*******************************************************************************/
unsigned long MS583703BA_getConversion(uint8_t command)
{
	unsigned long conversion = 0;
	uint8_t temp[3];
	HAL_I2C_Master_Transmit (&IIC_MS5837,MS5837,&command,1,HAL_MAX_DELAY);
	HAL_Delay(10);
	
	HAL_I2C_Master_Transmit(&IIC_MS5837, MS5837, &ADC_READ, 1, HAL_MAX_DELAY);
	
	HAL_I2C_Master_Receive(&IIC_MS5837,MS5837,temp,3,HAL_MAX_DELAY);
	
  conversion = (unsigned long)temp[0] * 65536 + (unsigned long)temp[1] * 256 + (unsigned long)temp[2];
	
	return conversion;
}

void MS5837_init(void)
{	 
  uint8_t num[2];
  uint8_t i; 
	 
	MS583703BA_RESET();	 // Reset Device  ��λMS5837
	HAL_Delay(20);       //��λ����ʱ��ע�������ʱ��һ����Ҫ�ģ��������̵��ƺ���������20ms��
	 
  for (i=1;i<=6;i++) 
	{
		HAL_I2C_Mem_Read(&IIC_MS5837,MS5837,0xA0 + (i*2),1,num,2,HAL_MAX_DELAY);
		
		Cal_C[i] = (((uint16_t)num[0] << 8) | num[1]);//��λ����
		
	}
	
	for(i=0;i<5;i++)
	{
		HAL_Delay(1);
		MS5837_Getdata();   //��ȡ����ѹ
    Atmdsphere_Pressure+=Pressure;	
	}
	Atmdsphere_Pressure=Atmdsphere_Pressure/5;
}

///***********************************************
//  * @brief  ��ȡ��ѹ
//  * @param  None
//  * @retval None
//************************************************/
void MS5837_Getdata(void)
{
	D1_Pres= MS583703BA_getConversion(0x48);
	D2_Temp = MS583703BA_getConversion(0x58);
	
	if(D2_Temp > (((uint32_t)Cal_C[5])*256))
	{		
		dT=D2_Temp - (((uint32_t)Cal_C[5])*256);
		TEMP=2000+dT*((uint32_t)Cal_C[6])/8388608;	
		OFF_=(uint32_t)Cal_C[2]*65536+((uint32_t)Cal_C[4]*dT)/128;
		SENS=(uint32_t)Cal_C[1]*32768+((uint32_t)Cal_C[3]*dT)/256;		
	}
	else 
	{	
		dT=(((uint32_t)Cal_C[5])*256) - D2_Temp;			
		TEMP=2000-dT*((uint32_t)Cal_C[6])/8388608;	
		OFF_=(uint32_t)Cal_C[2]*65536-((uint32_t)Cal_C[4]*dT)/128;
		SENS=(uint32_t)Cal_C[1]*32768-((uint32_t)Cal_C[3]*dT)/256;		
	}
//	printf("1:%d\t%d\t%llu\t%llu\t%f\t%llu\r\n",D1_Pres,D2_Temp,dT,TEMP,OFF_,SENS);
//printf("%d\t",Cal_C[5]);
	if(TEMP<2000)  // low temp
	{
	  Aux = (2000-TEMP)*(2000-TEMP);	
		T2 = 3*(dT*dT)/8589934592; 
		OFF2 = 3*Aux/2;
		SENS2 = 5*Aux/8;	
//		printf("2:%f\t%d\t%d\t%d\r\n",Aux,T2,OFF2,SENS2);		
	}
	else
	{
//		printf("!!");
		
	  Aux = (TEMP-2000)*(TEMP-2000);		
	  T2 = 2*(dT*dT)/137438953472;
		OFF2 = 1*Aux/16;
		SENS2 = 0;	
	}
	OFF_ = OFF_ - OFF2;
	SENS = SENS - SENS2;	
	TEMP=(TEMP-T2)/100;	
  Pressure= ((D1_Pres*SENS/2097152-OFF_)/8192)/10;
	Depth=0.983615*(Pressure-Atmdsphere_Pressure);	 
}
