//=============================================================================
//
// �X�s�[�h�^�̏��� [speedplayer.cpp]
// Author : ���я��� & �R���֎j
//
//=============================================================================
#include "player.h"
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
HRESULT CSpeedPlayer::Init(D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40],int nNumPlayer, int nType)
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
	m_fSpeed = 1.0f;
	m_nSprintCounter = 0;
	m_nSprintTimer = 0;
	m_nRecastCounter = 0;
	m_nRecastTimer = 0;
	m_nButtonCounter = 0;
	m_bRecast = false;
	m_bSprint = false;
	m_nControllerIndx = 0;
	m_nControllerType = 0;
	m_bSprintMotion = false;
	m_fInertia = GetInertia();			//����

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
	//�v���C���[�̏I������
	CPlayer::Uninit();

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
void  CSpeedPlayer::Update(void)
{
	//�Q�[���̏�Ԃ��擾
	int nGameState = CGame::GetGameState();

	if (nGameState != CGame::GAMESTATE_FIRSTCOUNTDOWN && nGameState != CGame::GAMESTATE_END)
	{
		//�X�s�[�h�^�̃X�L������
		SprintUpdate();
	}

	//�v���C���[�̍X�V
	CPlayer::Update();
}

//=============================================================================
// �X�v�����g����
//=============================================================================
void  CSpeedPlayer::SprintUpdate(void)
{
	CGame *pGame = CManager::GetGame();
	CUI *pUi = pGame->GetUI();
	CInputKeyboard * pInputKeyboard = CManager::GetInputkeyboard();	//�L�[�{�[�h�̎擾
	CSound *pSound = CManager::GetSound();

	D3DXVECTOR3 pos = CCharacter::GetPos();							//�ʒu�̎擾

	if (m_nControllerType == 0)
	{
		//�R���g���[���[����
		PlayerActionPad();
	}
	else if (m_nControllerType == 1)
	{
		//�}�E�X����
		PlayerActionMouse();
	}
	else
	{
		//�L�[�{�[�h����
		PlayerActionKeyboard();
	}
	
	//�X�v�����g��
	if (m_bSprint == true)
	{
		pUi->GetSkilicon(m_nNumPlayer)->RevivalIconMask();
		m_nSprintCounter++;
		//m_PlayerState = PLAYERSTATE_ACTION;
		int nHeight = rand() % 100 + 30;

		m_pLoadEffectSpeed->SetPos(D3DXVECTOR3(pos.x, pos.y + nHeight, pos.z));
		m_pLoadEffectSpeed->OnTimeEffect();

		if (m_nSprintCounter % 60 == 0)
		{
			m_nSprintTimer++;
			if (m_nSprintTimer >= MAX_SPRINTTIMER)
			{//8�b�������畁�ʂ̑����ɖ߂�
				m_fSpeed = 1.0f;					//�X�s�[�h�����Ƃɖ߂�
				m_nSprintTimer = 0;					//�X�v�����g�^�C�}�[��������
				m_bRecast = true;					//���L���X�g���ɂ���
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

	//���L���X�g��
	if (m_bRecast == true)
	{
		m_nRecastCounter++;
		pSound->StopSound(CSound::SOUND_LABEL_SE030);
		if (m_nRecastCounter % 60 == 0)
		{
			m_nRecastTimer++;
			if (m_nRecastTimer <= RECAST)
			{//10�b�ȉ���������X�v�����g���g�p�ł��Ȃ�
				m_bSprint = false;
			}
			else
			{//10�b��������X�v�����g���g�p�ł���悤�ɂ���

				m_bRecast = false;		//���L���X�g������Ȃ�
				m_nRecastTimer = 0;		//���L���X�g�^�C�}�[�̏�����
				m_nButtonCounter = 0;	//�X�L���{�^����������悤�ɂ���
				m_bSprint = false;
			}
		}
	}

	CDebugProc::Print("�R���g���[���[ : %d\n", m_nControllerType);

}

//=============================================================================
// PAD����
//=============================================================================
void  CSpeedPlayer::PlayerActionPad(void)
{
	CGame *pGame = CManager::GetGame();
	CUI *pUi = pGame->GetUI();
	CInputXPad * pXPad = CManager::GetXPad();
	CSound *pSound = CManager::GetSound();

	if (pXPad->GetTrigger(XINPUT_GAMEPAD_B, m_nControllerIndx) == true)
	{
		if (m_bRecast == false && m_nButtonCounter == 0
			&& m_PlayerState != PLAYERSTATE_DAMAGE && m_PlayerState != PLAYERSTATE_BLOWAWAY)
		{
			pSound->PlaySound(CSound::SOUND_LABEL_SE030);
			//pSound->SetVolume(CSound::SOUND_LABEL_SE030, 10.0f);
			m_fSpeed = 1.5f;		//�X�s�[�h�^�̑�����1.5�{�ɂ���
			m_nButtonCounter = 1;	//�{�^���������Ȃ��悤�ɂ���
			m_bSprint = true;		//�X�v�����g���ɂ���
			m_bSprintMotion = true;	//�X�v�����g���[�V���������ǂ���
		}
	}
}

//=============================================================================
// MOUSE����
//=============================================================================
void  CSpeedPlayer::PlayerActionMouse(void)
{
	CGame *pGame = CManager::GetGame();
	CUI *pUi = pGame->GetUI();
	CRawMouse *pRawMouse = CManager::GetRawMouse();					//RawMouse�̎擾
	CSound *pSound = CManager::GetSound();

	if (pRawMouse->GetTrigger(m_nControllerIndx,CRawMouse::RIMS_BUTTON_RIGHT) == true)
	{
		if (m_bRecast == false && m_nButtonCounter == 0
			&& m_PlayerState != PLAYERSTATE_DAMAGE && m_PlayerState != PLAYERSTATE_BLOWAWAY)
		{
			pSound->PlaySound(CSound::SOUND_LABEL_SE030);
			//pSound->SetVolume(CSound::SOUND_LABEL_SE030, 10.0f);
			m_fSpeed = 1.5f;		//�X�s�[�h�^�̑�����1.5�{�ɂ���
			m_nButtonCounter = 1;	//�{�^���������Ȃ��悤�ɂ���
			m_bSprint = true;		//�X�v�����g���ɂ���
			m_bSprintMotion = true;	//�X�v�����g���[�V���������ǂ���
		}
	}
}

//=============================================================================
// MOUSE����
//=============================================================================
void  CSpeedPlayer::PlayerActionKeyboard(void)
{
	CGame *pGame = CManager::GetGame();
	CUI *pUi = pGame->GetUI();
	CRawMouse *pRawMouse = CManager::GetRawMouse();					//RawMouse�̎擾
	CInputKeyboard * pInputKeyboard = CManager::GetInputkeyboard();	//�L�[�{�[�h�̎擾
	CSound *pSound = CManager::GetSound();

	if (pInputKeyboard->GetKeyboardPress(DIK_RETURN) == true)
	{
		if (m_bRecast == false && m_nButtonCounter == 0
			&& m_PlayerState != PLAYERSTATE_DAMAGE && m_PlayerState != PLAYERSTATE_BLOWAWAY)
		{
			pSound->PlaySound(CSound::SOUND_LABEL_SE030);
			//pSound->SetVolume(CSound::SOUND_LABEL_SE030,10.0f);

			m_fSpeed = 1.5f;		//�X�s�[�h�^�̑�����1.5�{�ɂ���
			m_nButtonCounter = 1;	//�{�^���������Ȃ��悤�ɂ���
			m_bSprint = true;		//�X�v�����g���ɂ���
			m_bSprintMotion = true;	//�X�v�����g���[�V���������ǂ���
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void  CSpeedPlayer::Draw(void)
{
	CPlayer::Draw();

	if (m_pLoadEffectSpeed != NULL)
	{
		m_pLoadEffectSpeed->Draw();
	}
}

//=============================================================================
//�I�u�W�F�N�g�̐�������
//=============================================================================
CSpeedPlayer *CSpeedPlayer::Create(D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40],int nNumPlayer, int nType,
	CHARCTERTYPE charctertype, int nControllerIndx, int nControllerType)
{
	CSpeedPlayer *pSpeedPlayer = NULL;
	if (pSpeedPlayer == NULL)
	{
		//�������𓮓I�m��
		pSpeedPlayer = new CSpeedPlayer;
		if (pSpeedPlayer != NULL)
		{//NULL�`�F�b�N
			pSpeedPlayer->SetType(nType);									//�Q�[���p���^�C�v
			pSpeedPlayer->m_nCuntry[nNumPlayer] = nType;					//���U���g�p�̍��^�C�v
			pSpeedPlayer->m_CharcterType = charctertype;					//�Q�[���p�̃L�����^�C�v
			pSpeedPlayer->m_CharcterTypeResult[nNumPlayer] = charctertype;	//���U���g�p�̃L�����^�C�v
			pSpeedPlayer->Init(pos,ModelTxt,MotionTxt,nNumPlayer,nType);			
			pSpeedPlayer->m_nControllerIndx = nControllerIndx;			//�g�p���Ă�R���g���[���[�̔ԍ�
			pSpeedPlayer->m_nControllerType = nControllerType;			//�g�p���Ă�R���g���[���[�̃^�C�v
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

//=============================================================================
// ���C���̐�������
//=============================================================================
void CSpeedPlayer::CreateOrbitLine(void)
{
	return;
	/*if (m_pOrbitLine == NULL)
	{
		m_pOrbitLine = CSceneOrbit::Create(CSceneOrbit::TYPE_PLAYER, CCharacter::GetPos());
		m_pOrbitLine->SetMtxParent(&m_pModel[10]->GetMtxWorld());
		m_pOrbitLine->SetMtxParentEnd(&m_pModel[1]->GetMtxWorld());
	}*/
}

//=============================================================================
// ���C���̔j��
//=============================================================================
void CSpeedPlayer::UninitOrtbitLine(void)
{
	/*if (m_pOrbitLine != NULL)
	{
		m_pOrbitLine->Uninit();
		m_pOrbitLine = NULL;
	}*/
}