//=============================================================================
//
// �`���[�g���A���X�s�[�h�^�̏��� [tutorialspeedplayer.cpp]
// Author : ���я��� & �R���֎j & ��������
//
//=============================================================================
#include "tutorialplayer.h"
#include "manager.h"
#include "camera.h"
#include "input.h"
#include "loadEffect.h"
#include "line.h"
#include "sceneOrbit.h"
#include "model.h"
#include "UI.h"
#include "skilicon.h"
#include "RawMouse.h"
#include "debuglog.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_SPRINTTIMER (8)		//�X�v�����g����
#define RECAST (20)				//�X�v�����g�̃��L���X�g���� 20
//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CTutorialSpeedPlayer::CTutorialSpeedPlayer(){}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CTutorialSpeedPlayer::~CTutorialSpeedPlayer(){}

//=============================================================================
// ����������
//=============================================================================
HRESULT CTutorialSpeedPlayer::Init(D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40],int nNumPlayer, int nType)
{
	//�v���C���[�̃i���o�[���i�[
	m_nNumPlayer = nNumPlayer;

	//������
	m_bWalk = true;
	m_bSprintMotion = false;
	//�L�����N�^�[�̏�����
	CCharacter::Init(nNumPlayer,ModelTxt, MotionTxt, m_CharcterType,nType);
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
	m_pLoadEffectSpeed = NULL;

	//�R�s�[���C���̏�����
	for (int nCnt = 0; nCnt < MAX_TERRITORY; nCnt++)
	{
		m_apCopyLine[nCnt] = NULL;
	}

	//�G�t�F�N�g�̏�����
	if (m_pLoadEffect == NULL)
	{
		m_pLoadEffect = CLoadEffect::Create(1, D3DXVECTOR3(pos.x, pos.y + 60.0f, pos.z), 6);
	}
	if (m_pLoadEffectSpeed == NULL)
	{
		m_pLoadEffectSpeed = CLoadEffect::Create(2, D3DXVECTOR3(pos.x, pos.y + 30.0f, pos.z), 6);
	}

	//���[�V�����̏����擾
	m_pMotion = CCharacter::GetMotion();
	ResetLine();	//���C���̏�����

	//�X�s�[�h�v���C���[�p�̕ϐ���������
	m_nSprintCounter = 0;
	m_nSprintTimer = 0;
	m_nRecastCounter = 0;
	m_nRecastTimer = 0;
	m_nButtonCounter = 0;
	m_bRecast = false;
	m_bSprint = false;
	m_bSprintMotion = false;
	m_fInertia = GetInertia();			//����
	m_bUseSkill = false;

	//�ړI�n�܂ł̃��X�g�ݒ�
	SetAimPos();

	//�ړI�n�܂ł̃��[�g��ݒ�
	SetMovePlayer(true, 1);

	return S_OK;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CTutorialSpeedPlayer::Init(void)
{
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void  CTutorialSpeedPlayer::Uninit(void)
{
	//�v���C���[�̏I������
	CTutorialPlayer::Uninit();

	if (m_pLoadEffectSpeed != NULL)
	{
		m_pLoadEffectSpeed->Uninit();
		delete m_pLoadEffectSpeed;
		m_pLoadEffectSpeed = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void  CTutorialSpeedPlayer::Update(void)
{
	//�Q�[���̏�Ԃ��擾
	int nGameState = CGame::GetGameState();

	if (nGameState != CGame::GAMESTATE_FIRSTCOUNTDOWN && nGameState != CGame::GAMESTATE_END)
	{
		//�X�s�[�h�^�̃X�L������
		SprintUpdate();
	}

	if ((m_nNumPos == 3) && (!m_bUseSkill))
	{//�X�L�����g�p������Ԃɂ���
		SkillOn();
		m_bUseSkill = true;
		m_fSpeed = 4.6f;
	}

	//�v���C���[�̍X�V
	CTutorialPlayer::Update();
}

//=============================================================================
// �X�v�����g����
//=============================================================================
void  CTutorialSpeedPlayer::SprintUpdate(void)
{
	D3DXVECTOR3 pos = CCharacter::GetPos();							//�ʒu�̎擾

	//�X�v�����g��
	if (m_bSprint == true)
	{
		int nHeight = rand() % 100 + 30;
		m_nSprintCounter++;
		m_pLoadEffectSpeed->SetPos(D3DXVECTOR3(pos.x, pos.y + nHeight, pos.z));
		m_pLoadEffectSpeed->OnTimeEffect();

		if (m_nSprintCounter % 60 == 0)
		{
			m_nSprintTimer++;
			if (m_nSprintTimer >= MAX_SPRINTTIMER)
			{//8�b�������畁�ʂ̑����ɖ߂�
				m_fSpeed = 1.0f;					//�X�s�[�h�����Ƃɖ߂�
				m_nSprintTimer = 0;					//�X�v�����g�^�C�}�[��������
				m_nSprintCounter = 0;
				m_bWalk = true;						//�ړ����[�V�������ł����Ԃɂ���
				m_bSprintMotion = false;
				m_bSprint = false;

				if (m_PlayerState != PLAYERSTATE_DAMAGE)
				{
					m_PlayerState = PLAYERSTATE_NONE;	//�ړ����
				}
			}
		}
	}

	////���L���X�g��
	//if (m_bRecast == true)
	//{
	//	m_nRecastCounter++;
	//	if (m_nRecastCounter % 60 == 0)
	//	{
	//		m_nRecastTimer++;
	//		if (m_nRecastTimer <= RECAST)
	//		{//10�b�ȉ���������X�v�����g���g�p�ł��Ȃ�
	//			m_bSprint = false;
	//		}
	//		else
	//		{//10�b��������X�v�����g���g�p�ł���悤�ɂ���

	//			m_bRecast = false;		//���L���X�g������Ȃ�
	//			m_nRecastTimer = 0;		//���L���X�g�^�C�}�[�̏�����
	//			m_nButtonCounter = 0;	//�X�L���{�^����������悤�ɂ���
	//			m_bSprint = false;
	//		}
	//	}
	//}

#if _DEBUG
	DebugInput();	//�f�o�b�O�p���͏���
#endif
}

////=============================================================================
//// PAD����
////=============================================================================
//void  CTutorialSpeedPlayer::PlayerActionPad(void)
//{
//	CGame *pGame = CManager::GetGame();
//	CUI *pUi = pGame->GetUI();
//	CInputXPad * pXPad = CManager::GetXPad();
//
//	if (pXPad->GetTrigger(XINPUT_GAMEPAD_B, m_nControllerIndx) == true)
//	{
//		if (m_bRecast == false && m_nButtonCounter == 0
//			&& m_PlayerState != PLAYERSTATE_DAMAGE && m_PlayerState != PLAYERSTATE_BLOWAWAY)
//		{
//			m_fSpeed = 1.5f;		//�X�s�[�h�^�̑�����1.5�{�ɂ���
//			m_nButtonCounter = 1;	//�{�^���������Ȃ��悤�ɂ���
//			m_bSprint = true;		//�X�v�����g���ɂ���
//			m_bSprintMotion = true;	//�X�v�����g���[�V���������ǂ���
//			pUi->GetSkilicon(m_nNumPlayer)->RevivalIconMask();
//			//m_PlayerState = PLAYERSTATE_ACTION;	//�X�v�����g���
//
//		}
//	}
//}

//=============================================================================
// �`�揈��
//=============================================================================
void  CTutorialSpeedPlayer::Draw(void)
{
	CTutorialPlayer::Draw();

	if (m_pLoadEffectSpeed != NULL)
	{
		m_pLoadEffectSpeed->Draw();
	}
}

//=============================================================================
//�I�u�W�F�N�g�̐�������
//=============================================================================
CTutorialSpeedPlayer *CTutorialSpeedPlayer::Create(D3DXVECTOR3 pos, int nNumPlayer, int nType)
{
	CTutorialSpeedPlayer *pSpeedPlayer = NULL;
	if (pSpeedPlayer == NULL)
	{
		//�������𓮓I�m��
		pSpeedPlayer = new CTutorialSpeedPlayer;
		if (pSpeedPlayer != NULL)
		{//NULL�`�F�b�N
			pSpeedPlayer->SetType(nType);														//�Q�[���p���^�C�v
			pSpeedPlayer->m_nCuntry[nNumPlayer] = nType;										//���U���g�p�̍��^�C�v
			pSpeedPlayer->m_CharcterType = CCharacter::CHARCTERTYPE_SPEED;						//�Q�[���p�̃L�����^�C�v
			pSpeedPlayer->m_CharcterTypeResult[nNumPlayer] = CCharacter::CHARCTERTYPE_SPEED;	//���U���g�p�̃L�����^�C�v
			pSpeedPlayer->Init(pos, "data\\TEXT\\ModelLoad\\SPEEDPLAYER_LOAD.txt", "data\\TEXT\\MotionLoad\\SPEEDPLAYER_MOTION.txt", nNumPlayer,nType);
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
// �X�L����������
//=============================================================================
void CTutorialSpeedPlayer::SkillOn(void)
{
	m_fSpeed = 1.5f;		//�X�s�[�h�^�̑�����1.5�{�ɂ���
	m_nButtonCounter = 1;	//�{�^���������Ȃ��悤�ɂ���
	m_bSprint = true;		//�X�v�����g���ɂ���
	m_bSprintMotion = true;	//�X�v�����g���[�V���������ǂ���
}

//=============================================================================
// �f�o�b�O�p���͏���
//=============================================================================
void CTutorialSpeedPlayer::DebugInput(void)
{
	CInputKeyboard * pKeyboard = CManager::GetInputkeyboard();

	if (pKeyboard->GetKeyboardTrigger(DIK_SPACE) == true)
	{//�X�L����������
		SkillOn();
	}
}

//=============================================================================
//
//=============================================================================
void  CTutorialSpeedPlayer::Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size){}

//=============================================================================
// ���C���̐�������
//=============================================================================
void CTutorialSpeedPlayer::CreateOrbitLine(void){}

//=============================================================================
// ���C���̔j��
//=============================================================================
void CTutorialSpeedPlayer::UninitOrtbitLine(void){}