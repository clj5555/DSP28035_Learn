// TI 文件版本 $Revision: /main/1 $
// 提交日期 $Date: December 5, 2008   18:01:06 $
//###########################################################################
//
// 文件名: DSP2803x_PieCtrl.c
//
// 标题: DSP2803x 设备 PIE 控制寄存器初始化函数.
//
//###########################################################################
// $TI 发布: 2803x C/C++ 头文件 V1.21 $
// $发布日期: December 1, 2009 $
//###########################################################################

#include "DSP2803x_Device.h"     // 包含 DSP2803x 设备头文件
#include "DSP2803x_Examples.h"   // 包含 DSP2803x 示例头文件

//---------------------------------------------------------------------------
// InitPieCtrl: 
//---------------------------------------------------------------------------
// 此函数将 PIE 控制寄存器初始化为已知状态。
//
void InitPieCtrl(void)
{
    // 禁用 CPU 级别的中断:
    DINT;

    // 禁用 PIE
    PieCtrlRegs.PIECTRL.bit.ENPIE = 0;

	// 清除所有 PIEIER 寄存器:
	PieCtrlRegs.PIEIER1.all = 0;
	PieCtrlRegs.PIEIER2.all = 0;
	PieCtrlRegs.PIEIER3.all = 0;	
	PieCtrlRegs.PIEIER4.all = 0;
	PieCtrlRegs.PIEIER5.all = 0;
	PieCtrlRegs.PIEIER6.all = 0;
	PieCtrlRegs.PIEIER7.all = 0;
	PieCtrlRegs.PIEIER8.all = 0;
	PieCtrlRegs.PIEIER9.all = 0;
	PieCtrlRegs.PIEIER10.all = 0;
	PieCtrlRegs.PIEIER11.all = 0;
	PieCtrlRegs.PIEIER12.all = 0;

	// 清除所有 PIEIFR 寄存器:
	PieCtrlRegs.PIEIFR1.all = 0;
	PieCtrlRegs.PIEIFR2.all = 0;
	PieCtrlRegs.PIEIFR3.all = 0;	
	PieCtrlRegs.PIEIFR4.all = 0;
	PieCtrlRegs.PIEIFR5.all = 0;
	PieCtrlRegs.PIEIFR6.all = 0;
	PieCtrlRegs.PIEIFR7.all = 0;
	PieCtrlRegs.PIEIFR8.all = 0;
	PieCtrlRegs.PIEIFR9.all = 0;
	PieCtrlRegs.PIEIFR10.all = 0;
	PieCtrlRegs.PIEIFR11.all = 0;
	PieCtrlRegs.PIEIFR12.all = 0;

}	

//---------------------------------------------------------------------------
// EnableInterrupts: 
//---------------------------------------------------------------------------
// 此函数启用 PIE 模块和 CPU 中断
//
void EnableInterrupts()
{

    // 启用 PIE
    PieCtrlRegs.PIECTRL.bit.ENPIE = 1;
    		
	// 允许 PIE 向 CPU 发送脉冲
	PieCtrlRegs.PIEACK.all = 0xFFFF;  

	// 启用 CPU 级别的中断 
    EINT;

}

//===========================================================================
// 文件结束




