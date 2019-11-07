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
CSpeedPlayer::CSpeedPlayer()
{

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
HRESULT CSpeedPlayer::Init(D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40],int nNumPlayer)
{
	//�v���C���[�̃i���o�[���i�[
	m_nNumPlayer = nNumPlayer;

	//������
	m_bWalk = true;

	//�L�����N�^�[�̏�����
	CCharacter::Init(ModelTxt, MotionTxt);
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
		m_pLoadEffect = CLoadEffect::Create(1, D3DXVECTOR3(pos.x, pos.y + 60.0f, pos.z), 8);
	}

	//���[�V�����̏����擾
	m_pMotion = CCharacter::GetMotion();
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
	//�v���C���[�̈ړ�
	//SPlayerMove();
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
CSpeedPlayer *CSpeedPlayer::Create(D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40],int nNumPlayer, TYPE type, PLAYERTYPE playertype)
{
	CSpeedPlayer *pSpeedPlayer = NULL;
	if (pSpeedPlayer == NULL)
	{
		//�������𓮓I�m��
		pSpeedPlayer = new CSpeedPlayer;
		if (pSpeedPlayer != NULL)
		{//NULL�`�F�b�N
			pSpeedPlayer->SetType(type);
			pSpeedPlayer->m_PlayerType = playertype;
			pSpeedPlayer->Init(pos,ModelTxt,MotionTxt,nNumPlayer);
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
//
////=============================================================================
////
////=============================================================================
//void  CPlayer::Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
//{
//
//}