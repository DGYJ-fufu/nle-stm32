/***********************************************************************************
*	Project:          SHT1x/7x demo program (V2.4)
*	Filename:         SHT1x.c
***********************************************************************************/
#ifndef _SHT1X_H_
#define	_SHT1X_H_


#define LittleEndian true//大端(big-endian)还是小端(little-endian)是相对于一个字来说的，指在一个字中各个字节的顺序。
#define MCU_32_BIT   true//8位机，还是32位机，此处定义

enum {TEMP,HUMI};

#if(MCU_32_BIT!=true) //8位平台
typedef union
{
    unsigned int i;//16bit
    float f;
} value;
#else //32位平台
typedef union
{
    unsigned short int i;//16bit
    float f;
} value;
#endif

//-----------------------------------------------------------------------------
void SHT1X_Init(void);

//----------------------------------------------------------------------------------
void Serial_Init(void);

//----------------------------------------------------------------------------------
char s_write_byte(unsigned char value);

//----------------------------------------------------------------------------------
char s_read_byte(unsigned char ack);

//----------------------------------------------------------------------------------
void s_transstart(void);

//----------------------------------------------------------------------------------
void s_connectionreset(void);

//----------------------------------------------------------------------------------
char s_softreset(void);

//----------------------------------------------------------------------------------
char s_read_statusreg(unsigned char *p_value, unsigned char *p_checksum);

//----------------------------------------------------------------------------------
char s_write_statusreg(unsigned char *p_value);

//----------------------------------------------------------------------------------
char s_measure(unsigned char *p_value, unsigned char *p_checksum, unsigned char mode);

//----------------------------------------------------------------------------------------
void calc_sth11(float *p_humidity ,float *p_temperature);

//--------------------------------------------------------------------
float calc_dewpoint(float h,float t);

//----------------------------------------------------------------------------------
void call_sht1x(float *tem_val, float *hum_val);//读取温湿度


#endif /*_SHT1X_H_*/
