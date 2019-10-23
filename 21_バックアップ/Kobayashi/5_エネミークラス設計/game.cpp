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
#include "loadText.h"
#include "player.h"
#include "enemy.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define LOAD_UI		("data\\TEXT\\UI\\Game.txt")
#define VECTOR_ZERO	(D3DXVECTOR3(0.0f, 0.0f, 0.0f))
#define LOAD_OBJECT		("data/TEXT/Stage.txt")
#define LOAD_TERRITORY	("data/TEXT/Territory.txt")

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
CPause *CGame::m_pPause = NULL;
bool CGame::m_bPause = false;
CGame::GAMESTATE CGame::m_gameState = GAMESTATE_NONE;
CSceneMeshFiled * CGame::m_pMeshFiled = NULL;
//CPlayer *CGame::m_m_pPlayer[MAX_NUMPLAYER] = {};
//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CGame::CGame()
{
	m_gameState = GAMESTATE_NONE;
	m_nCounterGameState = 0;
	m_NowGameState = GAMESTATE_NONE;
	m_pLoadObject = NULL;
	m_pLoadTerritory = NULL;

	for (int nCntPlayer = 0; nCntPlayer < MAX_NUMPLAYER; nCntPlayer++)
	{
		m_pPlayer[nCntPlayer] = NULL;
		m_pGameCamera[nCntPlayer] = NULL;
	}
	for (int nCnt = 0; nCnt < ENEMY_MAX; nCnt++)
	{
		m_pEnemy[nCnt] = NULL;
	}
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

	//�ϐ��̏�����
	m_bPause = false;					//�|�[�Y��������
	m_pLoadObject = NULL;
	m_pLoadTerritory = NULL;

	//�n�ʂ̐���
	if (m_pMeshFiled == NULL) { m_pMeshFiled = CSceneMeshFiled::Create(VECTOR_ZERO); }
	
	//�v���C���[�̐���
	m_pPlayer[0] = CPlayer::Create(D3DXVECTOR3(-50.0f, 0.0f, 0.0f), CPlayer::NUM_PLAYER_01, "data\\TEXT\\PLAYER_LODE.txt", "data\\TEXT\\PLAYER_MOTION.txt");
	m_pPlayer[1] = CPlayer::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), CPlayer::NUM_PLAYER_02, "data\\TEXT\\PLAYER_LODE.txt", "data\\TEXT\\PLAYER_MOTION.txt");
	m_pPlayer[2] = CPlayer::Create(D3DXVECTOR3(50.0f, 0.0f, 0.0f), CPlayer::NUM_PLAYER_03, "data\\TEXT\\PLAYER_LODE.txt", "data\\TEXT\\PLAYER_MOTION.txt");
	m_pPlayer[3] = CPlayer::Create(D3DXVECTOR3(100.0f, 20.0f, 0.0f), CPlayer::NUM_PLAYER_04, "data\\TEXT\\PLAYER_LODE.txt", "data\\TEXT\\PLAYER_MOTION.txt");

	//	---<<���т��ǉ����܂���>>��	��	��	��	��	��	��	��	��	��
	m_pEnemy[0] = CEnemy::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f),"data\\TEXT\\ENEMY_MODEL.txt", "data\\TEXT\\ENEMY_MOTION.txt");
	//	��	��	��	��	��	��	��	��	��	��	��	��	��	��	��	��


	//�J�����̐���(1P:����)
	if (m_pGameCamera[0] == NULL)
	{
		D3DVIEWPORT9 viewport;
		viewport.X = 0;							//�r���[�|�[�g�̍���X���W
		viewport.Y = 0;							//�r���[�|�[�g�̍���Y���W
		viewport.Width = SCREEN_WIDTH / 2;		//��
		viewport.Height = SCREEN_HEIGHT / 2;	//����
		viewport.MaxZ = 1.0f;
		viewport.MinZ = 0.0f;
		m_pGameCamera[0] = new CGameCamera;
		m_pGameCamera[0]->Init(m_pPlayer[0]);
		m_pGameCamera[0]->SetViewport(viewport);
	}

	////�J�����̐���(2P:�E��)
	if (m_pGameCamera[1] == NULL)
	{
		D3DVIEWPORT9 viewport;
		viewport.X = SCREEN_WIDTH / 2;			//�r���[�|�[�g�̍���X���W
		viewport.Y = 0;							//�r���[�|�[�g�̍���Y���W
		viewport.Width = SCREEN_WIDTH / 2;		//��
		viewport.Height = SCREEN_HEIGHT / 2;	//����
		viewport.MaxZ = 1.0f;
		viewport.MinZ = 0.0f;
		m_pGameCamera[1] = new CGameCamera;
		m_pGameCamera[1]->Init(m_pPlayer[1]);
		m_pGameCamera[1]->SetViewport(viewport);
	}

	//�J�����̐���(3P:����)
	if (m_pGameCamera[2] == NULL)
	{
		D3DVIEWPORT9 viewport;
		viewport.X = 0;						//�r���[�|�[�g�̍���X���W
		viewport.Y = SCREEN_HEIGHT / 2;		//�r���[�|�[�g�̍���Y���W
		viewport.Width = SCREEN_WIDTH / 2;	//��
		viewport.Height = SCREEN_HEIGHT / 2;//����
		viewport.MaxZ = 1.0f;
		viewport.MinZ = 0.0f;

		m_pGameCamera[2] = new CGameCamera;
		m_pGameCamera[2]->Init(m_pPlayer[2]);
		m_pGameCamera[2]->SetViewport(viewport);
	}

	//�J�����̐���(4P:�E��)
	if (m_pGameCamera[3] == NULL)
	{
		D3DVIEWPORT9 viewport;
		viewport.X = SCREEN_WIDTH / 2;		//�r���[�|�[�g�̍���X���W
		viewport.Y = SCREEN_HEIGHT / 2;		//�r���[�|�[�g�̍���Y���W
		viewport.Width = SCREEN_WIDTH / 2;	//��
		viewport.Height = SCREEN_HEIGHT / 2;//����
		viewport.MaxZ = 1.0f;
		viewport.MinZ = 0.0f;

		m_pGameCamera[3] = new CGameCamera;
		m_pGameCamera[3]->Init(m_pPlayer[3]);
		m_pGameCamera[3]->SetViewport(viewport);
	}

	//�ǂݍ��ݏ���
	if (m_pLoadObject == NULL) { m_pLoadObject = CLoadTextObject::Create(LOAD_OBJECT); }				//�I�u�W�F�N�g
	if (m_pLoadTerritory == NULL) { m_pLoadTerritory = CLoadTextTerritory::Create(LOAD_TERRITORY); }	//�e���g���[

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
	for (int nCnt = 0; nCnt < MAX_NUMPLAYER; nCnt++)
	{
		if (m_pGameCamera[nCnt] != NULL)
		{
			m_pGameCamera[nCnt]->Uninit();
			delete m_pGameCamera[nCnt];
			m_pGameCamera[nCnt] = NULL;
		}
	}


	//�ϐ��̏�����
	m_pMeshFiled = NULL;

	//�I�u�W�F�N�g�̔j��
	if (m_pLoadObject != NULL)
	{
		m_pLoadObject->Uninit();
		delete m_pLoadObject;
		m_pLoadObject = NULL;
	}

	//�e���g���[�̔j��
	if (m_pLoadTerritory != NULL)
	{
		m_pLoadTerritory->Uninit();
		delete m_pLoadTerritory;
		m_pLoadTerritory = NULL;
	}

	//�x�[�X�̔j��
	CModeBase::Uninit();
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

	//�Q�[���J�����̍X�V����
	for (int nCnt = 0; nCnt < MAX_NUMPLAYER; nCnt++)
	{
		if (m_pGameCamera[nCnt] != NULL)
		{
			m_pGameCamera[nCnt]->Update();
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CGame::Draw(void)
{
	//�J����4�����̏���
	//�J�����̐ݒ�
	if (m_pGameCamera[0] != NULL) { m_pGameCamera[0]->Set(); }
	CManager::GetRenderer()->GetDevice()->Clear(0,
		NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL),
		D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);
	//���ׂĕ`�悷��(����)
	CScene::DrawAll();

	//�J�����̐ݒ�
	if (m_pGameCamera[1] != NULL) { m_pGameCamera[1]->Set(); }
	//���ׂĕ`�悷��i�E��j
	CManager::GetRenderer()->GetDevice()->Clear(0,
		NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL),
		D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);
	CScene::DrawAll();

	//�J�����̐ݒ�
	if (m_pGameCamera[2] != NULL) { m_pGameCamera[2]->Set(); }
	CManager::GetRenderer()->GetDevice()->Clear(0,
		NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL),
		D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);
	//���ׂĕ`�悷��i�����j
	CScene::DrawAll();

	//�J�����̐ݒ�
	if (m_pGameCamera[3] != NULL) { m_pGameCamera[3]->Set(); }
	CManager::GetRenderer()->GetDevice()->Clear(0,
		NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL),
		D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);
	//���ׂĕ`�悷��i�E���j
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