//=============================================================================
//
// ���U���g�p�v���C���[�̏��� [charResult.cpp]
// Author : ���� �����@[ SATO ASUMI ]
//
//=============================================================================
#include "charResult.h"
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
#include "Territory.h"
#include "library.h"
#include "line.h"
#include "loadEffect.h"
#include "RawMouse.h"
#include "select.h"
#include "camera.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MODEL_RENG		(50)
#define MAX_BUNDCNT		(120)
#define MAX_BULLET		(50)
#define SPEED			(5.5f);

//�r���[�|�[�g�̈ʒu�ݒ�
#define POS_V	(D3DXVECTOR3(0.0f, 70.0f, -100.0f))
#define POS_R	(D3DXVECTOR3(0.0f, 70.0f, 0.0f))
#define POS_U	(D3DXVECTOR3(0.0f, 1.0f, 0.0f))

//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CCharResult::CCharResult(int nPriority, OBJTYPE objType) : CCharacter(nPriority,objType)
{
	m_pModel = NULL;
	m_pMotion = NULL;
	m_move = INITIALIZE_VECTOR3;
	m_rot = INITIALIZE_VECTOR3;
	m_PlayerType = PLAYERTYPE_SPEED;
	m_MotionType = MOTION_TYPE_NONE;
}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CCharResult::~CCharResult(){}

//=============================================================================
// ����������
//=============================================================================
HRESULT CCharResult::Init(D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40],int nNumPlayer, int nCuntry)
{
	//����������
	m_MotionType = MOTION_TYPE_NONE;
	m_bStopMotion = false;
	m_move = INITIALIZE_VECTOR3;

	//�L�����N�^�[�̏�����
	CCharacter::Init(nNumPlayer,ModelTxt, MotionTxt,m_CharcterType, nCuntry);
	CCharacter::SetPos(pos);

	//���f���̏����擾
	m_pModel = CCharacter::GetModel();

	//���[�V�����̏����擾
	m_pMotion = CCharacter::GetMotion();

	//�ړ������̏�����
	InitMove();
	return S_OK;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CCharResult::Init(void)
{
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void  CCharResult::Uninit(void)
{
	CCharacter::Uninit();
	
	//���[�V�����̔j��
	if (m_pMotion != NULL)
	{
		m_pMotion = NULL;
	}

	//���f���̔j��
	if (m_pModel != NULL)
	{
		m_pModel = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void  CCharResult::Update(void)
{
	if (m_bStopMotion) { return; }
	if (m_pMotion == NULL) { return; }

	UpdateMove();
	CCharacter::Update();

 	int nKey = m_pMotion->GetKey();
	int nFram = m_pMotion->GetnCountMotion();

	if (m_CharcterType == CCharacter::CHARCTERTYPE_POWER)
	{
		if ((nKey == 14) && (nFram == 14)) 
		{ 
			m_bStopMotion = true; 
		}
	}
	else if (m_CharcterType == CCharacter::CHARCTERTYPE_TECHNIQUE)
	{
		if ((nKey == 6) && (nFram == 9))
		{ 
			m_bStopMotion = true;
		}
	}

	if (m_pMotion->GetNumMotion() == (int)m_MotionType) { return; }
	
	switch (m_MotionType)
	{
	case MOTION_TYPE_WIN:
		m_pMotion->SetNumMotion(MOTION_TYPE_WIN);
		break;

	case MOTION_TYPE_NONE:
		m_pMotion->SetNumMotion(MOTION_TYPE_NONE);
		break;
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void  CCharResult::Draw(void)
{
	CCharacter::Draw();
}

//=============================================================================
//�I�u�W�F�N�g�̐�������
//=============================================================================
CCharResult *CCharResult::Create(D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40], CHARCTERTYPE charactertype, int nCharaNum, int nCuntry)
{
	CCharResult *pPlayer = NULL;
	if (pPlayer == NULL)
	{
		//�������𓮓I�m��
		pPlayer = new CCharResult;
		if (pPlayer != NULL)
		{//NULL�`�F�b�N
			pPlayer->m_CharcterType = charactertype;
			pPlayer->Init(pos, ModelTxt,MotionTxt, nCharaNum, nCuntry);
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
	return pPlayer;
}

//=============================================================================
// �ړ������̏�����
//=============================================================================
void CCharResult::InitMove(void)
{
	if (m_CharcterType != CCharacter::CHARCTERTYPE_SPEED) { return; }

	D3DXVECTOR3 Pos = CCharacter::GetPos();		//�ʒu���擾
	float fAngle = CFunctionLib::CalcAngleToDest(40.0f, 0.0f, Pos.x, Pos.z);

	//�ړ��ʂ����߂�
	m_move.x = sinf(fAngle) * -SPEED;
	m_move.z = cosf(fAngle) * -SPEED;

	//�L�����̌�����ς���
	CCharacter::SetRot(D3DXVECTOR3(0.0f, fAngle, 0.0F));
}

//=============================================================================
// �ړ�����
//=============================================================================
void CCharResult::UpdateMove(void)
{
	if (m_CharcterType != CCharacter::CHARCTERTYPE_SPEED) { return; }

	D3DXVECTOR3 Pos = CCharacter::GetPos();		//�ʒu���擾
	Pos += m_move;

	//�ʒu�C��
	int nCount = 0;
	if (Pos.x < 40.0f)
	{
		Pos.x = 40.0f;
		nCount++;
	} 
	if (Pos.z < 0.0f)
	{
		Pos.z = 0.0f;
		nCount++;
	}

	if (nCount == 2) { m_MotionType = MOTION_TYPE_NONE; }

	CCharacter::SetPos(Pos);
}

//=============================================================================
//
//=============================================================================
void  CCharResult::Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size){}