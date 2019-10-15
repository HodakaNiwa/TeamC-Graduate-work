//	���@���@���@���@���@���@���@���@���@���@���@���@���@���@���@���@���@��
//					�v���C���[�Ǘ� [player.cpp]
//			Author : Kobayashi_Sho-hei / ���� ����
//	���@���@���@���@���@���@���@���@���@���@���@���@���@���@���@���@���@��
//	---<<#include>>---
#include "player.h"
#include "model.h"
//==============================================
//					�R���X�g���N�^
//==============================================
CPlayer::CPlayer()
{

}

//==============================================
//					�f�X�g���N�^
//==============================================
CPlayer::~CPlayer()
{

}

//==============================================
//					����
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
//					������
////==============================================
HRESULT CPlayer::Init(char ModelTxt[40], char MotionTxt[40])
{
	CCharacter::Init(ModelTxt, MotionTxt);

	m_pModel = CCharacter::GetModel();
	m_pModel[0]->SetFirstPos(D3DXVECTOR3(50.0f, 20.0f, 80.0f));	//	�����lpos(����ł�)
	return S_OK;
}

//==============================================
//					�I��
//==============================================
void CPlayer::Uninit()
{
	CCharacter::Uninit();
}

//==============================================
//					�X�V
//==============================================
void CPlayer::Update()
{
	CCharacter::Update();
}

//==============================================
//					�`��
//==============================================
void CPlayer::Draw()
{
	CCharacter::Draw();
}
