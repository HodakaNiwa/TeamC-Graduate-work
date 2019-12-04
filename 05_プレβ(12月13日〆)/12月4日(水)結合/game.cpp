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
#include "UI.h"
#include "collision.h"
#include "mine.h"
#include "skilicon.h"
#include "audience.h"
#include "effect3D.h"
#include "effectManager.h"
#include "model.h"
#include "ringRender.h"
#include "territoryRender.h"
#include "emitter.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
//#define GAME_DEBUG

#define LOAD_UI		("data\\TEXT\\UI\\Game.txt")
#define VECTOR_ZERO	(D3DXVECTOR3(0.0f, 0.0f, 0.0f))
#define LOAD_OBJECT		("data/TEXT/Stage.txt")
#define LOAD_TERRITORY	("data/TEXT/Territory.txt")
#define LOAD_EFFECT	("data/TEXT/EFFECT/Particle.txt")
#define PLAYER_MAX (1)

#define DEBUG_NUMPLAYER         (2)
#define DEBUG_CAMERA (D3DXVECTOR3(0.0f,2500.0f,180.0f))

// ��]�J�����p(3�l�v���C���̂ݓK�p����)
#define ROTATIONCAMERA_POSVHEIGHT (500.0f)
#define ROTATIONCAMERA_POSR       (D3DXVECTOR3(0.0f,0.0f,0.0f))
#define ROTATIONCAMERA_ROTSPEED   (0.003f)
#define ROTATIONCAMERA_LENGTH     (700.0f)

// �v���C���[�z�u�p
#define PLAYER_SPAWN_LENGTH_X   (1100.0f)
#define PLAYER_SPAWN_LENGTH_Z   (1090.0f)
#define PLAYER_SPAWN_RIVISION_X (-90.0f)
#define PLAYER_SPAWN_RIVISION_Z (170.0f)

// �}�b�v�̌����Ȃ��ǔz�u�p
#define MAPCOLLIDER_NUM     (4)
#define MAPCOLLIDER_POS_0   (D3DXVECTOR3(0.0f, 0.0f, 1400.0f))
#define MAPCOLLIDER_POS_1   (D3DXVECTOR3(1100.0f, 0.0f, 0.0f))
#define MAPCOLLIDER_POS_2   (D3DXVECTOR3(0.0f, 0.0f, -1020.0f))
#define MAPCOLLIDER_POS_3   (D3DXVECTOR3(-1300.0f, 0.0f, 0.0f))
#define MAPCOLLIDER_WIDTH_0 (3000.0f)
#define MAPCOLLIDER_WIDTH_1 (100.0f)
#define MAPCOLLIDER_HEIGHT  (300.0f)
#define MAPCOLLIDER_DEPTH_0 (100.0f)
#define MAPCOLLIDER_DEPTH_1 (3000.0f)

// �G�t�F�N�g�f�[�^�̃t�@�C����
#define EFFECTMANAGER_FILENAME "data/TEXT/EFFECT/ringEffect.txt"

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
CPause *CGame::m_pPause = NULL;
bool CGame::m_bPause = false;
bool CGame::m_bDebugCamera = false;
CGame::GAMESTATE CGame::m_gameState = GAMESTATE_NONE;
CSceneMeshFiled * CGame::m_pMeshFiled = NULL;
CLoadTextEffect * CGame::m_pLoadEffect = NULL;
int CGame::m_nEnemyNumResult = 0;
int CGame::m_nScore[MAX_CHARACTER] = {};
int CGame::m_nCountMakeScore[MAX_CHARACTER] = {};
int CGame::m_nCountGetTerritry[MAX_CHARACTER] = {};
int CGame::m_nCountRobottedTerritory[MAX_CHARACTER] = {};

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
	m_pRotCamera = NULL;
	m_nEnemyNum = 0;

	m_nNumPlay = CSelect::GetEntryPlayer();
	for (int nCnt = 0; nCnt < m_nNumPlay; nCnt++)
	{
		m_nPlayerType[nCnt] = 0;	//�v���C���[�̃^�C�v
		m_nCountry[nCnt] = 0;		//��
		m_nControllerNum[nCnt] = 0;
		m_nControllerType[nCnt] = 0;
	}
	
	for (int nCntPlayer = 0; nCntPlayer < m_nNumPlay; nCntPlayer++)
	{
		m_pPlayer[nCntPlayer] = NULL;
		m_pGameCamera[nCntPlayer] = NULL;
	}

	for (int nCnt = 0; nCnt < ENEMY_MAX; nCnt++)
	{
		m_pEnemy[nCnt] = NULL;
	}

	for (int nCnt = 0; nCnt < MAX_CHARACTER; nCnt++)
	{
		m_pCharacter[nCnt] = NULL;
	}

	for (int nCnt = 0; nCnt < MAX_GAMECAMERA; nCnt++)
	{
		m_pGameCamera[nCnt] = NULL;
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
	m_nSpeedNo = 0;
	m_nPowerNo = 0;
	m_nSweeperNo = 0;
	m_bTypeMax = false;
	m_pEffectManager = NULL;
	m_pEmitter = NULL;

	//�e�N�X�`���̓ǂݍ���
	CNumber2D::Load();
	CTimerLogo::Load();
	CSkilicon::Load();
	CEffect3D::Load();

	//��Ղ̓ǂݍ���
	CSceneOrbit::Load();

	//�n���̓ǂݍ���
	CMine::Load();

	//�A�E�g���C���ǂݍ���
	CModel::ShaderLoad();
	//�G�t�F�N�g�̓ǂݍ���
	if (m_pLoadEffect == NULL)
	{
		m_pLoadEffect = CLoadTextEffect::Create(LOAD_EFFECT);
		CParticle::LoadTexture();
		CEffectTool::LoadEffect();
	}
#ifdef GAME_DEBUG
	//�J�E���g�_�E�����
	CGame::SetGameState(CGame::GAMESTATE_NORMAL);

	//�n�ʂ̐���
	if (m_pFieldManager == NULL)
	{
		m_pFieldManager = new CFieldManager;
		m_pFieldManager->Init();
	}

#else
	//�J�E���g�_�E�����
	CGame::SetGameState(CGame::GAMESTATE_NORMAL);
	//CGame::SetGameState(CGame::GAMESTATE_FIRSTCOUNTDOWN);

	//�J�E���g�_�E���^�C�}�[�̐���
	//CTimer::Create(CTimer::TYPE_COUNTER, 7, D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXVECTOR3(50.0f, 80.0f, 0.0f));

	//�Q�[���^�C�}�[�̐����i���X�g5�b�ŕ`�悳���j
	//CTimer::Create(CTimer::TYPE_GAME, 7, D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXVECTOR3(50.0f, 80.0f, 0.0f));

	//�n�ʂ̐���
	if (m_pFieldManager == NULL)
	{
		m_pFieldManager = new CFieldManager;
		m_pFieldManager->Init();
		m_pFieldManager->SetState(CFieldManager::STATE_RANDOM);
	}

#endif
	// �e�L�����N�^�[�̈ʒu��ݒ肷�鏈��
	SettingCharaPosition();

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
		m_pDebugCamera->SetPosV(DEBUG_CAMERA);
		//m_pDebugCamera->SetPosV(D3DXVECTOR3(14.0f, 255.0f, -204.0f));
	}

	//�ǂݍ��ݏ���
	if (m_pLoadObject == NULL) { m_pLoadObject = CLoadTextObject::Create(LOAD_OBJECT); }				//�I�u�W�F�N�g
	if (m_pLoadTerritory == NULL) { m_pLoadTerritory = CLoadTextTerritory::Create(LOAD_TERRITORY); }	//�e���g���[

	//�e���g���[�̃G���A��������
	CTerritory::SetErea();

	//	---<<���т��ǉ����܂���>>��	��	��	��	��	��	��	��	��	��
	for (int nCntEnemy = m_nNumPlay; nCntEnemy < ENEMY_MAX;nCntEnemy++)
	{
		//	�X�s�[�h
		if (m_nSpeedNo < 3 - (int)m_bTypeMax)
		{
			m_nSpeedNo += 1;
			m_pCharacter[nCntEnemy] = CTypeSpeed::Create(nCntEnemy, m_nSpeedNo,CEnemy::TYPE_BRAZIL, CCharacter::CHARCTERTYPE_SPEED,
				InitPos[nCntEnemy],"data\\TEXT\\ModelLoad\\SPEEDPLAYER_LOAD.txt", "data\\TEXT\\MotionLoad\\SPEEDPLAYER_MOTION.txt");
		}
		//	�]��
		else if (m_nPowerNo < 3 - (int)m_bTypeMax)
		{
			m_nPowerNo += 1;
			m_pCharacter[nCntEnemy] = CTypePower::Create(nCntEnemy, CEnemy::TYPE_BRAZIL, CCharacter::CHARCTERTYPE_POWER,
				InitPos[nCntEnemy], "data\\TEXT\\ModelLoad\\POWERPLAYER_LOAD.txt", "data\\TEXT\\MotionLoad\\POWERPLAYER_MOTION.txt");
		}
		//	�n��
		else if (m_nSweeperNo < 3)
		{
			m_nSweeperNo += 1;
			m_pCharacter[nCntEnemy] = CTypeSweeper::Create(nCntEnemy, CEnemy::TYPE_BRAZIL, CCharacter::CHARCTERTYPE_TECHNIQUE,
				InitPos[nCntEnemy], "data\\TEXT\\ModelLoad\\TEQPLAYER_LOAD.txt", "data\\TEXT\\MotionLoad\\TEQPLAYER_MOTION.txt");
		}

		m_nEnemyNum++;
		m_nEnemyNumResult++;
	}
	//	��	��	��	��	��	��	��	��	��	��	��	��	��	��	��	��

	// �e�N�X�`����ǂݍ���
	LoadUI(LOAD_UI);

	// UI�𐶐����鏈��
	CreateUI();

	// �}�b�v�̌����Ȃ��ǂ�z�u����
	CreateMapCollider();

	//�ϋq�𐶐�
	CAudience::Create();

	// �G�t�F�N�g�Ǌ��N���X�𐶐�
	m_pEffectManager = CEffectManager::Create(EFFECTMANAGER_FILENAME);

	// �����O�`��N���X�𐶐�
	CRingRender::Create();

	// �e���g���[�`��N���X�𐶐�
	CTerritoryRender::Create();
}

//=============================================================================
// �v���C���[�̐�������
//=============================================================================
void CGame::CreateInitPlayer(void)
{
	//�Z���N�g�̏����擾
	CSelect *pSelect = CManager::GetSelect();
	m_nNumPlay = CSelect::GetEntryPlayer();
	if (m_nNumPlay == 0)
	{
		m_nNumPlay = DEBUG_NUMPLAYER;
	}
	
	for (int nCnt = 0; nCnt < m_nNumPlay; nCnt++)
	{//�v���C�l������
#ifdef GAME_DEBUG
	 //�f�o�b�O�p
		m_nPlayerType[nCnt] = CCharacter::CHARCTERTYPE_TECHNIQUE;
		m_nCountry[nCnt] = CCharacter::TYPE_AMERICA;
		m_nControllerNum[nCnt] = 0;	//�R���g���[���[�̔ԍ����擾
		m_nControllerType[nCnt] = 1;		//�R���g���[���[�̃^�C�v���擾
#else
		m_nPlayerType[nCnt] = pSelect->GetPlayerType(nCnt);			//�v���C���[�̃^�C�v������
		m_nCountry[nCnt] = pSelect->GetPlayerCuntry(nCnt);			//�����擾
		m_nControllerNum[nCnt] = pSelect->GetControlerNum(nCnt);	//�R���g���[���[�̔ԍ����擾
		m_nControllerType[nCnt] = pSelect->GetInputType(nCnt);		//�R���g���[���[�̃^�C�v���擾
#endif

		if (m_nPlayerType[nCnt] == 0)
		{//�X�s�[�h�^�v���C���[�̐���
			m_nSpeedNo += 1;

			m_pCharacter[nCnt] = CSpeedPlayer::Create(InitPos[nCnt],
				"data\\TEXT\\ModelLoad\\SPEEDPLAYER_LOAD.txt",
				"data\\TEXT\\MotionLoad\\SPEEDPLAYER_MOTION.txt", nCnt, m_nCountry[nCnt],
				CCharacter::CHARCTERTYPE_SPEED, m_nControllerNum[nCnt], m_nControllerType[nCnt]);	//�̉�
		}
		else if (m_nPlayerType[nCnt] == 1)
		{//�p���[�^�v���C���[�̐���
			m_nPowerNo += 1;

			m_pCharacter[nCnt] = CPowerPlayer::Create(InitPos[nCnt],
				"data\\TEXT\\ModelLoad\\POWERPLAYER_LOAD.txt",
				"data\\TEXT\\MotionLoad\\POWERPLAYER_MOTION.txt", nCnt, m_nCountry[nCnt],
				CCharacter::CHARCTERTYPE_POWER, m_nControllerNum[nCnt], m_nControllerType[nCnt]);
		}
		else if (m_nPlayerType[nCnt] == 2)
		{//�n���^�v���C���[�̐���
			m_nSweeperNo += 1;

			m_pCharacter[nCnt] = CTechniquePlayer::Create(InitPos[nCnt],
				"data\\TEXT\\ModelLoad\\TEQPLAYER_LOAD.txt",
				"data\\TEXT\\MotionLoad\\TEQPLAYER_MOTION.txt", nCnt, m_nCountry[nCnt],
				CCharacter::CHARCTERTYPE_TECHNIQUE, m_nControllerNum[nCnt], m_nControllerType[nCnt]);
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
		if (m_pRotCamera == NULL)
		{//�J�����̐���(4P:�E��)
			D3DVIEWPORT9 viewport;
			viewport.X = (SCREEN_WIDTH + 8) / 2;		//�r���[�|�[�g�̍���X���W
			viewport.Y = (SCREEN_HEIGHT + 5) / 2;		//�r���[�|�[�g�̍���Y���W
			viewport.Width = 1260 / 2;	//��
			viewport.Height = 700 / 2;//����
			viewport.MaxZ = 1.0f;
			viewport.MinZ = 0.0f;
			m_pRotCamera = CRotationCamera::Create(ROTATIONCAMERA_POSVHEIGHT, ROTATIONCAMERA_POSR,
				ROTATIONCAMERA_ROTSPEED, ROTATIONCAMERA_LENGTH);
			m_pRotCamera->SetViewport(viewport);
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
	//�Z���N�g�̏����擾
	CSelect *pSelect = CManager::GetSelect();

	int nAllCharacter = m_nNumPlay + m_nEnemyNum;

	// ���������m��
	if (m_pUI != NULL) { return; }
	m_pUI = CUI::Create(m_nNumPlay, nAllCharacter);

	// �X�L���A�C�R���𐶐�
	for (int nCnt = 0; nCnt < m_nNumPlay; nCnt++)
	{
		m_nPlayerType[nCnt] = pSelect->GetPlayerType(nCnt);			//�v���C���[�̃^�C�v������

		if (m_nPlayerType[nCnt] == 0)
		{//�X�s�[�h�^�v���C���[�̐���
			m_pUI->CreateSkilicon(m_pCharacter[nCnt], nCnt, 20 * 60);
		}
		else if (m_nPlayerType[nCnt] == 1)
		{//�p���[�^�v���C���[�̐���
			m_pUI->CreateSkilicon(m_pCharacter[nCnt], nCnt, 3 * 60);

		}
		else if (m_nPlayerType[nCnt] == 2)
		{//�n���^�v���C���[�̐���
			m_pUI->CreateSkilicon(m_pCharacter[nCnt], nCnt, 1 * 60);

		}
	}
}

//=============================================================================
// �e�L�����N�^�[�̈ʒu��ݒ肷�鏈��
//=============================================================================
void CGame::SettingCharaPosition(void)
{
	D3DXVECTOR3 pos = VECTOR_ZERO;	// �ʒu
	float fAngle = 0.0f;			// �ʒu�̌v�Z�p
	int nSetCharaNum = 0;			// �L���������l�z�u������
	int nRandomChara = 0;			// �z�u����L�����̎�ޕ����p
	int nSetIdx = 0;				// �z�u�ʒu�����_���p
	bool bSet[MAX_PLAYERNUM * 2];	// �Z�b�g�������ǂ���
	ZeroMemory(&bSet, sizeof(bool) * 8);

	// 8�p�`��Ƀ����_���ɓG�ƃv���C���[��z�u����
	for (int nCnt = 0; nCnt < MAX_PLAYERNUM * 2; nCnt++)
	{// �v���C�l�����J��Ԃ�
		while (1)
		{// �z�u����ʒu�����߂�
			nSetIdx = rand() % (MAX_PLAYERNUM * 2);
			if (bSet[nSetIdx] == false)
			{
				bSet[nSetIdx] = true;
				fAngle = 0.0f;
				break;
			}
		}

		// �p�x��i�߂�
		fAngle += ((D3DX_PI * 2.0f) / (MAX_PLAYERNUM * 2.0f)) * nSetIdx;
		if (fAngle > D3DX_PI)
		{
			fAngle -= D3DX_PI * 2.0f;
		}

		// �ʒu���v�Z
		pos.x = sinf(fAngle) * PLAYER_SPAWN_LENGTH_X + PLAYER_SPAWN_RIVISION_X;
		pos.z = cosf(fAngle) * PLAYER_SPAWN_LENGTH_Z + PLAYER_SPAWN_RIVISION_Z;

		// �ʒu������
		//if (m_pCharacter[nSetCharaNum] != NULL)
		//{
		//}
			InitPos[nSetCharaNum] = pos;
			nSetCharaNum++;
	}
}

//=============================================================================
// �}�b�v�̌����Ȃ��ǂ𐶐����鏈��
//=============================================================================
void CGame::CreateMapCollider(void)
{
	D3DXVECTOR3 pos[MAPCOLLIDER_NUM]
	{
		MAPCOLLIDER_POS_0,
		MAPCOLLIDER_POS_1,
		MAPCOLLIDER_POS_2,
		MAPCOLLIDER_POS_3
	};
	float fWidth[MAPCOLLIDER_NUM]
	{
		MAPCOLLIDER_WIDTH_0,
		MAPCOLLIDER_WIDTH_1,
		MAPCOLLIDER_WIDTH_0,
		MAPCOLLIDER_WIDTH_1
	};
	float fDepth[MAPCOLLIDER_NUM]
	{
		MAPCOLLIDER_DEPTH_0,
		MAPCOLLIDER_DEPTH_1,
		MAPCOLLIDER_DEPTH_0,
		MAPCOLLIDER_DEPTH_1
	};
	for (int nCnt = 0; nCnt < MAPCOLLIDER_NUM; nCnt++)
	{
		CBoxCollider::Create(pos[nCnt], D3DXVECTOR3(1.0f, 1.0f, 1.0f), fWidth[nCnt], MAPCOLLIDER_HEIGHT, fDepth[nCnt], true);
	}
}

//=============================================================================
// �I������
//=============================================================================
void CGame::Uninit(void)
{	
	//�X�R�A�̏��
	GetCharInfo();
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

	//��]�J�����̔j��
	if (m_pRotCamera != NULL)
	{
		m_pRotCamera->Uninit();
		delete m_pRotCamera;
		m_pRotCamera = NULL;
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

	// �G�t�F�N�g�Ǌ��N���X�̔j��
	if (m_pEffectManager != NULL)
	{
		m_pEffectManager->Uninit();
		delete m_pEffectManager;
		m_pEffectManager = NULL;
	}

	if (m_pEmitter != NULL)
	{
		m_pEmitter = NULL;
	}
	//�e�N�X�`���̔j��
	CNumber2D::Unload();
	CTimerLogo::Unload();
	CSkilicon::UnLoad();
	CEffect3D::UnLoad();

	//�A�E�g���C���j��
	CModel::ShaderUnLoad();

	//��Ղ̔j��
	CSceneOrbit::Unload();

	//�n���̔j��
	CMine::UnLoad();

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
	if (m_pUI != NULL)
	{
		//�X�R�A���擾����
		CScore *pScore = NULL;
		for (int nCnt = 0; nCnt < MAX_CHARACTER; nCnt++)
		{
			pScore = m_pUI->GetScoreGame(nCnt);
			if (pScore == NULL) { continue; }
			m_nScore[nCnt] = pScore->GetScore();
		}
		m_pUI->Uninit();
		delete m_pUI;
		m_pUI = NULL;
	}
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
		if (m_pRotCamera != NULL)
		{//��]�J�����̍X�V
			m_pRotCamera->Update();
		}
	}
	else
	{
		if (m_pDebugCamera != NULL) { m_pDebugCamera->Update(); }
	}

	CInputMouse *pInputMouse = CManager::GetInputMouse();

	if (m_pFieldManager != NULL) { m_pFieldManager->Update(); }

	//if (pInputMouse->GetPress(pInputMouse->BUTTON_LEFT) == true)
	//{
	//	CFade::SetFade(CManager::MODE_RESULT);
	//}

	// UI�̍X�V����
	UpdateUI();

	//�f�o�b�N�L�[
	if (pCInputKeyBoard->GetKeyboardTrigger(DIK_1) == true) { CFade::SetFade(CManager::MODE_RESULT); }
	if (pCInputKeyBoard->GetKeyboardTrigger(DIK_7) == true) { m_bDebugCamera = m_bDebugCamera ? false : true; }

	if (pCInputKeyBoard->GetKeyboardTrigger(DIK_8) == true)
	{
		m_pEmitter = m_pEffectManager->SetEffect(INITIALIZE_VECTOR3, INITIALIZE_VECTOR3, 4);
	}

	if (pCInputKeyBoard->GetKeyboardTrigger(DIK_9) == true)
	{
		m_pEmitter->Uninit();
	}
}

//=============================================================================
// UI�̍X�V����
//=============================================================================
void CGame::UpdateUI(void)
{
	if (m_pUI == NULL) { return; }
	m_pUI->Update();
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

			// �A�C�R���̕`��
			DrawUI_Icon(0);
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

			// �A�C�R���̕`��
			DrawUI_Icon(0);

			//�J�����̐ݒ�
			if (m_pGameCamera[1] != NULL) { m_pGameCamera[1]->Set(); }
			//���ׂĕ`�悷��i�E��j
			CManager::GetRenderer()->GetDevice()->Clear(0,
				NULL,
				(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL),
				D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);
			CScene::DrawAll();

			// �A�C�R���̕`��
			DrawUI_Icon(1);
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

			// �A�C�R���̕`��
			DrawUI_Icon(0);

			//�J�����̐ݒ�
			if (m_pGameCamera[1] != NULL) { m_pGameCamera[1]->Set(); }
			//���ׂĕ`�悷��i�E��j
			CManager::GetRenderer()->GetDevice()->Clear(0,
				NULL,
				(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL),
				D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);
			CScene::DrawAll();

			// �A�C�R���̕`��
			DrawUI_Icon(1);

			//�J�����̐ݒ�
			if (m_pGameCamera[2] != NULL) { m_pGameCamera[2]->Set(); }
			CManager::GetRenderer()->GetDevice()->Clear(0,
				NULL,
				(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL),
				D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);
			//���ׂĕ`�悷��i�����j
			CScene::DrawAll();

			// �A�C�R���̕`��
			DrawUI_Icon(2);

			//�J�����̐ݒ�
			if (m_pRotCamera != NULL) { m_pRotCamera->Set(); }
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

			// �A�C�R���̕`��
			DrawUI_Icon(0);

			//�J�����̐ݒ�
			if (m_pGameCamera[1] != NULL) { m_pGameCamera[1]->Set(); }
			//���ׂĕ`�悷��i�E��j
			CManager::GetRenderer()->GetDevice()->Clear(0,
				NULL,
				(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL),
				D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);
			CScene::DrawAll();

			// �A�C�R���̕`��
			DrawUI_Icon(1);

			//�J�����̐ݒ�
			if (m_pGameCamera[2] != NULL) { m_pGameCamera[2]->Set(); }
			CManager::GetRenderer()->GetDevice()->Clear(0,
				NULL,
				(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL),
				D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);
			//���ׂĕ`�悷��i�����j
			CScene::DrawAll();

			// �A�C�R���̕`��
			DrawUI_Icon(2);

			//�J�����̐ݒ�
			if (m_pGameCamera[3] != NULL) { m_pGameCamera[3]->Set(); }
			CManager::GetRenderer()->GetDevice()->Clear(0,
				NULL,
				(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL),
				D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);
			//���ׂĕ`�悷��i�E���j
			CScene::DrawAll();

			// �A�C�R���̕`��
			DrawUI_Icon(3);
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

	if (m_pUI != NULL)
	{
		m_pUI->Draw();
	}

	for (int nCntLayer = 0; nCntLayer < MAX_LAYER; nCntLayer++)
	{
			//�g�b�v�̈ʒu��ۑ�����
			CScene * pScene = CScene::GetTop(nCntLayer);

			while (pScene != NULL)
			{
				CScene * pSceneNext = pScene->GetpNext();	//���̃I�u�W�F�N�g�̃|�C���^��ۑ�����

				if (pScene->GetObjType() == CScene2D::OBJTYPE_2DPOLYGON 
					|| pScene->GetObjType() == CScene::OBJTYPE_FADE
					|| pScene->GetObjType() == CScene::OBJTYPE_SCORE)
				{
					pScene->Draw();
				}

				pScene = pSceneNext;					//�|�C���^��i�߂�
			}
	}
}

//=============================================================================
// �A�C�R����`�悷�鏈��
//=============================================================================
void CGame::DrawUI_Icon(const int nIdx)
{
	if (m_pUI == NULL) { return; }
	m_pUI->DrawIcon(nIdx);
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

//=============================================================================
// �L�����̎擾���
//=============================================================================
void CGame::GetCharInfo(void)
{
	for (int nCnt = 0; nCnt < MAX_CHARACTER; nCnt++)
	{
		if (m_pCharacter[nCnt] != NULL)
		{
			m_nCountMakeScore[nCnt] = m_pCharacter[nCnt]->GetCountMakeShape();					//�}�`������������擾
			m_nCountGetTerritry[nCnt] = m_pCharacter[nCnt]->GetCountGetTerritory();				//�e���g���[�̎擾��
			m_nCountRobottedTerritory[nCnt] = m_pCharacter[nCnt]->GetCountRobbtedTerritory();	//�e���g���[�̒D��ꂽ��
		}
	}
}