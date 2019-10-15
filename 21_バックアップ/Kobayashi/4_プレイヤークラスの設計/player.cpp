//	@@@@@@@@@@@@@@@@@
//					vC[Ç [player.cpp]
//			Author : Kobayashi_Sho-hei / ¬Ñ «º
//	@@@@@@@@@@@@@@@@@
//	---<<#include>>---
#include "player.h"
#include "model.h"
//==============================================
//					RXgN^
//==============================================
CPlayer::CPlayer()
{

}

//==============================================
//					fXgN^
//==============================================
CPlayer::~CPlayer()
{

}

//==============================================
//					¶¬
//==============================================
CPlayer* CPlayer::Create(char ModelTxt[40], char MotionTxt[40])
{
	CPlayer * pPlayer = NULL;
	if (pPlayer == NULL)
	{
		pPlayer = new CPlayer;
		pPlayer->Init(ModelTxt, MotionTxt);
	}

	return pPlayer;
}

//==============================================
//					ú»
////==============================================
HRESULT CPlayer::Init(char ModelTxt[40], char MotionTxt[40])
{
	CCharacter::Init(ModelTxt, MotionTxt);

	m_pModel = CCharacter::GetModel();
	m_pModel[0]->SetFirstPos(D3DXVECTOR3(50.0f, 20.0f, 80.0f));	//	úlpos(¦èÅ¿)
	return S_OK;
}

//==============================================
//					I¹
//==============================================
void CPlayer::Uninit()
{
	CCharacter::Uninit();
}

//==============================================
//					XV
//==============================================
void CPlayer::Update()
{
	CCharacter::Update();
}

//==============================================
//					`æ
//==============================================
void CPlayer::Draw()
{
	CCharacter::Draw();
}
