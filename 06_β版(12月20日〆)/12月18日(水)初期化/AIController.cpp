//@@@@@@@@@@@@@@@@@
//					AIRg[[ [AIController.cpp]
//				Author : Kobayashi_Sho-hei / ¬Ñ «º
//@@@@@@@@@@@@@@@@@
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
//!						«««â®ÖÜÆß«««
//=======================================================================

//-----------------------------------------------------------------------
//						2_ÔÌ£ðßé
//-----------------------------------------------------------------------

float CAIController::dist(D3DXVECTOR3 target, D3DXVECTOR3 owner)
{
	float fPosX, fPosZ, fOut;

	fPosX = owner.x - target.x;
	fPosZ = owner.z - target.z;
	
	fOut = (float)sqrt((fPosX*fPosX) + (fPosZ*fPosZ));

	return fOut;
}
