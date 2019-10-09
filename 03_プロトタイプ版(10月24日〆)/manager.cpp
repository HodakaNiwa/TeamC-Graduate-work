//=============================================================================
//
// �}�l�[�W���[���� [manager.cpp]
// Author : ���������@SATOU ASUMI
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
#include "gamepad.h"
#include "result.h"
#include "select.h"

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
#define RANK_1				(2000)									//�����L���O�̏����X�R�A
#define RANK_2				(1500)
#define RANK_3				(1000)
#define RANK_4				(800)
#define RANK_5				(500)

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================
CRenderer * CManager::m_pRenderer = NULL;
CSound * CManager::m_pSound = NULL;
CInputKeyboard * CManager::m_pInputKeyboard = NULL;
CGamePad * CManager::m_pInputGamePad = NULL;
CLight * CManager::m_pLight = NULL;

//���
CTitle * CManager::m_pTitle = NULL;
CSelect * CManager::m_pSelect = NULL;
CTutorial * CManager::m_pTutorial = NULL;
CGame * CManager::m_pGame = NULL;
CResult * CManager::m_pResult = NULL;
CRanking * CManager::m_pRanking = NULL;

CFade * CManager::m_pFade = NULL;

int CManager::m_nScore = 0;
int CManager::m_nRankScore[RANK] = {};
int CManager::m_nMaxConbo = 0;
int CManager::m_nDethButterfly = 0;

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
	//�X�R�A�̐ݒ�
	m_nRankScore[0] = RANK_1;
	m_nRankScore[1] = RANK_2;
	m_nRankScore[2] = RANK_3;
	m_nRankScore[3] = RANK_4;
	m_nRankScore[4] = RANK_5;

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

		if (FAILED(m_pRenderer->Init(hWnd, TRUE))) //FALSE�őS���
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

	//���͏���(�Q�[���p�b�h)
	if (m_pInputGamePad == NULL)
	{
		//�C���v�b�g�N���X�̐���
		m_pInputGamePad = new CGamePad;

		if (FAILED(m_pInputGamePad->Init(hInstance, hWnd)))
		{
			return S_OK;
		}
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CManager::Uninit(void)
{
	//�e��ʂ̔j��
	if (m_pTitle != NULL)		//�^�C�g�����
	{
		m_pTitle->Uninit();
		delete m_pTitle;
		m_pTitle = NULL;
	}
	if (m_pSelect != NULL)		//�Z���N�g���
	{
		m_pSelect->Uninit();
		delete m_pSelect;
		m_pSelect = NULL;
	}
	if (m_pTutorial != NULL)	//�`���[�g���A�����
	{
		m_pTutorial->Uninit();
		delete m_pTutorial;
		m_pTutorial = NULL;
	}
	if (m_pGame != NULL)		//�Q�[�����
	{
		m_pGame->Uninit();
		delete m_pGame;
		m_pGame = NULL;
	}
	if (m_pResult != NULL)		//���U���g���
	{
		m_pResult->Uninit();
		delete m_pResult;
		m_pResult = NULL;
	}
	if (m_pRanking != NULL)		//�����L���O���
	{
		m_pRanking->Uninit();
		delete m_pRanking;
		m_pRanking = NULL;
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

	//���͏���(�Q�[���p�b�h)�̔j��
	if (m_pInputGamePad != NULL)
	{
		m_pInputGamePad->Uninit();
		delete m_pInputGamePad;
		m_pInputGamePad = NULL;
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

	//�Q�[���p�b�h�̍X�V
	if (m_pInputGamePad != NULL) { m_pInputGamePad->Update(); };

	switch (m_mode)
	{
	case MODE_TITLE:	
		if (m_pTitle != NULL) { m_pTitle->Update(); }			//�^�C�g�����
		break;

	case MODE_SELECT:
		if (m_pSelect != NULL) { m_pSelect->Update(); }			//�Z���N�g���
		break;

	case MODE_TUTORIAL:
		if (m_pTutorial != NULL) { m_pTutorial->Update(); }		//�`���[�g���A�����
		break;

	case MODE_GAME:		
		if (m_pGame != NULL) { m_pGame->Update(); }				//�Q�[�����
		break;

	case MODE_RESULT:											//���U���g���
		if (m_pResult != NULL) { m_pResult->Update(); }
		break;

	case MODE_RANKING:	
		if (m_pRanking != NULL) { m_pRanking->Update(); }		//�����L���O���
		break;
	}
	
	if (m_pRenderer != NULL) { m_pRenderer->Update(); }		//�����_�����O
	if(m_pLight != NULL){ m_pLight->Update(); }				//���C�g
}

//=============================================================================
// �`�揈��
//=============================================================================
void CManager::Draw(void)
{
	switch (m_mode)
	{
	case MODE_TITLE:	
		if (m_pTitle != NULL) { m_pTitle->Draw(); }			//�^�C�g�����
		break;

	case MODE_SELECT:
		if (m_pSelect != NULL) { m_pSelect->Draw(); }		//�Z���N�g���

	case MODE_TUTORIAL:
		if (m_pTutorial != NULL) { m_pTutorial->Draw(); }	//�`���[�g���A�����
		break;

	case MODE_GAME:		
		if (m_pGame != NULL) { m_pGame->Draw(); }			//�Q�[�����
		break;

	case MODE_RESULT:
		if (m_pResult != NULL) { m_pResult->Draw(); }		//���U���g���
		break;

	case MODE_RANKING:	
		if (m_pRanking != NULL) { m_pRanking->Draw(); }		//�����L���O���
		break;
	}

	//�����_�����O
	if (m_pRenderer != NULL){ m_pRenderer->Draw(); }
}

//=============================================================================
// ���[�h�̐ݒ�
//=============================================================================
void CManager::SetMode(MODE mode)
{
	//��ʂ�����
	switch (m_mode)
	{
	case MODE_TITLE:	//�^�C�g�����
		if (m_pTitle != NULL)
		{
			CSound::StopSound(CSound::SOUND_LABEL_BGM000);
			m_pTitle->Uninit();
			delete m_pTitle;
			m_pTitle = NULL;
		}
		break;

	case MODE_SELECT:	//�Z���N�g���
		if (m_pSelect != NULL)
		{
			CSound::StopSound(CSound::SOUND_LABEL_BGM005);
			m_pSelect->Uninit();
			delete m_pSelect;
			m_pSelect = NULL;
		}
		break;

	case MODE_TUTORIAL:	//�`���[�g���A�����
		if (m_pTutorial != NULL)
		{
			CSound::StopSound(CSound::SOUND_LABEL_BGM005);
			m_pTutorial->Uninit();
			delete m_pTutorial;
			m_pTutorial = NULL;
		}
		break;

	case MODE_GAME:		//�Q�[�����
		if (m_pGame != NULL)
		{
			m_pGame->Uninit();
			delete m_pGame;
			m_pGame = NULL;
		}
		break;

	case MODE_RESULT:
		if (m_pResult != NULL)		//���U���g���
		{
			m_pResult->Uninit();
			delete m_pResult;
			m_pResult = NULL;
		}
		break;

	case MODE_RANKING:	//�����L���O���
		if (m_pRanking != NULL)
		{
			CSound::StopSound(CSound::SOUND_LABEL_BGM004);
			m_pRanking->Uninit();
			delete m_pRanking;
			m_pRanking = NULL;
		}
		break;
	}

	//���̉�ʃ��[�h�̐ݒ�
	m_mode = mode;

	//��ʂ𐶐�����
	switch (m_mode)
	{
	case MODE_TITLE:	//�^�C�g�����
		if (m_pTitle == NULL)
		{
			CSound::PlaySound(CSound::SOUND_LABEL_BGM000);
			m_pTitle = new CTitle;
			m_pTitle->Init();
		}
		break;

	case MODE_SELECT:
		if (m_pSelect == NULL)
		{
			CSound::PlaySound(CSound::SOUND_LABEL_BGM005);
			m_pSelect = new CSelect;
			m_pSelect->Init();
		}
		break;

	case MODE_TUTORIAL:	//�`���[�g���A�����
		if (m_pTutorial == NULL)
		{
			CSound::PlaySound(CSound::SOUND_LABEL_BGM005);
			m_pTutorial = new CTutorial;
			m_pTutorial->Init();
		}
		break;

	case MODE_GAME:		//�Q�[�����
		if (m_pGame == NULL)
		{
			CSound::PlaySound(CSound::SOUND_LABEL_BGM001);
			m_pGame = new CGame;
			m_pGame->Init();
		}
		break;

	case MODE_RESULT:	//���U���g���
		if (m_pResult == NULL)
		{
			CSound::PlaySound(CSound::SOUND_LABEL_BGM001);
			m_pResult = new CResult;
			m_pResult->Init();
		}
		break;

	case MODE_RANKING:	//�����L���O���
		if (m_pRanking == NULL)
		{
			CSound::PlaySound(CSound::SOUND_LABEL_BGM004);
			m_pRanking = new CRanking;
			m_pRanking->Init();
		}
		break;
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