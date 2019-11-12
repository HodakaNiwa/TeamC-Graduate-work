//=============================================================================
//
// ���f������ [model.cpp]
// Author : Kobayashi & Yamashita
//
//=============================================================================
#include "player.h"
#include "manager.h"
#include "camera.h"
#include "input.h"
#include "inputmouce.h"
#include "loadEffect.h"
//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CPowerPlayer::CPowerPlayer()
{

}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CPowerPlayer::~CPowerPlayer()
{

}



//=============================================================================
// ����������
//=============================================================================
HRESULT CPowerPlayer::Init(D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40], int nNumPlayer)
{
	//�v���C���[�̃i���o�[���i�[
	m_nNumPlayer = nNumPlayer;

	//������
	m_bWalk = true;

	//�L�����N�^�[�̏�����
	CCharacter::Init(nNumPlayer,ModelTxt, MotionTxt);
	CCharacter::SetPos(pos);

	//���f���̎擾
	m_pModel = CCharacter::GetModel();

	//���C���ϐ��̏�����
	m_nMaxLineTime = FIRST_LINETIME;
	m_nLineTime = m_nMaxLineTime;
	m_bBlockStartTerritory = false;
	m_pOldStartTerritory = NULL;
	m_nCountTime = 0;
	m_pOrbitLine = NULL;
	m_bMakeShape = false;
	m_nCntTimeCopyLine = 0;

	//�R�s�[���C���̏�����
	for (int nCnt = 0; nCnt < MAX_TERRITORY; nCnt++)
	{
		m_apCopyLine[nCnt] = NULL;
	}
	//�G�t�F�N�g�̏�����
	if (m_pLoadEffect == NULL)
	{
		m_pLoadEffect = CLoadEffect::Create(4, D3DXVECTOR3(pos.x, pos.y + 60.0f, pos.z), 5);
	}
	//���[�V�����̏����擾
	m_pMotion = CCharacter::GetMotion();
	m_fSpeed = 1.0f;
	ResetLine();	//���C���̏�����

	return S_OK;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CPowerPlayer::Init(void)
{
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void  CPowerPlayer::Uninit(void)
{
	CPlayer::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void  CPowerPlayer::Update(void)
{
	CPlayer::Update();
	//�v���C���[�̈ړ�
	//SPlayerMove();
}


//=============================================================================
// �`�揈��
//=============================================================================
void  CPowerPlayer::Draw(void)
{
	CPlayer::Draw();
}

//=============================================================================
//�I�u�W�F�N�g�̐�������
//=============================================================================
CPowerPlayer *CPowerPlayer::Create(D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40], int nNumPlayer, int nType, PLAYERTYPE playertype)
{
	CPowerPlayer *pPowerplayer = NULL;
	if (pPowerplayer == NULL)
	{
		//�������𓮓I�m��
		pPowerplayer = new CPowerPlayer;
		if (pPowerplayer != NULL)
		{//NULL�`�F�b�N
			pPowerplayer->SetType(nType);
			pPowerplayer->m_PlayerType = playertype;
			pPowerplayer->Init(pos, ModelTxt, MotionTxt, nNumPlayer);
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
	return pPowerplayer;
}

//=============================================================================
//
//=============================================================================
void  CPowerPlayer::Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
{

}