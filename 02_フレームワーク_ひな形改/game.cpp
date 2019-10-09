//=============================================================================
//
// �Q�[���̏��� [game.cpp]
// Author : ��������
//
//=============================================================================
#include "game.h"
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "scene2D.h"
#include "input.h"
#include "fade.h"
#include "pause.h"
#include "sound.h"
#include "fade.h"
#include "time.h"
#include "loadText.h"
#include "time.h"
#include "camera.h"
#include "sceneMeshfiled.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define LOAD_UI		("data\\TEXT\\UI\\Game.txt")
#define VECTOR_ZERO	(D3DXVECTOR3(0.0f, 0.0f, 0.0f))

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
CPause *CGame::m_pPause = NULL;
bool CGame::m_bPause = false;
CGame::GAMESTATE CGame::m_gameState = GAMESTATE_NONE;
CSceneMeshFiled * CGame::m_pMeshFiled = NULL;

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CGame::CGame()
{
	m_gameState = GAMESTATE_NONE;
	m_nCounterGameState = 0;
	m_NowGameState = GAMESTATE_NONE;
	m_pGameCamera = NULL;
}
//=============================================================================
// �f�X�g���N�^
//=============================================================================
CGame::~CGame(){}

//=============================================================================
// ����������
//=============================================================================
void CGame::Init(void)
{
	//�x�[�X�̏�����
	CModeBase::Init();

	m_bPause = false;					//�|�[�Y��������

	//�n�ʂ̐���
	if (m_pMeshFiled == NULL) { m_pMeshFiled = CSceneMeshFiled::Create(VECTOR_ZERO); }
	
	//�J�����̐���
	if (m_pGameCamera == NULL)
	{
		m_pGameCamera = new CGameCamera;
		m_pGameCamera->Init();
	}

	//UI�̓ǂݍ��ݏ���
	LoadUI(LOAD_UI);
}

//=============================================================================
// �I������
//=============================================================================
void CGame::Uninit(void)
{
	//-----------------
	//		Uninit
	//-----------------
	//�|�[�Y�폜
	if (m_pPause != NULL)
	{
		m_pPause->Uninit();
		m_pPause = NULL;
	}

	//�J�����̔j��
	if (m_pGameCamera != NULL)
	{
		m_pGameCamera->Uninit();
		delete m_pGameCamera;
		m_pGameCamera = NULL;
	}

	//�ϐ��̏�����
	m_pMeshFiled = NULL;

	//�x�[�X�̔j��
	CModeBase::Uninit();

	//�t�F�[�h�ȊO�폜
	CScene::ReleaseFade();
}
//=============================================================================
// �X�V����
//=============================================================================
void CGame::Update(void)
{
	//���͏��
	CInputKeyboard *pCInputKeyBoard = CManager::GetInputkeyboard();

	//�T�E���h�̏��
	CSound *pSound = CManager::GetSound();

	if (m_pPause == false)
	{//�J����
		//���݂̏�Ԃ�ۑ�
		m_NowGameState = GetGameState();

		switch (m_NowGameState)
		{
		case GAMESTATE_CLEAR:	//�Q�[�����N���A�������
			m_nCounterGameState++;

			if (m_nCounterGameState >= 60)
			{//��ʁi���[�h�j�̐ݒ�
				if (CFade::GetFadeMode() != CFade::FADE_OUT)
				{
					CFade::SetFade(CManager::MODE_RESULT);
				}
			}
			break;
		case GAMESTATE_OVER:	//�Q�[���ŕ������Ƃ��̏��
			m_nCounterGameState++;

			if (m_nCounterGameState >= 60)
			{//��ʁi���[�h�j�̐ݒ�
				CFade::SetFade(CManager::MODE_RESULT);
			}
			break;
		}
	}

	//�|�[�Y�̏���
	if (pCInputKeyBoard->GetKeyboardTrigger(DIK_P) == true)
	{//P�L�[�������ꂽ��
		int nType = 0;
		m_bPause = m_bPause ? false : true;

		switch (m_bPause)
		{
		case true:
			if (m_pPause == NULL)
			{
				//�|�[�Y�̐���
				m_pPause = CPause::Create();
			}
			break;
		case false:
			if (m_pPause != NULL)
			{
				//�|�[�Y���폜
				m_pPause->Uninit();
				m_pPause = NULL;
			}
		}
	}

	//�f�o�b�N�L�[
	if (pCInputKeyBoard->GetKeyboardTrigger(DIK_1) == true) { CFade::SetFade(CManager::MODE_RESULT); }

	//�J�����̍X�V����
	if (m_pGameCamera != NULL) { m_pGameCamera->Update(); }
}

//=============================================================================
// �`�揈��
//=============================================================================
void CGame::Draw(void)
{
	//�J�����̐ݒ�
	if (m_pGameCamera != NULL) { m_pGameCamera->Set(); }

	//���ׂĕa�炷��
	CScene::DrawAll();
}

//=============================================================================
// �|�[�Y�����������ǂ���
//=============================================================================
void CGame::SetPause(bool bPause)
{
	m_bPause = bPause;

	switch (m_bPause)
	{
	case true:
		if (m_pPause == NULL)
		{
			//�|�[�Y�̐���
			m_pPause = CPause::Create();
		}
		break;
	case false:
		if (m_pPause != NULL)
		{
			m_pPause->Uninit();
			m_pPause = NULL;
		}
	}
}