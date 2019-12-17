//◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆
//					AIコントローラー処理 [AIController.cpp]
//				Author : Kobayashi_Sho-hei / 小林 将兵
//◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆
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
//!						↓↓↓補完関数まとめ↓↓↓
//=======================================================================

//-----------------------------------------------------------------------
//						2点間の距離を求める処理
//-----------------------------------------------------------------------

float CAIController::dist(D3DXVECTOR3 target, D3DXVECTOR3 owner)
{
	float fPosX, fPosZ, fOut;

	fPosX = owner.x - target.x;
	fPosZ = owner.z - target.z;
	
	fOut = (float)sqrt((fPosX*fPosX) + (fPosZ*fPosZ));

	return fOut;
}
