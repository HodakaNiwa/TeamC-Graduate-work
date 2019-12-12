//=============================================================================
//
// �Q�[���̏��� [game.cpp]
// Author : �������� �F �R���֎j �F ���я��� �F �O�H�ۋM �F �r�J�R�N
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
#include "fieldRender.h"
#include "ringRender.h"
#include "territoryRender.h"
#include "robot.h"
#include "robotUI.h"
#include "sky.h"
#include "field.h"
#include "divisionWall.h"
#include "ring.h"
#include "debuglog.h"
#include "exprotiontrace.h" //��
#include "crack.h"			//��
#include "ripples.h"		//��

int g_nCntGame = 0;
//*****************************************************************************
// �}�N����`
//*****************************************************************************
//#define GAME_DEBUG
//#define ROBOT_EVENT
#define AREABONUS_EVENT
#define DIVISION_EVENT

#define LOAD_UI						("data\\TEXT\\UI\\Game.txt")
#define VECTOR_ZERO					(D3DXVECTOR3(0.0f, 0.0f, 0.0f))
#define LOAD_OBJECT					("data/TEXT/Stage.txt")
#define LOAD_TERRITORY				("data/TEXT/Territory.txt")
#define LOAD_EFFECT					("data/TEXT/EFFECT/Particle.txt")
#define PLAYER_MAX					(1)

// �f�o�b�O
#define DEBUG_NUMPLAYER				 (2)
#define DEBUG_CAMERA				 (D3DXVECTOR3(0.0f,2500.0f,180.0f))

// ��]�J�����p(3�l�v���C���̂ݓK�p����)
#define ROTATIONCAMERA_POSVHEIGHT	 (500.0f)
#define ROTATIONCAMERA_POSR			 (D3DXVECTOR3(0.0f,0.0f,0.0f))
#define ROTATIONCAMERA_ROTSPEED		 (0.003f)
#define ROTATIONCAMERA_LENGTH		 (700.0f)

// �v���C���[�z�u�p
#define PLAYER_SPAWN_LENGTH_X		 (1100.0f)
#define PLAYER_SPAWN_LENGTH_Z		 (1090.0f)
#define PLAYER_SPAWN_RIVISION_X		 (-90.0f)
#define PLAYER_SPAWN_RIVISION_Z		 (170.0f)

// �}�b�v�̌����Ȃ��ǔz�u�p
#define MAPCOLLIDER_NUM				 (4)
#define MAPCOLLIDER_POS_0			 (D3DXVECTOR3(0.0f, 0.0f, 1400.0f))
#define MAPCOLLIDER_POS_1			 (D3DXVECTOR3(1100.0f, 0.0f, 0.0f))
#define MAPCOLLIDER_POS_2			 (D3DXVECTOR3(0.0f, 0.0f, -1020.0f))
#define MAPCOLLIDER_POS_3			 (D3DXVECTOR3(-1300.0f, 0.0f, 0.0f))
#define MAPCOLLIDER_WIDTH_0			 (3000.0f)
#define MAPCOLLIDER_WIDTH_1			 (100.0f)
#define MAPCOLLIDER_HEIGHT			 (300.0f)
#define MAPCOLLIDER_DEPTH_0			 (100.0f)
#define MAPCOLLIDER_DEPTH_1			 (3000.0f)

// �G�t�F�N�g�f�[�^�̃t�@�C����
#define EFFECTMANAGER_FILENAME "data/TEXT/EFFECT/ringEffect.txt"

// �G���A�{�[�i�X�C�x���g�p
#define AREABONUSEVENT_TIME 				(30 * 60)
#define AREABONUSEVENT_BG_MOVE				(3.0f)
#define AREABONUSEVENT_BG_MOVETIME			(40)
#define AREABONUSEVENT_LOGO_ALPHA 			(0.03f)
#define AREABONUSEVENT_LOGO_WIDTH_START_0	(700.0f)
#define AREABONUSEVENT_LOGO_HEIGHT_START_0	(80.0f)
#define AREABONUSEVENT_LOGO_WIDTH_START_1	(1000.0f)
#define AREABONUSEVENT_LOGO_HEIGHT_START_1	(100.0f)
#define AREABONUSEVENT_LOGO_WIDTH_FINISH	(800.0f)
#define AREABONUSEVENT_LOGO_HEIGHT_FINISH	(110.0f)
#define AREABONUSEVENT_LOGO_TEXIDX_START	(7)
#define AREABONUSEVENT_LOGO_TEXIDX_FINISH	(12)
#define AREABONUSEVENT_FIELD_ALPHA			(0.01f)

// ���f�C�x���g�p
#define DIVISIONEVENT_TIME 					(30 * 60)
#define DIVISIONEVENT_CAMERA_POSVHEIGHT		(800.0f)
#define DIVISIONEVENT_CAMERA_LENGTH			(1000.0f)
#define DIVISIONEVENT_BG_MOVE				(3.0f)
#define DIVISIONEVENT_BG_MOVETIME			(40)
#define DIVISIONEVENT_LOGO_ALPHA 			(0.03f)
#define DIVISIONEVENT_LOGO_WIDTH_START_0	(700.0f)
#define DIVISIONEVENT_LOGO_HEIGHT_START_0	(80.0f)
#define DIVISIONEVENT_LOGO_WIDTH_START_1	(1000.0f)
#define DIVISIONEVENT_LOGO_HEIGHT_START_1	(100.0f)
#define DIVISIONEVENT_LOGO_WIDTH_FINISH		(800.0f)
#define DIVISIONEVENT_LOGO_HEIGHT_FINISH	(110.0f)
#define DIVISIONEVENT_LOGO_TEXIDX_START		(13)
#define DIVISIONEVENT_LOGO_TEXIDX_FINISH	(16)
#define DIVISIONEVENT_VERTICAL_IDX			(0)
#define DIVISIONEVENT_HORIZON_IDX			(1)
#define DIVISIONEVENT_COLLIDER_MAX			(3000.0f)
#define DIVISIONEVENT_COLLIDER_MIN			(30.0f)
#define DIVISIONEVENT_WALL_WIDTH			(2500.0f)
#define DIVISIONEVENT_WALL_HEIGHT			(1000.0f)
#define DIVISIONEVENT_WALL_TEXWIDTH			(4.0f)
#define DIVISIONEVENT_WALL_TEXHEIGHT		(2.0f)
#define DIVISIONEVENT_WALL_COL				(D3DXCOLOR(1.0f,1.0f,1.0f,0.0f))
#define DIVISIONEVENT_WALL_PRIORITY			(9)
#define DIVISIONEVENT_WALL_TEXIDX			(17)
#define DIVISIONEVENT_WALL_ALPHA_MAX		(0.6f)
#define DIVISIONEVENT_WALL_ALPHA_ADD		(0.005f)

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
bool CGame::m_bDebugCamera = false;
CGame::GAMESTATE CGame::m_gameState = GAMESTATE_NONE;
CLoadTextEffect * CGame::m_pLoadEffect = NULL;
CEventCamera * CGame::m_pEventCamera = NULL;

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
	//�ϐ��̏�����
	m_gameState = GAMESTATE_NONE;
	m_nCounterGameState = 0;
	m_NowGameState = GAMESTATE_NONE;
	m_pLoadObject = NULL;
	m_pLoadTerritory = NULL;
	m_pUI = NULL;
	m_pRotCamera = NULL;
	m_nEnemyNum = 0;
	m_pRoboCharacter = NULL;	
	m_nCntRobot = 0;			
	m_bEveCam = false;			
	m_pRobotUI = NULL;	
	m_pEventCamera = NULL; 
	m_nCounter = 0;
	m_pRingRender = NULL;	
	m_bSprintFlag = false;

	//�v���C���[�̐l�����擾
	m_nNumPlay = CSelect::GetEntryPlayer();

	for (int nCnt = 0; nCnt < m_nNumPlay; nCnt++)
	{
		m_nPlayerType[nCnt] = 0;		//�v���C���[�̃^�C�v
		m_nCountry[nCnt] = 0;			//��
		m_nControllerNum[nCnt] = 0;		//�R���g���[���[�̐�
		m_nControllerType[nCnt] = 0;	//�R���g���[���[�̃^�C�v
	}
	
	for (int nCntPlayer = 0; nCntPlayer < m_nNumPlay; nCntPlayer++)
	{
		m_pGameCamera[nCntPlayer] = NULL;
	}

	for (int nCnt = 0; nCnt < MAX_CHARACTER; nCnt++)
	{
		m_pCharacter[nCnt] = NULL;
	}

	for (int nCnt = 0; nCnt < MAX_GAMECAMERA; nCnt++)
	{
		m_pGameCamera[nCnt] = NULL;
	}

	// �G���A�{�[�i�X�C�x���g�p�ϐ�
	m_bAreaBonusEventFlag = false;
	m_nAreaBonusIdx = 0;
	m_nAreaBonusEventCounter = 0;
	m_pAreaBonusBgUp = NULL;
	m_pAreaBonusBgDown = NULL;
	m_pAreaBonusLogo[0] = NULL;
	m_pAreaBonusLogo[1] = NULL;

	// ���f�C�x���g�p�ϐ�
	m_bDivisionEventFlag = false;
	m_nDivisionDirection = 0;
	m_nDivisionEventCounter = 0;
	m_pDivisionBgUp = NULL;
	m_pDivisionBgDown = NULL;
	m_pDivisionLogo[0] = NULL;
	m_pDivisionLogo[1] = NULL;
	m_pDivisionCollider = NULL;
	m_pDivisionWall = NULL;
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
	m_pLoadObject = NULL;				//�I�u�W�F�N�g
	m_pLoadTerritory = NULL;			//�e���g���[
	m_pDebugCamera = NULL;				//�f�o�b�N�J����
	m_pFieldManager = NULL;				//�t�B�[���h
	m_nSpeedNo = 0;						//�X�s�[�h�^�̐�
	m_nPowerNo = 0;						//�p���[�^�̐�
	m_nSweeperNo = 0;					//�n���^�̐�
	m_bTypeMax = false;					//					
	m_pEffectManager = NULL;			//�G�t�F�N�g

	//�e�N�X�`���̓ǂݍ���
	CNumber2D::Load();					//����
	CTimerLogo::Load();					//�^�C�}�[�A�i�E���X
	CSkilicon::Load();					//�X�L���A�C�R��
	CEffect3D::Load();					//�G�t�F�N�g3D
	CRobotUI::Load();					//���{�b�gUI
	CExprosionTrace::LoadTex();			//�����Ձ@��
	CCrack::LoadTex();					//�Ђъ���@��
	CRipples::LoadTex();				//�g��@��

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
		m_pFieldManager->GetFieldRender(CFieldManager::FIELD_TYPE_MAIN)->SetDisp(true);
	}

#else
	//�J�E���g�_�E�����
	//CGame::SetGameState(CGame::GAMESTATE_NORMAL);
	CGame::SetGameState(CGame::GAMESTATE_FIRSTCOUNTDOWN);

	//�J�E���g�_�E���^�C�}�[�̐���
	CTimer::Create(CTimer::TYPE_COUNTER, 7, D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXVECTOR3(50.0f, 80.0f, 0.0f));

	//�Q�[���^�C�}�[�̐����i���X�g5�b�ŕ`�悳���j
	CTimer::Create(CTimer::TYPE_GAME, 7, D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXVECTOR3(50.0f, 80.0f, 0.0f));

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

	//�����O�̑�����������
	CRing::ResetMaxRing();

	//�ǂݍ��ݏ���
	if (m_pLoadObject == NULL) { m_pLoadObject = CLoadTextObject::Create(LOAD_OBJECT); }				//�I�u�W�F�N�g
	if (m_pLoadTerritory == NULL) { m_pLoadTerritory = CLoadTextTerritory::Create(LOAD_TERRITORY); }	//�e���g���[

	//�e���g���[�̃G���A��������
	CTerritory::SetErea();

	//	---<<���т��ǉ����܂���>>��	��	��	��	��	��	��	��	��	��
	for (int nCntEnemy = m_nNumPlay; nCntEnemy < ENEMY_MAX;nCntEnemy++)
	{
		//���������_���ɂ���
		int nRandCuntry = rand() % CCharacter::TYPE_MAX;

		//	�X�s�[�h
		if (m_nSpeedNo < 3 - (int)m_bTypeMax)
		{
			m_nSpeedNo += 1;
			m_pCharacter[nCntEnemy] = CTypeSpeed::Create(nCntEnemy, m_nSpeedNo, nRandCuntry, CCharacter::CHARCTERTYPE_SPEED,
				InitPos[nCntEnemy],"data\\TEXT\\ModelLoad\\SPEEDPLAYER_LOAD.txt", "data\\TEXT\\MotionLoad\\SPEEDPLAYER_MOTION.txt");
		}
		//	�]��
		else if (m_nPowerNo < 3 - (int)m_bTypeMax)
		{
			m_nPowerNo += 1;
			m_pCharacter[nCntEnemy] = CTypePower::Create(nCntEnemy, nRandCuntry, CCharacter::CHARCTERTYPE_POWER,
				InitPos[nCntEnemy], "data\\TEXT\\ModelLoad\\POWERPLAYER_LOAD.txt", "data\\TEXT\\MotionLoad\\POWERPLAYER_MOTION.txt");
		}
		//	�n��
		else if (m_nSweeperNo < 3)
		{
			m_nSweeperNo += 1;
			m_pCharacter[nCntEnemy] = CTypeSweeper::Create(nCntEnemy, nRandCuntry, CCharacter::CHARCTERTYPE_TECHNIQUE,
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
	m_pRingRender = CRingRender::Create();	

	// �e���g���[�`��N���X�𐶐�
	CTerritoryRender::Create();

	// ��̐���
	CSky::Load();
	CSky::Create();

	g_nCntGame++;
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
		m_nControllerType[nCnt] = 2;		//�R���g���[���[�̃^�C�v���擾
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
// ���{�b�g�̐���
//=============================================================================
void CGame::CreateRobot(void)
{
	m_pRoboCharacter = CRobot::Create(D3DXVECTOR3(0.0f, 1000.0f, 0.0f),
		"data\\TEXT\\ModelLoad\\ROBOT_LOAD.txt",
		"data\\TEXT\\MotionLoad\\ROBOT_MOTION.txt", ROBOT_CHARNUM, CCharacter::TYPE_MAX, CCharacter::CHARCTERTYPE_ROBOT);
	m_pRoboCharacter->CreateRobotUI(0);
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
// �C�x���g�J�����̐�������
//=============================================================================
void CGame::CreateInitEventCamera(void)
{
	// �C�x���g�J����
	if (m_pEventCamera == NULL && m_pRoboCharacter != NULL)
	{
		D3DVIEWPORT9 viewport;
		viewport.X = 0;							//�r���[�|�[�g�̍���X���W
		viewport.Y = 0;							//�r���[�|�[�g�̍���Y���W
		viewport.Width = 1280;		//��
		viewport.Height = 720;	//����
		viewport.MaxZ = 1.0f;
		viewport.MinZ = 0.0f;
		m_pEventCamera = new CEventCamera;
		m_pEventCamera->Init(D3DXVECTOR3(0.0f, 100.0f, -500.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			(CCharacter *)m_pRoboCharacter);
		m_pEventCamera->SetViewport(viewport);
	}
}

//=============================================================================
// UI�𐶐����鏈��
//=============================================================================
void CGame::CreateUI(void)
{
	//�Z���N�g�̏����擾
	CSelect *pSelect = CManager::GetSelect();

	//�S�ẴL�����N�^�[
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
			m_pUI->CreateSkilicon(m_pCharacter[nCnt], nCnt, 7 * 60);

		}
		else if (m_nPlayerType[nCnt] == 2)
		{//�n���^�v���C���[�̐���
			m_pUI->CreateSkilicon(m_pCharacter[nCnt], nCnt, 20 * 60);

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

		//�����ʒu�̐ݒ�
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

	//�Q�[���J�����̔j��
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

	// ���{�b�g�̔j��
	m_pRoboCharacter = NULL;

	// �C�x���g�J�����̔j��
	if (m_pEventCamera != NULL)
	{
		m_pEventCamera->Uninit();
		delete m_pEventCamera;
		m_pEventCamera = NULL;
	}

	//�e�N�X�`���̔j��
	CNumber2D::Unload();
	CTimerLogo::Unload();
	CSkilicon::UnLoad();
	CEffect3D::UnLoad();
	CRobotUI::UnLoad();
	CExprosionTrace::UnloadTex(); //��
	CCrack::UnloadTex();		  //��
	CRipples::UnloadTex();		  //��

	//�A�E�g���C���j��
	CModel::ShaderUnLoad();

	//��Ղ̔j��
	CSceneOrbit::Unload();

	//�n���̔j��
	CMine::UnLoad();

	// ��̔j��
	CSky::UnLoad();

	//���b�V���t�B�[���h�̔j��
	if (m_pFieldManager != NULL)
	{
		m_pFieldManager->Uninit();
		delete m_pFieldManager;
		m_pFieldManager = NULL;
	}

	// UI�̊J��
	ReleaseUI();
	ReleaseAreaBounsEventUI();

	ReleaseDivisionEventUI();

	// �C�x���g�p�̃|�C���^�ϐ�����ɂ���
	m_pDivisionCollider = NULL;
	m_pDivisionWall = NULL;

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
	{//�f�o�b�O�J�����̍X�V
		if (m_pDebugCamera != NULL) { m_pDebugCamera->Update(); }
	}

	//�t�B�[���h�}�l�[�W���[�̍X�V
	if (m_pFieldManager != NULL) { m_pFieldManager->Update(); }

	// �C�x���g�J�����̍X�V
	if (m_pEventCamera != NULL) { m_pEventCamera->Update(); }

	// UI�̍X�V����
	UpdateUI();

	// ���{�b�gUI�̍X�V����
	UpdateRobotUI();

	//�f�o�b�N�L�[
	if (pCInputKeyBoard->GetKeyboardTrigger(DIK_1) == true) { CFade::SetFade(CManager::MODE_RESULT); }
	if (pCInputKeyBoard->GetKeyboardTrigger(DIK_7) == true) { m_bDebugCamera = m_bDebugCamera ? false : true; }
	if (pCInputKeyBoard->GetKeyboardTrigger(DIK_0) == true)
	{
		StartAreaBonusEvent();
	}
	if (pCInputKeyBoard->GetKeyboardTrigger(DIK_9) == true)
	{
		StartDivisionEvent();
	}

	// ���{�b�g�C�x���g�̐���
#ifdef ROBOT_EVENT
	if (m_pRoboCharacter == NULL)
	{
		if (m_nCntRobot == 400)
		{
			CreateRobot();					// ���{�b�g�̐���
			CreateInitEventCamera();		// �C�x���g�J�����̐���
			m_nCntRobot = 0;
		}
		else
		{
			m_nCntRobot++;
		}
	}
#endif

	// �G���A�{�[�i�X�C�x���g�̒ǉ�
#ifdef AREABONUS_EVENT
	if (m_bAreaBonusEventFlag == true && m_pEventCamera == NULL)
	{
		UpdateAreaBonusEvent();
	}
	if (m_bAreaBonusEventFlag == true && m_pEventCamera != NULL)
	{
		UpdateAreaBonusUI();
	}
#endif
	// ���f�C�x���g�̒ǉ�
#ifdef AREABONUS_EVENT
	if (m_bDivisionEventFlag == true && m_pEventCamera == NULL)
	{
		UpdateDivisionEvent();
	}
	if (m_bDivisionEventFlag == true && m_pEventCamera != NULL)
	{
		UpdateDivisionUI();
	}
#endif

	// �C�x���g�J�����̍폜
	if (m_bEveCam == true && m_pEventCamera != NULL)
	{// ���S�t���O���������Ƃ�
		m_pEventCamera->Uninit();
		delete m_pEventCamera;
		m_pEventCamera = NULL;
		m_bEveCam = false;	// ���S�t���O��߂�
	}

	//m_nCounter++;
	//if (m_nCounter >= 3)
	//{
	//	if (CFade::GetFadeMode() == CFade::FADE_NONE)
	//	{
	//		CFade::SetFade(CManager::MODE_GAME);
	//		m_nCounter = 0;
	//	}
	//}

	CDebugProc::Print("�Q�[���J�E���^�[ : %d\n", g_nCntGame);
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

	// �C�x���g�J�����̕`��
	if (m_pEventCamera != NULL)
	{
		//�J�����̐ݒ�
		if (m_pEventCamera != NULL) { m_pEventCamera->Set(); }
		CManager::GetRenderer()->GetDevice()->Clear(0,
			NULL,
			(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL),
			D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);
		//���ׂĕ`�悷��
		CScene::DrawAll();

		DrawRobotUI();	// ���{�b�gUI�̕`��

		// �G���A�{�[�i�X�C�x���gUI�̕`��
		DrawAreaBounsEventUI();

		// ���f�C�x���gUI�̕`��
		DrawDivisionEventUI();
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

//=============================================================================
// ���{�b�gUI�̊J������
//=============================================================================
void CGame::ReleaseRobotUI(void)
{
	if (m_pRobotUI != NULL)
	{
		m_pRobotUI->Uninit();
		delete m_pRobotUI;
		m_pRobotUI = NULL;
	}
}

//=============================================================================
// ���{�b�gUI�̍X�V����
//=============================================================================
void CGame::UpdateRobotUI(void)
{
	CRobot *pRobot = GetRobot();	// ���{�b�g�̎擾

	if (m_pRoboCharacter != NULL)
	{
		m_pRobotUI = pRobot->GetRobotUI();

		if (m_pRobotUI == NULL) { return; }
		m_pRobotUI->Update();
	}
}

//=============================================================================
// ���{�b�gUI�̕`�揈��
//=============================================================================
void CGame::DrawRobotUI(void)
{
	if (m_pRobotUI == NULL) { return; }
	m_pRobotUI->Draw();
}

//=============================================================================
// �G���A�{�[�i�X�C�x���g���X�^�[�g�����鏈��
//=============================================================================
void CGame::StartAreaBonusEvent(void)
{
	// �t���O��ݒ�
	m_bAreaBonusEventFlag = true;

	// �J�E���^�[��������
	m_nAreaBonusEventCounter = 0;
	m_nAreaBonusUICounter = 0;

	// �{�[�i�X��t�^����G���A�̔ԍ���ݒ�
	DecideAreaBonusIdxFromRandom();

	// �C�x���g�p�J�����𐶐�
	CreateEventCamera_AreaBonusStart();

	// �w�i�𐶐�
	CreateAreaBonusBg_Start();

	// ���S�𐶐�
	CreateAreaBonusLogo_Start();

	// �n�ʂ̈ʒu��ݒ�
	SetEventFieldPosition();

	SprintFlag();
}

//=============================================================================
// �G���A�{�[�i�X�C�x���g(�J�n��)�p�J�����𐶐�����
//=============================================================================
void CGame::CreateEventCamera_AreaBonusStart(void)
{
	if (m_pEventCamera != NULL) { return; }

	// �`��̈�̐ݒ�
	D3DVIEWPORT9 viewport;
	viewport.X = 0;
	viewport.Y = 0;
	viewport.Width = SCREEN_WIDTH;
	viewport.Height = SCREEN_HEIGHT;
	viewport.MaxZ = 1.0f;
	viewport.MinZ = 0.0f;

	// �C�x���g�J�����̐���
	m_pEventCamera = new CEventCamera;
	if (m_pEventCamera != NULL)
	{
		m_pEventCamera->Init(D3DXVECTOR3(-30.0f, 2700.0f, -10.0f), D3DXVECTOR3(-30.0f, 0.0f, 70.0f),
			NULL, CEventCamera::TYPE_AREABONUS_START);
		m_pEventCamera->SetViewport(viewport);
	}
}

//=============================================================================
// �G���A�{�[�i�X�C�x���g(�J�n��)�p�w�i�𐶐�����
//=============================================================================
void CGame::CreateAreaBonusBg_Start(void)
{
	// �㑤
	if (m_pAreaBonusBgUp == NULL)
	{
		m_pAreaBonusBgUp = CIcon2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, -200.0f, 0.0f),
			D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), SCREEN_WIDTH, 300.0f);
	}

	// ����
	if (m_pAreaBonusBgDown == NULL)
	{
		m_pAreaBonusBgDown = CIcon2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT + 200.0f, 0.0f),
			D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), SCREEN_WIDTH, 300.0f);
	}
}

//=============================================================================
// �G���A�{�[�i�X�C�x���g(�J�n��)�p���S�𐶐�����
//=============================================================================
void CGame::CreateAreaBonusLogo_Start(void)
{
	if (m_pAreaBonusLogo[0] == NULL)
	{
		m_pAreaBonusLogo[0] = CIcon2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, 260.0f, 0.0f),
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f), AREABONUSEVENT_LOGO_WIDTH_START_0, AREABONUSEVENT_LOGO_HEIGHT_START_0);
		if (m_pAreaBonusLogo[0] == NULL) { return; }
		m_pAreaBonusLogo[0]->BindTexture(GetTexture()[AREABONUSEVENT_LOGO_TEXIDX_START]);
	}

	if (m_pAreaBonusLogo[1] == NULL)
	{
		m_pAreaBonusLogo[1] = CIcon2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, 440.0f, 0.0f),
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f), AREABONUSEVENT_LOGO_WIDTH_START_1, AREABONUSEVENT_LOGO_HEIGHT_START_1);
		if (m_pAreaBonusLogo[1] == NULL) { return; }
		m_pAreaBonusLogo[1]->BindTexture(GetTexture()[(AREABONUSEVENT_LOGO_TEXIDX_START + 1) + m_nAreaBonusIdx]);
	}
}

//=============================================================================
// �C�x���g�p�̒n�ʂ̈ʒu��ݒ肷��
//=============================================================================
void CGame::SetEventFieldPosition(void)
{
	// �n�ʃN���X���擾
	if (m_pFieldManager == NULL) { return; }
	CField *pField = m_pFieldManager->GetEventField();
	if (pField == NULL) { return; }

	// �n�ʂ̈ʒu��ݒ�
	D3DXVECTOR3 pos = D3DXVECTOR3(62.5f, 0.3f, 62.5f);
	switch (m_nAreaBonusIdx)
	{
	case 0:
		pos.x -= 1000.0f + 62.5f + (62.5f * 0.5f);
		pos.z += 1000.0f;
		break;
	case 1:
		pos.x += 1000.0f - (62.5f * 2.0f);
		pos.z += 1000.0f;
		break;
	case 2:
		pos.x -= 1000.0f + 62.5f + (62.5f * 0.5f);
		pos.z -= 1000.0f + 62.5f;
		break;
	case 3:
		pos.x += 1000.0f - (62.5f * 2.0f);
		pos.z -= 1000.0f + 62.5f;
		break;
	}
	pField->SetPos(pos);

	// �n�ʂ̐F��ݒ�
	pField->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));

	// �n�ʂ�`�悷��ݒ��
	m_pFieldManager->GetFieldRender(CFieldManager::FIELD_TYPE_EVENT)->SetDisp(true);
}

//=============================================================================
// �G���A�{�[�i�X�C�x���g(�I����)�p�J�����𐶐�����
//=============================================================================
void CGame::CreateEventCamera_AreaBonusFinish(void)
{
	if (m_pEventCamera != NULL) { return; }

	// �`��̈�̐ݒ�
	D3DVIEWPORT9 viewport;
	viewport.X = 0;
	viewport.Y = 0;
	viewport.Width = SCREEN_WIDTH;
	viewport.Height = SCREEN_HEIGHT;
	viewport.MaxZ = 1.0f;
	viewport.MinZ = 0.0f;

	// �C�x���g�J�����̐���
	m_pEventCamera = new CEventCamera;
	if (m_pEventCamera != NULL)
	{
		m_pEventCamera->Init(D3DXVECTOR3(-30.0f, 2700.0f, -10.0f), D3DXVECTOR3(-30.0f, 0.0f, 70.0f),
			NULL, CEventCamera::TYPE_AREABONUS_FINISH);
		m_pEventCamera->SetViewport(viewport);
	}
}

//=============================================================================
// �G���A�{�[�i�X�C�x���g(�I����)�p�w�i�𐶐�����
//=============================================================================
void CGame::CreateAreaBonusBg_Finish(void)
{
	// �㑤
	if (m_pAreaBonusBgUp == NULL)
	{
		m_pAreaBonusBgUp = CIcon2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, -200.0f, 0.0f),
			D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), SCREEN_WIDTH, 300.0f);
	}

	// ����
	if (m_pAreaBonusBgDown == NULL)
	{
		m_pAreaBonusBgDown = CIcon2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT + 200.0f, 0.0f),
			D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), SCREEN_WIDTH, 300.0f);
	}
}

//=============================================================================
// �G���A�{�[�i�X�C�x���g(�I����)�p���S�𐶐�����
//=============================================================================
void CGame::CreateAreaBonusLogo_Finish(void)
{
	if (m_pAreaBonusLogo[0] == NULL)
	{
		m_pAreaBonusLogo[0] = CIcon2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f),
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f), AREABONUSEVENT_LOGO_WIDTH_FINISH, AREABONUSEVENT_LOGO_HEIGHT_FINISH);
		if (m_pAreaBonusLogo[0] == NULL) { return; }
		m_pAreaBonusLogo[0]->BindTexture(GetTexture()[AREABONUSEVENT_LOGO_TEXIDX_FINISH]);
	}
}

//=============================================================================
// �G���A�{�[�i�X��t�^����G���A�̔ԍ��������_���Őݒ肷��
//=============================================================================
void CGame::DecideAreaBonusIdxFromRandom(void)
{
	// 4�̃G���A(����, �E��, ����, �E��)�ԍ����猈��
	m_nAreaBonusIdx = rand() % 4;
}

//=============================================================================
// �G���A�{�[�i�X�C�x���g�̏I������
//=============================================================================
void CGame::FinishAreaBonusEvent(void)
{
	// �C�x���g�p�J�����𐶐�
	CreateEventCamera_AreaBonusFinish();

	// �w�i�𐶐�
	CreateAreaBonusBg_Finish();

	// ���S�𐶐�
	CreateAreaBonusLogo_Finish();

	//�X�v�����g�T�E���h�ւ̃t���O
	SprintFlag();
}

//=============================================================================
// �G���A�{�[�i�X�C�x���g�p��UI���J�����鏈��
//=============================================================================
void CGame::ReleaseAreaBounsEventUI(void)
{
	// �w�i(�㑤)
	if (m_pAreaBonusBgUp != NULL)
	{
		m_pAreaBonusBgUp->Uninit();
		delete m_pAreaBonusBgUp;
		m_pAreaBonusBgUp = NULL;
	}

	// �w�i(����)
	if (m_pAreaBonusBgDown != NULL)
	{
		m_pAreaBonusBgDown->Uninit();
		delete m_pAreaBonusBgDown;
		m_pAreaBonusBgDown = NULL;
	}

	// ���S
	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		if (m_pAreaBonusLogo[nCnt] != NULL)
		{
			m_pAreaBonusLogo[nCnt]->Uninit();
			delete m_pAreaBonusLogo[nCnt];
			m_pAreaBonusLogo[nCnt] = NULL;
		}
	}

	//�X�v�����g�T�E���h
	SprintSound();
}

//=============================================================================
// �X�v�����g�̃T�E���h����
//=============================================================================
void CGame::SprintSound(void)
{
	CSound *pSound = CManager::GetSound();
	//�t���O�̉��(�֐�)
	if (m_bSprintFlag == true)
	{
		//�T�E���h��炷
		pSound->PlaySound(CSound::SOUND_LABEL_SE030);
		m_bSprintFlag = false;
	}
}

//=============================================================================
// �X�v�����g�̃T�E���h�ւ̃t���O����
//=============================================================================
void CGame::SprintFlag(void)
{
	CSound *pSound = CManager::GetSound();
	for (int nCnt = 0; nCnt < m_nNumPlay; nCnt++)
	{
		if (m_pCharacter[nCnt] != NULL)
		{
			bool bSprint = m_pCharacter[nCnt]->GetSprint();
			if (bSprint == true)
			{
				m_bSprintFlag = true;
				bSprint = false;
			}
			else
			{
				//�T�E���h���~�߂�
				//pSound->StopSound(CSound::SOUND_LABEL_SE030);
			}
		}
	}
}

//=============================================================================
// �C�x���g�p�̒n�ʂ��B������
//=============================================================================
void CGame::HideEventField(void)
{
	if (m_pFieldManager == NULL) { return; }
	m_pFieldManager->GetFieldRender(CFieldManager::FIELD_TYPE_EVENT)->SetDisp(false);
}

//=============================================================================
// �G���A�{�[�i�X�C�x���g�̍X�V����
//=============================================================================
void CGame::UpdateAreaBonusEvent(void)
{
	// �J�E���^�[�𑝉�
	m_nAreaBonusEventCounter++;
	if (m_nAreaBonusEventCounter >= AREABONUSEVENT_TIME)
	{// �o�ߎ��Ԃ�������x���܂���
		m_nAreaBonusUICounter = 0;
		FinishAreaBonusEvent();
	}
}

//=============================================================================
// �G���A�{�[�i�X�C�x���gUI�̍X�V����
//=============================================================================
void CGame::UpdateAreaBonusUI(void)
{
	// �J�E���^�[����
	m_nAreaBonusUICounter++;

	// �J�E���^�[�����l�܂Œ��܂�܂Ŕw�i�𓮂���
	if (m_nAreaBonusUICounter <= AREABONUSEVENT_BG_MOVETIME)
	{
		// �㑤
		D3DXVECTOR3 pos = INITIALIZE_VECTOR3;
		if (m_pAreaBonusBgUp != NULL)
		{
			pos = m_pAreaBonusBgUp->GetPos();
			pos.y += AREABONUSEVENT_BG_MOVE;
			m_pAreaBonusBgUp->SetPos(pos);
			m_pAreaBonusBgUp->SetVtxBuffPos();
		}

		// ����
		if (m_pAreaBonusBgDown != NULL)
		{
			pos = m_pAreaBonusBgDown->GetPos();
			pos.y -= AREABONUSEVENT_BG_MOVE;
			m_pAreaBonusBgDown->SetPos(pos);
			m_pAreaBonusBgDown->SetVtxBuffPos();
		}
		return;
	}

	// �����x��������
	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		if (m_pAreaBonusLogo[nCnt] != NULL)
		{
			D3DXCOLOR col = m_pAreaBonusLogo[nCnt]->GetCol();
			col.a += AREABONUSEVENT_LOGO_ALPHA;
			m_pAreaBonusLogo[nCnt]->SetCol(col);
			m_pAreaBonusLogo[nCnt]->SetVtxBuffCol();
		}
	}


}

//=============================================================================
// �G���A�{�[�i�X�C�x���g�p�̒n�ʂ̍X�V����(�J�n��)
//=============================================================================
void CGame::UpdateAreaBonusField_Start(void)
{
	if (m_nAreaBonusUICounter <= AREABONUSEVENT_BG_MOVETIME) { return; }

	// �n�ʃN���X���擾
	if (m_pFieldManager == NULL) { return; }
	CField *pField = m_pFieldManager->GetEventField();
	if (pField == NULL) { return; }

	// �����x���グ��
	D3DXCOLOR col = pField->GetCol();
	col.a += AREABONUSEVENT_FIELD_ALPHA;
	if (col.a >= 1.0f)
	{
		col.a = 1.0f;
	}
	pField->SetColor(col);
}

//=============================================================================
// �G���A�{�[�i�X�C�x���g�p�̒n�ʂ̍X�V����(�I����)
//=============================================================================
void CGame::UpdateAreaBonusField_Finish(void)
{
	if (m_nAreaBonusUICounter <= AREABONUSEVENT_BG_MOVETIME) { return; }

	// �n�ʃN���X���擾
	if (m_pFieldManager == NULL) { return; }
	CField *pField = m_pFieldManager->GetEventField();
	if (pField == NULL) { return; }

	// �����x��������
	D3DXCOLOR col = pField->GetCol();
	col.a -= AREABONUSEVENT_FIELD_ALPHA;
	if (col.a <= 0.0f)
	{
		col.a = 0.0f;
	}
	pField->SetColor(col);
}

//=============================================================================
// �G���A�{�[�i�X�C�x���g�̕`�揈��
//=============================================================================
void CGame::DrawAreaBounsEventUI(void)
{
	// �w�i(�㑤)
	if (m_pAreaBonusBgUp != NULL)
	{
		m_pAreaBonusBgUp->Draw();
	}

	// �w�i(����)
	if (m_pAreaBonusBgDown != NULL)
	{
		m_pAreaBonusBgDown->Draw();
	}

	// ���S
	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		if (m_pAreaBonusLogo[nCnt] != NULL)
		{
			m_pAreaBonusLogo[nCnt]->Draw();
		}
	}
}

//=============================================================================
// ���f�C�x���g���X�^�[�g�����鏈��
//=============================================================================
void CGame::StartDivisionEvent(void)
{
	// �t���O��ݒ�
	m_bDivisionEventFlag = true;

	// �J�E���^�[��������
	m_nDivisionEventCounter = 0;
	m_nDivisionUICounter = 0;

	// ���f��������������_���Őݒ�
	m_nDivisionDirection = rand() % 2;

	// �C�x���g�p�J�����𐶐�
	CreateEventCamera_DivisionStart();

	// �w�i�𐶐�
	CreateDivisionBg_Start();

	// ���S�𐶐�
	CreateDivisionLogo_Start();

	// ���f�p�R���C�_�[�𐶐�
	CreateDivisionCollider();

	// ���f�p�̕ǂ𐶐�
	CreateDivisionWall();

	// �L�����N�^�[�̈ʒu�����炵�Ă���
	ShiftCharaPosition();

	SprintFlag();
}

//=============================================================================
// ���f�C�x���g(�J�n��)�p�J�����𐶐�����
//=============================================================================
void CGame::CreateEventCamera_DivisionStart(void)
{
	if (m_pEventCamera != NULL) { return; }

	// �`��̈�̐ݒ�
	D3DVIEWPORT9 viewport;
	viewport.X = 0;
	viewport.Y = 0;
	viewport.Width = SCREEN_WIDTH;
	viewport.Height = SCREEN_HEIGHT;
	viewport.MaxZ = 1.0f;
	viewport.MinZ = 0.0f;

	// ���_�ʒu�̐ݒ�
	D3DXVECTOR3 posV = INITIALIZE_VECTOR3;
	posV.x = 0.0f;
	posV.y = DIVISIONEVENT_CAMERA_POSVHEIGHT;
	posV.z = DIVISIONEVENT_CAMERA_LENGTH;

	// �C�x���g�J�����̐���
	m_pEventCamera = new CEventCamera;
	if (m_pEventCamera != NULL)
	{
		m_pEventCamera->Init(posV, INITIALIZE_VECTOR3,
			NULL, CEventCamera::TYPE_DIVISION_START);
		m_pEventCamera->SetViewport(viewport);
	}
}

//=============================================================================
// ���f�C�x���g(�J�n��)�p�w�i�𐶐�����
//=============================================================================
void CGame::CreateDivisionBg_Start(void)
{
	// �㑤
	if (m_pDivisionBgUp == NULL)
	{
		m_pDivisionBgUp = CIcon2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, -200.0f, 0.0f),
			D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), SCREEN_WIDTH, 300.0f);
	}

	// ����
	if (m_pDivisionBgDown == NULL)
	{
		m_pDivisionBgDown = CIcon2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT + 200.0f, 0.0f),
			D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), SCREEN_WIDTH, 300.0f);
	}
}

//=============================================================================
// ���f�C�x���g(�J�n��)�p���S�𐶐�����
//=============================================================================
void CGame::CreateDivisionLogo_Start(void)
{
	if (m_pDivisionLogo[0] == NULL)
	{
		m_pDivisionLogo[0] = CIcon2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, 260.0f, 0.0f),
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f), DIVISIONEVENT_LOGO_WIDTH_START_0, DIVISIONEVENT_LOGO_HEIGHT_START_0);
		if (m_pDivisionLogo[0] == NULL) { return; }
		m_pDivisionLogo[0]->BindTexture(GetTexture()[DIVISIONEVENT_LOGO_TEXIDX_START]);
	}

	if (m_pDivisionLogo[1] == NULL)
	{
		m_pDivisionLogo[1] = CIcon2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, 440.0f, 0.0f),
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f), DIVISIONEVENT_LOGO_WIDTH_START_1, DIVISIONEVENT_LOGO_HEIGHT_START_1);
		if (m_pDivisionLogo[1] == NULL) { return; }
		m_pDivisionLogo[1]->BindTexture(GetTexture()[(DIVISIONEVENT_LOGO_TEXIDX_START + 1) + m_nDivisionDirection]);
	}
}

//=============================================================================
// ���f�C�x���g(�J�n��)�p�R���C�_�[�𐶐�����
//=============================================================================
void CGame::CreateDivisionCollider(void)
{
	if (m_pDivisionCollider != NULL) { return; }

	float fWidth = 0.0f;
	float fDepth = 0.0f;
	D3DXVECTOR3 pos = INITIALIZE_VECTOR3;
	switch (m_nDivisionDirection)
	{// ���f�̕����ɂ���ď����킯
	case DIVISIONEVENT_VERTICAL_IDX:
		fWidth = DIVISIONEVENT_COLLIDER_MIN;
		fDepth = DIVISIONEVENT_COLLIDER_MAX;
		pos.x -= 62.5f;
		break;
	case DIVISIONEVENT_HORIZON_IDX:
		fWidth = DIVISIONEVENT_COLLIDER_MAX;
		fDepth = DIVISIONEVENT_COLLIDER_MIN;
		pos.z += 31.25f;
		break;
	}

	// �{�b�N�X�R���C�_�[�𐶐�
	m_pDivisionCollider = CBoxCollider::Create(pos, D3DXVECTOR3(1.0f, 1.0f, 1.0f),
		fWidth, 1000.0f, fDepth, true);
}

//=============================================================================
// ���f�C�x���g(�J�n��)�p�̕ǂ𐶐�����
//=============================================================================
void CGame::CreateDivisionWall(void)
{
	if (m_pDivisionWall != NULL) { return; }
	D3DXVECTOR3 pos = INITIALIZE_VECTOR3;
	D3DXVECTOR3 rot = INITIALIZE_VECTOR3;
	switch (m_nDivisionDirection)
	{// ���f�̕����ɂ���ď����킯
	case DIVISIONEVENT_VERTICAL_IDX:
		rot.y += D3DX_PI * 0.5f;
		pos.x -= 62.5f;
		pos.z += 156.25f;
		break;
	case DIVISIONEVENT_HORIZON_IDX:
		pos.x -= 75.0f;
		pos.z += 31.25f;
		break;
	}

	// �ǂ𐶐�
	m_pDivisionWall = CDivisionWall::Create(pos, rot, DIVISIONEVENT_WALL_COL,
		DIVISIONEVENT_WALL_WIDTH, DIVISIONEVENT_WALL_HEIGHT, DIVISIONEVENT_WALL_PRIORITY, 0.0f, 0.0f,
		DIVISIONEVENT_WALL_TEXWIDTH, DIVISIONEVENT_WALL_TEXHEIGHT);

	// �e�N�X�`�������蓖�Ă�
	if (m_pDivisionWall == NULL) { return; }
	m_pDivisionWall->BindTexture(GetTexture()[DIVISIONEVENT_WALL_TEXIDX]);
}

//=============================================================================
// �L�����N�^�[�̈ʒu�����炷����(�R���C�_�[�̓����ɓ���̂�h�~����)
//=============================================================================
void CGame::ShiftCharaPosition(void)
{
	D3DXVECTOR3 CharaPos = INITIALIZE_VECTOR3;
	for (int nCntChara = 0; nCntChara < m_nNumPlay + m_nEnemyNum; nCntChara++)
	{
		// �L�����N�^�[���R���C�_�[�̓����ɂ��Ȃ�������
		if (m_pCharacter[nCntChara] == NULL) { continue; }
		CharaPos = m_pCharacter[nCntChara]->GetPos();
		if (CheckInsideCollider(CharaPos) == false) { return; }

		// �����ɂ���Ȃ�R���C�_�[�̊O���ɉ����o��
		PushOutColliderOutSide(CharaPos);

		// �L�����N�^�[�̈ʒu�̐ݒ�
		m_pCharacter[nCntChara]->SetPos(CharaPos);
	}
}

//=============================================================================
// �R���C�_�[�̓����ɂ��邩���肷�鏈��
//=============================================================================
bool CGame::CheckInsideCollider(D3DXVECTOR3 pos)
{
	if (m_pDivisionCollider == NULL) { return false; }
	D3DXVECTOR3 ColliderPos = m_pDivisionCollider->GetPos();
	float fColliderWidth = m_pDivisionCollider->GetWidth();
	float fColliderDepth = m_pDivisionCollider->GetDepth();
	if (pos.x + 15.0f >= ColliderPos.x - fColliderWidth && pos.x - 15.0f <= ColliderPos.x + fColliderWidth
		&& pos.z + 15.0f >= ColliderPos.z - fColliderDepth && pos.z - 15.0f <= ColliderPos.z + fColliderDepth)
	{// �R���C�_�[�̓����ɂ���
		return true;
	}

	return false;
}

//=============================================================================
// �R���C�_�[�̊O���ɉ����o������
//=============================================================================
void CGame::PushOutColliderOutSide(D3DXVECTOR3 &pos)
{
	if (m_pDivisionCollider == NULL) { return; }
	D3DXVECTOR3 ColliderPos = m_pDivisionCollider->GetPos();
	float fMoveX = 0.0f;
	float fMoveZ = 0.0f;
	switch (m_nDivisionDirection)
	{// ���f�̕����ɂ���ď����킯
	case DIVISIONEVENT_VERTICAL_IDX:
		fMoveX += DIVISIONEVENT_COLLIDER_MIN + 2.0f;
		break;
	case DIVISIONEVENT_HORIZON_IDX:
		fMoveZ += DIVISIONEVENT_COLLIDER_MIN + 2.0f;
		break;
	}

	// �����_���łǂ��瑤�ɉ����o�����ݒ�
	int nRandom = rand() % 2;
	switch (nRandom)
	{
	case 0:
		break;
	case 1:
		fMoveX *= -1.0f;
		fMoveZ *= -1.0f;
		break;
	}

	switch (m_nDivisionDirection)
	{// ���f�̕����ɂ���ď����킯
	case DIVISIONEVENT_VERTICAL_IDX:
		pos.x = fMoveX + ColliderPos.x;
		break;
	case DIVISIONEVENT_HORIZON_IDX:
		pos.z = fMoveZ + ColliderPos.z;
		break;
	}
}

//=============================================================================
// ���f�C�x���g(�I����)�p�J�����𐶐�����
//=============================================================================
void CGame::CreateEventCamera_DivisionFinish(void)
{
	if (m_pEventCamera != NULL) { return; }

	// �`��̈�̐ݒ�
	D3DVIEWPORT9 viewport;
	viewport.X = 0;
	viewport.Y = 0;
	viewport.Width = SCREEN_WIDTH;
	viewport.Height = SCREEN_HEIGHT;
	viewport.MaxZ = 1.0f;
	viewport.MinZ = 0.0f;

	// ���_�ʒu�̐ݒ�
	D3DXVECTOR3 posV = INITIALIZE_VECTOR3;
	posV.x = 0.0f;
	posV.y = DIVISIONEVENT_CAMERA_POSVHEIGHT;
	posV.z = DIVISIONEVENT_CAMERA_LENGTH;

	// �C�x���g�J�����̐���
	m_pEventCamera = new CEventCamera;
	if (m_pEventCamera != NULL)
	{
		m_pEventCamera->Init(posV, INITIALIZE_VECTOR3,
			NULL, CEventCamera::TYPE_DIVISION_FINISH);
		m_pEventCamera->SetViewport(viewport);
	}
}

//=============================================================================
// ���f�C�x���g(�I����)�p�w�i�𐶐�����
//=============================================================================
void CGame::CreateDivisionBg_Finish(void)
{
	// �㑤
	if (m_pDivisionBgUp == NULL)
	{
		m_pDivisionBgUp = CIcon2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, -200.0f, 0.0f),
			D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), SCREEN_WIDTH, 300.0f);
	}

	// ����
	if (m_pDivisionBgDown == NULL)
	{
		m_pDivisionBgDown = CIcon2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT + 200.0f, 0.0f),
			D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), SCREEN_WIDTH, 300.0f);
	}
}

//=============================================================================
// ���f�C�x���g(�I����)�p���S�𐶐�����
//=============================================================================
void CGame::CreateDivisionLogo_Finish(void)
{
	if (m_pDivisionLogo[0] == NULL)
	{
		m_pDivisionLogo[0] = CIcon2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f),
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f), DIVISIONEVENT_LOGO_WIDTH_FINISH, DIVISIONEVENT_LOGO_HEIGHT_FINISH);
		if (m_pDivisionLogo[0] == NULL) { return; }
		m_pDivisionLogo[0]->BindTexture(GetTexture()[DIVISIONEVENT_LOGO_TEXIDX_FINISH]);
	}
}

//=============================================================================
// ���f�C�x���g(�I����)�p�R���C�_�[���J������
//=============================================================================
void CGame::ReleaseDivisionCollider(void)
{
	if (m_pDivisionCollider != NULL)
	{
		m_pDivisionCollider->Uninit();
		m_pDivisionCollider = NULL;
	}
}

//=============================================================================
// ���f�C�x���g(�I����)�p�̕ǂ��J������
//=============================================================================
void CGame::ReleaseDivisionWall(void)
{
	if (m_pDivisionWall != NULL)
	{
		m_pDivisionWall->Uninit();
		m_pDivisionWall = NULL;
	}
}

//=============================================================================
// ���f�C�x���g�̏I������
//=============================================================================
void CGame::FinishDivisionEvent(void)
{
	// �C�x���g�p�J�����𐶐�
	CreateEventCamera_DivisionFinish();

	// �w�i�𐶐�
	CreateDivisionBg_Finish();

	// ���S�𐶐�
	CreateDivisionLogo_Finish();

	// �R���C�_�[��j��
	ReleaseDivisionCollider();

	//�X�v�����g�T�E���h�̃t���O
	SprintFlag();
}

//=============================================================================
// ���f�C�x���g�p��UI���J�����鏈��
//=============================================================================
void CGame::ReleaseDivisionEventUI(void)
{
	// �w�i(�㑤)
	if (m_pDivisionBgUp != NULL)
	{
		m_pDivisionBgUp->Uninit();
		delete m_pDivisionBgUp;
		m_pDivisionBgUp = NULL;
	}

	// �w�i(����)
	if (m_pDivisionBgDown != NULL)
	{
		m_pDivisionBgDown->Uninit();
		delete m_pDivisionBgDown;
		m_pDivisionBgDown = NULL;
	}

	// ���S
	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		if (m_pDivisionLogo[nCnt] != NULL)
		{
			m_pDivisionLogo[nCnt]->Uninit();
			delete m_pDivisionLogo[nCnt];
			m_pDivisionLogo[nCnt] = NULL;
		}
	}

	//�X�v�����g�T�E���h
	SprintSound();
}

//=============================================================================
// ���f�C�x���g�̍X�V����
//=============================================================================
void CGame::UpdateDivisionEvent(void)
{
	// �J�E���^�[�𑝉�
	m_nDivisionEventCounter++;
	if (m_nDivisionEventCounter >= DIVISIONEVENT_TIME)
	{// �o�ߎ��Ԃ�������x���܂���
		m_nDivisionUICounter = 0;
		FinishDivisionEvent();
	}
}

//=============================================================================
// ���f�C�x���g�pUI�̍X�V����
//=============================================================================
void CGame::UpdateDivisionUI(void)
{
	// �J�E���^�[����
	m_nDivisionUICounter++;

	// �J�E���^�[�����l�܂Œ��܂�܂Ŕw�i�𓮂���
	if (m_nDivisionUICounter <= DIVISIONEVENT_BG_MOVETIME)
	{
		// �㑤
		D3DXVECTOR3 pos = INITIALIZE_VECTOR3;
		if (m_pDivisionBgUp != NULL)
		{
			pos = m_pDivisionBgUp->GetPos();
			pos.y += DIVISIONEVENT_BG_MOVE;
			m_pDivisionBgUp->SetPos(pos);
			m_pDivisionBgUp->SetVtxBuffPos();
		}

		// ����
		if (m_pDivisionBgDown != NULL)
		{
			pos = m_pDivisionBgDown->GetPos();
			pos.y -= DIVISIONEVENT_BG_MOVE;
			m_pDivisionBgDown->SetPos(pos);
			m_pDivisionBgDown->SetVtxBuffPos();
		}
		return;
	}

	// �����x��������
	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		if (m_pDivisionLogo[nCnt] != NULL)
		{
			D3DXCOLOR col = m_pDivisionLogo[nCnt]->GetCol();
			col.a += DIVISIONEVENT_LOGO_ALPHA;
			m_pDivisionLogo[nCnt]->SetCol(col);
			m_pDivisionLogo[nCnt]->SetVtxBuffCol();
		}
	}
}

//=============================================================================
// ���f�C�x���g�p��(�J�n��)�̍X�V����
//=============================================================================
void CGame::UpdateDivisionWall_Start(void)
{
	if (m_pDivisionWall == NULL) { return; }

	// �����x���グ��
	D3DXCOLOR col = m_pDivisionWall->GetCol();
	col.a += DIVISIONEVENT_WALL_ALPHA_ADD;
	if (col.a >= DIVISIONEVENT_WALL_ALPHA_MAX)
	{
		col.a = DIVISIONEVENT_WALL_ALPHA_MAX;
	}
	m_pDivisionWall->SetCol(col);
	m_pDivisionWall->SetVtxBuffCol();
}

//=============================================================================
// ���f�C�x���g�p��(�I����)�̍X�V����
//=============================================================================
void CGame::UpdateDivisionWall_Finish(void)
{
	if (m_pDivisionWall == NULL) { return; }

	// �����x��������
	D3DXCOLOR col = m_pDivisionWall->GetCol();
	col.a -= DIVISIONEVENT_WALL_ALPHA_ADD;
	if (col.a <= 0.0f)
	{
		col.a = 0.0f;
	}
	m_pDivisionWall->SetCol(col);
	m_pDivisionWall->SetVtxBuffCol();
}

//=============================================================================
// ���f�C�x���g�̕`�揈��
//=============================================================================
void CGame::DrawDivisionEventUI(void)
{
	// �w�i(�㑤)
	if (m_pDivisionBgUp != NULL)
	{
		m_pDivisionBgUp->Draw();
	}

	// �w�i(����)
	if (m_pDivisionBgDown != NULL)
	{
		m_pDivisionBgDown->Draw();
	}

	// ���S
	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		if (m_pDivisionLogo[nCnt] != NULL)
		{
			m_pDivisionLogo[nCnt]->Draw();
		}
	}
}