
//=============================================================================
//
// ���f������ [model.cpp]
// Author : Kobayashi & Yamashita
//
//=============================================================================
#include "speedplayer.h"
#include "input.h"
#include "camera.h"
#include "manager.h"
#include "renderer.h"
#include "scene2D.h"
#include "scene3D.h"
#include "game.h"
#include "title.h"
#include "player.h"
#include "tutorial.h"
#include "collision.h"
#include "model.h"
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//�ÓI�����o�ϐ��錾
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define SPEED (2.0f)
#define MODEL_RENG (50)
#define MAX_BUNDCNT (120)
#define MAX_BULLET (50)

//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CSpeedPlayer::CSpeedPlayer()
{
	m_Angle = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CSpeedPlayer::~CSpeedPlayer()
{

}



//=============================================================================
// ����������
//=============================================================================
HRESULT CSpeedPlayer::Init(D3DXVECTOR3 pos, int nNumPlayer)
{
	m_nNumPlayer = nNumPlayer;
	CPlayer::Create(D3DXVECTOR3(-50.0f, 0.0f, 0.0f), m_NumPlayer, "data\\TEXT\\MOTION\\SpeedPlayer_downModel.txt", "data\\TEXT\\MOTION\\SpeedPlayer_downMotion.txt",CPlayer::TYPE_ENGLAND);
	CPlayer::Create(D3DXVECTOR3(-50.0f,55.0f,0.0f), m_NumPlayer, "data\\TEXT\\MOTION\\SpeedPlayer_upModel.txt", "data\\TEXT\\MOTION\\SpeedPlayer_upMotion.txt", CPlayer::TYPE_ENGLAND);

	m_pModel = CCharacter::GetModel();
	CCharacter::SetPos(pos);

	return S_OK;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CSpeedPlayer::Init(void)
{

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void  CSpeedPlayer::Uninit(void)
{
	CPlayer::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void  CSpeedPlayer::Update(void)
{
	CPlayer::Update();
}

//=============================================================================
// �`�揈��
//=============================================================================
void  CSpeedPlayer::Draw(void)
{
	CPlayer::Draw();
}

//=============================================================================
//�I�u�W�F�N�g�̐�������
//=============================================================================
CSpeedPlayer *CSpeedPlayer::Create(D3DXVECTOR3 pos, int nNumPlayer,TYPE type)
{
	CSpeedPlayer *pSpeedPlayer = NULL;

	if (pSpeedPlayer == NULL)
	{
		//�������𓮓I�m��
		pSpeedPlayer = new CSpeedPlayer;

		if (pSpeedPlayer != NULL)
		{//NULL�`�F�b�N

		 //����������
			pSpeedPlayer->Init(pos, nNumPlayer);
			pSpeedPlayer->SetType(type);
		}
		else
		{
			MessageBox(0, "�v���C���[��NULL�ł���", "�x��", MB_OK);
		}
	}
	else
	{
		MessageBox(0, "�v���C���[��NULL����Ȃ��ł�", "�x��", MB_OK);
	}
	return pSpeedPlayer;
}

//=============================================================================
//
//=============================================================================
void  CSpeedPlayer::Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
{

}

////=============================================================================
////�@�v���C���[�̈ړ�����
////=============================================================================
//void  CPlayer::PlayerMove(void)
//{
//	CInputKeyboard * pInputKeyboard = CManager::GetInputkeyboard();	//�L�[�{�[�h�̎擾
//	CGamePad * pGamePad = CManager::GetInputGamePad();				//�Q�[���p�b�h�̎擾
//	CInputXPad * pXPad = CManager::GetXPad();
//	float fRot = pXPad->GetStickRot(0, m_NumPlayer);
//
//
//	CManager *pManager = NULL;
//	CGame *pGame = pManager->GetGame();
//	CGameCamera *pGameCamera = pGame->GetGameCamera(0);
//
//	//�����ƈʒu�̎擾
//	D3DXVECTOR3 rot = pGameCamera->GetRot();
//	D3DXVECTOR3 pos = CCharacter::GetPos();
//
//	//�X�e�[�^�X�̎擾
//	int   nAttack = GetAttack();
//	float fAttackSpeed = GetAttackSpeed();
//	float fRange = GetRange();
//	float fSpeed = GetSpeed();
//	float fInertia = GetInertia();
//	float fLineTime = GetLineTime();
//
//	//�O��̈ʒu���i�[
//	m_posOld = pos;
//
//	//�v���C���[�̈ړ������i�L�[�{�[�h�j
//	if (pInputKeyboard->GetKeyboardPress(DIK_LEFT) == true)
//	{
//		if (pInputKeyboard->GetKeyboardPress(DIK_UP) == true)
//		{//�|���S��������Ɉړ�
//			m_move.x -= sinf(rot.y + (-D3DX_PI * 0.25f)) * fSpeed;
//			m_move.z -= cosf(rot.y + (-D3DX_PI * 0.25f)) * fSpeed;
//			m_Angle.y = rot.y - (D3DX_PI * 0.25f);
//		}
//		else if (pInputKeyboard->GetKeyboardPress(DIK_DOWN) == true)
//		{//�|���S���������Ɉړ�
//			m_move.x -= sinf(rot.y + (-D3DX_PI * 0.75f)) * fSpeed;
//			m_move.z -= cosf(rot.y + (-D3DX_PI * 0.75f)) * fSpeed;
//			m_Angle.y = rot.y - (D3DX_PI * 0.75f);
//		}
//		else
//		{//�|���S�������Ɉړ�
//			m_move.x -= sinf(rot.y + (-D3DX_PI * 0.5f)) * fSpeed;
//			m_move.z -= cosf(rot.y + (-D3DX_PI * 0.5f)) * fSpeed;
//			m_Angle.y = rot.y - (D3DX_PI * 0.5f);
//		}
//	}
//	else if (pInputKeyboard->GetKeyboardPress(DIK_RIGHT) == true)
//	{
//		if (pInputKeyboard->GetKeyboardPress(DIK_UP) == true)
//		{//�|���S�����E��Ɉړ�
//			m_move.x -= sinf(rot.y + (D3DX_PI * 0.25f)) * fSpeed;
//			m_move.z -= cosf(rot.y + (D3DX_PI * 0.25f)) * fSpeed;
//			m_Angle.y = rot.y + (D3DX_PI * 0.25f);
//		}
//		else if (pInputKeyboard->GetKeyboardPress(DIK_DOWN) == true)
//		{//�|���S�����E���Ɉړ�
//			m_move.x -= sinf(rot.y + (D3DX_PI * 0.75f)) * fSpeed;
//			m_move.z -= cosf(rot.y + (D3DX_PI * 0.75f)) * fSpeed;
//			m_Angle.y = rot.y + (D3DX_PI * 0.75f);
//		}
//		else
//		{//�|���S�����E�Ɉړ�
//			m_move.x -= sinf(rot.y + (D3DX_PI * 0.5f)) * SPEED;
//			m_move.z -= cosf(rot.y + (D3DX_PI * 0.5f)) * SPEED;
//			m_Angle.y = rot.y + (D3DX_PI * 0.5f);
//		}
//	}
//	else if (pInputKeyboard->GetKeyboardPress(DIK_UP) == true)
//	{
//		m_move.x -= sinf(rot.y) * SPEED;
//		m_move.z -= cosf(rot.y) * SPEED;
//		m_Angle.y = rot.y;
//	}
//	else if (pInputKeyboard->GetKeyboardPress(DIK_DOWN) == true)
//	{
//		m_move.x -= sinf(rot.y + D3DX_PI * 1.0f) * fSpeed;
//		m_move.z -= cosf(rot.y + D3DX_PI * 1.0f) * fSpeed;
//		m_Angle.y = D3DX_PI + rot.y;
//	}
//
//	//�R���g���[���[�iXInput�j
//	if (pXPad->GetStick(0, m_NumPlayer) == true)
//	{
//		m_move.x -= sinf(fRot + rot.y) * fSpeed;
//		m_move.z -= cosf(fRot + rot.y) * fSpeed;
//		m_Angle.y = rot.y + fRot;
//	}
//
//	D3DXVECTOR3 DiffAngle;
//
//	// �v���C���[�̊p�x���C��
//	DiffAngle.y = m_Angle.y - m_rot.y;   //�������v�Z
//
//	if (DiffAngle.y > D3DX_PI)
//	{//D3DX_PI�𒴂���
//		DiffAngle.y -= D3DX_PI * 2.0f;
//	}
//	if (DiffAngle.y < -D3DX_PI)
//	{//-D3DX_PI�𒴂���
//		DiffAngle.y += D3DX_PI * 2.0f;
//	}
//
//	//������{���Œ���
//	m_rot.y += DiffAngle.y * 0.1f;
//
//	if (m_rot.y > D3DX_PI)
//	{//D3DX_PI�𒴂���
//		m_rot.y -= D3DX_PI * 2.0f;
//	}
//	if (m_rot.y < -D3DX_PI)
//	{//-D3DX_PI�𒴂���
//		m_rot.y += D3DX_PI * 2.0f;
//	}
//
//	pos += m_move;
//
//	//����
//	m_move.x += (0.0f - m_move.x) * fInertia;
//	m_move.z += (0.0f - m_move.z) * fInertia;
//
//	CCharacter::SetPos(pos);
//	CCharacter::SetRot(m_rot);
//}


////=============================================================================
////�@�v���C���[�ƃe���g���[�̓����蔻��
////=============================================================================
//void CPlayer::Collision(void)
//{
//	//�e��l�̎擾
//	D3DXVECTOR3 pos = CCharacter::GetPos();
//	D3DXVECTOR3 posOld = m_posOld;
//	D3DXVECTOR3 Move = INITIALIZE_VECTOR3;
//	D3DXVECTOR3 ColRange;
//	//����J�n
//	CCollider *pCol = NULL;
//	CCollider *pColNext = NULL;
//
//	for (int nCntType = 0; nCntType < CCollider::TYPE_MAX; nCntType++)
//	{
//		pCol = CCollider::GetTop(nCntType);
//
//		while (pCol != NULL)
//		{
//			pColNext = pCol->GetNext();
//			CollisionCollider(pCol, pos, posOld, m_move, D3DXVECTOR3(15.0f, 15.0f, 15.0f));
//			pCol = pColNext;
//		}
//	}
//	CCharacter::SetPos(pos);
//}

////=============================================================================
////�@�R���C�_�[�Ƃ̓����蔻�菈��
////=============================================================================
//bool CPlayer::CollisionCollider(CCollider *pCollider, D3DXVECTOR3 &pos, D3DXVECTOR3 &posOld, D3DXVECTOR3 &Move, D3DXVECTOR3 &ColRange)
//{
//	if (pCollider->GetType() == CCollider::TYPE_BOX)
//	{
//		if (CollisionBoxCollider((CBoxCollider*)pCollider, pos, posOld, Move, ColRange) == true)
//		{
//			int nData = 0;
//		}
//	}
//	return false;
//}
//
////=============================================================================
////�@�{�b�N�X�R���C�_�[�Ƃ̓����蔻�菈��
////=============================================================================
//bool CPlayer::CollisionBoxCollider(CBoxCollider *pBoxCollider, D3DXVECTOR3 &pos, D3DXVECTOR3 &posOld, D3DXVECTOR3 &Move, D3DXVECTOR3 &ColRange)
//{
//	bool bLand = false;
//
//	if (pBoxCollider->Collision(&pos, &posOld, &Move, ColRange, this, &bLand) == true)
//	{
//		if (bLand == true)
//		{
//
//		}
//		return true;
//	}
//	return false;
//}