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
#include "timer.h"
#include "camera.h"
#include "sceneMeshfiled.h"
#include "loadText.h"
#include "player.h"
#include "minimap.h"
#include "number2D.h"
#include "score.h"
#include "timerlogo.h"
#include "enemy.h"
#include "particle.h"
#include "effecttool.h"
#include "loadEffect.h"
#include "inputmouce.h"
#include "select.h"
#include "fieldmanager.h"
#include "Territory.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define LOAD_UI		("data\\TEXT\\UI\\Game.txt")
#define VECTOR_ZERO	(D3DXVECTOR3(0.0f, 0.0f, 0.0f))
#define LOAD_OBJECT		("data/TEXT/Stage.txt")
#define LOAD_TERRITORY	("data/TEXT/Territory.txt")
#define LOAD_EFFECT	("data/TEXT/EFFECT/Particle.txt")
#define PLAYER_MAX	(1)
//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
CPause *CGame::m_pPause = NULL;
bool CGame::m_bPause = false;
bool CGame::m_bDebugCamera = false;
CGame::GAMESTATE CGame::m_gameState = GAMESTATE_NONE;
CSceneMeshFiled * CGame::m_pMeshFiled = NULL;
CLoadTextEffect * CGame::m_pLoadEffect = NULL;

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
	m_pUI = NULL;

	m_nNumPlay = CSelect::GetEntryPlayer();
	for (int nCnt = 0; nCnt < m_nNumPlay; nCnt++)
	{
		m_nPlayerType[nCnt] = 0;	//�v���C���[�̃^�C�v
		m_nCountry[nCnt] = 0;		//��
	}
	
	for (int nCntPlayer = 0; nCntPlayer < m_nNumPlay; nCntPlayer++)
	{
		m_pPlayer[nCntPlayer] = NULL;
		m_pSpeedPlayer[nCntPlayer] = NULL;
		m_pGameCamera[nCntPlayer] = NULL;
	}
	//	---<<���т��ǉ����܂���>>��	��	��	��	��	��	��	��	��	��
	m_nEnemyNum = 1;//��ł�
	//m_nEnemyNum = 8 - m_nNumPlay // �ő�Q��������v���C���[�������������G�l�~�[�̐�
	//	��	��	��	��	��	��	��	��	��	��	��	��	��	��	��	��

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
	m_pDebugCamera = NULL;				//�f�o�b�N�J����
	m_pFieldManager = NULL;

	//�e�N�X�`���̓ǂݍ���
	CNumber2D::Load();
	CTimerLogo::Load();
	//��Ղ̓ǂݍ���
	CSceneOrbit::Load();

	//�G�t�F�N�g�̓ǂݍ���
	if (m_pLoadEffect == NULL)
	{
		m_pLoadEffect = CLoadTextEffect::Create(LOAD_EFFECT);
		CParticle::LoadTexture();
		CEffectTool::LoadEffect();
	}
	//�J�E���g�_�E�����
	CManager::SetGameState(CManager::GAMESTATE_GAME);

	//�n�ʂ̐���
	//if (m_pMeshFiled == NULL) { m_pMeshFiled = CSceneMeshFiled::Create(VECTOR_ZERO); }
	
	if (m_pFieldManager == NULL)
	{
		m_pFieldManager = new CFieldManager;
		m_pFieldManager->Init();
		//m_pFieldManager->SetState(CFieldManager::STATE_RANDOM);
	}

	//�v���C���[�̐���
	CreateInitPlayer();
	
	//�Q�[���J�����̐���
	CreateInitCamera();

	//�f�o�b�N�J�����̐���
	if (m_pDebugCamera == NULL)
	{
		D3DVIEWPORT9 viewport;
		viewport.X = 0;		//�r���[�|�[�g�̍���X���W
		viewport.Y = 0;		//�r���[�|�[�g�̍���Y���W
		viewport.Width = SCREEN_WIDTH;	//��
		viewport.Height = SCREEN_HEIGHT;//����
		viewport.MaxZ = 1.0f;
		viewport.MinZ = 0.0f;

		m_pDebugCamera = new CCamera;
		m_pDebugCamera->Init();
		m_pDebugCamera->SetViewport(viewport);
		m_pDebugCamera->SetPosV(D3DXVECTOR3(14.0f, 370.0f, -304.0f));
		m_pDebugCamera->SetPosV(D3DXVECTOR3(14.0f, 255.0f, -204.0f));
	}

	//�ǂݍ��ݏ���
	if (m_pLoadObject == NULL) { m_pLoadObject = CLoadTextObject::Create(LOAD_OBJECT); }				//�I�u�W�F�N�g
	if (m_pLoadTerritory == NULL) { m_pLoadTerritory = CLoadTextTerritory::Create(LOAD_TERRITORY); }	//�e���g���[

	//�e���g���[�̃G���A��������
	CTerritory::SetErea();

	//�~�j�}�b�v�̐���
	//CMiniMap::Create();

	//�J�E���g�_�E���^�C�}�[�̐���
	//CTimer::Create(CTimer::TYPE_COUNTER, 7, D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXVECTOR3(50.0f, 50.0f, 0.0f));

	//�Q�[���^�C�}�[�̐����i���X�g5�b�ŕ`�悳���j
	//CTimer::Create(CTimer::TYPE_GAME, 7, D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXVECTOR3(50.0f, 50.0f, 0.0f));

	////	---<<���т��ǉ����܂���>>��	��	��	��	��	��	��	��	��	��
	m_pCharacter[PLAYER_MAX] = CEnemy::Create(PLAYER_MAX, CEnemy::TYPE_BRAZIL, D3DXVECTOR3(0.0f, 0.0f, 0.0f), "data\\TEXT\\ModelLoad\\ENEMY_MODEL.txt", "data\\TEXT\\MotionLoad\\ENEMY_MOTION.txt");
	//m_pCharacter[PLAYER_MAX + 1] = CEnemy::Create(PLAYER_MAX+1, CEnemy::TYPE_RUSSIA, D3DXVECTOR3(100.0f, 0.0f, 0.0f), "data\\TEXT\\ModelLoad\\ENEMY_MODEL.txt", "data\\TEXT\\MotionLoad\\ENEMY_MOTION.txt");
	//m_pCharacter[PLAYER_MAX + 2] = CEnemy::Create(PLAYER_MAX + 2, D3DXVECTOR3(-100.0f, 0.0f, 0.0f), "data\\TEXT\\ModelLoad\\ENEMY_MODEL.txt", "data\\TEXT\\MotionLoad\\ENEMY_MOTION.txt");
	//m_pEnemy[3] = CEnemy::Create(3, D3DXVECTOR3(200.0f, 0.0f, 0.0f), "data\\TEXT\\ModelLoad\\ENEMY_MODEL.txt", "data\\TEXT\\MotionLoad\\ENEMY_MOTION.txt");
	//	��	��	��	��	��	��	��	��	��	��	��	��	��	��	��	��

	// UI��ǂݍ��ޏ���
	LoadUI(LOAD_UI);

	// UI�𐶐����鏈��
	CreateUI();
}

//=============================================================================
// �v���C���[�̐�������
//=============================================================================
void CGame::CreateInitPlayer(void)
{
	//�Z���N�g�̏����擾
	CSelect *pSelect = CManager::GetSelect();

	if (m_nNumPlay == 0)
	{//�f�o�b�O�p
		m_nNumPlay = PLAYER_MAX;
		for (int nCnt = 0; nCnt < m_nNumPlay; nCnt++)
		{//�v���C�l������
			//m_nPlayerType[nCnt] = pSelect->PLAYERTYPE_SPEED1;	//�v���C���[�̃^�C�v������
			//m_nCountry[nCnt] = pSelect->COUNTRY_JPN;			//�����擾

		//	if (m_nPlayerType[nCnt] == pSelect->PLAYERTYPE_SPEED1)
			{//�X�s�[�h�^�v���C���[�̐���
				m_pCharacter[nCnt] = CSpeedPlayer::Create(D3DXVECTOR3(-50.0f + (nCnt * 50.0f), 0.0f, 0.0f),
					"data\\TEXT\\ModelLoad\\SPEEDPLAYER_LOAD.txt",
					"data\\TEXT\\MotionLoad\\SPEEDPLAYER_MOTION.txt", nCnt, CPlayer::TYPE_AMERICA, CPlayer::PLAYERTYPE_SPEED);	//�̉�
			}
			//else if (m_nPlayerType[nCnt] == pSelect->PLAYERTYPE_POWER)
			//{//�p���[�^�v���C���[�̐���
			 //�v���C���[�̐���
				/*m_pPlayer[nCnt] = CPowerPlayer::Create(D3DXVECTOR3(-50.0f + (nCnt * 50.0f), 0.0f, 0.0f),
					"data\\TEXT\\ModelLoad\\POWERPLAYER_LOAD.txt",
					"data\\TEXT\\MotionLoad\\POWERPLAYER_MOTION.txt", nCnt, CPlayer::TYPE_AMERICA, CPlayer::PLAYERTYPE_POWER);*/
			//}
			//else if (m_nPlayerType[nCnt] == pSelect->PLAYERTYPE_TEQ)
			//{//�n���^�v���C���[�̐���
			//	m_pPlayer[nCnt] = CPlayer::Create(D3DXVECTOR3(-50.0f, 0.0f, 0.0f),
			//		"data\\TEXT\\ModelLoad\\PLAYER_LOAD.txt",
			//		"data\\TEXT\\MotionLoad\\PLAYER_MOTION.txt", nCnt, CPlayer::TYPE_AMERICA, CPlayer::PLAYERTYPE_POWER);
			//}
		}
	}
}

//=============================================================================
// �Q�[���J�����̐�������
//=============================================================================
void CGame::CreateInitCamera(void)
{
	if (m_nNumPlay == 1)
	{//�v���C�l����1�l��������
		if (m_pGameCamera[0] == NULL)
		{//�J�����̐���(1P:����)
			D3DVIEWPORT9 viewport;
			viewport.X = 0;							//�r���[�|�[�g�̍���X���W
			viewport.Y = 0;							//�r���[�|�[�g�̍���Y���W
			viewport.Width = 1280;		//��
			viewport.Height = 720;	//����
			viewport.MaxZ = 1.0f;
			viewport.MinZ = 0.0f;
			m_pGameCamera[0] = new CGameCamera;
			m_pGameCamera[0]->Init((CPlayer *)m_pCharacter[0]);
			m_pGameCamera[0]->SetViewport(viewport);
		}
	}
	else if (m_nNumPlay == 2)
	{//�v���C�l����2�l��������
		if (m_pGameCamera[0] == NULL)
		{//�J�����̐���(1P:����)
			D3DVIEWPORT9 viewport;
			viewport.X = 5;							//�r���[�|�[�g�̍���X���W
			viewport.Y = 5;							//�r���[�|�[�g�̍���Y���W
			viewport.Width = 1260 / 2;		//��
			viewport.Height = 700;	//����
			viewport.MaxZ = 1.0f;
			viewport.MinZ = 0.0f;
			m_pGameCamera[0] = new CGameCamera;
			m_pGameCamera[0]->Init((CPlayer *)m_pCharacter[0]);
			m_pGameCamera[0]->SetViewport(viewport);
		}
		if (m_pGameCamera[1] == NULL)
		{//�J�����̐���(2P:�E��)
			D3DVIEWPORT9 viewport;
			viewport.X = (SCREEN_WIDTH + 8) / 2;			//�r���[�|�[�g�̍���X���W
			viewport.Y = 5;							//�r���[�|�[�g�̍���Y���W
			viewport.Width = 1260 / 2;		//��
			viewport.Height = 700;	//����
			viewport.MaxZ = 1.0f;
			viewport.MinZ = 0.0f;
			m_pGameCamera[1] = new CGameCamera;
			m_pGameCamera[1]->Init((CPlayer *)m_pCharacter[1]);
			m_pGameCamera[1]->SetViewport(viewport);
		}
	}
	else if (m_nNumPlay == 3)
	{//�v���C�l����3�l��������
		if (m_pGameCamera[0] == NULL)
		{//�J�����̐���(1P:����)
			D3DVIEWPORT9 viewport;
			viewport.X = 5;							//�r���[�|�[�g�̍���X���W
			viewport.Y = 5;							//�r���[�|�[�g�̍���Y���W
			viewport.Width = 1260 / 2;		//��
			viewport.Height = 700 / 2;	//����
			viewport.MaxZ = 1.0f;
			viewport.MinZ = 0.0f;
			m_pGameCamera[0] = new CGameCamera;
			m_pGameCamera[0]->Init((CPlayer *)m_pCharacter[0]);
			m_pGameCamera[0]->SetViewport(viewport);
		}
		if (m_pGameCamera[1] == NULL)
		{//�J�����̐���(2P:�E��)
			D3DVIEWPORT9 viewport;
			viewport.X = (SCREEN_WIDTH + 8) / 2;			//�r���[�|�[�g�̍���X���W
			viewport.Y = 5;							//�r���[�|�[�g�̍���Y���W
			viewport.Width = 1260 / 2;		//��
			viewport.Height = 700 / 2;	//����
			viewport.MaxZ = 1.0f;
			viewport.MinZ = 0.0f;
			m_pGameCamera[1] = new CGameCamera;
			m_pGameCamera[1]->Init((CPlayer *)m_pCharacter[1]);
			m_pGameCamera[1]->SetViewport(viewport);
		}
		if (m_pGameCamera[2] == NULL)
		{//�J�����̐���(3P:����)
			D3DVIEWPORT9 viewport;
			viewport.X = 5;						//�r���[�|�[�g�̍���X���W
			viewport.Y = (SCREEN_HEIGHT + 5) / 2;		//�r���[�|�[�g�̍���Y���W
			viewport.Width = 1260 / 2;	//��
			viewport.Height = 700 / 2;//����
			viewport.MaxZ = 1.0f;
			viewport.MinZ = 0.0f;
			m_pGameCamera[2] = new CGameCamera;
			m_pGameCamera[2]->Init((CPlayer *)m_pCharacter[2]);
			m_pGameCamera[2]->SetViewport(viewport);
		}
	}
	else if (m_nNumPlay == 4)
	{//�v���C�l����4�l��������
		if (m_pGameCamera[0] == NULL)
		{//�J�����̐���(1P:����)
			D3DVIEWPORT9 viewport;
			viewport.X = 5;							//�r���[�|�[�g�̍���X���W
			viewport.Y = 5;							//�r���[�|�[�g�̍���Y���W
			viewport.Width = 1260 / 2;		//��
			viewport.Height = 700 / 2;	//����
			viewport.MaxZ = 1.0f;
			viewport.MinZ = 0.0f;
			m_pGameCamera[0] = new CGameCamera;
			m_pGameCamera[0]->Init((CPlayer *)m_pCharacter[0]);
			m_pGameCamera[0]->SetViewport(viewport);
		}
		if (m_pGameCamera[1] == NULL)
		{//�J�����̐���(2P:�E��)
			D3DVIEWPORT9 viewport;
			viewport.X = (SCREEN_WIDTH + 8) / 2;			//�r���[�|�[�g�̍���X���W
			viewport.Y = 5;							//�r���[�|�[�g�̍���Y���W
			viewport.Width = 1260 / 2;		//��
			viewport.Height = 700 / 2;	//����
			viewport.MaxZ = 1.0f;
			viewport.MinZ = 0.0f;
			m_pGameCamera[1] = new CGameCamera;
			m_pGameCamera[1]->Init((CPlayer *)m_pCharacter[1]);
			m_pGameCamera[1]->SetViewport(viewport);
		}
		if (m_pGameCamera[2] == NULL)
		{//�J�����̐���(3P:����)
			D3DVIEWPORT9 viewport;
			viewport.X = 5;						//�r���[�|�[�g�̍���X���W
			viewport.Y = (SCREEN_HEIGHT + 5) / 2;		//�r���[�|�[�g�̍���Y���W
			viewport.Width = 1260 / 2;	//��
			viewport.Height = 700 / 2;//����
			viewport.MaxZ = 1.0f;
			viewport.MinZ = 0.0f;
			m_pGameCamera[2] = new CGameCamera;
			m_pGameCamera[2]->Init((CPlayer *)m_pCharacter[2]);
			m_pGameCamera[2]->SetViewport(viewport);
		}
		if (m_pGameCamera[3] == NULL)
		{//�J�����̐���(4P:�E��)
			D3DVIEWPORT9 viewport;
			viewport.X = (SCREEN_WIDTH + 8) / 2;		//�r���[�|�[�g�̍���X���W
			viewport.Y = (SCREEN_HEIGHT + 5) / 2;		//�r���[�|�[�g�̍���Y���W
			viewport.Width = 1260 / 2;	//��
			viewport.Height = 700 / 2;//����
			viewport.MaxZ = 1.0f;
			viewport.MinZ = 0.0f;
			m_pGameCamera[3] = new CGameCamera;
			m_pGameCamera[3]->Init((CPlayer *)m_pCharacter[3]);
			m_pGameCamera[3]->SetViewport(viewport);
		}
	}
}

//=============================================================================
// UI�𐶐����鏈��
//=============================================================================
void CGame::CreateUI(void)
{
	
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
	for (int nCnt = 0; nCnt < m_nNumPlay; nCnt++)
	{
		if (m_pGameCamera[nCnt] != NULL)
		{
			m_pGameCamera[nCnt]->Uninit();
			delete m_pGameCamera[nCnt];
			m_pGameCamera[nCnt] = NULL;
		}
	}

	//�f�o�b�N�J�����̔j��
	if (m_pDebugCamera != NULL)
	{
		m_pDebugCamera->Uninit();
		delete m_pDebugCamera;
		m_pDebugCamera = NULL;
	}

	//�ϐ��̏�����
	m_pMeshFiled = NULL;
	m_nNumPlay = 0;
	m_nEnemyNum = 0;

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

	//�G�t�F�N�g�̔j��
	if (m_pLoadEffect != NULL)
	{
		CEffectTool::UnloadEffect();
		CParticle::UnloadTexture();

		m_pLoadEffect->Uninit();
		delete m_pLoadEffect;
		m_pLoadEffect = NULL;
	}

	//�e�N�X�`���̔j��
	CNumber2D::Unload();
	CTimerLogo::Unload();

	//��Ղ̔j��
	CSceneOrbit::Unload();

	//���b�V���t�B�[���h�̔j��
	if (m_pFieldManager != NULL)
	{
		m_pFieldManager->Uninit();
		delete m_pFieldManager;
		m_pFieldManager = NULL;
	}

	// UI�̊J��
	ReleaseUI();

	//�x�[�X�̔j��
	CModeBase::Uninit();
}

//=============================================================================
// UI�N���X�̊J������
//=============================================================================
void CGame::ReleaseUI(void)
{
	
}

//=============================================================================
// �X�V����
//=============================================================================
void CGame::Update(void)
{
	//CLoadEffect::Create(0, D3DXVECTOR3(-50.0f, 0.0f, 0.0f), 4);


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

	if (!m_bDebugCamera)
	{
		//�Q�[���J�����̍X�V����
		for (int nCnt = 0; nCnt < m_nNumPlay; nCnt++)
		{
			if (m_pGameCamera[nCnt] != NULL)
			{
				m_pGameCamera[nCnt]->Update();
			}
		}
	}
	else
	{
		if (m_pDebugCamera != NULL) { m_pDebugCamera->Update(); }
	}

	CInputMouse *pInputMouse = CManager::GetInputMouse();

	m_pFieldManager->Update();

	//if (pInputMouse->GetPress(pInputMouse->BUTTON_LEFT) == true)
	//{
	//	CFade::SetFade(CManager::MODE_RESULT);
	//}

	// UI�̍X�V����
	UpdateUI();

	//�f�o�b�N�L�[
	if (pCInputKeyBoard->GetKeyboardTrigger(DIK_1) == true) { CFade::SetFade(CManager::MODE_GAME); }
	if (pCInputKeyBoard->GetKeyboardTrigger(DIK_7) == true) { m_bDebugCamera = m_bDebugCamera ? false : true; }
}

//=============================================================================
// UI�̍X�V����
//=============================================================================
void CGame::UpdateUI(void)
{

}

//=============================================================================
// �`�揈��
//=============================================================================
void CGame::Draw(void)
{
	//�����_�����O�N���X���擾
	CRenderer * pRenderer = NULL;
	pRenderer = CManager::GetRenderer();

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//�J����4�����̏���
	if (!m_bDebugCamera)
	{
		if (m_nNumPlay == 1)
		{
			//�J�����̐ݒ�
			if (m_pGameCamera[0] != NULL) { m_pGameCamera[0]->Set(); }
			CManager::GetRenderer()->GetDevice()->Clear(0,
				NULL,
				(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL),
				D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);
			//���ׂĕ`�悷��(����)
			CScene::DrawAll();
		}
		else if (m_nNumPlay == 2)
		{
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
		}
		else if (m_nNumPlay == 3)
		{
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
		}
		else if (m_nNumPlay == 4)
		{
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
	}
	else
	{
		//�f�o�b�N�J�����̐ݒ�
		if (m_pDebugCamera != NULL)
		{
			m_pDebugCamera->Set();

			CManager::GetRenderer()->GetDevice()->Clear(0,
				NULL,
				(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL),
				D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

			//���ׂĕ`�悷��
			CScene::DrawAll();
		}
	}
	// �`��̈����ʑS�̂ɐݒ�
	D3DVIEWPORT9 ViewPort, ViewPortDef;
	pDevice->GetViewport(&ViewPortDef);
	ViewPort.X = 0;			//�r���[�|�[�g�̍���X���W
	ViewPort.Y = 0;							//�r���[�|�[�g�̍���Y���W
	ViewPort.Width = SCREEN_WIDTH;		//��
	ViewPort.Height = SCREEN_HEIGHT;	//����
	ViewPort.MaxZ = 1.0f;
	ViewPort.MinZ = 0.0f;
	pDevice->SetViewport(&ViewPort);

	for (int nCntLayer = 0; nCntLayer < MAX_LAYER; nCntLayer++)
	{
			//�g�b�v�̈ʒu��ۑ�����
			CScene * pScene = CScene::GetTop(nCntLayer);

			while (pScene != NULL)
			{
				CScene * pSceneNext = pScene->GetpNext();	//���̃I�u�W�F�N�g�̃|�C���^��ۑ�����

				if (pScene->GetObjType() == CScene2D::OBJTYPE_2DPOLYGON)
				{
					pScene->Draw();
				}

				pScene = pSceneNext;					//�|�C���^��i�߂�
			}
	}
	// �`��̈��߂�
	pDevice->SetViewport(&ViewPortDef);
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