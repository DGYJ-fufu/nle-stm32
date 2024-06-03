//=============================================================================
//    S E N S I R I O N
//=============================================================================
// Project   :  SHT3x Sample Code (V1.1)
// File      :  sht3x.c (V1.1)
// Brief     :  Sensor Layer: Implementation of functions for sensor access.
//=============================================================================

//-- Includes -----------------------------------------------------------------
#include <math.h>          //library 
#include <stdio.h>
#include "stm32l1xx_hal.h" // controller register definitions 
#include "sht1x.h"         // controller register definitions 
#include "sht3x.h"


//-- Defines ------------------------------------------------------------------
// Generator polynomial for CRC
#define POLYNOMIAL  0x131 // P(x) = x^8 + x^5 + x^4 + 1 = 100110001

#define s_noACK 0
#define s_ACK   1
//adr  command  r/w
#define STATUS_REG_W 0x06   //000   0011    0
#define STATUS_REG_R 0x07   //000   0011    1
#define MEASURE_TEMP 0x03   //000   0001    1
#define MEASURE_HUMI 0x05   //000   0010    1
#define RESET        0x1e   //000   1111    0

//=============================================================================
// IO-Pins                            /* -- adapt the defines for your uC -- */
//-----------------------------------------------------------------------------
#ifndef I2C_ONLY
// Reset on port B, bit 12
#define RESET_LOW()  (GPIOB->BSRR = 0x10000000) // set Reset to low
#define RESET_HIGH() (GPIOB->BSRR = 0x00001000) // set Reset to high
// Alert on port B, bit 10
#define ALERT_READ   (GPIOB->IDR  & 0x0400)     // read Alert
#else
// Reset
#define RESET_LOW()  ; // set Reset to low
#define RESET_HIGH() ; // set Reset to high
// Alert
#define ALERT_READ   (0x00)     // read Alert
#endif
//=============================================================================


//-- Defines ------------------------------------------------------------------
// Serial IO-Pins                        /* -- adapt the defines for your uC -- */
#define SERIAL_SCK_Pin GPIO_PIN_6
#define SERIAL_SCK_Port GPIOB
#define SERIAL_DATA_Pin GPIO_PIN_7
#define SERIAL_DATA_Port GPIOB

// DATA
#define DATA_LOW()  (HAL_GPIO_WritePin(SERIAL_DATA_Port, SERIAL_DATA_Pin, GPIO_PIN_RESET))    // set DATA to low 置低电平
#define DATA_HIGH() (HAL_GPIO_WritePin(SERIAL_DATA_Port, SERIAL_DATA_Pin, GPIO_PIN_SET))      // set DATA to open-drain 置高电平
#define DATA_OUT(n) (HAL_GPIO_WritePin(SERIAL_DATA_Port, SERIAL_DATA_Pin, (GPIO_PinState)n))  // DATA = n
#define DATA_READ() ((unsigned char)HAL_GPIO_ReadPin(SERIAL_DATA_Port, SERIAL_DATA_Pin))      // read DATA

// SCK
#define SCK_LOW()  (HAL_GPIO_WritePin(SERIAL_SCK_Port, SERIAL_SCK_Pin, GPIO_PIN_RESET))       // set SCK to low 置低电平
#define SCK_HIGH() (HAL_GPIO_WritePin(SERIAL_SCK_Port, SERIAL_SCK_Pin, GPIO_PIN_SET))         // set SCK to open-drain 置高电平
#define SCK_OUT(n) (HAL_GPIO_WritePin(SERIAL_SCK_Port, SERIAL_SCK_Pin, (GPIO_PinState)n))     // SCK = n

#ifndef SERIAL_INPUT
#define SERIAL_INPUT  1//置成输入
#endif
#ifndef SERIAL_OUTPUT
#define SERIAL_OUTPUT 0//置成输出
#endif

//-- Global variables ---------------------------------------------------------
uint8_t ShtFlag = 0x00;//0-SHT3X,1-SHT1X

static u8t _i2cAddress; // I2C Address

//-- Static function prototypes -----------------------------------------------
static etError SHT3X_WriteAlertLimitData(ft humidity, ft temperature);
static etError SHT3X_ReadAlertLimitData(ft* humidity, ft* temperature);
static etError SHT3X_StartWriteAccess(void);
static etError SHT3X_StartReadAccess(void);
static void SHT3X_StopAccess(void);
static etError SHT3X_WriteCommand(etCommands command);
static etError SHT3X_Read2BytesAndCrc(u16t* data, etI2cAck finaleAckNack,
                                      u8t timeout);
static etError SHT3X_Write2BytesAndCrc(u16t data);
static u8t SHT3X_CalcCrc(u8t data[], u8t nbrOfBytes);
static etError SHT3X_CheckCrc(u8t data[], u8t nbrOfBytes, u8t checksum);
static ft SHT3X_CalcTemperature(u16t rawValue);
static ft SHT3X_CalcHumidity(u16t rawValue);
static u16t SHT3X_CalcRawTemperature(ft temperature);
static u16t SHT3X_CalcRawHumidity(ft humidity);

//=============================================================================
//    S E N S I R I O N
//=============================================================================
// Project   :  SHT3x Sample Code (V1.1)
// File      :  i2c_hal.c (V1.1)
// Brief     :  I2C驱动程序
//=============================================================================

//-- Includes -----------------------------------------------------------------
#include "hal_i2c.h"

//-- Defines ------------------------------------------------------------------
// I2C IO-Pins                        /* -- adapt the defines for your uC -- */
#define I2C_SCL_Pin GPIO_PIN_6
#define I2C_SCL_Port GPIOB
#define I2C_SDA_Pin GPIO_PIN_7
#define I2C_SDA_Port GPIOB

// SDA
#define SDA_LOW()  (HAL_GPIO_WritePin(I2C_SDA_Port, I2C_SDA_Pin, GPIO_PIN_RESET)) // set SDA to low 置低电平
#define SDA_OPEN() (HAL_GPIO_WritePin(I2C_SDA_Port, I2C_SDA_Pin, GPIO_PIN_SET))   // set SDA to open-drain 置高电平
#define SDA_READ   ((u8t)HAL_GPIO_ReadPin(I2C_SDA_Port, I2C_SDA_Pin))     // read SDA

// SCL /* -- adapt the defines for your uC -- */
#define SCL_LOW()  (HAL_GPIO_WritePin(I2C_SCL_Port, I2C_SCL_Pin, GPIO_PIN_RESET)) // set SCL to low 置低电平
#define SCL_OPEN() (HAL_GPIO_WritePin(I2C_SCL_Port, I2C_SCL_Pin, GPIO_PIN_SET))   // set SCL to open-drain 置高电平
#define SCL_READ   ((u8t)HAL_GPIO_ReadPin(I2C_SCL_Port, I2C_SCL_Pin))     // read SCL

#define I2C_INPUT  1//置成输入
#define I2C_OUTPUT 0//置成输出

//-- Static function prototypes -----------------------------------------------
static etError I2c_WaitWhileClockStreching(u8t timeout);

//-----------------------------------------------------------------------------
void I2c_sdaInOutInit(u8t InOut)                      /* -- 根据硬件平台进行修改，初始化SDA引脚，InOut-1输入，InOut-0输出 -- */
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  
  /*Configure GPIO pins*/
  GPIO_InitStruct.Pin = I2C_SDA_Pin;
  if(InOut != 1)
  {
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  }
  else
  {
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  }
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(I2C_SDA_Port, &GPIO_InitStruct);
}

void I2c_SclInOutInit(u8t InOut)                      /* -- 根据硬件平台进行修改，初始化SCL引脚，InOut-1输入，InOut-0输出 -- */
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  
  /*Configure GPIO pins*/
  GPIO_InitStruct.Pin = I2C_SCL_Pin;
  if(InOut != 1)
  {
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  }
  else
  {
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  }
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(I2C_SCL_Port, &GPIO_InitStruct);
}

void I2c_Init(void)                      /* -- adapt the init for your uC -- */
{
#if 0
    //初始化I2C总线IO口SDA、SCL
    ;
    ;
    SDA_OPEN();                  // I2C-bus idle mode SDA released
    SCL_OPEN();                  // I2C-bus idle mode SCL released

    // SDA
    // SCL
    // set open-drain output for SDA and SCL 
    //SDA、SCL设置为漏极开路
    ;
    ;
#else
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  
  __HAL_RCC_GPIOB_CLK_ENABLE();
  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, I2C_SCL_Pin|I2C_SDA_Pin, GPIO_PIN_SET);

  /*Configure GPIO pins : PBPin PBPin */
  GPIO_InitStruct.Pin = I2C_SCL_Pin|I2C_SDA_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
#endif
}

//-----------------------------------------------------------------------------
void I2c_StartCondition(void)
{
    SDA_OPEN();
    DelayMicroSeconds(1);
    SCL_OPEN();
    DelayMicroSeconds(1);
    SDA_LOW();
    DelayMicroSeconds(10);  // hold time start condition (t_HD;STA)
    SCL_LOW();
    DelayMicroSeconds(10);
}

//-----------------------------------------------------------------------------
void I2c_StopCondition(void)
{
    SCL_LOW();
    DelayMicroSeconds(1);
    SDA_LOW();
    DelayMicroSeconds(1);
    SCL_OPEN();
    DelayMicroSeconds(10);  // set-up time stop condition (t_SU;STO)
    SDA_OPEN();
    DelayMicroSeconds(10);
}

//-----------------------------------------------------------------------------
etError I2c_WriteByte(u8t txByte)
{
    etError error = NO_ERROR;
    u8t     mask;
    for(mask = 0x80; mask > 0; mask >>= 1)// shift bit for masking (8 times)
    {
        if((mask & txByte) == 0) SDA_LOW(); // masking txByte, write bit to SDA-Line
        else                     SDA_OPEN();
        DelayMicroSeconds(1);               // data set-up time (t_SU;DAT)
        SCL_OPEN();                         // generate clock pulse on SCL
        DelayMicroSeconds(5);               // SCL high time (t_HIGH)
        SCL_LOW();
        DelayMicroSeconds(1);               // data hold time(t_HD;DAT)
    }
    SDA_OPEN();                           // release SDA-line
    SCL_OPEN();                           // clk #9 for ack
    DelayMicroSeconds(1);                 // data set-up time (t_SU;DAT)
    I2c_sdaInOutInit(I2C_INPUT);          //SDA置为输入
    if(SDA_READ) error = ACK_ERROR;       // check ack from i2c slave
    SCL_LOW();
    DelayMicroSeconds(20);                // wait to see byte package on scope
    I2c_sdaInOutInit(I2C_OUTPUT);         //SDA置为输出
    return error;                         // return error code
}

//-----------------------------------------------------------------------------
etError I2c_ReadByte(u8t *rxByte, etI2cAck ack, u8t timeout)
{
    etError error = NO_ERROR;
    u8t mask;
    *rxByte = 0x00;
    SDA_OPEN();                            // release SDA-line
    I2c_sdaInOutInit(I2C_INPUT);           //SDA置为输入
    for(mask = 0x80; mask > 0; mask >>= 1) // shift bit for masking (8 times)
    {
        SCL_OPEN();                          // start clock on SCL-line
        DelayMicroSeconds(1);                // clock set-up time (t_SU;CLK)
        error = I2c_WaitWhileClockStreching(timeout);// wait while clock streching
        DelayMicroSeconds(3);                // SCL high time (t_HIGH)
        if(SDA_READ) *rxByte |= mask;        // read bit
        SCL_LOW();
        DelayMicroSeconds(1);                // data hold time(t_HD;DAT)
    }
    I2c_sdaInOutInit(I2C_OUTPUT);          //SDA置为输出
    if(ack == ACK) SDA_LOW();              // send acknowledge if necessary
    else           SDA_OPEN();
    DelayMicroSeconds(1);                  // data set-up time (t_SU;DAT)
    SCL_OPEN();                            // clk #9 for ack
    DelayMicroSeconds(5);                  // SCL high time (t_HIGH)
    SCL_LOW();
    SDA_OPEN();                            // release SDA-line
    DelayMicroSeconds(20);                 // wait to see byte package on scope

    return error;                          // return with no error
}

//-----------------------------------------------------------------------------
etError I2c_GeneralCallReset(void)
{
    etError error;

    I2c_StartCondition();
    error = I2c_WriteByte(0x00);
    if(error == NO_ERROR) error = I2c_WriteByte(0x06);

    return error;
}

//-----------------------------------------------------------------------------
void DelayMicroSeconds(u32t nbrOfUs)   /* -- adapt this delay for your uC -- */
{
    u32t i;
    for(i = 0; i < nbrOfUs; i++)
    {
        __nop();  // nop's may be added or removed for timing adjustment
        __nop();
        __nop();
        __nop();
    }
}

//-----------------------------------------------------------------------------
static etError I2c_WaitWhileClockStreching(u8t timeout)
{
    etError error = NO_ERROR;
    I2c_SclInOutInit(I2C_INPUT);          //SCL置为输入
    while(SCL_READ == 0)
    {
        if(timeout-- == 0) return TIMEOUT_ERROR;
        DelayMicroSeconds(1000);
    }
    I2c_SclInOutInit(I2C_OUTPUT);         //SCL置为输出
    return error;
}

void connectionreset(void)
{  
    //代码预留区
}

char s_softreset(void)
//----------------------------------------------------------------------------------
// resets the sensor by a softreset
{
    unsigned char error=0;
    s_connectionreset();              //reset communication
    error+=s_write_byte(RESET);       //send RESET-command to sensor
    return error;                     //error=1 in case of no response form the sensor
}

//-- Static function prototypes -----------------------------------------------
static void Serial_DelayUS(unsigned long nbrOfUs)   /* -- adapt this delay for your uC -- */
{
    unsigned long i,j;
    for(i = 0; i < nbrOfUs; i++)
    for(j = 0; j < 8; j++)
    {
        __nop();  // nop's may be added or removed for timing adjustment
        __nop();
        __nop();
        __nop();
    }
}

void SHT1X_Init(void)                      /* -- 初始化GPIO和SHT1X初始状态 -- */
{
    Serial_Init();
    s_connectionreset();
    s_softreset();
    Serial_DelayUS((11+5)*1000);//至少要延时等待11ms，SHT1X才会就绪
}

//-----------------------------------------------------------------------------
void Serial_DataInOutInit(unsigned char InOut)                      /* -- 根据硬件平台进行修改，初始化DATA引脚，InOut-1输入，InOut-0输出 -- */
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /*Configure GPIO pins*/
    GPIO_InitStruct.Pin = SERIAL_DATA_Pin;
    if(InOut != 1)
    {
        GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
    }
    else
    {
        GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    }
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(SERIAL_DATA_Port, &GPIO_InitStruct);
}

void Serial_SckInOutInit(unsigned char InOut)                      /* -- 根据硬件平台进行修改，初始化SCK引脚，InOut-1输入，InOut-0输出 -- */
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /*Configure GPIO pins*/
    GPIO_InitStruct.Pin = SERIAL_SCK_Pin;
    if(InOut != 1)
    {
        GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
    }
    else
    {
        GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    }
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(SERIAL_SCK_Port, &GPIO_InitStruct);
}

void Serial_Init(void)                      //-- adapt the init for your uC --
{
    //初始化Serial总线IO口DATA、SCK
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    __HAL_RCC_GPIOB_CLK_ENABLE();
    //Configure GPIO pin Output Level 
    HAL_GPIO_WritePin(GPIOB, SERIAL_SCK_Pin|SERIAL_DATA_Pin, GPIO_PIN_SET);//SERIAL_SCK|SERIAL_DATA置1

    //Configure GPIO pins : PBPin PBPin 
    GPIO_InitStruct.Pin = SERIAL_SCK_Pin|SERIAL_DATA_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);//DATA、SCK设置为漏极开路
}

//----------------------------------------------------------------------------------
char s_write_byte(unsigned char value)
//----------------------------------------------------------------------------------
// writes a byte on the Sensibus and checks the acknowledge
{
    unsigned char i,error=0;
    for(i=0x80; i>0; i/=2)           //shift bit for masking
    {
        if (i & value)
            DATA_HIGH();          //masking value with i , write to SENSI-BUS
        else
            DATA_LOW();
        Serial_DelayUS(1);                        //observe setup time
        SCK_HIGH();                          //clk for SENSI-BUS
        Serial_DelayUS(3);        //pulswith approx. 5 us
        SCK_LOW();
        Serial_DelayUS(1);                         //observe hold time
    }
    DATA_HIGH();                           //release DATA-line
    Serial_DelayUS(1);                          //observe setup time
    SCK_HIGH();                            //clk #9 for ack
    Serial_DataInOutInit(SERIAL_INPUT);          //DATA置为输入
    for(i=0; i<0xFF; i++)
    {
      error=DATA_READ();                       //check ack (DATA will be pulled down by SHT11)
      if(error != 1) break;
      Serial_DelayUS(1);
    }
    Serial_DataInOutInit(SERIAL_OUTPUT);         //DATA置为输出
    SCK_LOW();
    return error;                     //error=1 in case of no acknowledge
}

//----------------------------------------------------------------------------------
char s_read_byte(unsigned char ack)
//----------------------------------------------------------------------------------
// reads a byte form the Sensibus and gives an acknowledge in case of "ack=1"
{
    unsigned char i,val=0;
    DATA_HIGH();                           //release DATA-line
    Serial_DataInOutInit(SERIAL_INPUT);          //DATA置为输入
    for (i=0x80; i>0; i/=2)           //shift bit for masking
    {
        SCK_HIGH();                          //clk for SENSI-BUS
        Serial_DelayUS(3);
        if (DATA_READ())                    //read bit
        {val = (val|i);}        
        SCK_LOW();
        Serial_DelayUS(2);
    }
    Serial_DataInOutInit(SERIAL_OUTPUT);         //DATA置为输出
    DATA_OUT(!ack);                        //in case of "ack==1" pull down DATA-Line
    Serial_DelayUS(1);                          //observe setup time
    SCK_HIGH();                            //clk #9 for ack
    Serial_DelayUS(3);          //pulswith approx. 5 us
    SCK_LOW();
    Serial_DelayUS(1);                          //observe hold time
    DATA_HIGH();                           //release DATA-line
    return val;
}

//----------------------------------------------------------------------------------
void s_transstart(void)
//----------------------------------------------------------------------------------
// generates a transmission start
//       _____         ________
// DATA:      |_______|
//           ___     ___
// SCK : ___|   |___|   |______
{
    DATA_HIGH();
    SCK_LOW();                   //Initial state
    Serial_DelayUS(1);
    SCK_HIGH();
    Serial_DelayUS(1);
    DATA_LOW();
    Serial_DelayUS(1);
    SCK_LOW();
    Serial_DelayUS(3);
    SCK_HIGH();
    Serial_DelayUS(1);
    DATA_HIGH();
    Serial_DelayUS(1);
    SCK_LOW();
}


//----------------------------------------------------------------------------------
void s_connectionreset(void)
//----------------------------------------------------------------------------------
// communication reset: DATA-line=1 and at least 9 SCK cycles followed by transstart
//       _____________________________________________________         ________
// DATA:                                                      |_______|
//          _    _    _    _    _    _    _    _    _        ___     ___
// SCK : __| |__| |__| |__| |__| |__| |__| |__| |__| |______|   |___|   |______
{
    unsigned char i;
    DATA_HIGH();
    SCK_LOW();                    //Initial state
    for(i=0; i<9; i++)                //9 SCK cycles
    {
        Serial_DelayUS(1);
        SCK_HIGH();
        Serial_DelayUS(1);
        SCK_LOW();
    }
    s_transstart();                   //transmission start
}

//----------------------------------------------------------------------------------
char s_read_statusreg(unsigned char *p_value, unsigned char *p_checksum)
//----------------------------------------------------------------------------------
// reads the status register with checksum (8-bit)
{
    unsigned char error=0;
    s_transstart();                   //transmission start
    error=s_write_byte(STATUS_REG_R); //send command to sensor
    *p_value=s_read_byte(s_ACK);        //read status register (8-bit)
    *p_checksum=s_read_byte(s_noACK);   //read checksum (8-bit)
    return error;                     //error=1 in case of no response form the sensor
}

//----------------------------------------------------------------------------------
char s_write_statusreg(unsigned char *p_value)
//----------------------------------------------------------------------------------
// writes the status register with checksum (8-bit)
{
    unsigned char error=0;
    s_transstart();                   //transmission start
    error+=s_write_byte(STATUS_REG_W);//send command to sensor
    error+=s_write_byte(*p_value);    //send value of status register
    return error;                     //error>=1 in case of no response form the sensor
}

//----------------------------------------------------------------------------------
char s_measure(unsigned char *p_value, unsigned char *p_checksum, unsigned char mode)
//----------------------------------------------------------------------------------
// makes a measurement (humidity/temperature) with checksum
{
    unsigned char error=0;
    unsigned int i;

    s_transstart();                   //transmission start
    switch(mode)
    {   //send command to sensor
    case TEMP	:
        error+=s_write_byte(MEASURE_TEMP);
        break;
    case HUMI	:
        error+=s_write_byte(MEASURE_HUMI);
        break;
    default     :
        break;
    }
    Serial_DataInOutInit(SERIAL_INPUT);          //DATA置为输入
    for (i=0; i<65535; i++)
    {
        if(DATA_READ()==0) break; //wait until sensor has finished the measurement
        Serial_DelayUS(3);
    }
    if(DATA_READ()) error+=1;                // or timeout (~2 sec.) is reached
    Serial_DataInOutInit(SERIAL_OUTPUT);         //DATA置为输出
#if(LittleEndian!=true)
    //p_value采用大端格式存储
    *(p_value)  =s_read_byte(s_ACK);    //read the first byte (MSB)
    *(p_value+1)=s_read_byte(s_ACK);    //read the second byte (LSB)
#else
    //p_value采用小端格式存储
    *(p_value+1)=s_read_byte(s_ACK);    //read the first byte (MSB)
    *(p_value)  =s_read_byte(s_ACK);    //read the second byte (LSB)
#endif
    *p_checksum =s_read_byte(s_noACK);  //read checksum
    return error;
}

//----------------------------------------------------------------------------------------
void calc_sth11(float *p_humidity ,float *p_temperature)
//----------------------------------------------------------------------------------------
// calculates temperature [癈] and humidity [%RH]
// input :  humi [Ticks] (12 bit)
//          temp [Ticks] (14 bit)
// output:  humi [%RH]
//          temp [癈]
{
    const float C1=-2.0468;           // for 12 Bit RH
    const float C2=+0.0367;           // for 12 Bit RH
    const float C3=-0.0000015955;     // for 12 Bit RH
    const float T1=+0.01;             // for 12 Bit RH
    const float T2=+0.00008;          // for 12 Bit RH

    float rh=*p_humidity;             // rh:      Humidity [Ticks] 12 Bit
    float t=*p_temperature;           // t:       Temperature [Ticks] 14 Bit
    float rh_lin;                     // rh_lin:  Humidity linear
    float rh_true;                    // rh_true: Temperature compensated humidity
    float t_C;                        // t_C   :  Temperature [癈]

    t_C=t*0.01 - 40.1;                //calc. temperature [癈] from 14 bit temp. ticks @ 5V
    rh_lin=C3*rh*rh + C2*rh + C1;     //calc. humidity from ticks to [%RH]
    rh_true=(t_C-25)*(T1+T2*rh)+rh_lin;   //calc. temperature compensated humidity [%RH]
    if(rh_true>100)rh_true=100;       //cut if the value is outside of
    if(rh_true<0.1)rh_true=0.1;       //the physical possible range

    *p_temperature=t_C;               //return temperature [癈]
    *p_humidity=rh_true;              //return humidity[%RH]
}

//--------------------------------------------------------------------
/*float calc_dewpoint(float h,float t)
//--------------------------------------------------------------------
// calculates dew point
// input:   humidity [%RH], temperature [癈]
// output:  dew point [癈]
{
    float k,dew_point ;

    k = (log10(h)-2)/0.4343 + (17.62*t)/(243.12+t);
    dew_point = 243.12*k/(17.62-k);
    return dew_point;
}*/

//----------------------------------------------------------------------------------
// 读取SH1x温湿度值
//      *tem_val:温度存放地址
//      *hum_val:湿度存放地址
// 程序工作流程
// 1. connection reset
// 2. measure humidity [ticks](12 bit) and temperature [ticks](14 bit)
// 3. calculate humidity [%RH] and temperature [癈]
// 4. calculate dew point [癈]
// 5. end
//----------------------------------------------------------------------------------
void call_sht1x(float *tem_val, float *hum_val)
{ 
    
//#define xDewPoint
  
    value humi_val,temp_val;
//#ifdef DewPoint
    //float dew_point;
//#endif

    unsigned char error,checksum;

    s_connectionreset();
    error=0;
    error+=s_measure((unsigned char*) &humi_val.i,&checksum,HUMI);  //measure humidity
    error+=s_measure((unsigned char*) &temp_val.i,&checksum,TEMP);  //measure temperature
    if(error!=0) 
    {
        s_connectionreset();                 //in case of an error: connection reset
    }
    else
    {   humi_val.f=(float)humi_val.i;                   //converts integer to float
        temp_val.f=(float)temp_val.i;                   //converts integer to float
        calc_sth11(&humi_val.f,&temp_val.f);            //calculate humidity, temperature
#ifdef DewPoint
        dew_point=calc_dewpoint(humi_val.f,temp_val.f); //calculate dew point
#endif
        //printf("Temp:%5.1f℃ Humi:%5.1f%%\n",temp_val.f,humi_val.f);
    }
    //四舍五入
//    *tem_val=(int)(((temp_val.f*10)+5)/10);
//    *hum_val=(int)(((humi_val.f*10)+5)/10);
		*tem_val=temp_val.f;
    *hum_val=humi_val.f;
}

//-----------------------------------------------------------------------------
void SHTXX_Init(void)
{
    etError error; // error code

    error = SHT3X_Init(DEFAULT_SHT3X_ADDR);
    if(error != NO_ERROR)
    {
        SHT1X_Init();
        ShtFlag = 0x01;
    }
    if(ShtFlag != 0x01)
    {
        //printf("温湿度传感器型号：SHT3X\r\n");
    }
    else
    {
        //printf("温湿度传感器型号：SHT1X\r\n");
    }
}

//初始化温湿度模块
void hal_temHumInit(void)
{ 
    SHTXX_Init();
}  

//-----------------------------------------------------------------------------
etError SHT3X_Init(u8t i2cAddress)          /* -- adapt the init for your uC -- */
{
    etError   error = NO_ERROR;       // error code
#if 0 //原版
  
    // init I/O-pins 初始化MCU控制复位Reset、警报Alert引脚功能
    ;// Alert引脚
    ;// Reset引脚

    RESET_LOW();
    
    I2c_Init(); //Init I2C
    SHT3X_SetI2cAdr(i2cAddress);
    //Release reset
    RESET_HIGH();
    return error;
#else
    u32t      serialNumber;// serial number
    //Init I2C
    I2c_Init();
    SHT3X_SetI2cAdr(i2cAddress);
    //Reset
    SHT3X_SoftReset();
    // wait 50ms after power on
    DelayMicroSeconds(50000);
    error = SHT3x_ReadSerialNumber(&serialNumber);
    //printf("SHT3X Serial Number = %lu\r\n", serialNumber);
    return error;
#endif
}

//-----------------------------------------------------------------------------
void SHT3X_SetI2cAdr(u8t i2cAddress)
{
    _i2cAddress = i2cAddress;
}

//-----------------------------------------------------------------------------
etError SHT3x_ReadSerialNumber(u32t* serialNumber)
{
    etError error; // error code
    u16t serialNumWords[2];

    error = SHT3X_StartWriteAccess();

    // write "read serial number" command
    error |= SHT3X_WriteCommand(CMD_READ_SERIALNBR);
    // if no error, start read access
    if(error == NO_ERROR) error = SHT3X_StartReadAccess();
    // if no error, read first serial number word
    if(error == NO_ERROR) error = SHT3X_Read2BytesAndCrc(&serialNumWords[0], ACK, 100);
    // if no error, read second serial number word
    if(error == NO_ERROR) error = SHT3X_Read2BytesAndCrc(&serialNumWords[1], NACK, 0);

    SHT3X_StopAccess();

    // if no error, calc serial number as 32-bit integer
    if(error == NO_ERROR)
    {
        *serialNumber = (serialNumWords[0] << 16) | serialNumWords[1];
    }

    return error;
}

//-----------------------------------------------------------------------------
etError SHT3X_ReadStatus(u16t* status)
{
    etError error; // error code

    error = SHT3X_StartWriteAccess();

    // if no error, write "read status" command
    if(error == NO_ERROR) error = SHT3X_WriteCommand(CMD_READ_STATUS);
    // if no error, start read access
    if(error == NO_ERROR) error = SHT3X_StartReadAccess();
    // if no error, read status
    if(error == NO_ERROR) error = SHT3X_Read2BytesAndCrc(status, NACK, 0);

    SHT3X_StopAccess();

    return error;
}

//-----------------------------------------------------------------------------
etError SHT3X_ClearAllAlertFlags(void)
{
    etError error; // error code

    error = SHT3X_StartWriteAccess();

    // if no error, write clear status register command
    if(error == NO_ERROR) error = SHT3X_WriteCommand(CMD_CLEAR_STATUS);

    SHT3X_StopAccess();

    return error;
}

//-----------------------------------------------------------------------------
etError SHT3X_GetTempAndHumi(ft* temperature, ft* humidity,
                             etRepeatability repeatability, etMode mode,
                             u8t timeout)
{
    etError error;

    switch(mode)
    {
    case MODE_CLKSTRETCH: // get temperature with clock stretching mode
        error = SHT3X_GetTempAndHumiClkStretch(temperature, humidity,
                                               repeatability, timeout);
        break;
    case MODE_POLLING:    // get temperature with polling mode
        error = SHT3X_GetTempAndHumiPolling(temperature, humidity,
                                            repeatability, timeout);
        break;
    default:
        error = PARM_ERROR;
        break;
    }

    return error;
}


//-----------------------------------------------------------------------------
etError SHT3X_GetTempAndHumiClkStretch(ft* temperature, ft* humidity,
                                       etRepeatability repeatability,
                                       u8t timeout)
{
    etError error;        // error code
    u16t    rawValueTemp; // temperature raw value from sensor
    u16t    rawValueHumi; // humidity raw value from sensor

    error = SHT3X_StartWriteAccess();

    // if no error ...
    if(error == NO_ERROR)
    {
        // start measurement in clock stretching mode
        // use depending on the required repeatability, the corresponding command
        switch(repeatability)
        {
        case REPEATAB_LOW:
            error = SHT3X_WriteCommand(CMD_MEAS_CLOCKSTR_L);
            break;
        case REPEATAB_MEDIUM:
            error = SHT3X_WriteCommand(CMD_MEAS_CLOCKSTR_M);
            break;
        case REPEATAB_HIGH:
            error = SHT3X_WriteCommand(CMD_MEAS_CLOCKSTR_H);
            break;
        default:
            error = PARM_ERROR;
            break;
        }
    }

    // if no error, start read access
    if(error == NO_ERROR) error = SHT3X_StartReadAccess();
    // if no error, read temperature raw values
    if(error == NO_ERROR) error = SHT3X_Read2BytesAndCrc(&rawValueTemp, ACK, timeout);
    // if no error, read humidity raw values
    if(error == NO_ERROR) error = SHT3X_Read2BytesAndCrc(&rawValueHumi, NACK, 0);

    SHT3X_StopAccess();

    // if no error, calculate temperature in 癈 and humidity in %RH
    if(error == NO_ERROR)
    {
        *temperature = SHT3X_CalcTemperature(rawValueTemp);
        *humidity = SHT3X_CalcHumidity(rawValueHumi);
    }

    return error;
}

//-----------------------------------------------------------------------------
etError SHT3X_GetTempAndHumiPolling(ft* temperature, ft* humidity,
                                    etRepeatability repeatability,
                                    u8t timeout)
{
    etError error;           // error code
    u16t    rawValueTemp;    // temperature raw value from sensor
    u16t    rawValueHumi;    // humidity raw value from sensor

    error  = SHT3X_StartWriteAccess();

    // if no error ...
    if(error == NO_ERROR)
    {
        // start measurement in polling mode
        // use depending on the required repeatability, the corresponding command
        switch(repeatability)
        {
        case REPEATAB_LOW:
            error = SHT3X_WriteCommand(CMD_MEAS_POLLING_L);
            break;
        case REPEATAB_MEDIUM:
            error = SHT3X_WriteCommand(CMD_MEAS_POLLING_M);
            break;
        case REPEATAB_HIGH:
            error = SHT3X_WriteCommand(CMD_MEAS_POLLING_H);
            break;
        default:
            error = PARM_ERROR;
            break;
        }
    }

    // if no error, wait until measurement ready
    if(error == NO_ERROR)
    {
        // poll every 1ms for measurement ready until timeout
        while(timeout--)
        {
            // check if the measurement has finished
            error = SHT3X_StartReadAccess();

            // if measurement has finished -> exit loop
            if(error == NO_ERROR) break;

            // delay 1ms
            DelayMicroSeconds(1000);
        }

        // check for timeout error
        if(timeout == 0) error = TIMEOUT_ERROR;
    }

    // if no error, read temperature and humidity raw values
    if(error == NO_ERROR)
    {
        error |= SHT3X_Read2BytesAndCrc(&rawValueTemp, ACK, 0);
        error |= SHT3X_Read2BytesAndCrc(&rawValueHumi, NACK, 0);
    }

    SHT3X_StopAccess();

    // if no error, calculate temperature in 癈 and humidity in %RH
    if(error == NO_ERROR)
    {
        *temperature = SHT3X_CalcTemperature(rawValueTemp);
        *humidity = SHT3X_CalcHumidity(rawValueHumi);
    }

    return error;
}

//-----------------------------------------------------------------------------
etError SHT3X_StartPeriodicMeasurment(etRepeatability repeatability,
                                      etFrequency frequency)
{
    etError error;        // error code

    error = SHT3X_StartWriteAccess();

    // if no error, start periodic measurement
    if(error == NO_ERROR)
    {
        // use depending on the required repeatability and frequency,
        // the corresponding command
        switch(repeatability)
        {
        case REPEATAB_LOW: // low repeatability
            switch(frequency)
            {
            case FREQUENCY_HZ5:  // low repeatability,  0.5 Hz
                error |= SHT3X_WriteCommand(CMD_MEAS_PERI_05_L);
                break;
            case FREQUENCY_1HZ:  // low repeatability,  1.0 Hz
                error |= SHT3X_WriteCommand(CMD_MEAS_PERI_1_L);
                break;
            case FREQUENCY_2HZ:  // low repeatability,  2.0 Hz
                error |= SHT3X_WriteCommand(CMD_MEAS_PERI_2_L);
                break;
            case FREQUENCY_4HZ:  // low repeatability,  4.0 Hz
                error |= SHT3X_WriteCommand(CMD_MEAS_PERI_4_L);
                break;
            case FREQUENCY_10HZ: // low repeatability, 10.0 Hz
                error |= SHT3X_WriteCommand(CMD_MEAS_PERI_10_L);
                break;
            default:
                error |= PARM_ERROR;
                break;
            }
            break;

        case REPEATAB_MEDIUM: // medium repeatability
            switch(frequency)
            {
            case FREQUENCY_HZ5:  // medium repeatability,  0.5 Hz
                error |= SHT3X_WriteCommand(CMD_MEAS_PERI_05_M);
                break;
            case FREQUENCY_1HZ:  // medium repeatability,  1.0 Hz
                error |= SHT3X_WriteCommand(CMD_MEAS_PERI_1_M);
                break;
            case FREQUENCY_2HZ:  // medium repeatability,  2.0 Hz
                error |= SHT3X_WriteCommand(CMD_MEAS_PERI_2_M);
                break;
            case FREQUENCY_4HZ:  // medium repeatability,  4.0 Hz
                error |= SHT3X_WriteCommand(CMD_MEAS_PERI_4_M);
                break;
            case FREQUENCY_10HZ: // medium repeatability, 10.0 Hz
                error |= SHT3X_WriteCommand(CMD_MEAS_PERI_10_M);
                break;
            default:
                error |= PARM_ERROR;
                break;
            }
            break;

        case REPEATAB_HIGH: // high repeatability
            switch(frequency)
            {
            case FREQUENCY_HZ5:  // high repeatability,  0.5 Hz
                error |= SHT3X_WriteCommand(CMD_MEAS_PERI_05_H);
                break;
            case FREQUENCY_1HZ:  // high repeatability,  1.0 Hz
                error |= SHT3X_WriteCommand(CMD_MEAS_PERI_1_H);
                break;
            case FREQUENCY_2HZ:  // high repeatability,  2.0 Hz
                error |= SHT3X_WriteCommand(CMD_MEAS_PERI_2_H);
                break;
            case FREQUENCY_4HZ:  // high repeatability,  4.0 Hz
                error |= SHT3X_WriteCommand(CMD_MEAS_PERI_4_H);
                break;
            case FREQUENCY_10HZ: // high repeatability, 10.0 Hz
                error |= SHT3X_WriteCommand(CMD_MEAS_PERI_10_H);
                break;
            default:
                error |= PARM_ERROR;
                break;
            }
            break;
        default:
            error |= PARM_ERROR;
            break;
        }
    }

    SHT3X_StopAccess();

    return error;
}

//-----------------------------------------------------------------------------
etError SHT3X_ReadMeasurementBuffer(ft* temperature, ft* humidity)
{
    etError  error;        // error code
    u16t     rawValueTemp; // temperature raw value from sensor
    u16t     rawValueHumi; // humidity raw value from sensor

    error = SHT3X_StartWriteAccess();

    // if no error, read measurements
    if(error == NO_ERROR) error = SHT3X_WriteCommand(CMD_FETCH_DATA);
    if(error == NO_ERROR) error = SHT3X_StartReadAccess();
    if(error == NO_ERROR) error = SHT3X_Read2BytesAndCrc(&rawValueTemp, ACK, 0);
    if(error == NO_ERROR) error = SHT3X_Read2BytesAndCrc(&rawValueHumi, NACK, 0);

    // if no error, calculate temperature in 癈 and humidity in %RH
    if(error == NO_ERROR)
    {
        *temperature = SHT3X_CalcTemperature(rawValueTemp);
        *humidity = SHT3X_CalcHumidity(rawValueHumi);
    }

    SHT3X_StopAccess();

    return error;
}

//-----------------------------------------------------------------------------
etError SHT3X_EnableHeater(void)
{
    etError error; // error code

    error = SHT3X_StartWriteAccess();

    // if no error, write heater enable command
    if(error == NO_ERROR) error = SHT3X_WriteCommand(CMD_HEATER_ENABLE);

    SHT3X_StopAccess();

    return error;
}

//-----------------------------------------------------------------------------
etError SHT3X_DisableHeater(void)
{
    etError error; // error code

    error = SHT3X_StartWriteAccess();

    // if no error, write heater disable command
    if(error == NO_ERROR) error = SHT3X_WriteCommand(CMD_HEATER_DISABLE);

    SHT3X_StopAccess();

    return error;
}


//-----------------------------------------------------------------------------
etError SHT3X_SetAlertLimits(ft humidityHighSet,   ft temperatureHighSet,
                             ft humidityHighClear, ft temperatureHighClear,
                             ft humidityLowClear,  ft temperatureLowClear,
                             ft humidityLowSet,    ft temperatureLowSet)
{
    etError  error;  // error code

    // write humidity & temperature alter limits, high set
    error = SHT3X_StartWriteAccess();
    if(error == NO_ERROR) error = SHT3X_WriteCommand(CMD_W_AL_LIM_HS);
    if(error == NO_ERROR) error = SHT3X_WriteAlertLimitData(humidityHighSet,
                                      temperatureHighSet);
    SHT3X_StopAccess();

    if(error == NO_ERROR)
    {
        // write humidity & temperature alter limits, high clear
        error = SHT3X_StartWriteAccess();
        if(error == NO_ERROR) error = SHT3X_WriteCommand(CMD_W_AL_LIM_HC);
        if(error == NO_ERROR) error = SHT3X_WriteAlertLimitData(humidityHighClear,
                                          temperatureHighClear);
        SHT3X_StopAccess();
    }

    if(error == NO_ERROR)
    {
        // write humidity & temperature alter limits, low clear
        error = SHT3X_StartWriteAccess();
        if(error == NO_ERROR) error = SHT3X_WriteCommand(CMD_W_AL_LIM_LC);
        if(error == NO_ERROR) error = SHT3X_WriteAlertLimitData(humidityLowClear,
                                          temperatureLowClear);
        SHT3X_StopAccess();
    }

    if(error == NO_ERROR)
    {
        // write humidity & temperature alter limits, low set
        error = SHT3X_StartWriteAccess();
        if(error == NO_ERROR) error = SHT3X_WriteCommand(CMD_W_AL_LIM_LS);
        if(error == NO_ERROR) error = SHT3X_WriteAlertLimitData(humidityLowSet,
                                          temperatureLowSet);
        SHT3X_StopAccess();
    }

    return error;
}

//-----------------------------------------------------------------------------
etError SHT3X_GetAlertLimits(ft* humidityHighSet,   ft* temperatureHighSet,
                             ft* humidityHighClear, ft* temperatureHighClear,
                             ft* humidityLowClear,  ft* temperatureLowClear,
                             ft* humidityLowSet,    ft* temperatureLowSet)
{
    etError  error;  // error code

    // read humidity & temperature alter limits, high set
    error = SHT3X_StartWriteAccess();
    if(error == NO_ERROR) error = SHT3X_WriteCommand(CMD_R_AL_LIM_HS);
    if(error == NO_ERROR) error = SHT3X_StartReadAccess();
    if(error == NO_ERROR) error = SHT3X_ReadAlertLimitData(humidityHighSet,
                                      temperatureHighSet);
    SHT3X_StopAccess();

    if(error == NO_ERROR)
    {
        // read humidity & temperature alter limits, high clear
        error = SHT3X_StartWriteAccess();
        if(error == NO_ERROR) error = SHT3X_WriteCommand(CMD_R_AL_LIM_HC);
        if(error == NO_ERROR) error = SHT3X_StartReadAccess();
        if(error == NO_ERROR) error = SHT3X_ReadAlertLimitData(humidityHighClear,
                                          temperatureHighClear);
        SHT3X_StopAccess();
    }

    if(error == NO_ERROR)
    {
        // read humidity & temperature alter limits, low clear
        error = SHT3X_StartWriteAccess();
        if(error == NO_ERROR) error = SHT3X_WriteCommand(CMD_R_AL_LIM_LC);
        if(error == NO_ERROR) error = SHT3X_StartReadAccess();
        if(error == NO_ERROR) error = SHT3X_ReadAlertLimitData(humidityLowClear,
                                          temperatureLowClear);
        SHT3X_StopAccess();
    }

    if(error == NO_ERROR)
    {
        // read humidity & temperature alter limits, low set
        error = SHT3X_StartWriteAccess();
        if(error == NO_ERROR) error = SHT3X_WriteCommand(CMD_R_AL_LIM_LS);
        if(error == NO_ERROR) error = SHT3X_StartReadAccess();
        if(error == NO_ERROR) error = SHT3X_ReadAlertLimitData(humidityLowSet,
                                          temperatureLowSet);
        SHT3X_StopAccess();
    }

    return error;
}

//-----------------------------------------------------------------------------
bt SHT3X_ReadAlert(void)
{
    // read alert pin
    return (ALERT_READ != 0) ? TRUE : FALSE;
}

//-----------------------------------------------------------------------------
etError SHT3X_SoftReset(void)
{
    etError error; // error code

    error = SHT3X_StartWriteAccess();

    // write reset command
    error |= SHT3X_WriteCommand(CMD_SOFT_RESET);

    SHT3X_StopAccess();

    // if no error, wait 50 ms after reset
    if(error == NO_ERROR) DelayMicroSeconds(50000);

    return error;
}

//-----------------------------------------------------------------------------
void SHT3X_HardReset(void)
{
    // set reset low
    RESET_LOW();

    // wait 100 ms
    DelayMicroSeconds(100000);

    // release reset
    RESET_HIGH();

    // wait 50 ms after reset
    DelayMicroSeconds(50000);
}


//-----------------------------------------------------------------------------
static etError SHT3X_WriteAlertLimitData(ft humidity, ft temperature)
{
    etError  error;           // error code

    i16t rawHumidity;
    i16t rawTemperature;

    if((humidity < 0.0f) || (humidity > 100.0f)
            || (temperature < -45.0f) || (temperature > 130.0f))
    {
        error = PARM_ERROR;
    }
    else
    {
        rawHumidity    = SHT3X_CalcRawHumidity(humidity);
        rawTemperature = SHT3X_CalcRawTemperature(temperature);

        error = SHT3X_Write2BytesAndCrc((rawHumidity & 0xFE00) | ((rawTemperature >> 7) & 0x001FF));
    }

    return error;
}

//-----------------------------------------------------------------------------
static etError SHT3X_ReadAlertLimitData(ft* humidity, ft* temperature)
{
    etError  error;           // error code
    u16t     data;

    error = SHT3X_Read2BytesAndCrc(&data, NACK, 0);

    if(error == NO_ERROR)
    {
        *humidity = SHT3X_CalcHumidity(data & 0xFE00);
        *temperature = SHT3X_CalcTemperature(data << 7);
    }

    return error;
}

//-----------------------------------------------------------------------------
static etError SHT3X_StartWriteAccess(void)
{
    etError error; // error code

    // write a start condition
    I2c_StartCondition();

    // write the sensor I2C address with the write flag
    error = I2c_WriteByte(_i2cAddress << 1);

    return error;
}

//-----------------------------------------------------------------------------
static etError SHT3X_StartReadAccess(void)
{
    etError error; // error code

    // write a start condition
    I2c_StartCondition();

    // write the sensor I2C address with the read flag
    error = I2c_WriteByte(_i2cAddress << 1 | 0x01);

    return error;
}

//-----------------------------------------------------------------------------
static void SHT3X_StopAccess(void)
{
    // write a stop condition
    I2c_StopCondition();
}

//-----------------------------------------------------------------------------
static etError SHT3X_WriteCommand(etCommands command)
{
    etError error; // error code

    // write the upper 8 bits of the command to the sensor
    error  = I2c_WriteByte(command >> 8);

    // write the lower 8 bits of the command to the sensor
    error |= I2c_WriteByte(command & 0xFF);

    return error;
}

//-----------------------------------------------------------------------------
static etError SHT3X_Read2BytesAndCrc(u16t* data, etI2cAck finaleAckNack,
                                      u8t timeout)
{
    etError error;    // error code
    u8t     bytes[2]; // read data array
    u8t     checksum; // checksum byte

    // read two data bytes and one checksum byte
    error = I2c_ReadByte(&bytes[0], ACK, timeout);
    if(error == NO_ERROR) error = I2c_ReadByte(&bytes[1], ACK, 0);
    if(error == NO_ERROR) error = I2c_ReadByte(&checksum, finaleAckNack, 0);

    // verify checksum
    if(error == NO_ERROR) error = SHT3X_CheckCrc(bytes, 2, checksum);

    // combine the two bytes to a 16-bit value
    *data = (bytes[0] << 8) | bytes[1];

    return error;
}

//-----------------------------------------------------------------------------
static etError SHT3X_Write2BytesAndCrc(u16t data)
{
    etError error;    // error code
    u8t     bytes[2]; // read data array
    u8t     checksum; // checksum byte

    bytes[0] = data >> 8;
    bytes[1] = data & 0xFF;
    checksum = SHT3X_CalcCrc(bytes, 2);

    // write two data bytes and one checksum byte
    error = I2c_WriteByte(bytes[0]); // write data MSB
    if(error == NO_ERROR) error = I2c_WriteByte(bytes[1]); // write data LSB
    if(error == NO_ERROR) error = I2c_WriteByte(checksum); // write checksum

    return error;
}

//-----------------------------------------------------------------------------
static u8t SHT3X_CalcCrc(u8t data[], u8t nbrOfBytes)
{
    u8t bit;        // bit mask
    u8t crc = 0xFF; // calculated checksum
    u8t byteCtr;    // byte counter

    // calculates 8-Bit checksum with given polynomial
    for(byteCtr = 0; byteCtr < nbrOfBytes; byteCtr++)
    {
        crc ^= (data[byteCtr]);
        for(bit = 8; bit > 0; --bit)
        {
            if(crc & 0x80) crc = (crc << 1) ^ POLYNOMIAL;
            else           crc = (crc << 1);
        }
    }

    return crc;
}

//-----------------------------------------------------------------------------
static etError SHT3X_CheckCrc(u8t data[], u8t nbrOfBytes, u8t checksum)
{
    u8t crc;     // calculated checksum

    // calculates 8-Bit checksum
    crc = SHT3X_CalcCrc(data, nbrOfBytes);

    // verify checksum
    if(crc != checksum) return CHECKSUM_ERROR;
    else                return NO_ERROR;
}

//-----------------------------------------------------------------------------
static ft SHT3X_CalcTemperature(u16t rawValue)
{
    // calculate temperature [癈]
    // T = -45 + 175 * rawValue / (2^16-1)
    return 175.0f * (ft)rawValue / 65535.0f - 45.0f;
}

//-----------------------------------------------------------------------------
static ft SHT3X_CalcHumidity(u16t rawValue)
{
    // calculate relative humidity [%RH]
    // RH = rawValue / (2^16-1) * 100
    return 100.0f * (ft)rawValue / 65535.0f;
}

//-----------------------------------------------------------------------------
static u16t SHT3X_CalcRawTemperature(ft temperature)
{
    // calculate raw temperature [ticks]
    // rawT = (temperature + 45) / 175 * (2^16-1)
    return (temperature + 45.0f) / 175.0f * 65535.0f;
}

//-----------------------------------------------------------------------------
static u16t SHT3X_CalcRawHumidity(ft humidity)
{
    // calculate raw relative humidity [ticks]
    // rawRH = humidity / 100 * (2^16-1)
    return humidity / 100.0f * 65535.0f;
}

//----------------------------------------------------------------------------------
void call_sht11(float *tem_val, float *hum_val)
{   
    etError error = NO_ERROR;
    ft temperature; 
    ft humidity;
    if(ShtFlag != 0x00)
    {
        call_sht1x(tem_val, hum_val);
    }
    else
    {
        // demonstrate a single shot measurement with polling and 50ms timeout
        error = SHT3X_GetTempAndHumi(&temperature, &humidity, REPEATAB_HIGH, MODE_POLLING, 200);
        if(error != NO_ERROR) // do error handling here
        {
            error = SHT3X_SoftReset();
            // ... if the soft reset fails, do a hard reset
            if(error != NO_ERROR)
            {
                SHT3X_HardReset();
            }
            //printf("error=%d\r\n", error);
        }
        //printf("Temperature=%f℃, Humidity=%f%%\r\n", temperature, humidity);
        *tem_val=temperature;
        *hum_val=humidity;
    } 
    //printf("Temperature=%d℃, Humidity=%d%%\r\n", *tem_val, *hum_val);
}
