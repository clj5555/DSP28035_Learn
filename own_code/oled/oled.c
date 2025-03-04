/*
 * oled.c
 *  Created on: 2022年5月1日
 *      Author: ZGJ
 *      @brief: OLED显示函数接口
 */
#include "oled.h"
#include "iic.h"
#include "font.h"

void delay_1ms(Uint16 t)
{
    while(t--)
    {
        DELAY_US(1000);
    }
}




/**
 *  @brief                   写入一个字节
 *  @parameter                  dat：写入的数据 ； rom：准备写入数据的寄存器地址
 *  @return_value               无
 */
void Send_Byte(unsigned char dat,Uint16 rom)
{
    while(WriteData(devAddr,rom,1,&dat));//等待发送成功
}

void OLED_WR_Byte(unsigned dat,unsigned cmd)
{
    if(cmd == OLED_CMD)
    {
        Send_Byte(dat, 0x00);   //发送命令
    }
    else
    {
        Send_Byte(dat, 0x40);   //发送数据
    }
}



/**
 * 填充图片数据到OLED显示屏
 * 
 * @param fill_Data 要填充的数据，每个像素点的数据将被设置为这个值
 * 
 * 此函数通过逐行写入相同的数据来填充整个OLED屏幕它首先设置页面和列地址，
 * 然后在每个页面中的每一列写入指定的数据这个过程会重复直到整个屏幕被填充
 */
void fill_picture(unsigned char fill_Data)
{
    unsigned char m,n;
    // 遍历OLED的每个页面（共8页）
    for(m=0;m<8;m++)
    {
        // 设置当前操作的页面地址
        OLED_WR_Byte(0xb0+m,0);     //page0-page1
        // 设置列地址的低8位
        OLED_WR_Byte(0x02,0);       //low column start address
        // 设置列地址的高8位
        OLED_WR_Byte(0x10,0);       //high column start address
        // 遍历每一页中的每一列（共128列）
        for(n=0;n<128;n++)
        {
            // 向当前列写入指定的数据
            OLED_WR_Byte(fill_Data,1);
        }
    }
}



void OLED_Set_Pos(unsigned char x, unsigned char y)
{
    OLED_WR_Byte(0xb0+y,OLED_CMD);
    OLED_WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);
    OLED_WR_Byte((x&0x0f),OLED_CMD);
}



//x:0~127
//y:0~63
//size:16/12
/**
 * 在OLED显示屏上显示一个字符
 * 
 * @param x 字符显示的起始列地址
 * @param y 字符显示的起始行地址
 * @param chr 需要显示的字符
 * @param Char_Size 字符的大小，支持16x16点阵和12x6点阵
 */
void OLED_ShowChar(unsigned char x,unsigned char y,unsigned char chr,unsigned char Char_Size)
{
    unsigned char c=0,i=0;
    
    // 将字符转换为对应的字形索引
    c=chr-' ';
    
    // 如果x超过最大列数，则自动换行
    if(x>Max_Column-1){x=0;y=y+2;}
    
    // 根据字符大小选择不同的显示方式
    if(Char_Size ==16)
    {
        // 设置显示位置
        OLED_Set_Pos(x,y);
        // 显示16点阵的上半部分
        for(i=0;i<8;i++)
        OLED_WR_Byte(F8X16[c*16+i],OLED_DATA);
        // 设置显示位置到下半部分
        OLED_Set_Pos(x,y+1);
        // 显示16点阵的下半部分
        for(i=0;i<8;i++)
        OLED_WR_Byte(F8X16[c*16+i+8],OLED_DATA);
    }
    else
    {
        // 设置显示位置
        OLED_Set_Pos(x,y);
        // 显示12点阵的字符
        for(i=0;i<6;i++)
        OLED_WR_Byte(F6x8[c][i],OLED_DATA);
    }
}



Uint32 oled_pow(unsigned char m,unsigned char n)
{
    Uint32 result=1;
    while(n--)result*=m;
    return result;
}

/**
 * 在OLED显示屏上显示数字
 * 
 * @param x 数字显示的起始横坐标
 * @param y 数字显示的起始纵坐标
 * @param num 要显示的数字
 * @param len 数字显示的长度
 * @param size2 字符大小
 * 
 * 此函数的作用是在OLED屏幕上以指定的位置、长度和字符大小来显示一个数字
 * 它通过将数字拆分为单个字符，并根据指定的坐标和大小逐个显示这些字符来实现
 */
void OLED_ShowNum(unsigned char x,unsigned char y,Uint32 num,unsigned char len,unsigned char size2)
{
    unsigned char t,temp;
    unsigned char enshow=0;
    
    // 遍历每个要显示的数字位
    for(t=0;t<len;t++)
    {
        // 计算当前位的数字值
        temp=(num/oled_pow(10,len-t-1))%10;
        
        // 判断是否开始显示数字，避免前导零
        if(enshow==0&&t<(len-1))
        {
            if(temp==0)
            {
                // 如果当前位为零且尚未开始显示，显示空格并跳过当前循环
                OLED_ShowChar(x+(size2/2)*t,y,' ',size2);
                continue;
            }else enshow=1;
        }
        
        // 显示当前位的数字
        OLED_ShowChar(x+(size2/2)*t,y,temp+'0',size2);
    }
}


/**
 * 在OLED显示屏上显示字符串
 * 
 * @param x 字符串显示的起始X坐标
 * @param y 字符串显示的起始Y坐标
 * @param chr 指向字符串的指针
 * @param Char_Size 字符的大小
 * 
 * 此函数通过调用OLED_ShowChar函数逐个显示字符串中的字符
 * 当一行显示满时，自动换行显示
 */
void OLED_ShowString(unsigned char x,unsigned char y,unsigned char *chr,unsigned char Char_Size)
{
    unsigned char j=0;
    // 遍历字符串中的每个字符，直到遇到字符串结束符'\0'
    while (chr[j]!='\0')
    {
        // 在指定位置显示一个字符
        OLED_ShowChar(x,y,chr[j],Char_Size);
        // 增加X坐标，准备显示下一个字符
        x+=8;
        // 如果X坐标超过120，则重置X坐标并增加Y坐标，实现自动换行
        if(x>120){x=0;y+=2;}
        // 增加字符串索引，准备显示下一个字符
        j++;
    }
}





void OLED_Clear(void)
{
    unsigned char i,n;
    for(i=0;i<8;i++)
    {
        OLED_WR_Byte (0xb0+i,OLED_CMD);
        OLED_WR_Byte (0x02,OLED_CMD);
        OLED_WR_Byte (0x10,OLED_CMD);
        for(n=0;n<128;n++)OLED_WR_Byte(0,OLED_DATA);
    }
}

//此初始化函数应在IIC初始化之后
void OLED_Init(void)
{
    OLED_WR_Byte(0xAE,OLED_CMD);//--display off
    OLED_WR_Byte(0x00,OLED_CMD);//---set low column address
    OLED_WR_Byte(0x10,OLED_CMD);//---set high column address
    OLED_WR_Byte(0x40,OLED_CMD);//--set start line address
    OLED_WR_Byte(0xB0,OLED_CMD);//--set page address
    OLED_WR_Byte(0x81,OLED_CMD); // contract control
    OLED_WR_Byte(0xFF,OLED_CMD);//--128
    OLED_WR_Byte(0xA1,OLED_CMD);//set segment remap
    OLED_WR_Byte(0xA6,OLED_CMD);//--normal / reverse
    OLED_WR_Byte(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)
    OLED_WR_Byte(0x3F,OLED_CMD);//--1/32 duty
    OLED_WR_Byte(0xC8,OLED_CMD);//Com scan direction
    OLED_WR_Byte(0xD3,OLED_CMD);//-set display offset
    OLED_WR_Byte(0x00,OLED_CMD);//

    OLED_WR_Byte(0xD5,OLED_CMD);//set osc division
    OLED_WR_Byte(0x80,OLED_CMD);//

    OLED_WR_Byte(0xD8,OLED_CMD);//set area color mode off
    OLED_WR_Byte(0x05,OLED_CMD);//

    OLED_WR_Byte(0xD9,OLED_CMD);//Set Pre-Charge Period
    OLED_WR_Byte(0xF1,OLED_CMD);//

    OLED_WR_Byte(0xDA,OLED_CMD);//set com pin configuartion
    OLED_WR_Byte(0x12,OLED_CMD);//

    OLED_WR_Byte(0xDB,OLED_CMD);//set Vcomh
    OLED_WR_Byte(0x30,OLED_CMD);//

    OLED_WR_Byte(0x8D,OLED_CMD);//set charge pump enable
    OLED_WR_Byte(0x14,OLED_CMD);//

    OLED_WR_Byte(0xAF,OLED_CMD);//--turn on oled panel
}

