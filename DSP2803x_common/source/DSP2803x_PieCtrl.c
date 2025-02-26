// TI �ļ��汾 $Revision: /main/1 $
// �ύ���� $Date: December 5, 2008   18:01:06 $
//###########################################################################
//
// �ļ���: DSP2803x_PieCtrl.c
//
// ����: DSP2803x �豸 PIE ���ƼĴ�����ʼ������.
//
//###########################################################################
// $TI ����: 2803x C/C++ ͷ�ļ� V1.21 $
// $��������: December 1, 2009 $
//###########################################################################

#include "DSP2803x_Device.h"     // ���� DSP2803x �豸ͷ�ļ�
#include "DSP2803x_Examples.h"   // ���� DSP2803x ʾ��ͷ�ļ�

//---------------------------------------------------------------------------
// InitPieCtrl: 
//---------------------------------------------------------------------------
// �˺����� PIE ���ƼĴ�����ʼ��Ϊ��֪״̬��
//
void InitPieCtrl(void)
{
    // ���� CPU ������ж�:
    DINT;

    // ���� PIE
    PieCtrlRegs.PIECTRL.bit.ENPIE = 0;

	// ������� PIEIER �Ĵ���:
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

	// ������� PIEIFR �Ĵ���:
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
// �˺������� PIE ģ��� CPU �ж�
//
void EnableInterrupts()
{

    // ���� PIE
    PieCtrlRegs.PIECTRL.bit.ENPIE = 1;
    		
	// ���� PIE �� CPU ��������
	PieCtrlRegs.PIEACK.all = 0xFFFF;  

	// ���� CPU ������ж� 
    EINT;

}

//===========================================================================
// �ļ�����




