#include "sci.h"
#include "DSP2803x_Sci.h"
#include "time.h"
//���ڵĲ���������SCIHBAUD��SCILBAUD��ɵ�һ��16λ�Ĵ���BRR��LSPCLK��������ʱ�ӹ�ͬ������
//���ǵĹ�ϵ��:SCI Buad(������) = LSPCLK / [(BRR +1)*8],����֮��:BRR = (LSPCLK/Buad*8) - 1
//ͬʱҪע��,���BRR = 0,��ôBuad = LSPCLK/16.
//����LSPCLK����ʱ�ӷ�Ƶ�õ���,��ʾ������ʱ��Ϊ60M,����ʱ�ӷ�Ƶ�Ĵ���Ϊ0x02,��Ӧ4��Ƶ,
//��LSPCLKΪ60/4=15MHZ(�����InitSysCtrl()�����˽����ʱ�ӼĴ��������ù���)

//TX_INT_ENΪ1ʹ�ܷ����ж�,Ϊ0��ʹ�ܷ����ж�
#define TX_INT_EN           0
#define SCI_FIFO_EN         1  //ʹ��FIFO����������д0

interrupt void USARTA_RxIntHandler(void);       //�����ж��жϺ���
#if TX_INT_EN       > 0
    interrupt void USARTA_TxIntHandler(void);   //�����ж��жϺ���
#endif

#define USART1_RX_LENGTH_MAX 200

//�жϽ����õı�־�ͻ����������
Uint16    usart1_rx_length = 0;
Uint16   Frame_Length = 0;
Uint16  COM1_RxBuff[USART1_RX_LENGTH_MAX]={0};//Uint16 COM1_RxBuff[15]={0}
Uint16  COM1_TxBuff[USART1_RX_LENGTH_MAX]={0};//int8��f103��û�а�λ�Ĵ���
Uint32  Gu32_modbus_outputIO[USART1_RX_LENGTH_MAX];
Uint16  Gu16_modbus_bits_outputIO[USART1_RX_LENGTH_MAX];


unsigned int    time_usart1,time_usart2;



/**
 * @brief USARTA�����жϴ�����
 */
interrupt void USARTA_RxIntHandler(void)
{
    Uint16 rev=0;
    // �����ջ������Ƿ�׼����
    if((SciaRegs.SCIRXST.bit.RXRDY) == 1)
    {
        // ����ʱ���
        time_usart1 = 0;
        // �����յ������ݴ洢����������
        COM1_RxBuff[usart1_rx_length] = SciaRegs.SCIRXBUF.all;
//        rev= SciaRegs.SCIRXBUF.all ;
//        USART_Transmit(rev);

        // ������յ����ݳ���δ�������ֵ�������ӳ��ȼ���
        if(usart1_rx_length < USART1_RX_LENGTH_MAX)//Frame_Length
        {
            usart1_rx_length ++;
        }
    }
        PieCtrlRegs.PIEACK.bit.ACK9 = 1;
}



#if TX_INT_EN       > 0
//���ʹ���˷����ж�,���жϷ�������Ҫ������Ҫ����û�����
interrupt void USARTA_TxIntHandler(void)
{
    Uint16 data = 0xAA;

    SciaRegs.SCITXBUF = data;               //ģ�ⷢ��һ������
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
 * @brief ��FIFO��ʼ��USARTAģ��
 * @param buad ������
 */
void USARTA_Init1(Uint32 baud)
{
    // ���㲨���ʼĴ�����ֵ����ʽ�е�1875000�ǻ���ϵͳʱ�Ӽ���ó��Ĳο�ֵ
    Uint16 brr_reg = (1875000/baud) - 1;    //15000000/8 = 1875000  //12000000/8=1500000

    // ��ʼ��USARTAʹ�õ�GPIO�˿�
    InitSciaGpio();

    // ���ô������ݸ�ʽ��1��ֹͣλ���޻���ģʽ������żУ�飬8������λ���첽ģʽ��������Э��
    SciaRegs.SCICCR.all =0x0007;

    // ʹ��TX��RX���ڲ�SCICLK������RX ERR��SLEEP��TXWAKE
    SciaRegs.SCICTL1.all =0x0003;
    // ���ÿ��ƼĴ���2�����λ
    SciaRegs.SCICTL2.all =0x0003;

    // ���ݺ궨������Ƿ�ʹ�ܷ����ж�
#if TX_INT_EN       > 0
    SciaRegs.SCICTL2.bit.TXINTENA =1;       // ʹ�ܷ����ж�
#else
    SciaRegs.SCICTL2.bit.TXINTENA =0;       // ��ֹ�����ж�
#endif
    SciaRegs.SCICTL2.bit.RXBKINTENA =1;       // ʹ�ܽ����ж�

    // ���ò����ʼĴ����ĸ�8λ�͵�8λ
    SciaRegs.SCIHBAUD    =(brr_reg>>8)&0x00FF;
    SciaRegs.SCILBAUD    =brr_reg&0x00FF;
    SciaRegs.SCICTL1.all = 0x0023;  // Relinquish SCI from Reset

    // ����PIE�ж�
    EALLOW;
    PieVectTable.SCIRXINTA= &USARTA_RxIntHandler;
#if TX_INT_EN       > 0
    PieVectTable.SCITXINTA = &USARTA_TxIntHandler;
#endif
    EDIS;

    // ʹ��PIE�ж�
    PieCtrlRegs.PIECTRL.bit.ENPIE = 1;      // Enable the PIE block
    PieCtrlRegs.PIEIER9.bit.INTx1=1;        // PIE Group 9, INT1
#if TX_INT_EN       > 0
    PieCtrlRegs.PIEIER9.bit.INTx2=1;        // PIE Group 9, INT2
#endif
    IER |= M_INT9;                          // Enable CPU INT
}



/**
 * @brief ��FIFO��ʼ��USARTAģ��
 * @param buad ������
 */
void USARTA_Init2(Uint32 baud)
{
    // ���㲨���ʼĴ�����ֵ����ʽ�е�1875000�ǻ���ϵͳʱ�Ӽ���ó��Ĳο�ֵ
       Uint16 brr_reg = (1875000/baud) - 1;    //15000000/8 = 1875000  //12000000/8=1500000
       InitSciaGpio();//init sci gpio
       SciaRegs.SCICTL1.bit.SWRESET = 0;

       SciaRegs.SCICCR.all =0x0007;        // 1 stop bit,  No loopback
                                           // No parity,8 char bits,��У��λ
                                           // 8������λ
       // ���ò����ʼĴ����ĸ�8λ�͵�8λ
       SciaRegs.SCIHBAUD    =(brr_reg>>8)&0x00FF;
       SciaRegs.SCILBAUD    =brr_reg&0x00FF;

       SciaRegs.SCICTL1.bit.SWRESET = 1;     // Relinquish SCI from Reset
       SciaRegs.SCIFFTX.bit.SCIRST=1;

       SciaRegs.SCIFFRX.bit.RXFFIL  = 1;  //����FIFO���

       SciaRegs.SCICTL1.all |=0x0003;      // enable TX, RX, internal SCICLK,
                                           // Disable RX ERR, SLEEP, TXWAKE

       SciaRegs.SCIFFTX.bit.SCIFFENA = 1;      //ʹ��FIFO�ж�
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
 * ����һ���ֽ�
 * @param data Ҫ��������ݣ�����ΪUint16����ʾ16λ�޷�������
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
 * USART_GetChar�������ڴ�һ�������ж�ȡһ֡���ݣ�������Ƿ���յ�һ֡���������ݡ�
 * �˺������ȼ�鴫���֡�����Ƿ���֮ǰ�Ĳ�ͬ�������ͬ�����֡���ȡ�
 * Ȼ���鵱ǰ���յ������ݳ����Ƿ����֡���ȣ������ȣ���ʾ���յ�һ֡���������ݣ�
 * ����Щ���ݸ��Ƶ��û��ṩ�������У������ý��յ������ݳ��ȼ���������󷵻�1��ʾ�ɹ���
 * ������յ������ݳ��Ȳ�����֡���ȣ��򷵻�0����ʾû�н��յ�������һ֡���ݡ�
 */
Uint16 USART_GetChar(Uint16* p_array, Uint16 frame_len)
{
    Uint16 i = 0;
    // ��鲢����֡����
    if(Frame_Length != frame_len)
    {
        Frame_Length = frame_len;
    }
    // ����Ƿ���յ�һ֡����������
    if(usart1_rx_length == Frame_Length)
    {
        // �����յ������ݸ��Ƶ��û��ṩ��������
        for(i=0; i< Frame_Length; i++)
        {
            p_array[i] = COM1_RxBuff[i];
        }
        // ���ý��յ������ݳ��ȼ�����
        usart1_rx_length = 0;
        // ����1��ʾ�ɹ����յ�һ֡����������
        return 1;
    }
    // ����0��ʾû�н��յ�������һ֡����
    return 0;
}


/**
 * ��USART���յ������лָ�������Ϣ��
 * ������ͨ��USART����һ֡���ݣ�Ȼ�������ݵ�����������Ч�ԡ����������Ч���������������Ϣ�������ֽڡ�
 * @param p_rate ָ��һ��int32������ָ�룬���ڴ洢�����������ֵ��
 */
Uint16 USART_GetRate(int32* p_rate)
{
    // ������ʱ�洢���յ���USART���ݵ����飬����9���޷��Ŷ�����Ԫ�ء�
    Uint16 temp_array[9] = {0};
    // ����ѭ���ļ�����������
    Uint16 i =0;
    // ������ʱ��������ֵ�ı�����
    int32 temp_rate = 0;
    // ���ڴ洢�����ֽڵı�����
    Uint16 ch_cmd = 0;
    
    // ����Ƿ�ɹ����յ������ݣ�������յ����������У����ָ�����������˲�ѯ�ٶȡ�
    if(USART_GetChar(temp_array, 9))
    {
        // �����յ�������֡����ʼ�ͽ�����־�Ƿ���ȷ����ȷ�����ݵ������ԡ�
        if((temp_array[0] == 0xaa)&&(temp_array[8] == 0x55))
        {
            // �ӽ��յ�������֡�лָ�����ֵ�����ݷֲ��ڵ�4����7���ֽ��С�
            for(i = 3; i < 7; i ++)
            {
                // ͨ��λ��������ɢ���ֽ���ϳ�����������ֵ��
                temp_rate |= temp_array[i]<<((i-3)<<3);//8*(i-3)
            }
            // ��ȡ�����ֽڣ�������֡�еĵ�3���ֽڡ�
            ch_cmd = temp_array[2];
            // ���ָ�������ֵ�洢��ͨ���������ݵĵ�ַ�С�
            *p_rate = temp_rate;
        }
    }
    // ���������ֽڣ���Ϊ����ֵ�ĸ�����Ϣ��
    return ch_cmd;
}


void USART1_SendString(char *msg)
{
    int i=0;
    // ѭ�������ַ����е�ÿ���ַ���ֱ�������ַ���������'\0'
    while(msg[i] != '\0')
    {
        // ����USART_Transmit�������͵�ǰ�ַ�
        USART_Transmit(msg[i]);
        // ��������������ָ����һ���ַ�
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

//������
//void USART_Debug(Uint16 debug_data)
//{
//    USART_Transmit(0xaa);
//    USART_Transmit(debug_data);
//    USART_Transmit(0x55);
//}


/**
 * ͨ��USART����һ������
 * 
 * �ú�����һ��32λ�����ݷֽ�ɶ���ֽڣ�������һ���ĸ�ʽͨ��USART���ͳ�ȥ
 * �ڷ�������֮ǰ�������һ��У��ͣ�ch_XOR������ȷ�����ݵ�������
 * 
 * @param Data Ҫ���͵�32λ����
 * @param Is_H ��־λ��ָʾ�Ƿ��ȷ��͸��ֽ�
 * @param Line_ID ��·ID�����ڱ�ʶ��ͬ����·
 */
void USART_SendLine(int32 Data ,Uint16 Is_H,Uint16 Line_ID)
{
    Uint16 Temp_A_L;
    Uint16 Temp_A_H;
    Uint16 Temp_B_L;
    int16 Temp_B_H;

    Uint16 ch_XOR = 0;

    // ��32λ���ݷֽ��4���ֽ�
    Temp_A_L = Data;
    Temp_A_H = Data >> 8;
    Temp_B_L = Data >> 16;
    Temp_B_H = Data >> 24;

    // ����У���
    ch_XOR ^= 0x09;
    ch_XOR ^= Line_ID;

    ch_XOR ^= Temp_B_H;
    ch_XOR ^= Temp_B_L;
    ch_XOR ^= Temp_A_H;
    ch_XOR ^= Temp_A_L;

    // ���͹̶���ʽ����ʼ�ֽ�
    USART_Transmit(0xaa);
    USART_Transmit(0x09);
    USART_Transmit(Line_ID);

    // ����Is_H��־���������ֽڵ�˳��
    if(Is_H)                            // �Ƿ����ȷ����ֽڣ�
    {
        USART_Transmit(Temp_B_H);       // ��
        USART_Transmit(Temp_B_L);
        USART_Transmit(Temp_A_H);
        USART_Transmit(Temp_A_L);
    }
    else
    {
        USART_Transmit(Temp_A_L);       // ����
        USART_Transmit(Temp_A_H);
        USART_Transmit(Temp_B_L);
        USART_Transmit(Temp_B_H);
    }

    // ����У��ͼ��̶���ʽ�Ľ����ֽ�
    USART_Transmit(ch_XOR);
    USART_Transmit(0x55);
}


//////////////����///////////////////
#define PRINT_HEX_EN        1

#if PRINT_HEX_EN        > 0
/////////////////////////////////////////////////////////////////////
//�����ô��ڴ�ӡ����
# define UartPutChar    USART_Transmit
# define Printc         USART_Transmit

/********************************************************************
�������ܣ�����һ���ַ�����
��ڲ�����pd��Ҫ���͵��ַ���ָ�롣
��    �أ��ޡ�
��    ע���ޡ�
********************************************************************/
void Prints(const void * pstr)
{
    Uint16* pd = (Uint16*)pstr;
    while((*pd)!='\0') //�����ַ�����ֱ������0�Ž���
    {
        UartPutChar(*pd); //����һ���ַ�
        pd++;  //�ƶ�����һ���ַ�
    }
}

////////////////////////End of function//////////////////////////////
/********************************************************************
�������ܣ�������ת��ʮ�����ַ������͡�
��ڲ�����x������ʾ��������
��    �أ��ޡ�
��    ע���ޡ�
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
�������ܣ�����������ʮ�����Ʒ��͡�
��ڲ����������͵�������
��    �أ��ޡ�
��    ע���ޡ�
********************************************************************/
void PrintShortIntHex(Uint16 x)
{
    Uint16 i;
    Uint16 display_buffer[7];
    display_buffer[6]=0;
    display_buffer[0]='0';
    display_buffer[1]='x';
    for(i=5;i>=2;i--) //������ת��Ϊ4���ֽڵ�HEXֵ
    {
        display_buffer[i]=HexTable[(x&0xf)];
        x>>=4;
    }
    Prints(display_buffer);
}




/********************************************************************
*�������ܣ���HEX��ʽ����һ��byte�����ݡ�
*��ڲ����������͵�����
 * ��ӡһ��16λ�޷���������ʮ�����Ʊ�ʾ
 * 
 * �˺�����һ��16λ�޷�������ת��Ϊʮ�����Ƹ�ʽ����ͨ������Printc�������ַ���ӡ����
 * �����ȴ�ӡ��"0x"ǰ׺��Ȼ�������������Ϊ��4λ�͵�4λ���ֱ�ת��Ϊ��Ӧ��ʮ�������ַ�����ӡ
 * ����ӡһ���ո��ַ������ڸ�ʽ���
 * 
 * @param x 16λ�޷�������������ת��Ϊʮ�����Ʋ���ӡ
 */
void PrintHex(Uint16 x)
{
    // ��ӡʮ�����Ʊ�ʾ��ǰ׺"0x"
    Printc('0');
    Printc('x');
    
    // ������������ĸ�4λת��Ϊ��Ӧ��ʮ�������ַ�����ӡ
    Printc(HexTable[x>>4]);
    
    // ������������ĵ�4λת��Ϊ��Ӧ��ʮ�������ַ�����ӡ
    Printc(HexTable[x&0xf]);
    
    // ��ӡһ���ո��ַ������ڸ�ʽ���
    Printc(' ');
}
#endif




