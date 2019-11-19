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
#include "inputmouce.h"
#include "RawMouse.h"
#include "select.h"
#include "camera.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define SPEED			(2.0f)
#define MODEL_RENG		(50)
#define MAX_BUNDCNT		(120)
#define MAX_BULLET		(50)

//�r���[�|�[�g�̈ʒu�ݒ�
#define POS_V	(D3DXVECTOR3(0.0f, 70.0f, -100.0f))
#define POS_R	(D3DXVECTOR3(0.0f, 70.0f, 0.0f))
#define POS_U	(D3DXVECTOR3(0.0f, 1.0f, 0.0f))

//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CCharResult::CCharResult(int nPriority, OBJTYPE objType) : CCharacter(nPriority,objType){}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CCharResult::~CCharResult(){}

//=============================================================================
// ����������
//=============================================================================
HRESULT CCharResult::Init(D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40],int nNumPlayer)
{
	//�v���C���[�̃i���o�[���擾
	m_PlayerState = PLAYERSTATE_NONE;

	//�L�����N�^�[�̏�����
	CCharacter::Init(nNumPlayer,ModelTxt, MotionTxt);
	CCharacter::SetPos(pos);

	//���f���̏����擾
	m_pModel = CCharacter::GetModel();

	//���[�V�����̏����擾
	m_pMotion = CCharacter::GetMotion();
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
	CCharacter::Update();
	//CGame *pGame = CManager::GetGame();

	int nGameState = CGame::GetGameState();

	if (nGameState != CGame::GAMESTATE_END && nGameState != CGame::GAMESTATE_FIRSTCOUNTDOWN)
	{
		switch (m_PlayerState)
		{
		case PLAYERSTATE_NONE: break;
		case PLAYERSTATE_WALK: break;
		}
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
CCharResult *CCharResult::Create(D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40])
{
	CCharResult *pPlayer = NULL;
	if (pPlayer == NULL)
	{
		//�������𓮓I�m��
		pPlayer = new CCharResult;
		if (pPlayer != NULL)
		{//NULL�`�F�b�N
			pPlayer->Init(pos, ModelTxt,MotionTxt, 0);
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
//
//=============================================================================
void  CCharResult::Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size){}

//=============================================================================
// ���C���̐�������
//=============================================================================
void CCharResult::CreateOrbitLine(void){}

//=============================================================================
// ���C���̔j��
//=============================================================================
void CCharResult::UninitOrtbitLine(void){}