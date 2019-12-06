//=============================================================================
//
// �n���^�̏��� [teqolayer.cpp]
// Author : �R���֎j
//
//=============================================================================
#include "player.h"
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
#define RECOVERY_MINE_POINT (2)		//�}�C���g�p�|�C���g�̉񕜗�
#define RECOVERY_TIME (1)			//�g�p�|�C���g�񕜂܂ł̎���

//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CTechniquePlayer::CTechniquePlayer()
{
	m_nRecastCounter = 0;
	m_nRecastTimer = 0;
	m_nButtonCounter = 0;
	m_bRecast = false;
	m_nMinePoint = 0;
	m_nReCounter = 0;			//�񕜃J�E���^�[
	m_nReTimer = 0;				//�񕜃^�C�}�[
	m_nInstallationCounter = 0;
	m_nInstallationTimer = 0;
}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CTechniquePlayer::~CTechniquePlayer()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CTechniquePlayer::Init(D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40], int nNumPlayer, int nType)
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
	m_bCollision = false;
	m_nControllerIndx = 0;
	m_nControllerType = 0;
	m_nReCounter = 0;
	m_nReTimer = 0;
	m_nMinePoint = MAX_MINE_POINT;
	m_nInstallationCounter = 0;
	m_nInstallationTimer = 0;
	m_bMineUse = false;
	m_nDamageCounter = 0;
	m_fSpeed = 1.0f;
	m_bSuperArmor = false;
	m_fInertia = GetInertia();			//����
	m_bInit = false;
	m_nInitCnt = 0;
	return S_OK;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CTechniquePlayer::Init(void)
{
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void  CTechniquePlayer::Uninit(void)
{
	//�v���C���[�̔j��
	CPlayer::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void  CTechniquePlayer::Update(void)
{
	//�Q�[���̏�Ԃ��擾
	int nGameState = CGame::GetGameState();

	if (nGameState != CGame::GAMESTATE_FIRSTCOUNTDOWN && nGameState != CGame::GAMESTATE_END)
	{
		//�A�N�V�����̍X�V
		ActionUpdate();
	}

	//�v���C���[�̍X�V
	CPlayer::Update();
}

//=============================================================================
// �A�N�V�����̍X�V����
//=============================================================================
void  CTechniquePlayer::ActionUpdate(void)
{
	//Xinput�̎擾
	CInputXPad * pXPad = CManager::GetXPad();
	CRawMouse *pRawMouse = CManager::GetRawMouse();					//RawMouse�̎擾
	CGame *pGame = CManager::GetGame();
	CCharacter *pCharacter = pGame->GetChara(m_nNumPlayer);
	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVec3TransformCoord(&pos, &m_pModel[5]->GetPos(), &m_pModel[5]->GetMtxWorld());

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

	//0�ȉ��ɂȂ�܂Ń}�C�����g�p�ł���
	if (m_nMinePoint <= MAX_MINE_POINT)
	{
		if (m_nButtonCounter == 1)
		{
			//�n���̎g�p�|�C���g�����炷
			m_nMinePoint -= REDUCED_MINE_POINT;
			m_nButtonCounter = 0;
			if (m_nMinePoint <= 0)
			{
				m_nMinePoint = 0;	//0�ɂ���
			}
		}
	}

	if (m_bMineUse == true)
	{//�}�C����ݒu��
		m_bSuperArmor = true;			//������΂Ȃ��Ȃ�
		m_nInstallationCounter++;

		if (m_nInstallationCounter % 30 == 0)
		{
			m_nInstallationTimer++;
			if (m_nInstallationTimer == 1)
			{
				if (m_PlayerState != PLAYERSTATE_DAMAGE && m_PlayerState != PLAYERSTATE_BLOWAWAY)
				{
					//�n����ݒu
					CMine::Create(pos, pCharacter);
				}
				m_bMineUse = false;	//�}�C���ݒu�I��
				m_bInit = true;		//�l�̏�����
			}
		}
	}

	//�l�̏�����
	if (m_bInit == true)
	{
		m_nInitCnt++;
		if (m_nInitCnt >= 18)
		{
			m_nInstallationTimer = 0;
			m_nInitCnt = 0;
			m_bSuperArmor = false;
			m_bWalk = true;
			m_bInit = false;
			if (m_PlayerState != PLAYERSTATE_DAMAGE)
			{
				m_PlayerState = PLAYERSTATE_NONE;
			}
		}
	}

	//�񕜃J�E���^�[
	m_nReCounter++;
	if (m_nReCounter % 60 == 0)
	{
		//�񕜃^�C�}�[
		m_nReTimer++;
		if (m_nReTimer == RECOVERY_TIME)
		{
			//1�b���Ƃ�2�񕜂���
			m_nMinePoint += RECOVERY_MINE_POINT;
			m_nReTimer = 0;

			if (m_nMinePoint >= MAX_MINE_POINT)
			{
				//100�𒴂�����
				m_nMinePoint = MAX_MINE_POINT;
			}
		}
	}
}

//=============================================================================
// �R���g���[���[�̏���
//=============================================================================
void  CTechniquePlayer::PlayerActionPad(void)
{
	CGame *pGame = CManager::GetGame();
	CUI *pUi = pGame->GetUI();
	CInputXPad * pXPad = CManager::GetXPad();
	CCharacter *pCharacter = pGame->GetChara(m_nNumPlayer);
	//�T�E���h�̎擾
	CSound *pSound = CManager::GetSound();
	if (pXPad->GetTrigger(XINPUT_GAMEPAD_B, m_nControllerIndx) == true)
	{
		m_nMinePoint = 100;
	}
	if (pXPad->GetTrigger(XINPUT_GAMEPAD_B, m_nControllerIndx) == true)
	{
		if (m_PlayerState != PLAYERSTATE_DAMAGE && m_PlayerState != PLAYERSTATE_ACTION
			&& m_PlayerState != PLAYERSTATE_BLOWAWAY)
		{
			//�}�C���|�C���g��20�ȏ�Ȃ�ݒu�\
			if (m_nMinePoint >= REDUCED_MINE_POINT)
			{
				m_PlayerState = PLAYERSTATE_ACTION;
				m_pMotion->SetNumMotion(2);
				m_nButtonCounter = 1;	//
				pUi->GetSkilicon(m_nNumPlayer)->RevivalIconMask();	//�X�L���A�C�R��
				m_bMineUse = true;
				pSound->PlaySound(CSound::SOUND_LABEL_SE024);
				//pSound->SetVolume(CSound::SOUND_LABEL_SE024, 30.0f);
			}
		}
	}
}

//=============================================================================
// �}�E�X����̏���
//=============================================================================
void  CTechniquePlayer::PlayerActionMouse(void)
{
	CGame *pGame = CManager::GetGame();
	CUI *pUi = pGame->GetUI();
	CRawMouse *pRawMouse = CManager::GetRawMouse();					//RawMouse�̎擾
	CInputKeyboard * pInputKeyboard = CManager::GetInputkeyboard();	//�L�[�{�[�h�̎擾
																	//�T�E���h�̎擾
	CSound *pSound = CManager::GetSound();

	if (pRawMouse->GetTrigger(m_nControllerIndx, CRawMouse::RIMS_BUTTON_RIGHT) == true)
	{
		if (m_PlayerState != PLAYERSTATE_DAMAGE && m_PlayerState != PLAYERSTATE_ACTION
			&& m_PlayerState != PLAYERSTATE_BLOWAWAY)
		{
			//�}�C���|�C���g��20�ȏ�Ȃ�ݒu�\
			if (m_nMinePoint >= REDUCED_MINE_POINT)
			{
				m_PlayerState = PLAYERSTATE_ACTION;

				m_pMotion->SetNumMotion(2);
				m_nButtonCounter = 1;	//
				pUi->GetSkilicon(m_nNumPlayer)->RevivalIconMask();	//�X�L���A�C�R��
				m_bMineUse = true;
				pSound->PlaySound(CSound::SOUND_LABEL_SE024);
				//pSound->SetVolume(CSound::SOUND_LABEL_SE024, 30.0f);
			}
		}
	}
}

//=============================================================================
// �L�[�{�[�h����̏���
//=============================================================================
void  CTechniquePlayer::PlayerActionKeyboard(void)
{
	CGame *pGame = CManager::GetGame();
	CUI *pUi = pGame->GetUI();
	CInputKeyboard * pInputKeyboard = CManager::GetInputkeyboard();	//�L�[�{�[�h�̎擾
	CSound *pSound = CManager::GetSound();							//�T�E���h�̎擾
	if (pInputKeyboard->GetKeyboardPress(DIK_RETURN) == true)
	{
		if (m_PlayerState != PLAYERSTATE_DAMAGE && m_PlayerState != PLAYERSTATE_ACTION
			&& m_PlayerState != PLAYERSTATE_BLOWAWAY)
		{
			//�}�C���|�C���g��20�ȏ�Ȃ�ݒu�\
			if (m_nMinePoint >= REDUCED_MINE_POINT)
			{
				m_PlayerState = PLAYERSTATE_ACTION;

				m_pMotion->SetNumMotion(2);
				m_nButtonCounter = 1;	//
				pUi->GetSkilicon(m_nControllerIndx)->RevivalIconMask();	//�X�L���A�C�R��
				m_bMineUse = true;
				pSound->PlaySound(CSound::SOUND_LABEL_SE024);
				pSound->SetVolume(CSound::SOUND_LABEL_SE024, 20.0f);
			}
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void  CTechniquePlayer::Draw(void)
{
	//�v���C���[�̕`��
	CPlayer::Draw();
}

//=============================================================================
//�I�u�W�F�N�g�̐�������
//=============================================================================
CTechniquePlayer *CTechniquePlayer::Create(D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40], int nNumPlayer, int nType,
	CHARCTERTYPE charctertype, int nControllerIndx, int nControllerType)
{
	CTechniquePlayer *pTeqPlayer = NULL;
	if (pTeqPlayer == NULL)
	{
		//�������𓮓I�m��
		pTeqPlayer = new CTechniquePlayer;
		if (pTeqPlayer != NULL)
		{//NULL�`�F�b�N
			pTeqPlayer->SetType(nType);									//�Q�[���p���^�C�v
			pTeqPlayer->m_nCuntry[nNumPlayer] = nType;					//���U���g�p�̍��^�C�v
			pTeqPlayer->m_CharcterType = charctertype;					//�Q�[���p�̃L�����^�C�v
			pTeqPlayer->m_CharcterTypeResult[nNumPlayer] = charctertype;	//���U���g�p�̃L�����^�C�v
			pTeqPlayer->Init(pos, ModelTxt, MotionTxt, nNumPlayer, nType);		//�v���C���[�̏�����
			pTeqPlayer->m_nControllerIndx = nControllerIndx;			//�g�p���Ă�R���g���[���[�̔ԍ�
			pTeqPlayer->m_nControllerType = nControllerType;			//�g�p���Ă�R���g���[���[�̃^�C�v

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
//
//=============================================================================
void  CTechniquePlayer::Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
{

}


