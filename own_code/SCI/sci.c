#include "sci.h"
#include "DSP2803x_Sci.h"
#include "time.h"
//串口的波特率是由SCIHBAUD、SCILBAUD组成的一个16位寄存器BRR和LSPCLK低速外设时钟共同决定的
//它们的关系是:SCI Buad(波特率) = LSPCLK / [(BRR +1)*8],反推之即:BRR = (LSPCLK/Buad*8) - 1
//同时要注意,如果BRR = 0,那么Buad = LSPCLK/16.
//其中LSPCLK是主时钟分频得到的,本示例中主时钟为60M,低速时钟分频寄存器为0x02,对应4分频,
//即LSPCLK为60/4=15MHZ(请查阅InitSysCtrl()函数了解相关时钟寄存器的配置过程)

//TX_INT_EN为1使能发送中断,为0不使能发送中断
#define TX_INT_EN           0
#define SCI_FIFO_EN         1  //使能FIFO，若不是用写0

interrupt void USARTA_RxIntHandler(void);       //接收中断中断函数
#if TX_INT_EN       > 0
    interrupt void USARTA_TxIntHandler(void);   //发送中断中断函数
#endif

#define USART1_RX_LENGTH_MAX 200

//中断接收用的标志和缓存变量定义
Uint16    usart1_rx_length = 0;
Uint16   Frame_Length = 0;
Uint16  COM1_RxBuff[USART1_RX_LENGTH_MAX]={0};//Uint16 COM1_RxBuff[15]={0}
Uint16  COM1_TxBuff[USART1_RX_LENGTH_MAX]={0};//int8（f103）没有八位寄存器
Uint32  Gu32_modbus_outputIO[USART1_RX_LENGTH_MAX];
Uint16  Gu16_modbus_bits_outputIO[USART1_RX_LENGTH_MAX];


unsigned int    time_usart1,time_usart2;



/**
 * @brief USARTA接收中断处理函数
 */
interrupt void USARTA_RxIntHandler(void)
{
    Uint16 rev=0;
    // 检查接收缓冲器是否准备好
    if((SciaRegs.SCIRXST.bit.RXRDY) == 1)
    {
        // 重置时间戳
        time_usart1 = 0;
        // 将接收到的数据存储到缓冲区中
        COM1_RxBuff[usart1_rx_length] = SciaRegs.SCIRXBUF.all;
//        rev= SciaRegs.SCIRXBUF.all ;
//        USART_Transmit(rev);

        // 如果接收的数据长度未超过最大值，则增加长度计数
        if(usart1_rx_length < USART1_RX_LENGTH_MAX)//Frame_Length
        {
            usart1_rx_length ++;
        }
    }
        PieCtrlRegs.PIEACK.bit.ACK9 = 1;
}



#if TX_INT_EN       > 0
//如果使能了发送中断,该中断服务函数需要根据需要添加用户代码
interrupt void USARTA_TxIntHandler(void)
{
    Uint16 data = 0xAA;

    SciaRegs.SCITXBUF = data;               //模拟发送一个数据
    PieCtrlRegs.PIEACK.bit.ACK9 = 1;        // Issue PIE ack
}
#endif

void USARTA_Init(Uint32 baud)
{
    #if (SCI_FIFO_EN >0)
    {
        USARTA_Init2(baud);
    }
    #else
    {
        USARTA_Init1(baud);
    }
    #endif
}


/**
 * @brief 无FIFO初始化USARTA模块
 * @param buad 波特率
 */
void USARTA_Init1(Uint32 baud)
{
    // 计算波特率寄存器的值，公式中的1875000是基于系统时钟计算得出的参考值
    Uint16 brr_reg = (1875000/baud) - 1;    //15000000/8 = 1875000  //12000000/8=1500000

    // 初始化USARTA使用的GPIO端口
    InitSciaGpio();

    // 配置串口数据格式：1个停止位，无环回模式，无奇偶校验，8个数据位，异步模式，空闲线协议
    SciaRegs.SCICCR.all =0x0007;

    // 使能TX、RX和内部SCICLK，禁用RX ERR、SLEEP和TXWAKE
    SciaRegs.SCICTL1.all =0x0003;
    // 配置控制寄存器2的相关位
    SciaRegs.SCICTL2.all =0x0003;

    // 根据宏定义决定是否使能发送中断
#if TX_INT_EN       > 0
    SciaRegs.SCICTL2.bit.TXINTENA =1;       // 使能发送中断
#else
    SciaRegs.SCICTL2.bit.TXINTENA =0;       // 禁止发送中断
#endif
    SciaRegs.SCICTL2.bit.RXBKINTENA =1;       // 使能接收中断

    // 设置波特率寄存器的高8位和低8位
    SciaRegs.SCIHBAUD    =(brr_reg>>8)&0x00FF;
    SciaRegs.SCILBAUD    =brr_reg&0x00FF;
    SciaRegs.SCICTL1.all = 0x0023;  // Relinquish SCI from Reset

    // 配置PIE中断
    EALLOW;
    PieVectTable.SCIRXINTA= &USARTA_RxIntHandler;
#if TX_INT_EN       > 0
    PieVectTable.SCITXINTA = &USARTA_TxIntHandler;
#endif
    EDIS;

    // 使能PIE中断
    PieCtrlRegs.PIECTRL.bit.ENPIE = 1;      // Enable the PIE block
    PieCtrlRegs.PIEIER9.bit.INTx1=1;        // PIE Group 9, INT1
#if TX_INT_EN       > 0
    PieCtrlRegs.PIEIER9.bit.INTx2=1;        // PIE Group 9, INT2
#endif
    IER |= M_INT9;                          // Enable CPU INT
}



/**
 * @brief 有FIFO初始化USARTA模块
 * @param buad 波特率
 */
void USARTA_Init2(Uint32 baud)
{
    // 计算波特率寄存器的值，公式中的1875000是基于系统时钟计算得出的参考值
       Uint16 brr_reg = (1875000/baud) - 1;    //15000000/8 = 1875000  //12000000/8=1500000
       InitSciaGpio();//init sci gpio
       SciaRegs.SCICTL1.bit.SWRESET = 0;

       SciaRegs.SCICCR.all =0x0007;        // 1 stop bit,  No loopback
                                           // No parity,8 char bits,无校验位
                                           // 8个数据位
       // 设置波特率寄存器的高8位和低8位
       SciaRegs.SCIHBAUD    =(brr_reg>>8)&0x00FF;
       SciaRegs.SCILBAUD    =brr_reg&0x00FF;

       SciaRegs.SCICTL1.bit.SWRESET = 1;     // Relinquish SCI from Reset
       SciaRegs.SCIFFTX.bit.SCIRST=1;

       SciaRegs.SCIFFRX.bit.RXFFIL  = 1;  //设置FIFO深度

       SciaRegs.SCICTL1.all |=0x0003;      // enable TX, RX, internal SCICLK,
                                           // Disable RX ERR, SLEEP, TXWAKE

       SciaRegs.SCIFFTX.bit.SCIFFENA = 1;      //使能FIFO中断
       SciaRegs.SCIFFRX.bit.RXFFIENA=1;

       //interrupt isr
       EALLOW;
       PieVectTable.SCIRXINTA= &USARTA_RxIntHandler;
       //PieVectTable.SCITXINTA = &&UARTA_Txisr;
       EDIS;
       //sci interrupt in pie group 9.1(rx_ini) or 9.2(tx_int)
       PieCtrlRegs.PIEIER9.bit.INTx1=1;        // PIE Group 9, RX_INT1
       //PieCtrlRegs.PIEIER9.bit.INTx2=1;      // PIE Group 9, TX_INT2 DISEN
       IER |= M_INT9;                          // Enable CPU INT
       SciaRegs.SCIFFCT.all=0x00;
       SciaRegs.SCIFFTX.bit.TXFIFOXRESET=1;
       SciaRegs.SCIFFRX.bit.RXFIFORESET=1;
}



/**
 * 发送一个字节
 * @param data 要传输的数据，类型为Uint16，表示16位无符号整数
 */
void USART_Transmit(Uint16 data)
{
    while (SciaRegs.SCICTL2.bit.TXRDY == 0)
    {
        ;
    }
    SciaRegs.SCITXBUF = data;
}


/**
 * USART_GetChar函数用于从一个数组中读取一帧数据，并检查是否接收到一帧完整的数据。
 * 此函数首先检查传入的帧长度是否与之前的不同，如果不同则更新帧长度。
 * 然后检查当前接收到的数据长度是否等于帧长度，如果相等，表示接收到一帧完整的数据，
 * 将这些数据复制到用户提供的数组中，并重置接收到的数据长度计数器，最后返回1表示成功。
 * 如果接收到的数据长度不等于帧长度，则返回0，表示没有接收到完整的一帧数据。
 */
Uint16 USART_GetChar(Uint16* p_array, Uint16 frame_len)
{
    Uint16 i = 0;
    // 检查并更新帧长度
    if(Frame_Length != frame_len)
    {
        Frame_Length = frame_len;
    }
    // 检查是否接收到一帧完整的数据
    if(usart1_rx_length == Frame_Length)
    {
        // 将接收到的数据复制到用户提供的数组中
        for(i=0; i< Frame_Length; i++)
        {
            p_array[i] = COM1_RxBuff[i];
        }
        // 重置接收到的数据长度计数器
        usart1_rx_length = 0;
        // 返回1表示成功接收到一帧完整的数据
        return 1;
    }
    // 返回0表示没有接收到完整的一帧数据
    return 0;
}


/**
 * 从USART接收的数据中恢复速率信息。
 * 本函数通过USART接收一帧数据，然后检查数据的完整性与有效性。如果数据有效，则解析出速率信息和命令字节。
 * @param p_rate 指向一个int32变量的指针，用于存储解析后的速率值。
 */
Uint16 USART_GetRate(int32* p_rate)
{
    // 用于临时存储接收到的USART数据的数组，包含9个无符号短整型元素。
    Uint16 temp_array[9] = {0};
    // 用于循环的计数器变量。
    Uint16 i =0;
    // 用于临时计算速率值的变量。
    int32 temp_rate = 0;
    // 用于存储命令字节的变量。
    Uint16 ch_cmd = 0;
    
    // 检查是否成功接收到新数据，如果接收到则进行数据校验与恢复，这样提高了查询速度。
    if(USART_GetChar(temp_array, 9))
    {
        // 检查接收到的数据帧的起始和结束标志是否正确，以确保数据的完整性。
        if((temp_array[0] == 0xaa)&&(temp_array[8] == 0x55))
        {
            // 从接收到的数据帧中恢复速率值，数据分布在第4到第7个字节中。
            for(i = 3; i < 7; i ++)
            {
                // 通过位操作将分散的字节组合成完整的速率值。
                temp_rate |= temp_array[i]<<((i-3)<<3);//8*(i-3)
            }
            // 获取命令字节，即数据帧中的第3个字节。
            ch_cmd = temp_array[2];
            // 将恢复的速率值存储到通过参数传递的地址中。
            *p_rate = temp_rate;
        }
    }
    // 返回命令字节，作为速率值的附加信息。
    return ch_cmd;
}


void USART1_SendString(char *msg)
{
    int i=0;
    // 循环发送字符串中的每个字符，直到遇到字符串结束符'\0'
    while(msg[i] != '\0')
    {
        // 调用USART_Transmit函数发送当前字符
        USART_Transmit(msg[i]);
        // 索引变量自增，指向下一个字符
        i++;
    }
}


void Usart1_Send_String(Uint16 *buff,Uint16 len)
{
    Uint16 t;
    for(t=0;t<len;t++)
    {
        while((SciaRegs.SCIRXST.bit.RXRDY) == 1);
        USART_Transmit(buff[t]);
    }
}


void USART_SendString(char *p_STR)
{
    while(*p_STR)
    {
        USART_Transmit(*p_STR ++);
    }
}

//测试用
//void USART_Debug(Uint16 debug_data)
//{
//    USART_Transmit(0xaa);
//    USART_Transmit(debug_data);
//    USART_Transmit(0x55);
//}


/**
 * 通过USART发送一行数据
 * 
 * 该函数将一个32位的数据分解成多个字节，并按照一定的格式通过USART发送出去
 * 在发送数据之前，会计算一个校验和（ch_XOR），以确保数据的完整性
 * 
 * @param Data 要发送的32位数据
 * @param Is_H 标志位，指示是否先发送高字节
 * @param Line_ID 线路ID，用于标识不同的线路
 */
void USART_SendLine(int32 Data ,Uint16 Is_H,Uint16 Line_ID)
{
    Uint16 Temp_A_L;
    Uint16 Temp_A_H;
    Uint16 Temp_B_L;
    int16 Temp_B_H;

    Uint16 ch_XOR = 0;

    // 将32位数据分解成4个字节
    Temp_A_L = Data;
    Temp_A_H = Data >> 8;
    Temp_B_L = Data >> 16;
    Temp_B_H = Data >> 24;

    // 计算校验和
    ch_XOR ^= 0x09;
    ch_XOR ^= Line_ID;

    ch_XOR ^= Temp_B_H;
    ch_XOR ^= Temp_B_L;
    ch_XOR ^= Temp_A_H;
    ch_XOR ^= Temp_A_L;

    // 发送固定格式的起始字节
    USART_Transmit(0xaa);
    USART_Transmit(0x09);
    USART_Transmit(Line_ID);

    // 根据Is_H标志决定发送字节的顺序
    if(Is_H)                            // 是否是先发高字节？
    {
        USART_Transmit(Temp_B_H);       // 是
        USART_Transmit(Temp_B_L);
        USART_Transmit(Temp_A_H);
        USART_Transmit(Temp_A_L);
    }
    else
    {
        USART_Transmit(Temp_A_L);       // 不是
        USART_Transmit(Temp_A_H);
        USART_Transmit(Temp_B_L);
        USART_Transmit(Temp_B_H);
    }

    // 发送校验和及固定格式的结束字节
    USART_Transmit(ch_XOR);
    USART_Transmit(0x55);
}


//////////////发送///////////////////
#define PRINT_HEX_EN        1

#if PRINT_HEX_EN        > 0
/////////////////////////////////////////////////////////////////////
//调试用串口打印函数
# define UartPutChar    USART_Transmit
# define Printc         USART_Transmit

/********************************************************************
函数功能：发送一个字符串。
入口参数：pd：要发送的字符串指针。
返    回：无。
备    注：无。
********************************************************************/
void Prints(const void * pstr)
{
    Uint16* pd = (Uint16*)pstr;
    while((*pd)!='\0') //发送字符串，直到遇到0才结束
    {
        UartPutChar(*pd); //发送一个字符
        pd++;  //移动到下一个字符
    }
}

////////////////////////End of function//////////////////////////////
/********************************************************************
函数功能：将整数转按十进制字符串发送。
入口参数：x：待显示的整数。
返    回：无。
备    注：无。
********************************************************************/
void PrintLongInt(Uint32 x)
{
    int16 i;
    Uint16 display_buffer[10];

    for(i=9;i>=0;i--)
    {
        display_buffer[i]='0'+x%10;
        x/=10;
    }
    for(i=0;i<9;i++)
    {
        if(display_buffer[i]!='0')
        {
            break;
        }
    }
    for(;i<10;i++)
    {
        UartPutChar(display_buffer[i]);
    }
}

////////////////////////End of function//////////////////////////////



const Uint16 HexTable[]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
/********************************************************************
函数功能：将短整数按十六进制发送。
入口参数：待发送的整数。
返    回：无。
备    注：无。
********************************************************************/
void PrintShortIntHex(Uint16 x)
{
    Uint16 i;
    Uint16 display_buffer[7];
    display_buffer[6]=0;
    display_buffer[0]='0';
    display_buffer[1]='x';
    for(i=5;i>=2;i--) //将整数转换为4个字节的HEX值
    {
        display_buffer[i]=HexTable[(x&0xf)];
        x>>=4;
    }
    Prints(display_buffer);
}




/********************************************************************
*函数功能：以HEX格式发送一个byte的数据。
*入口参数：待发送的数据
 * 打印一个16位无符号整数的十六进制表示
 * 
 * 此函数将一个16位无符号整数转换为十六进制格式，并通过调用Printc函数逐字符打印出来
 * 它首先打印出"0x"前缀，然后将输入的整数分为高4位和低4位，分别转换为对应的十六进制字符并打印
 * 最后打印一个空格字符，用于格式间隔
 * 
 * @param x 16位无符号整数，将被转换为十六进制并打印
 */
void PrintHex(Uint16 x)
{
    // 打印十六进制表示的前缀"0x"
    Printc('0');
    Printc('x');
    
    // 将输入的整数的高4位转换为对应的十六进制字符并打印
    Printc(HexTable[x>>4]);
    
    // 将输入的整数的低4位转换为对应的十六进制字符并打印
    Printc(HexTable[x&0xf]);
    
    // 打印一个空格字符，用于格式间隔
    Printc(' ');
}
#endif




