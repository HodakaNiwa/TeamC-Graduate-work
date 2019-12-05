//=============================================================================
//
// �}�l�[�W���[���� [manager.cpp]
// Author : ���������@SATOU ASUMI : �R���֎j : ���я��� : �O�H�ۋM
//
//=============================================================================
#include "manager.h"
#include "sound.h"
#include "input.h"
#include "camera.h"
#include "light.h"
#include "scene2D.h"
#include "scene3D.h"
#include "sceneBillBorad.h"
#include "scene3D.h"
#include "LoadText.h"
#include "game.h"
#include "ranking.h"
#include "title.h"
#include "tutorial.h"
#include "Fade.h"
#include "bottun.h"
#include "result.h"
#include "select.h"
#include "modebase.h"
#include "time.h"
#include "RawMouse.h"
#include "collision.h"
#include "debuglog.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define TEXT_PLYER			("data/TEXT/TestPlayer2.txt")			//�v���C���[�̃��[�V�������e�L�X�g
#define TEXT_BIRD			("data/TEXT/bird.txt")					//���̃��[�V�������e�L�X�g
#define TEXT_RABIT			("data/TEXT/rabit.txt")					//�E�T�M�̃��[�V�������e�L�X�g
#define TEXT_BUTTERFLY		("data/TEXT/Butterfly.txt")				//���̃��[�V�������e�L�X�g
#define TEXT_PARTICLE		("data/TEXT//EFFECT/Particle.txt")		//�G�t�F�N�g�̏��e�L�X�g
#define TEXT_2DTEXTURE		("data/TEXT/2DTexture.txt")				//2D�e�N�X�`���̏��e�L�X�g
#define TEXT_STAGEGAME		("data/TEXT/STAGE/Stage.txt")			//�Q�[���X�e�[�W�̏��e�L�X�g
#define TEXT_STAGETUTORIAL	("data/TEXT/STAGE/TutorialStage.txt")	//�`���[�g���A���X�e�[�W�̏��e�L�X�g
#define RANK_1				(30)									//�����L���O�̏����X�R�A
#define RANK_2				(25)
#define RANK_3				(20)
#define RANK_4				(15)
#define RANK_5				(10)
#define RANK_6				(5)									//�����L���O�̏����X�R�A
#define RANK_7				(3)
#define RANK_8				(1)

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================
CRenderer * CManager::m_pRenderer = NULL;
CSound * CManager::m_pSound = NULL;
CInputKeyboard * CManager::m_pInputKeyboard = NULL;
CGamePad * CManager::m_pInputGamePad = NULL;
CLight * CManager::m_pLight = NULL;
CInputXPad * CManager::m_pXPad = NULL;
CInputMouse * CManager::m_pInputMouse = NULL;
CRawMouse * CManager::m_pRawMouse = NULL;
CDebugProc * CManager::m_pDebugProc = NULL;
HWND CManager::m_hWnd = NULL;

//���
CModeBase * CManager::m_pModeBase = NULL;

CFade * CManager::m_pFade = NULL;

int CManager::m_nScore[MAX_PLAYERNUM] = {};
int CManager::m_nCuntry[MAX_PLAYERNUM] = {};
int CManager::m_nCharType[MAX_PLAYERNUM] = {};
int CManager::m_nRankScore[RANK] = {};
int CManager::m_nRankCuntry[RANK] = {};
int CManager::m_nRankCharType[RANK] = {};

//���[�h
CManager::MODE CManager::m_mode = CManager::MODE_TITLE;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CManager::CManager(){}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CManager::~CManager(){}

//=============================================================================
// ����������
//=============================================================================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, bool bWindow)
{
	//�E�C���h�E�̕ۑ�
	m_hWnd = hWnd;

	//�X�R�A�̐ݒ�
	m_nRankScore[0] = RANK_1;
	m_nRankScore[1] = RANK_2;
	m_nRankScore[2] = RANK_3;
	m_nRankScore[3] = RANK_4;
	m_nRankScore[4] = RANK_5;
	m_nRankScore[5] = RANK_6;
	m_nRankScore[6] = RANK_7;
	m_nRankScore[7] = RANK_8;

	//���ԍ��̐ݒ�
	m_nRankCuntry[0] = 7;
	m_nRankCuntry[1] = 6;
	m_nRankCuntry[2] = 5;
	m_nRankCuntry[3] = 4;
	m_nRankCuntry[4] = 3;
	m_nRankCuntry[5] = 2;
	m_nRankCuntry[6] = 1;
	m_nRankCuntry[7] = 0;

	//�L�����^�C�v�̐ݒ�
	m_nRankCharType[0] = 0;
	m_nRankCharType[1] = 2;
	m_nRankCharType[2] = 2;
	m_nRankCharType[3] = 1;
	m_nRankCharType[4] = 1;
	m_nRankCharType[5] = 2;
	m_nRankCharType[6] = 0;
	m_nRankCharType[7] = 0;

	//�T�E���h����
	if (m_pSound == NULL)
	{
		m_pSound = new CSound;
		m_pSound->InitSound(hWnd);
	}

	//�����_�����O����
	if (m_pRenderer == NULL)
	{
		//�����_�����O�N���X�̐���
		m_pRenderer = new CRenderer;

		if (FAILED(m_pRenderer->Init(hWnd, bWindow))) //FALSE�őS���
		{
			return-1;
		}

		//�t�F�[�h�̐���
		if (m_pFade == NULL)
		{
			m_pFade = new CFade;
			m_pFade->Init();
		}

		SetMode(m_mode);	//��ʃ��[�h�̐ݒ�
	}

	//���C�g�̐���
	if (m_pLight == NULL)
	{
		m_pLight = new CLight;

		if (m_pLight != NULL)
		{
			m_pLight->Init();
		}
	}

	//���͏���(�L�[�{�[�h)
	if (m_pInputKeyboard == NULL)
	{
		//�C���v�b�g�N���X�̐���
		m_pInputKeyboard = new CInputKeyboard;

		if (FAILED(m_pInputKeyboard->Init(hInstance, hWnd)))
		{
			return S_OK;
		}
	}

	//���͏���(�Q�[���p�b�h(X�C���v�b�g))
	if (m_pXPad == NULL)
	{
		//�C���v�b�g�N���X�̐���
		m_pXPad = new CInputXPad;

		if (FAILED(m_pXPad->Init(hInstance, hWnd)))
		{
			return S_OK;
		}
	}

	//�f�o�b�O���O
	if (m_pDebugProc == NULL)
	{
		m_pDebugProc = new CDebugProc;
		if (m_pDebugProc != NULL)
		{
			m_pDebugProc->Init();
		}
	}
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CManager::Uninit(void)
{
	//�f�o�b�O���O�̔j��
	if (m_pDebugProc != NULL)
	{
		m_pDebugProc->Uninit();
		delete m_pDebugProc;
		m_pDebugProc = NULL;
	}

	//��ʂ̔j��
	if (m_pModeBase != NULL)
	{
		m_pModeBase->Uninit();
		delete m_pModeBase;
		m_pModeBase = NULL;
	}

	//�t�F�[�h��ʂ̔j��
	if (m_pFade != NULL)
	{
		m_pFade->Uninit();
		m_pFade = NULL;
	}

	//�S�Ẵ����������
	CScene::ReleaseAll();

	//���͏���(�L�[�{�[�h)�̔j��
	if (m_pInputKeyboard != NULL)
	{
		m_pInputKeyboard->Uninit();
		delete m_pInputKeyboard;
		m_pInputKeyboard = NULL;
	}

	//���͏���(�Q�[���p�b�h(X�C���v�b�g))�̔j��
	if (m_pXPad != NULL)
	{
		m_pXPad->Uninit();
		delete m_pXPad;
		m_pXPad = NULL;
	}

	//���͏���(���E�}�E�X)�̔j��
	if (m_pRawMouse != NULL)
	{
		m_pRawMouse->Uninit();
		delete m_pRawMouse;
		m_pRawMouse = NULL;
	}

	//���C�g�̔j��
	if (m_pLight != NULL)
	{
		m_pLight->Uninit();
		delete m_pLight;
		m_pLight = NULL;
	}

	//�����_�����O�̔j��
	if (m_pRenderer != NULL)
	{
		m_pRenderer->Uninit();
		delete m_pRenderer;
		m_pRenderer = NULL;
	}

	//�T�E���h�̔j��
	if (m_pSound != NULL)
	{
		m_pSound->UninitSound();
		delete m_pSound;
		m_pSound = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void CManager::Update(void)
{
	//���͏���(�L�[�{�[�h)
	if (m_pInputKeyboard != NULL) { m_pInputKeyboard->Update(); }

	//�Q�[���p�b�h�̍X�V(X�C���v�b�g)
	if (m_pXPad != NULL) { m_pXPad->Update(); }

	//�����_�����O�̍X�V
	if (m_pRenderer != NULL) { m_pRenderer->Update(); }

	//�Q�[����ʂ̍X�V
	if (m_pModeBase != NULL) { m_pModeBase->Update(); }

	//���C�g�̍X�V
	if(m_pLight != NULL){ m_pLight->Update(); }

	if (m_pInputKeyboard->GetKeyboardTrigger(DIK_F3) == true)
	{
		CCollider::SwicthDraw();
	}
	if (m_pInputKeyboard->GetKeyboardTrigger(DIK_F4) == true)
	{
		CDebugProc::SetDisp(CDebugProc::GetDisp() ? false : true);
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CManager::Draw(void)
{
	//�����_�����O
	if (m_pRenderer != NULL){ m_pRenderer->Draw(m_pModeBase); }
}

//=============================================================================
// ���[�h�̐ݒ�
//=============================================================================
void CManager::SetMode(MODE mode)
{
	//���̒�~
	CSound::StopSound();

	if (m_pModeBase != NULL)
	{
		m_pModeBase->Uninit();
		delete m_pModeBase;
		m_pModeBase = NULL;
	}

	//���̉�ʃ��[�h�̐ݒ�
	m_mode = mode;

	//��ʂ𐶐�����
	if (m_pModeBase == NULL)
	{
		switch (m_mode)
		{
		case MODE_TITLE:	//�^�C�g�����
			CSound::PlaySound(CSound::SOUND_LABEL_BGM000);
			m_pModeBase = new CTitle;
			break;

		case MODE_SELECT:	//�Z���N�g���
			CSound::PlaySound(CSound::SOUND_LABEL_BGM006);
			m_pModeBase = new CSelect;
			break;

		case MODE_TUTORIAL:	//�`���[�g���A�����
			m_pModeBase = new CTutorial;
			break;

		case MODE_GAME:		//�Q�[�����
			CSound::PlaySound(CSound::SOUND_LABEL_BGM001);
			m_pModeBase = new CGame;
			break;

		case MODE_RESULT:	//���U���g���
			m_pModeBase = new CResult;
			break;

		case MODE_RANKING:	//�����L���O���
			CSound::PlaySound(CSound::SOUND_LABEL_BGM004);
			m_pModeBase = new CRanking;
			break;
		}

		if (m_pModeBase != NULL) { m_pModeBase->Init(); }
	}
}

//=============================================================================
// �����_�����O�̎擾
//=============================================================================
CRenderer * CManager::GetRenderer(void)
{
	return m_pRenderer;
}

//=============================================================================
// ���͏����̎擾(�L�[�{�[�h)
//=============================================================================
CInputKeyboard * CManager::GetInputkeyboard(void)
{
	return m_pInputKeyboard;
}

//=============================================================================
// �Q�b�g�T�E���h
//=============================================================================
CSound * CManager::GetSound(void)
{
	return m_pSound;
}

//=============================================================================
// ���E�}�E�X�̐���
//=============================================================================
void CManager::CreateRawMouse(HWND hWnd)
{
	m_pRawMouse = CRawMouse::Create(hWnd);
}
