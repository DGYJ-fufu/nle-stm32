/* Includes ------------------------------------------------------------------*/

#ifndef __ADC_READER_H
#define __ADC_READER_H

#include "stm32l1xx_hal.h"
#include "board.h"

#ifdef __cplusplus
 extern "C" {
#endif 
	 
/**********************************************************************************************
*������void ADCS_Init()
*���ܣ���ʼ��ADCͨ��0��ͨ��2
*���룺��
*�������
*����˵������
**********************************************************************************************/
void ADCS_Init(void);
	 
/**********************************************************************************************
*������void AdcScanChannel()
*���ܣ�ת��ADC˫�ŵ��Ĵ�������Ϊ��ѹֵ
*���룺��
*�������
*����˵������
**********************************************************************************************/
void AdcScanChannel(void);
	 
/**********************************************************************************************
*������float AdcReadCh0()
*���ܣ����ADC0��õ�ѹֵ
*���룺��
*�������
*����˵������
**********************************************************************************************/
float AdcReadCh0(void);

/**********************************************************************************************
*������float AdcReadCh2()
*���ܣ����ADC2��õ�ѹֵ
*���룺��
*�������
*����˵������
**********************************************************************************************/
float AdcReadCh2(void);

#ifdef __cplusplus
}
#endif

#endif /* __ADC_READER_H */

