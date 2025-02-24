/*
 * iic.c
 *
 *  Created on: 2022年5月1日
 *      Author: ZGJ
 *      @brief: I2C通信应用函数接口
 */

#include "iic.h"

/**
 *  @brief               IIC模块初始化，设置IIC模块的功能引脚以及设置IIC的工作方式
 *  @parameter              无
 *  @return_value           无
 */
void Init_I2CA(void)
{
    I2caRegs.I2CMDR.all = 0x0000;   // 复位IIC

    EALLOW;
    GpioCtrlRegs.GPBPUD.bit.GPIO32 = 0;    // 使能(SDAA)上拉
    GpioCtrlRegs.GPBPUD.bit.GPIO33 = 0;    //  使能 (SCLA)上拉
    GpioCtrlRegs.GPBQSEL1.bit.GPIO32 = 3;  // 同步 (SDAA)
    GpioCtrlRegs.GPBQSEL1.bit.GPIO33 = 3;  // 同步 (SCLA)
    GpioCtrlRegs.GPBMUX1.bit.GPIO32 = 1;   // 配置 GPIO32为 SDAA
    GpioCtrlRegs.GPBMUX1.bit.GPIO33 = 1;   // 配置GPIO33 为SCLA

    // 预分频——时钟模块的频率
    I2caRegs.I2CPSC.all = 5;       // 预分频   IIC模块时钟需设置为7-12MHz，本实验设置为 (150/15 = 10MHz)
    I2caRegs.I2CCLKL = 10;   //时钟低电平时间值
    I2caRegs.I2CCLKH = 5;   //时钟高电平时间值
    I2caRegs.I2CIER.all = 0x00;      // Enable SCD & ARDY interrupts

    I2caRegs.I2CFFTX.all = 0x0000;   // Disable FIFO mode and TXFIFO
    I2caRegs.I2CFFRX.all = 0x0000;   // Disable RXFIFO, clear RXFFINT,
    I2caRegs.I2CMDR.all = 0x0020;   // IIC准备就绪
    EDIS;

}

/**
 *  @brief              IIC模块发送准备OK
 *  @parameter              无
 *  @return_value           无
 */
Uint16  I2C_xrdy()
{
    Uint16  t;
    t = I2caRegs.I2CSTR.bit.XRDY;   // IIC模块发送准备OK
    return t;
}

/**
 *  @brief              IIC模块接收准备OK
 *  @parameter              无
 *  @return_value           无
 */
Uint16  I2C_rrdy()
{
    Uint16  t;
    t = I2caRegs.I2CSTR.bit.RRDY;   //  IIC模块接收准备OK
    return t;
}

/**
 *  @brief                  IIC写数据
 *  @parameter              SlaveAddress：从机地址；  RomAddress：寄存器地址； number：写入数据的字节数；  *Wdata：写入数据的地址
 *  @return_value           状态标志
 */
Uint16 WriteData( Uint16 SlaveAddress, Uint16 RomAddress,Uint16 number, unsigned char *Wdata)
{
   Uint16 i;
   if (I2caRegs.I2CSTR.bit.BB == 1)
   {
      return I2C_BUS_BUSY_ERROR;   //返回总线忙错误状态
   }
   while(!I2C_xrdy());      //等待数据发送就绪，XRDY=1,表明发送寄存器已经准备好接受新的数据
   I2caRegs.I2CSAR = SlaveAddress&0xff;    //设备从地址
   I2caRegs.I2CCNT = number + 1;   //需要发送的字节数
   I2caRegs.I2CDXR = RomAddress&0xff;    //第一个发送字节为发送数据的目标寄存器地址
   DELAY_US(1);//等待数据完全赋值到I2CDXR，再使能IIC，必须要加上！
   I2caRegs.I2CMDR.all = 0x6E20;    //发送起始信号，内部数据计数器减到0时，发送停止信号，主机发送模式，使能IIC模式，
   for (i=0; i<number; i++)
   {
      while(!I2C_xrdy());   //等待数据发送就绪，发送下一个数据
      I2caRegs.I2CDXR = *Wdata&0xFF;
      Wdata++;
      if (I2caRegs.I2CSTR.bit.NACK == 1)    //送到无应答信号返回错误
          return    I2C_BUS_BUSY_ERROR;
   }
  // DELAY_US(1);
   return I2C_SUCCESS;         //发送成功
}


/**
 *  @brief                  IIC写数据
 *  @parameter              SlaveAddress：从机地址；  RomAddress：寄存器地址； number：写入数据的字节数；  *RamAddr：读出数据存放的地址
 *  @return_value           状态标志
 */
Uint16 ReadData( Uint16 SlaveAddress, Uint16 RomAddress,Uint16 number, Uint16  *RamAddr)
{
   Uint16  i,Temp;

   if (I2caRegs.I2CSTR.bit.BB == 1)  //返回总线忙错误状态
   {
       return I2C_BUS_BUSY_ERROR;
   }
   while(!I2C_xrdy());
   I2caRegs.I2CSAR = SlaveAddress&0xff;      //设备从地址
   I2caRegs.I2CCNT = 1;        //发送一个字节为要读取数据的寄存器地址
   I2caRegs.I2CDXR = RomAddress&0xff;
   DELAY_US(1);//等待数据完全赋值到I2CDXR，再使能IIC，必须要加上！
   I2caRegs.I2CMDR.all = 0x6620;   //主机发送模式，自动清除停止位不产生停止信号
   if (I2caRegs.I2CSTR.bit.NACK == 1)    //送到无应答信号返回错误
        return  I2C_BUS_BUSY_ERROR;
   DELAY_US(50);
   while(!I2C_xrdy());                  //调试过程中有时会卡在这一步,表示通讯不正常
   I2caRegs.I2CSAR = SlaveAddress&0xff;
   I2caRegs.I2CCNT = number;
   DELAY_US(1);//等待数据完全赋值到I2CDXR，再使能IIC，必须要加上！
   I2caRegs.I2CMDR.all = 0x6C20;   //主机接受模式，包含有停止信号
   if (I2caRegs.I2CSTR.bit.NACK == 1)
        return  I2C_BUS_BUSY_ERROR;
   for(i=0;i<number;i++)
   {
         while(!I2C_rrdy());
      Temp = I2caRegs.I2CDRR;
      if (I2caRegs.I2CSTR.bit.NACK == 1)
          return    I2C_BUS_BUSY_ERROR;
      *RamAddr = Temp;
      RamAddr++;
   }
   return I2C_SUCCESS;    //接受数据成功
}


/**
 * @brief 通过IIC协议向指定从设备的指定寄存器写入一个位数据
 * 
 * 该函数首先读取指定从设备和寄存器的当前数据，然后根据bitNum指定的位位置，
 * 修改数据中的相应位（设置为1或0），最后将修改后的数据写回同一寄存器。
 * 
 * @param slaveaddress 从设备的IIC地址
 * @param regaddress 寄存器地址，指示要操作的数据位置
 * @param bitNum 位编号，指定要修改的位位置，范围是0-7
 * @param data 要写入的位数据，非零表示设置位为1，0表示设置位为0
 */
void IICwriteBit(Uint16 slaveaddress, Uint16 regaddress, unsigned char bitNum, unsigned char data)
{
    Uint16 a;
    unsigned char b;
    
    // 延迟50微秒，确保IIC总线稳定
    DELAY_US(50);
    
    // 从指定的从设备和寄存器中读取当前数据
    ReadData(slaveaddress, regaddress, 1, &a);
    
    // 将读取的数据转换为unsigned char类型，以便进行位操作
    b = (unsigned char)(a & 0xff);
    
    // 根据data的值，将指定的位设置为1或0
    b = (data != 0) ? (b | (1 << bitNum)) : (b & ~(1 << bitNum));
    
    // 再次延迟50微秒，准备写入操作
    DELAY_US(50);
    
    // 将修改后的数据写回指定的从设备和寄存器
    WriteData(slaveaddress, regaddress, 1, &b);
}





/**
 * IICwriteBits函数用于通过IIC接口向特定从设备的特定寄存器写入指定位的数据。
 * @param slaveaddress 从设备的IIC地址。
 * @param regaddress 寄存器的地址。
 * @param bitStart 起始位的位置（从右侧开始计数）。
 * @param length 要写入的数据的位长度。
 * @param data 要写入的数据。
 * 此函数首先读取当前寄存器的值，然后根据参数修改相应的位，最后写回修改后的数据。
 */
void IICwriteBits(Uint16 slaveaddress,Uint16 regaddress,unsigned char bitStart,unsigned char length,unsigned char data)
{
    unsigned char b,mask;
    Uint16 a;
    // 延迟50微秒，确保总线稳定。
    DELAY_US(50);
    // 从指定的从设备和寄存器地址读取当前数据。
    ReadData(slaveaddress,regaddress,1,&a);
    // 将读取的数据转换为unsigned char类型。
    b=(unsigned char )(a&0xff);
    // 创建一个掩码，用于清除数据中的指定位。
    mask = (0xFF << (bitStart + 1)) | 0xFF >> ((8 - bitStart) + length - 1);
    // 将要写入的数据调整到正确的位位置。
    data <<= (8 - length);
    data >>= (7 - bitStart);
    // 使用掩码清除现有数据中的指定位。
    b &= mask;
    // 将调整后的数据与现有数据合并。
    b |= data;
    // 延迟50微秒，确保总线稳定。
    DELAY_US(50);
    // 将修改后的数据写回指定的从设备和寄存器地址。
    WriteData(slaveaddress,regaddress,1, &b);
}




