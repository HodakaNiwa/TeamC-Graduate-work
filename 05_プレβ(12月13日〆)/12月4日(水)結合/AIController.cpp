//���@���@���@���@���@���@���@���@���@���@���@���@���@���@���@���@���@��
//					AI�R���g���[���[���� [AIController.cpp]
//				Author : Kobayashi_Sho-hei / ���� ����
//���@���@���@���@���@���@���@���@���@���@���@���@���@���@���@���@���@��
#include "AIController.h"

CAIController::CAIController()
{

}

CAIController::~CAIController()
{

}

void CAIController::AIManual01()
{

}

void CAIController::AIManual02()
{

}

//=======================================================================
//!						�������⊮�֐��܂Ƃ߁�����
//=======================================================================

//-----------------------------------------------------------------------
//						2�_�Ԃ̋��������߂鏈��
//-----------------------------------------------------------------------

float CAIController::dist(D3DXVECTOR3 target, D3DXVECTOR3 owner)
{
	float fPosX, fPosZ, fOut;

	fPosX = owner.x - target.x;
	fPosZ = owner.z - target.z;
	
	fOut = (float)sqrt((fPosX*fPosX) + (fPosZ*fPosZ));

	return fOut;
}
