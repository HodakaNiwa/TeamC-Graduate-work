//	◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆
//					プレイヤー管理 [player.cpp]
//			Author : Kobayashi_Sho-hei / 小林 将兵
//	◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆
//	---<<#include>>---
#include "player.h"
#include "model.h"
//==============================================
//					コンストラクタ
//==============================================
CPlayer::CPlayer()
{

}

//==============================================
//					デストラクタ
//==============================================
CPlayer::~CPlayer()
{

}

//==============================================
//					生成
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
//					初期化
////==============================================
HRESULT CPlayer::Init(char ModelTxt[40], char MotionTxt[40])
{
	CCharacter::Init(ModelTxt, MotionTxt);

	m_pModel = CCharacter::GetModel();
	m_pModel[0]->SetFirstPos(D3DXVECTOR3(50.0f, 20.0f, 80.0f));	//	初期値pos(※手打ち)
	return S_OK;
}

//==============================================
//					終了
//==============================================
void CPlayer::Uninit()
{
	CCharacter::Uninit();
}

//==============================================
//					更新
//==============================================
void CPlayer::Update()
{
	CCharacter::Update();
}

//==============================================
//					描画
//==============================================
void CPlayer::Draw()
{
	CCharacter::Draw();
}
