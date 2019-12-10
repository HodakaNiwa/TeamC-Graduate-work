//=============================================================================
//
// �`���[�g���A���n���^�̏��� [tutorialteqolayer.cpp]
// Author : �R���֎j & ��������
//
//=============================================================================
#include "tutorialplayer.h"
#include "manager.h"
#include "camera.h"
#include "input.h"
#include "loadEffect.h"
#include "collision.h"
#include "model.h"
#include "UI.h"
#include "skilicon.h"
#include "RawMouse.h"
#include "mine.h"

//*****************************************************************************
// �}�N��
//*****************************************************************************
#define RECASTTIME (10)				//���L���X�g����
#define MAX_MINE_POINT (100)		//�}�C�����g�p�ł���|�C���g
#define REDUCED_MINE_POINT (20)		//�}�C�����g�p�������Ɍ���|�C���g
#define RECOVERY_MINE_POINT (10)	//�}�C���g�p�|�C���g�̉񕜗�
#define RECOVERY_TIME (5)			//�g�p�|�C���g�񕜂܂ł̎���

//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CTutorialTechniquePlayer::CTutorialTechniquePlayer()
{
	m_nRecastCounter = 0;
	m_nRecastTimer = 0;
	m_nButtonCounter = 0;
	m_bRecast = false;
	m_nInstallationCounter = 0;
	m_nInstallationTimer = 0;
}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CTutorialTechniquePlayer::~CTutorialTechniquePlayer(){}

//=============================================================================
// ����������
//=============================================================================
HRESULT CTutorialTechniquePlayer::Init(D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40], int nNumPlayer, int nType)
{
	//�v���C���[�̃i���o�[���i�[
	m_nNumPlayer = nNumPlayer;

	//������
	m_bWalk = true;

	//�L�����N�^�[�̏�����
	CCharacter::Init(nNumPlayer, ModelTxt, MotionTxt, m_CharcterType, nType);
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
	m_pLoadEffect = NULL;

	//�R�s�[���C���̏�����
	for (int nCnt = 0; nCnt < MAX_TERRITORY; nCnt++)
	{
		m_apCopyLine[nCnt] = NULL;
	}
	//�G�t�F�N�g�̏�����
	if (m_pLoadEffect == NULL)
	{
		m_pLoadEffect = CLoadEffect::Create(1, D3DXVECTOR3(pos.x, pos.y + 60.0f, pos.z), 5);
	}
	//���[�V�����̏����擾
	m_pMotion = CCharacter::GetMotion();
	ResetLine();	//���C���̏�����

	//�e�ϐ��̏�����
	m_nRecastCounter = 0;
	m_nRecastTimer = 0;
	m_nButtonCounter = 0;
	m_bRecast = false;
	m_nInstallationCounter = 0;
	m_nInstallationTimer = 0;
	m_bMineUse = false;
	m_nDamageCounter = 0;
	m_fInertia = GetInertia();			//����
	m_bSetMine = false;

	//�ړI�n�܂ł̃��X�g�ݒ�
	SetAimPos();

	//�ړ���Ԃɂ���
	SetMovePlayer(true, 3);
	return S_OK;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CTutorialTechniquePlayer::Init(void)
{
	//����������
	CTutorialPlayer::Init();
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void  CTutorialTechniquePlayer::Uninit(void)
{
	//�v���C���[�̔j��
	CTutorialPlayer::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void  CTutorialTechniquePlayer::Update(void)
{
	//���S�t���O���L����������
	if (GetDeth() == true) { return; }

	//�A�N�V�����̍X�V
	ActionUpdate();

	//�v���C���[�̍X�V
	CTutorialPlayer::Update();

	if (m_bMoveEnd == true)
	{//�n����u��
		SetMine();
		m_bMoveEnd = false;
	}
	else
	{
		if ((!m_bMineUse) && (m_pMotion->GetNumMotion() != PLAYERSTATE_ACTION) && (m_bSetMine))
		{//�n����u�����玟�̖ړI�n�Ɍ�����
			SetMovePlayer(true, 4);
			m_bSetMine = false;
		}
	}

#if _DEBUG
	DebugInput();	//�f�o�b�O�p���͏���
#endif
}

//=============================================================================
// �A�N�V�����̍X�V����
//=============================================================================
void  CTutorialTechniquePlayer::ActionUpdate(void)
{
	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVec3TransformCoord(&pos, &m_pModel[5]->GetPos(), &m_pModel[5]->GetMtxWorld());

	if (m_bMineUse == true)
	{//�n�����u�����Ԃ�������
		m_nInstallationCounter++;

		if (m_nInstallationCounter % 30 == 0)
		{
			m_nInstallationTimer++;
			if (m_nInstallationTimer == 1)
			{
				//�n����ݒu
				CMine::Create(pos, this);
				m_bMineUse = false;
				m_nInstallationCounter = 0;
				m_nInstallationTimer = 0;
			}
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void  CTutorialTechniquePlayer::Draw(void)
{
	//�v���C���[�̕`��
	CTutorialPlayer::Draw();
}

//=============================================================================
//�I�u�W�F�N�g�̐�������
//=============================================================================
CTutorialTechniquePlayer *CTutorialTechniquePlayer::Create(D3DXVECTOR3 pos, int nNumPlayer, int nType)
{
	CTutorialTechniquePlayer *pTeqPlayer = NULL;
	if (pTeqPlayer == NULL)
	{
		//�������𓮓I�m��
		pTeqPlayer = new CTutorialTechniquePlayer;
		if (pTeqPlayer != NULL)
		{//NULL�`�F�b�N
			pTeqPlayer->SetType(nType);												//�Q�[���p���^�C�v
			pTeqPlayer->m_nCuntry[nNumPlayer] = nType;								//���U���g�p�̍��^�C�v
			pTeqPlayer->m_CharcterType = CCharacter::CHARCTERTYPE_TECHNIQUE;		//�Q�[���p�̃L�����^�C�v
			pTeqPlayer->m_CharcterTypeResult[nNumPlayer] = CHARCTERTYPE_TECHNIQUE;	//���U���g�p�̃L�����^�C�v
			pTeqPlayer->Init(pos, "data\\TEXT\\ModelLoad\\TEQPLAYER_LOAD.txt", "data\\TEXT\\MotionLoad\\TEQPLAYER_MOTION.txt", nNumPlayer, nType);	//�v���C���[�̏�����
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
	return pTeqPlayer;
}

//=============================================================================
// �n���̐ݒ�
//=============================================================================
void CTutorialTechniquePlayer::SetMine(void)
{
	//�T�E���h�̎擾
	CSound *pSound = CManager::GetSound();
	m_PlayerState = PLAYERSTATE_ACTION;
	m_pMotion->SetNumMotion(PLAYERSTATE_ACTION);
	m_bMineUse = true;
	m_bSetMine = true;
	pSound->PlaySound(CSound::SOUND_LABEL_SE024);
}

//=============================================================================
// �f�o�b�O�p���͏���
//=============================================================================
void CTutorialTechniquePlayer::DebugInput(void)
{
	CInputKeyboard * pKeyboard = CManager::GetInputkeyboard();

	if (pKeyboard->GetKeyboardTrigger(DIK_SPACE) == true)
	{//�n����u��
		SetMine();
	}
}

//=============================================================================
//
//=============================================================================
void  CTutorialTechniquePlayer::Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size){}