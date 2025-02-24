// TI File $Revision: /main/1 $
// Checkin $Date: December 5, 2008   18:05:06 $
//###########################################################################
//
// FILE:	DSP2803x_Sci.h
//
// TITLE:	DSP2803x Device SCI Register Definitions.
//
//###########################################################################
// $TI Release: 2803x C/C++ Header Files V1.21 $
// $Release Date: December 1, 2009 $
//###########################################################################

#ifndef DSP2803x_SCI_H
#define DSP2803x_SCI_H

#ifdef __cplusplus
extern "C" {
#endif

//---------------------------------------------------------------------------
// SCI Individual Register Bit Definitions

//----------------------------------------------------------
// SCICCR communication control register bit definitions:
//

struct  SCICCR_BITS {        // bit    description
   Uint16 SCICHAR:3;         // 2:0    Character length control
   Uint16 ADDRIDLE_MODE:1;   // 3      ADDR/IDLE Mode control
   Uint16 LOOPBKENA:1;       // 4      Loop Back enable
   Uint16 PARITYENA:1;       // 5      Parity enable
   Uint16 PARITY:1;          // 6      Even or Odd Parity
   Uint16 STOPBITS:1;        // 7      Number of Stop Bits
   Uint16 rsvd1:8;           // 15:8   reserved
};

union SCICCR_REG {
   Uint16              all;
   struct SCICCR_BITS  bit;
};

//-------------------------------------------
// SCICTL1 control register 1 bit definitions:
//

struct  SCICTL1_BITS {       // bit    description
   Uint16 RXENA:1;           // 0      SCI receiver enable
   Uint16 TXENA:1;           // 1      SCI transmitter enable
   Uint16 SLEEP:1;           // 2      SCI sleep
   Uint16 TXWAKE:1;          // 3      Transmitter wakeup method
   Uint16 rsvd:1;            // 4      reserved
   Uint16 SWRESET:1;         // 5      Software reset
   Uint16 RXERRINTENA:1;     // 6      Recieve interrupt enable
   Uint16 rsvd1:9;           // 15:7   reserved

};

union SCICTL1_REG {
   Uint16               all;
   struct SCICTL1_BITS  bit;
};

//---------------------------------------------
// SCICTL2 control register 2 bit definitions:
//

struct  SCICTL2_BITS {       // bit    description
   Uint16 TXINTENA:1;        // 0      Transmit interrupt enable
   Uint16 RXBKINTENA:1;      // 1      Receiver-buffer break enable
   Uint16 rsvd:4;            // 5:2    reserved
   Uint16 TXEMPTY:1;         // 6      Transmitter empty flag
   Uint16 TXRDY:1;           // 7      Transmitter ready flag
   Uint16 rsvd1:8;           // 15:8   reserved

};

union SCICTL2_REG {
   Uint16               all;
   struct SCICTL2_BITS  bit;
};

//---------------------------------------------------
// SCIRXST Receiver status register bit definitions:
//

struct  SCIRXST_BITS {       // bit    description
   Uint16 rsvd:1;            // 0      reserved
   Uint16 RXWAKE:1;          // 1      Receiver wakeup detect flag
   Uint16 PE:1;              // 2      Parity error flag
   Uint16 OE:1;              // 3      Overrun error flag
   Uint16 FE:1;              // 4      Framing error flag
   Uint16 BRKDT:1;           // 5      Break-detect flag
   Uint16 RXRDY:1;           // 6      Receiver ready flag
   Uint16 RXERROR:1;         // 7      Receiver error flag

};

union SCIRXST_REG {
   Uint16               all;
   struct SCIRXST_BITS  bit;
};

//----------------------------------------------------
// SCIRXBUF Receiver Data Buffer with FIFO bit definitions:
//

struct  SCIRXBUF_BITS {      // bits   description
   Uint16 RXDT:8;            // 7:0    Receive word
   Uint16 rsvd:6;            // 13:8   reserved
   Uint16 SCIFFPE:1;         // 14     SCI PE error in FIFO mode
   Uint16 SCIFFFE:1;         // 15     SCI FE error in FIFO mode
};

union SCIRXBUF_REG {
   Uint16                all;
   struct SCIRXBUF_BITS  bit;
};

//--------------------------------------------------
// SCIPRI Priority control register bit definitions:
//
//

struct  SCIPRI_BITS {        // bit    description
   Uint16 rsvd:3;            // 2:0    reserved
   Uint16 FREE:1;            // 3      Free emulation suspend mode
   Uint16 SOFT:1;            // 4      Soft emulation suspend mode
   Uint16 rsvd1:3;           // 7:5    reserved
};

union SCIPRI_REG {
   Uint16              all;
   struct SCIPRI_BITS  bit;
};

//-------------------------------------------------
// SCI FIFO Transmit register bit definitions:
//
//

struct  SCIFFTX_BITS {       // bit    description
   Uint16 TXFFIL:5;          // 4:0    Interrupt level
   Uint16 TXFFIENA:1;        // 5      Interrupt enable
   Uint16 TXFFINTCLR:1;      // 6      Clear INT flag
   Uint16 TXFFINT:1;         // 7      INT flag
   Uint16 TXFFST:5;          // 12:8   FIFO status
   Uint16 TXFIFOXRESET:1;    // 13     FIFO reset
   Uint16 SCIFFENA:1;        // 14     Enhancement enable
   Uint16 SCIRST:1;          // 15     SCI reset rx/tx channels

};

union SCIFFTX_REG {
   Uint16               all;
   struct SCIFFTX_BITS  bit;
};

//------------------------------------------------
// SCI FIFO recieve register bit definitions:
//
//

struct  SCIFFRX_BITS {       // bits   description
   Uint16 RXFFIL:5;          // 4:0    Interrupt level
   Uint16 RXFFIENA:1;        // 5      Interrupt enable
   Uint16 RXFFINTCLR:1;      // 6      Clear INT flag
   Uint16 RXFFINT:1;         // 7      INT flag
   Uint16 RXFFST:5;          // 12:8   FIFO status
   Uint16 RXFIFORESET:1;     // 13     FIFO reset
   Uint16 RXFFOVRCLR:1;      // 14     Clear overflow
   Uint16 RXFFOVF:1;         // 15     FIFO overflow

};

union SCIFFRX_REG {
   Uint16               all;
   struct SCIFFRX_BITS  bit;
};

// SCI FIFO control register bit definitions:
struct  SCIFFCT_BITS {     // bits   description
   Uint16 FFTXDLY:8;         // 7:0    FIFO transmit delay
   Uint16 rsvd:5;            // 12:8   reserved
   Uint16 CDC:1;             // 13     Auto baud mode enable
   Uint16 ABDCLR:1;          // 14     Auto baud clear
   Uint16 ABD:1;             // 15     Auto baud detect
};

union SCIFFCT_REG {
   Uint16               all;
   struct SCIFFCT_BITS  bit;
};

//---------------------------------------------------------------------------
// SCI寄存器结构体：
//
struct SCI_REGS {
   union SCICCR_REG     SCICCR;     // 通信控制寄存器：配置SCI模块的基本操作模式和数据格式
   
   union SCICTL1_REG    SCICTL1;    // 控制寄存器1：用于启用或禁用SCI模块，以及控制发送和接收功能
   
   Uint16               SCIHBAUD;   // 波特率高字节寄存器：设置波特率的高8位
   
   Uint16               SCILBAUD;   // 波特率低字节寄存器：设置波特率的低8位
   
   union SCICTL2_REG    SCICTL2;    // 控制寄存器2：用于控制中断使能和其他辅助功能
   
   union SCIRXST_REG    SCIRXST;    // 接收状态寄存器：指示接收缓冲区的状态，如是否接收到有效数据
   
   Uint16               SCIRXEMU;   // 接收仿真缓冲寄存器：用于仿真调试时存储接收数据
   
   union SCIRXBUF_REG   SCIRXBUF;   // 接收数据缓冲寄存器：存储接收到的数据
   
   Uint16               rsvd1;      // 保留字段：未使用
   
   Uint16               SCITXBUF;   // 发送数据缓冲寄存器：存储待发送的数据
   
   union SCIFFTX_REG    SCIFFTX;    // FIFO发送寄存器：用于FIFO模式下的发送控制
   
   union SCIFFRX_REG    SCIFFRX;    // FIFO接收寄存器：用于FIFO模式下的接收控制
   
   union SCIFFCT_REG    SCIFFCT;    // FIFO控制寄存器：配置FIFO的工作模式和大小
   
   Uint16               rsvd2;      // 保留字段：未使用
   
   Uint16               rsvd3;      // 保留字段：未使用
   
   union SCIPRI_REG     SCIPRI;     // FIFO优先级控制寄存器：配置FIFO的优先级
};

//---------------------------------------------------------------------------
// SCI External References & Function Declarations:
//
extern volatile struct SCI_REGS SciaRegs;

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif  // end of DSP2803x_SCI_H definition

//===========================================================================
// End of file.
//===========================================================================

