//=============================================================================
//
// ゲームの処理 [game.cpp]
// Author : 佐藤安純 ： 山下敦史 ： 小林将兵 ： 丹羽保貴 ： 荒谷由朗
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
#include "exprotiontrace.h" //安
#include "crack.h"			//安
#include "ripples.h"		//安

int g_nCntGame = 0;
//*****************************************************************************
// マクロ定義
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

// デバッグ
#define DEBUG_NUMPLAYER				 (2)
#define DEBUG_CAMERA				 (D3DXVECTOR3(0.0f,2500.0f,180.0f))

// 回転カメラ用(3人プレイ時のみ適用する)
#define ROTATIONCAMERA_POSVHEIGHT	 (500.0f)
#define ROTATIONCAMERA_POSR			 (D3DXVECTOR3(0.0f,0.0f,0.0f))
#define ROTATIONCAMERA_ROTSPEED		 (0.003f)
#define ROTATIONCAMERA_LENGTH		 (700.0f)

// プレイヤー配置用
#define PLAYER_SPAWN_LENGTH_X		 (1100.0f)
#define PLAYER_SPAWN_LENGTH_Z		 (1090.0f)
#define PLAYER_SPAWN_RIVISION_X		 (-90.0f)
#define PLAYER_SPAWN_RIVISION_Z		 (170.0f)

// マップの見えない壁配置用
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

// エフェクトデータのファイル名
#define EFFECTMANAGER_FILENAME "data/TEXT/EFFECT/ringEffect.txt"

// エリアボーナスイベント用
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

// 分断イベント用
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
// 静的メンバ変数
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
// デフォルトコンストラクタ
//=============================================================================
CGame::CGame()
{
	//変数の初期化
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

	//プレイヤーの人数を取得
	m_nNumPlay = CSelect::GetEntryPlayer();

	for (int nCnt = 0; nCnt < m_nNumPlay; nCnt++)
	{
		m_nPlayerType[nCnt] = 0;		//プレイヤーのタイプ
		m_nCountry[nCnt] = 0;			//国
		m_nControllerNum[nCnt] = 0;		//コントローラーの数
		m_nControllerType[nCnt] = 0;	//コントローラーのタイプ
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

	// エリアボーナスイベント用変数
	m_bAreaBonusEventFlag = false;
	m_nAreaBonusIdx = 0;
	m_nAreaBonusEventCounter = 0;
	m_pAreaBonusBgUp = NULL;
	m_pAreaBonusBgDown = NULL;
	m_pAreaBonusLogo[0] = NULL;
	m_pAreaBonusLogo[1] = NULL;

	// 分断イベント用変数
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
// デストラクタ
//=============================================================================
CGame::~CGame(){}

//=============================================================================
// 初期化処理
//=============================================================================
void CGame::Init(void)
{
	//ベースの初期化
	CModeBase::Init();

	//変数の初期化
	m_pLoadObject = NULL;				//オブジェクト
	m_pLoadTerritory = NULL;			//テリトリー
	m_pDebugCamera = NULL;				//デバックカメラ
	m_pFieldManager = NULL;				//フィールド
	m_nSpeedNo = 0;						//スピード型の数
	m_nPowerNo = 0;						//パワー型の数
	m_nSweeperNo = 0;					//地雷型の数
	m_bTypeMax = false;					//					
	m_pEffectManager = NULL;			//エフェクト

	//テクスチャの読み込み
	CNumber2D::Load();					//数字
	CTimerLogo::Load();					//タイマーアナウンス
	CSkilicon::Load();					//スキルアイコン
	CEffect3D::Load();					//エフェクト3D
	CRobotUI::Load();					//ロボットUI
	CExprosionTrace::LoadTex();			//爆発跡　安
	CCrack::LoadTex();					//ひび割れ　安
	CRipples::LoadTex();				//波紋　安

	//奇跡の読み込み
	CSceneOrbit::Load();

	//地雷の読み込み
	CMine::Load();

	//アウトライン読み込み
	CModel::ShaderLoad();

	//エフェクトの読み込み
	if (m_pLoadEffect == NULL)
	{
		m_pLoadEffect = CLoadTextEffect::Create(LOAD_EFFECT);
		CParticle::LoadTexture();
		CEffectTool::LoadEffect();
	}
#ifdef GAME_DEBUG
	//カウントダウン状態
	CGame::SetGameState(CGame::GAMESTATE_NORMAL);

	//地面の生成
	if (m_pFieldManager == NULL)
	{
		m_pFieldManager = new CFieldManager;
		m_pFieldManager->Init();
		m_pFieldManager->GetFieldRender(CFieldManager::FIELD_TYPE_MAIN)->SetDisp(true);
	}

#else
	//カウントダウン状態
	//CGame::SetGameState(CGame::GAMESTATE_NORMAL);
	CGame::SetGameState(CGame::GAMESTATE_FIRSTCOUNTDOWN);

	//カウントダウンタイマーの生成
	CTimer::Create(CTimer::TYPE_COUNTER, 7, D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXVECTOR3(50.0f, 80.0f, 0.0f));

	//ゲームタイマーの生成（ラスト5秒で描画される）
	CTimer::Create(CTimer::TYPE_GAME, 7, D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXVECTOR3(50.0f, 80.0f, 0.0f));

	//地面の生成
	if (m_pFieldManager == NULL)
	{
		m_pFieldManager = new CFieldManager;
		m_pFieldManager->Init();
		m_pFieldManager->SetState(CFieldManager::STATE_RANDOM);
	}

#endif
	// 各キャラクターの位置を設定する処理
	SettingCharaPosition();

	//プレイヤーの生成
	CreateInitPlayer();
	
	//ゲームカメラの生成
	CreateInitCamera();

	//デバックカメラの生成
	if (m_pDebugCamera == NULL)
	{
		D3DVIEWPORT9 viewport;
		viewport.X = 0;		//ビューポートの左上X座標
		viewport.Y = 0;		//ビューポートの左上Y座標
		viewport.Width = SCREEN_WIDTH;	//幅
		viewport.Height = SCREEN_HEIGHT;//高さ
		viewport.MaxZ = 1.0f;
		viewport.MinZ = 0.0f;

		m_pDebugCamera = new CCamera;
		m_pDebugCamera->Init();
		m_pDebugCamera->SetViewport(viewport);
		m_pDebugCamera->SetPosV(DEBUG_CAMERA);
		//m_pDebugCamera->SetPosV(D3DXVECTOR3(14.0f, 255.0f, -204.0f));
	}

	//リングの総数を初期化
	CRing::ResetMaxRing();

	//読み込み処理
	if (m_pLoadObject == NULL) { m_pLoadObject = CLoadTextObject::Create(LOAD_OBJECT); }				//オブジェクト
	if (m_pLoadTerritory == NULL) { m_pLoadTerritory = CLoadTextTerritory::Create(LOAD_TERRITORY); }	//テリトリー

	//テリトリーのエリア分け処理
	CTerritory::SetErea();

	//	---<<小林が追加しました>>★	★	★	★	★	★	★	★	★	★
	for (int nCntEnemy = m_nNumPlay; nCntEnemy < ENEMY_MAX;nCntEnemy++)
	{
		//国をランダムにする
		int nRandCuntry = rand() % CCharacter::TYPE_MAX;

		//	スピード
		if (m_nSpeedNo < 3 - (int)m_bTypeMax)
		{
			m_nSpeedNo += 1;
			m_pCharacter[nCntEnemy] = CTypeSpeed::Create(nCntEnemy, m_nSpeedNo, nRandCuntry, CCharacter::CHARCTERTYPE_SPEED,
				InitPos[nCntEnemy],"data\\TEXT\\ModelLoad\\SPEEDPLAYER_LOAD.txt", "data\\TEXT\\MotionLoad\\SPEEDPLAYER_MOTION.txt");
		}
		//	脳筋
		else if (m_nPowerNo < 3 - (int)m_bTypeMax)
		{
			m_nPowerNo += 1;
			m_pCharacter[nCntEnemy] = CTypePower::Create(nCntEnemy, nRandCuntry, CCharacter::CHARCTERTYPE_POWER,
				InitPos[nCntEnemy], "data\\TEXT\\ModelLoad\\POWERPLAYER_LOAD.txt", "data\\TEXT\\MotionLoad\\POWERPLAYER_MOTION.txt");
		}
		//	地雷
		else if (m_nSweeperNo < 3)
		{
			m_nSweeperNo += 1;
			m_pCharacter[nCntEnemy] = CTypeSweeper::Create(nCntEnemy, nRandCuntry, CCharacter::CHARCTERTYPE_TECHNIQUE,
				InitPos[nCntEnemy], "data\\TEXT\\ModelLoad\\TEQPLAYER_LOAD.txt", "data\\TEXT\\MotionLoad\\TEQPLAYER_MOTION.txt");
		}

		m_nEnemyNum++;
		m_nEnemyNumResult++;
	}
	//	★	★	★	★	★	★	★	★	★	★	★	★	★	★	★	★

	// テクスチャを読み込む
	LoadUI(LOAD_UI);

	// UIを生成する処理
	CreateUI();

	// マップの見えない壁を配置する
	CreateMapCollider();

	//観客を生成
	CAudience::Create();

	// エフェクト管轄クラスを生成
	m_pEffectManager = CEffectManager::Create(EFFECTMANAGER_FILENAME);

	// リング描画クラスを生成
	m_pRingRender = CRingRender::Create();	

	// テリトリー描画クラスを生成
	CTerritoryRender::Create();

	// 空の生成
	CSky::Load();
	CSky::Create();

	g_nCntGame++;
}

//=============================================================================
// プレイヤーの生成処理
//=============================================================================
void CGame::CreateInitPlayer(void)
{
	//セレクトの情報を取得
	CSelect *pSelect = CManager::GetSelect();
	m_nNumPlay = CSelect::GetEntryPlayer();
	if (m_nNumPlay == 0)
	{
		m_nNumPlay = DEBUG_NUMPLAYER;
	}
	
	for (int nCnt = 0; nCnt < m_nNumPlay; nCnt++)
	{//プレイ人数分回す
#ifdef GAME_DEBUG
	 //デバッグ用
		m_nPlayerType[nCnt] = CCharacter::CHARCTERTYPE_TECHNIQUE;
		m_nCountry[nCnt] = CCharacter::TYPE_AMERICA;
		m_nControllerNum[nCnt] = 0;	//コントローラーの番号を取得
		m_nControllerType[nCnt] = 2;		//コントローラーのタイプを取得
#else
		m_nPlayerType[nCnt] = pSelect->GetPlayerType(nCnt);			//プレイヤーのタイプを所得
		m_nCountry[nCnt] = pSelect->GetPlayerCuntry(nCnt);			//国を取得
		m_nControllerNum[nCnt] = pSelect->GetControlerNum(nCnt);	//コントローラーの番号を取得
		m_nControllerType[nCnt] = pSelect->GetInputType(nCnt);		//コントローラーのタイプを取得
#endif

		if (m_nPlayerType[nCnt] == 0)
		{//スピード型プレイヤーの生成
			m_nSpeedNo += 1;

			m_pCharacter[nCnt] = CSpeedPlayer::Create(InitPos[nCnt],
				"data\\TEXT\\ModelLoad\\SPEEDPLAYER_LOAD.txt",
				"data\\TEXT\\MotionLoad\\SPEEDPLAYER_MOTION.txt", nCnt, m_nCountry[nCnt],
				CCharacter::CHARCTERTYPE_SPEED, m_nControllerNum[nCnt], m_nControllerType[nCnt]);	//体下
		}
		else if (m_nPlayerType[nCnt] == 1)
		{//パワー型プレイヤーの生成
			m_nPowerNo += 1;

			m_pCharacter[nCnt] = CPowerPlayer::Create(InitPos[nCnt],
				"data\\TEXT\\ModelLoad\\POWERPLAYER_LOAD.txt",
				"data\\TEXT\\MotionLoad\\POWERPLAYER_MOTION.txt", nCnt, m_nCountry[nCnt],
				CCharacter::CHARCTERTYPE_POWER, m_nControllerNum[nCnt], m_nControllerType[nCnt]);
		}
		else if (m_nPlayerType[nCnt] == 2)
		{//地雷型プレイヤーの生成
			m_nSweeperNo += 1;

			m_pCharacter[nCnt] = CTechniquePlayer::Create(InitPos[nCnt],
				"data\\TEXT\\ModelLoad\\TEQPLAYER_LOAD.txt",
				"data\\TEXT\\MotionLoad\\TEQPLAYER_MOTION.txt", nCnt, m_nCountry[nCnt],
				CCharacter::CHARCTERTYPE_TECHNIQUE, m_nControllerNum[nCnt], m_nControllerType[nCnt]);
		}
	}
}

//=============================================================================
// ロボットの生成
//=============================================================================
void CGame::CreateRobot(void)
{
	m_pRoboCharacter = CRobot::Create(D3DXVECTOR3(0.0f, 1000.0f, 0.0f),
		"data\\TEXT\\ModelLoad\\ROBOT_LOAD.txt",
		"data\\TEXT\\MotionLoad\\ROBOT_MOTION.txt", ROBOT_CHARNUM, CCharacter::TYPE_MAX, CCharacter::CHARCTERTYPE_ROBOT);
	m_pRoboCharacter->CreateRobotUI(0);
}

//=============================================================================
// ゲームカメラの生成処理
//=============================================================================
void CGame::CreateInitCamera(void)
{
	if (m_nNumPlay == 1)
	{//プレイ人数が1人だったら
		if (m_pGameCamera[0] == NULL)
		{//カメラの生成(1P:左上)
			D3DVIEWPORT9 viewport;
			viewport.X = 0;							//ビューポートの左上X座標
			viewport.Y = 0;							//ビューポートの左上Y座標
			viewport.Width = 1280;		//幅
			viewport.Height = 720;	//高さ
			viewport.MaxZ = 1.0f;
			viewport.MinZ = 0.0f;
			m_pGameCamera[0] = new CGameCamera;
			m_pGameCamera[0]->Init((CPlayer *)m_pCharacter[0]);
			m_pGameCamera[0]->SetViewport(viewport);
		}
	}
	else if (m_nNumPlay == 2)
	{//プレイ人数が2人だったら
		if (m_pGameCamera[0] == NULL)
		{//カメラの生成(1P:左上)
			D3DVIEWPORT9 viewport;
			viewport.X = 5;							//ビューポートの左上X座標
			viewport.Y = 5;							//ビューポートの左上Y座標
			viewport.Width = 1260 / 2;		//幅
			viewport.Height = 700;	//高さ
			viewport.MaxZ = 1.0f;
			viewport.MinZ = 0.0f;
			m_pGameCamera[0] = new CGameCamera;
			m_pGameCamera[0]->Init((CPlayer *)m_pCharacter[0]);
			m_pGameCamera[0]->SetViewport(viewport);
		}
		if (m_pGameCamera[1] == NULL)
		{//カメラの生成(2P:右上)
			D3DVIEWPORT9 viewport;
			viewport.X = (SCREEN_WIDTH + 8) / 2;			//ビューポートの左上X座標
			viewport.Y = 5;							//ビューポートの左上Y座標
			viewport.Width = 1260 / 2;		//幅
			viewport.Height = 700;	//高さ
			viewport.MaxZ = 1.0f;
			viewport.MinZ = 0.0f;
			m_pGameCamera[1] = new CGameCamera;
			m_pGameCamera[1]->Init((CPlayer *)m_pCharacter[1]);
			m_pGameCamera[1]->SetViewport(viewport);
		}
	}
	else if (m_nNumPlay == 3)
	{//プレイ人数が3人だったら
		if (m_pGameCamera[0] == NULL)
		{//カメラの生成(1P:左上)
			D3DVIEWPORT9 viewport;
			viewport.X = 5;							//ビューポートの左上X座標
			viewport.Y = 5;							//ビューポートの左上Y座標
			viewport.Width = 1260 / 2;		//幅
			viewport.Height = 700 / 2;	//高さ
			viewport.MaxZ = 1.0f;
			viewport.MinZ = 0.0f;
			m_pGameCamera[0] = new CGameCamera;
			m_pGameCamera[0]->Init((CPlayer *)m_pCharacter[0]);
			m_pGameCamera[0]->SetViewport(viewport);
		}
		if (m_pGameCamera[1] == NULL)
		{//カメラの生成(2P:右上)
			D3DVIEWPORT9 viewport;
			viewport.X = (SCREEN_WIDTH + 8) / 2;			//ビューポートの左上X座標
			viewport.Y = 5;							//ビューポートの左上Y座標
			viewport.Width = 1260 / 2;		//幅
			viewport.Height = 700 / 2;	//高さ
			viewport.MaxZ = 1.0f;
			viewport.MinZ = 0.0f;
			m_pGameCamera[1] = new CGameCamera;
			m_pGameCamera[1]->Init((CPlayer *)m_pCharacter[1]);
			m_pGameCamera[1]->SetViewport(viewport);
		}
		if (m_pGameCamera[2] == NULL)
		{//カメラの生成(3P:左下)
			D3DVIEWPORT9 viewport;
			viewport.X = 5;						//ビューポートの左上X座標
			viewport.Y = (SCREEN_HEIGHT + 5) / 2;		//ビューポートの左上Y座標
			viewport.Width = 1260 / 2;	//幅
			viewport.Height = 700 / 2;//高さ
			viewport.MaxZ = 1.0f;
			viewport.MinZ = 0.0f;
			m_pGameCamera[2] = new CGameCamera;
			m_pGameCamera[2]->Init((CPlayer *)m_pCharacter[2]);
			m_pGameCamera[2]->SetViewport(viewport);
		}
		if (m_pRotCamera == NULL)
		{//カメラの生成(4P:右下)
			D3DVIEWPORT9 viewport;
			viewport.X = (SCREEN_WIDTH + 8) / 2;		//ビューポートの左上X座標
			viewport.Y = (SCREEN_HEIGHT + 5) / 2;		//ビューポートの左上Y座標
			viewport.Width = 1260 / 2;	//幅
			viewport.Height = 700 / 2;//高さ
			viewport.MaxZ = 1.0f;
			viewport.MinZ = 0.0f;
			m_pRotCamera = CRotationCamera::Create(ROTATIONCAMERA_POSVHEIGHT, ROTATIONCAMERA_POSR,
				ROTATIONCAMERA_ROTSPEED, ROTATIONCAMERA_LENGTH);
			m_pRotCamera->SetViewport(viewport);
		}
	}
	else if (m_nNumPlay == 4)
	{//プレイ人数が4人だったら
		if (m_pGameCamera[0] == NULL)
		{//カメラの生成(1P:左上)
			D3DVIEWPORT9 viewport;
			viewport.X = 5;							//ビューポートの左上X座標
			viewport.Y = 5;							//ビューポートの左上Y座標
			viewport.Width = 1260 / 2;		//幅
			viewport.Height = 700 / 2;	//高さ
			viewport.MaxZ = 1.0f;
			viewport.MinZ = 0.0f;
			m_pGameCamera[0] = new CGameCamera;
			m_pGameCamera[0]->Init((CPlayer *)m_pCharacter[0]);
			m_pGameCamera[0]->SetViewport(viewport);
		}
		if (m_pGameCamera[1] == NULL)
		{//カメラの生成(2P:右上)
			D3DVIEWPORT9 viewport;
			viewport.X = (SCREEN_WIDTH + 8) / 2;			//ビューポートの左上X座標
			viewport.Y = 5;							//ビューポートの左上Y座標
			viewport.Width = 1260 / 2;		//幅
			viewport.Height = 700 / 2;	//高さ
			viewport.MaxZ = 1.0f;
			viewport.MinZ = 0.0f;
			m_pGameCamera[1] = new CGameCamera;
			m_pGameCamera[1]->Init((CPlayer *)m_pCharacter[1]);
			m_pGameCamera[1]->SetViewport(viewport);
		}
		if (m_pGameCamera[2] == NULL)
		{//カメラの生成(3P:左下)
			D3DVIEWPORT9 viewport;
			viewport.X = 5;						//ビューポートの左上X座標
			viewport.Y = (SCREEN_HEIGHT + 5) / 2;		//ビューポートの左上Y座標
			viewport.Width = 1260 / 2;	//幅
			viewport.Height = 700 / 2;//高さ
			viewport.MaxZ = 1.0f;
			viewport.MinZ = 0.0f;
			m_pGameCamera[2] = new CGameCamera;
			m_pGameCamera[2]->Init((CPlayer *)m_pCharacter[2]);
			m_pGameCamera[2]->SetViewport(viewport);
		}
		if (m_pGameCamera[3] == NULL)
		{//カメラの生成(4P:右下)
			D3DVIEWPORT9 viewport;
			viewport.X = (SCREEN_WIDTH + 8) / 2;		//ビューポートの左上X座標
			viewport.Y = (SCREEN_HEIGHT + 5) / 2;		//ビューポートの左上Y座標
			viewport.Width = 1260 / 2;	//幅
			viewport.Height = 700 / 2;//高さ
			viewport.MaxZ = 1.0f;
			viewport.MinZ = 0.0f;
			m_pGameCamera[3] = new CGameCamera;
			m_pGameCamera[3]->Init((CPlayer *)m_pCharacter[3]);
			m_pGameCamera[3]->SetViewport(viewport);
		}
	}
}

//=============================================================================
// イベントカメラの生成処理
//=============================================================================
void CGame::CreateInitEventCamera(void)
{
	// イベントカメラ
	if (m_pEventCamera == NULL && m_pRoboCharacter != NULL)
	{
		D3DVIEWPORT9 viewport;
		viewport.X = 0;							//ビューポートの左上X座標
		viewport.Y = 0;							//ビューポートの左上Y座標
		viewport.Width = 1280;		//幅
		viewport.Height = 720;	//高さ
		viewport.MaxZ = 1.0f;
		viewport.MinZ = 0.0f;
		m_pEventCamera = new CEventCamera;
		m_pEventCamera->Init(D3DXVECTOR3(0.0f, 100.0f, -500.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			(CCharacter *)m_pRoboCharacter);
		m_pEventCamera->SetViewport(viewport);
	}
}

//=============================================================================
// UIを生成する処理
//=============================================================================
void CGame::CreateUI(void)
{
	//セレクトの情報を取得
	CSelect *pSelect = CManager::GetSelect();

	//全てのキャラクター
	int nAllCharacter = m_nNumPlay + m_nEnemyNum;

	// メモリを確保
	if (m_pUI != NULL) { return; }
	m_pUI = CUI::Create(m_nNumPlay, nAllCharacter);

	// スキルアイコンを生成
	for (int nCnt = 0; nCnt < m_nNumPlay; nCnt++)
	{
		m_nPlayerType[nCnt] = pSelect->GetPlayerType(nCnt);			//プレイヤーのタイプを所得

		if (m_nPlayerType[nCnt] == 0)
		{//スピード型プレイヤーの生成
			m_pUI->CreateSkilicon(m_pCharacter[nCnt], nCnt, 20 * 60);	
		}
		else if (m_nPlayerType[nCnt] == 1)
		{//パワー型プレイヤーの生成
			m_pUI->CreateSkilicon(m_pCharacter[nCnt], nCnt, 7 * 60);

		}
		else if (m_nPlayerType[nCnt] == 2)
		{//地雷型プレイヤーの生成
			m_pUI->CreateSkilicon(m_pCharacter[nCnt], nCnt, 20 * 60);

		}
	}
}

//=============================================================================
// 各キャラクターの位置を設定する処理
//=============================================================================
void CGame::SettingCharaPosition(void)
{
	D3DXVECTOR3 pos = VECTOR_ZERO;	// 位置
	float fAngle = 0.0f;			// 位置の計算用
	int nSetCharaNum = 0;			// キャラを何人配置したか
	int nRandomChara = 0;			// 配置するキャラの種類分け用
	int nSetIdx = 0;				// 配置位置ランダム用
	bool bSet[MAX_PLAYERNUM * 2];	// セットしたかどうか
	ZeroMemory(&bSet, sizeof(bool) * 8);

	// 8角形上にランダムに敵とプレイヤーを配置する
	for (int nCnt = 0; nCnt < MAX_PLAYERNUM * 2; nCnt++)
	{// プレイ人数分繰り返し
		while (1)
		{// 配置する位置を決める
			nSetIdx = rand() % (MAX_PLAYERNUM * 2);
			if (bSet[nSetIdx] == false)
			{
				bSet[nSetIdx] = true;
				fAngle = 0.0f;
				break;
			}
		}

		// 角度を進める
		fAngle += ((D3DX_PI * 2.0f) / (MAX_PLAYERNUM * 2.0f)) * nSetIdx;
		if (fAngle > D3DX_PI)
		{
			fAngle -= D3DX_PI * 2.0f;
		}

		// 位置を計算
		pos.x = sinf(fAngle) * PLAYER_SPAWN_LENGTH_X + PLAYER_SPAWN_RIVISION_X;
		pos.z = cosf(fAngle) * PLAYER_SPAWN_LENGTH_Z + PLAYER_SPAWN_RIVISION_Z;

		//初期位置の設定
		InitPos[nSetCharaNum] = pos;
		nSetCharaNum++;
	}
}


//=============================================================================
// マップの見えない壁を生成する処理
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
// 終了処理
//=============================================================================
void CGame::Uninit(void)
{	
	//スコアの情報
	GetCharInfo();

	//-----------------
	//		Uninit
	//-----------------

	//ゲームカメラの破棄
	for (int nCnt = 0; nCnt < m_nNumPlay; nCnt++)
	{
		if (m_pGameCamera[nCnt] != NULL)
		{
			m_pGameCamera[nCnt]->Uninit();
			delete m_pGameCamera[nCnt];
			m_pGameCamera[nCnt] = NULL;
		}
	}

	//回転カメラの破棄
	if (m_pRotCamera != NULL)
	{
		m_pRotCamera->Uninit();
		delete m_pRotCamera;
		m_pRotCamera = NULL;
	}

	//デバックカメラの破棄
	if (m_pDebugCamera != NULL)
	{
		m_pDebugCamera->Uninit();
		delete m_pDebugCamera;
		m_pDebugCamera = NULL;
	}

	//変数の初期化
	m_nNumPlay = 0;

	//オブジェクトの破棄
	if (m_pLoadObject != NULL)
	{
		m_pLoadObject->Uninit();
		delete m_pLoadObject;
		m_pLoadObject = NULL;
	}

	//テリトリーの破棄
	if (m_pLoadTerritory != NULL)
	{
		m_pLoadTerritory->Uninit();
		delete m_pLoadTerritory;
		m_pLoadTerritory = NULL;
	}

	//エフェクトの破棄
	if (m_pLoadEffect != NULL)
	{
		CEffectTool::UnloadEffect();
		CParticle::UnloadTexture();

		m_pLoadEffect->Uninit();
		delete m_pLoadEffect;
		m_pLoadEffect = NULL;
	}

	// エフェクト管轄クラスの破棄
	if (m_pEffectManager != NULL)
	{
		m_pEffectManager->Uninit();
		delete m_pEffectManager;
		m_pEffectManager = NULL;
	}

	// ロボットの破棄
	m_pRoboCharacter = NULL;

	// イベントカメラの破棄
	if (m_pEventCamera != NULL)
	{
		m_pEventCamera->Uninit();
		delete m_pEventCamera;
		m_pEventCamera = NULL;
	}

	//テクスチャの破棄
	CNumber2D::Unload();
	CTimerLogo::Unload();
	CSkilicon::UnLoad();
	CEffect3D::UnLoad();
	CRobotUI::UnLoad();
	CExprosionTrace::UnloadTex(); //安
	CCrack::UnloadTex();		  //安
	CRipples::UnloadTex();		  //安

	//アウトライン破棄
	CModel::ShaderUnLoad();

	//奇跡の破棄
	CSceneOrbit::Unload();

	//地雷の破棄
	CMine::UnLoad();

	// 空の破棄
	CSky::UnLoad();

	//メッシュフィールドの破棄
	if (m_pFieldManager != NULL)
	{
		m_pFieldManager->Uninit();
		delete m_pFieldManager;
		m_pFieldManager = NULL;
	}

	// UIの開放
	ReleaseUI();
	ReleaseAreaBounsEventUI();

	ReleaseDivisionEventUI();

	// イベント用のポインタ変数を空にする
	m_pDivisionCollider = NULL;
	m_pDivisionWall = NULL;

	//ベースの破棄
	CModeBase::Uninit();

}

//=============================================================================
// UIクラスの開放処理
//=============================================================================
void CGame::ReleaseUI(void)
{
	if (m_pUI != NULL)
	{
		//スコアを取得する
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
// 更新処理
//=============================================================================
void CGame::Update(void)
{
	//入力情報
	CInputKeyboard *pCInputKeyBoard = CManager::GetInputkeyboard();

	//サウンドの情報
	CSound *pSound = CManager::GetSound();

	if (!m_bDebugCamera)
	{
		//ゲームカメラの更新処理
		for (int nCnt = 0; nCnt < m_nNumPlay; nCnt++)
		{
			if (m_pGameCamera[nCnt] != NULL)
			{
				m_pGameCamera[nCnt]->Update();
			}
		}
		if (m_pRotCamera != NULL)
		{//回転カメラの更新
			m_pRotCamera->Update();
		}
	}
	else
	{//デバッグカメラの更新
		if (m_pDebugCamera != NULL) { m_pDebugCamera->Update(); }
	}

	//フィールドマネージャーの更新
	if (m_pFieldManager != NULL) { m_pFieldManager->Update(); }

	// イベントカメラの更新
	if (m_pEventCamera != NULL) { m_pEventCamera->Update(); }

	// UIの更新処理
	UpdateUI();

	// ロボットUIの更新処理
	UpdateRobotUI();

	//デバックキー
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

	// ロボットイベントの生成
#ifdef ROBOT_EVENT
	if (m_pRoboCharacter == NULL)
	{
		if (m_nCntRobot == 400)
		{
			CreateRobot();					// ロボットの生成
			CreateInitEventCamera();		// イベントカメラの生成
			m_nCntRobot = 0;
		}
		else
		{
			m_nCntRobot++;
		}
	}
#endif

	// エリアボーナスイベントの追加
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
	// 分断イベントの追加
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

	// イベントカメラの削除
	if (m_bEveCam == true && m_pEventCamera != NULL)
	{// 死亡フラグが立ったとき
		m_pEventCamera->Uninit();
		delete m_pEventCamera;
		m_pEventCamera = NULL;
		m_bEveCam = false;	// 死亡フラグを戻す
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

	CDebugProc::Print("ゲームカウンター : %d\n", g_nCntGame);
}

//=============================================================================
// UIの更新処理
//=============================================================================
void CGame::UpdateUI(void)
{
	if (m_pUI == NULL) { return; }
	m_pUI->Update();
}

//=============================================================================
// 描画処理
//=============================================================================
void CGame::Draw(void)
{
	//レンダリングクラスを取得
	CRenderer * pRenderer = NULL;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//カメラ4分割の処理
	if (!m_bDebugCamera)
	{
		if (m_nNumPlay == 1)
		{
			//カメラの設定
			if (m_pGameCamera[0] != NULL) { m_pGameCamera[0]->Set(); }
			CManager::GetRenderer()->GetDevice()->Clear(0,
				NULL,
				(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL),
				D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);
			//すべて描画する(左上)
			CScene::DrawAll();

			// アイコンの描画
			DrawUI_Icon(0);
		}
		else if (m_nNumPlay == 2)
		{
			//カメラの設定
			if (m_pGameCamera[0] != NULL) { m_pGameCamera[0]->Set(); }
			CManager::GetRenderer()->GetDevice()->Clear(0,
				NULL,
				(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL),
				D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);
			//すべて描画する(左上)
			CScene::DrawAll();

			// アイコンの描画
			DrawUI_Icon(0);

			//カメラの設定
			if (m_pGameCamera[1] != NULL) { m_pGameCamera[1]->Set(); }
			//すべて描画する（右上）
			CManager::GetRenderer()->GetDevice()->Clear(0,
				NULL,
				(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL),
				D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);
			CScene::DrawAll();

			// アイコンの描画
			DrawUI_Icon(1);
		}
		else if (m_nNumPlay == 3)
		{
			//カメラの設定
			if (m_pGameCamera[0] != NULL) { m_pGameCamera[0]->Set(); }
			CManager::GetRenderer()->GetDevice()->Clear(0,
				NULL,
				(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL),
				D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);
			//すべて描画する(左上)
			CScene::DrawAll();

			// アイコンの描画
			DrawUI_Icon(0);

			//カメラの設定
			if (m_pGameCamera[1] != NULL) { m_pGameCamera[1]->Set(); }
			//すべて描画する（右上）
			CManager::GetRenderer()->GetDevice()->Clear(0,
				NULL,
				(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL),
				D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);
			CScene::DrawAll();

			// アイコンの描画
			DrawUI_Icon(1);

			//カメラの設定
			if (m_pGameCamera[2] != NULL) { m_pGameCamera[2]->Set(); }
			CManager::GetRenderer()->GetDevice()->Clear(0,
				NULL,
				(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL),
				D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);
			//すべて描画する（左下）
			CScene::DrawAll();

			// アイコンの描画
			DrawUI_Icon(2);

			//カメラの設定
			if (m_pRotCamera != NULL) { m_pRotCamera->Set(); }
			CManager::GetRenderer()->GetDevice()->Clear(0,
				NULL,
				(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL),
				D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);
			//すべて描画する（左下）
			CScene::DrawAll();
		}
		else if (m_nNumPlay == 4)
		{
			//カメラの設定
			if (m_pGameCamera[0] != NULL) { m_pGameCamera[0]->Set(); }
			CManager::GetRenderer()->GetDevice()->Clear(0,
				NULL,
				(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL),
				D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);
			//すべて描画する(左上)
			CScene::DrawAll();

			// アイコンの描画
			DrawUI_Icon(0);

			//カメラの設定
			if (m_pGameCamera[1] != NULL) { m_pGameCamera[1]->Set(); }
			//すべて描画する（右上）
			CManager::GetRenderer()->GetDevice()->Clear(0,
				NULL,
				(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL),
				D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);
			CScene::DrawAll();

			// アイコンの描画
			DrawUI_Icon(1);

			//カメラの設定
			if (m_pGameCamera[2] != NULL) { m_pGameCamera[2]->Set(); }
			CManager::GetRenderer()->GetDevice()->Clear(0,
				NULL,
				(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL),
				D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);
			//すべて描画する（左下）
			CScene::DrawAll();

			// アイコンの描画
			DrawUI_Icon(2);

			//カメラの設定
			if (m_pGameCamera[3] != NULL) { m_pGameCamera[3]->Set(); }
			CManager::GetRenderer()->GetDevice()->Clear(0,
				NULL,
				(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL),
				D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);
			//すべて描画する（右下）
			CScene::DrawAll();

			// アイコンの描画
			DrawUI_Icon(3);
		}
	}
	else
	{
		//デバックカメラの設定
		if (m_pDebugCamera != NULL)
		{
			m_pDebugCamera->Set();

			CManager::GetRenderer()->GetDevice()->Clear(0,
				NULL,
				(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL),
				D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

			//すべて描画する
			CScene::DrawAll();
		}
	}
	// 描画領域を画面全体に設定
	D3DVIEWPORT9 ViewPort, ViewPortDef;
	pDevice->GetViewport(&ViewPortDef);
	ViewPort.X = 0;			//ビューポートの左上X座標
	ViewPort.Y = 0;							//ビューポートの左上Y座標
	ViewPort.Width = SCREEN_WIDTH;		//幅
	ViewPort.Height = SCREEN_HEIGHT;	//高さ
	ViewPort.MaxZ = 1.0f;
	ViewPort.MinZ = 0.0f;
	pDevice->SetViewport(&ViewPort);

	if (m_pUI != NULL)
	{
		m_pUI->Draw();
	}

	for (int nCntLayer = 0; nCntLayer < MAX_LAYER; nCntLayer++)
	{
		//トップの位置を保存する
		CScene * pScene = CScene::GetTop(nCntLayer);

		while (pScene != NULL)
		{
			CScene * pSceneNext = pScene->GetpNext();	//次のオブジェクトのポインタを保存する

			if (pScene->GetObjType() == CScene2D::OBJTYPE_2DPOLYGON
				|| pScene->GetObjType() == CScene::OBJTYPE_FADE
				|| pScene->GetObjType() == CScene::OBJTYPE_SCORE)
			{
				pScene->Draw();
			}

			pScene = pSceneNext;					//ポインタを進める
		}
	}

	// イベントカメラの描画
	if (m_pEventCamera != NULL)
	{
		//カメラの設定
		if (m_pEventCamera != NULL) { m_pEventCamera->Set(); }
		CManager::GetRenderer()->GetDevice()->Clear(0,
			NULL,
			(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL),
			D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);
		//すべて描画する
		CScene::DrawAll();

		DrawRobotUI();	// ロボットUIの描画

		// エリアボーナスイベントUIの描画
		DrawAreaBounsEventUI();

		// 分断イベントUIの描画
		DrawDivisionEventUI();
	}
}

//=============================================================================
// アイコンを描画する処理
//=============================================================================
void CGame::DrawUI_Icon(const int nIdx)
{
	if (m_pUI == NULL) { return; }
	m_pUI->DrawIcon(nIdx);
}

//=============================================================================
// キャラの取得情報
//=============================================================================
void CGame::GetCharInfo(void)
{
	for (int nCnt = 0; nCnt < MAX_CHARACTER; nCnt++)
	{
		if (m_pCharacter[nCnt] != NULL)
		{
			m_nCountMakeScore[nCnt] = m_pCharacter[nCnt]->GetCountMakeShape();					//図形を作った数を取得
			m_nCountGetTerritry[nCnt] = m_pCharacter[nCnt]->GetCountGetTerritory();				//テリトリーの取得数
			m_nCountRobottedTerritory[nCnt] = m_pCharacter[nCnt]->GetCountRobbtedTerritory();	//テリトリーの奪われた数
		}
	}
}

//=============================================================================
// ロボットUIの開放処理
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
// ロボットUIの更新処理
//=============================================================================
void CGame::UpdateRobotUI(void)
{
	CRobot *pRobot = GetRobot();	// ロボットの取得

	if (m_pRoboCharacter != NULL)
	{
		m_pRobotUI = pRobot->GetRobotUI();

		if (m_pRobotUI == NULL) { return; }
		m_pRobotUI->Update();
	}
}

//=============================================================================
// ロボットUIの描画処理
//=============================================================================
void CGame::DrawRobotUI(void)
{
	if (m_pRobotUI == NULL) { return; }
	m_pRobotUI->Draw();
}

//=============================================================================
// エリアボーナスイベントをスタートさせる処理
//=============================================================================
void CGame::StartAreaBonusEvent(void)
{
	// フラグを設定
	m_bAreaBonusEventFlag = true;

	// カウンターを初期化
	m_nAreaBonusEventCounter = 0;
	m_nAreaBonusUICounter = 0;

	// ボーナスを付与するエリアの番号を設定
	DecideAreaBonusIdxFromRandom();

	// イベント用カメラを生成
	CreateEventCamera_AreaBonusStart();

	// 背景を生成
	CreateAreaBonusBg_Start();

	// ロゴを生成
	CreateAreaBonusLogo_Start();

	// 地面の位置を設定
	SetEventFieldPosition();

	SprintFlag();
}

//=============================================================================
// エリアボーナスイベント(開始時)用カメラを生成する
//=============================================================================
void CGame::CreateEventCamera_AreaBonusStart(void)
{
	if (m_pEventCamera != NULL) { return; }

	// 描画領域の設定
	D3DVIEWPORT9 viewport;
	viewport.X = 0;
	viewport.Y = 0;
	viewport.Width = SCREEN_WIDTH;
	viewport.Height = SCREEN_HEIGHT;
	viewport.MaxZ = 1.0f;
	viewport.MinZ = 0.0f;

	// イベントカメラの生成
	m_pEventCamera = new CEventCamera;
	if (m_pEventCamera != NULL)
	{
		m_pEventCamera->Init(D3DXVECTOR3(-30.0f, 2700.0f, -10.0f), D3DXVECTOR3(-30.0f, 0.0f, 70.0f),
			NULL, CEventCamera::TYPE_AREABONUS_START);
		m_pEventCamera->SetViewport(viewport);
	}
}

//=============================================================================
// エリアボーナスイベント(開始時)用背景を生成する
//=============================================================================
void CGame::CreateAreaBonusBg_Start(void)
{
	// 上側
	if (m_pAreaBonusBgUp == NULL)
	{
		m_pAreaBonusBgUp = CIcon2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, -200.0f, 0.0f),
			D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), SCREEN_WIDTH, 300.0f);
	}

	// 下側
	if (m_pAreaBonusBgDown == NULL)
	{
		m_pAreaBonusBgDown = CIcon2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT + 200.0f, 0.0f),
			D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), SCREEN_WIDTH, 300.0f);
	}
}

//=============================================================================
// エリアボーナスイベント(開始時)用ロゴを生成する
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
// イベント用の地面の位置を設定する
//=============================================================================
void CGame::SetEventFieldPosition(void)
{
	// 地面クラスを取得
	if (m_pFieldManager == NULL) { return; }
	CField *pField = m_pFieldManager->GetEventField();
	if (pField == NULL) { return; }

	// 地面の位置を設定
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

	// 地面の色を設定
	pField->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));

	// 地面を描画する設定に
	m_pFieldManager->GetFieldRender(CFieldManager::FIELD_TYPE_EVENT)->SetDisp(true);
}

//=============================================================================
// エリアボーナスイベント(終了時)用カメラを生成する
//=============================================================================
void CGame::CreateEventCamera_AreaBonusFinish(void)
{
	if (m_pEventCamera != NULL) { return; }

	// 描画領域の設定
	D3DVIEWPORT9 viewport;
	viewport.X = 0;
	viewport.Y = 0;
	viewport.Width = SCREEN_WIDTH;
	viewport.Height = SCREEN_HEIGHT;
	viewport.MaxZ = 1.0f;
	viewport.MinZ = 0.0f;

	// イベントカメラの生成
	m_pEventCamera = new CEventCamera;
	if (m_pEventCamera != NULL)
	{
		m_pEventCamera->Init(D3DXVECTOR3(-30.0f, 2700.0f, -10.0f), D3DXVECTOR3(-30.0f, 0.0f, 70.0f),
			NULL, CEventCamera::TYPE_AREABONUS_FINISH);
		m_pEventCamera->SetViewport(viewport);
	}
}

//=============================================================================
// エリアボーナスイベント(終了時)用背景を生成する
//=============================================================================
void CGame::CreateAreaBonusBg_Finish(void)
{
	// 上側
	if (m_pAreaBonusBgUp == NULL)
	{
		m_pAreaBonusBgUp = CIcon2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, -200.0f, 0.0f),
			D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), SCREEN_WIDTH, 300.0f);
	}

	// 下側
	if (m_pAreaBonusBgDown == NULL)
	{
		m_pAreaBonusBgDown = CIcon2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT + 200.0f, 0.0f),
			D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), SCREEN_WIDTH, 300.0f);
	}
}

//=============================================================================
// エリアボーナスイベント(終了時)用ロゴを生成する
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
// エリアボーナスを付与するエリアの番号をランダムで設定する
//=============================================================================
void CGame::DecideAreaBonusIdxFromRandom(void)
{
	// 4つのエリア(左上, 右上, 左下, 右下)番号から決定
	m_nAreaBonusIdx = rand() % 4;
}

//=============================================================================
// エリアボーナスイベントの終了処理
//=============================================================================
void CGame::FinishAreaBonusEvent(void)
{
	// イベント用カメラを生成
	CreateEventCamera_AreaBonusFinish();

	// 背景を生成
	CreateAreaBonusBg_Finish();

	// ロゴを生成
	CreateAreaBonusLogo_Finish();

	//スプリントサウンドへのフラグ
	SprintFlag();
}

//=============================================================================
// エリアボーナスイベント用のUIを開放する処理
//=============================================================================
void CGame::ReleaseAreaBounsEventUI(void)
{
	// 背景(上側)
	if (m_pAreaBonusBgUp != NULL)
	{
		m_pAreaBonusBgUp->Uninit();
		delete m_pAreaBonusBgUp;
		m_pAreaBonusBgUp = NULL;
	}

	// 背景(下側)
	if (m_pAreaBonusBgDown != NULL)
	{
		m_pAreaBonusBgDown->Uninit();
		delete m_pAreaBonusBgDown;
		m_pAreaBonusBgDown = NULL;
	}

	// ロゴ
	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		if (m_pAreaBonusLogo[nCnt] != NULL)
		{
			m_pAreaBonusLogo[nCnt]->Uninit();
			delete m_pAreaBonusLogo[nCnt];
			m_pAreaBonusLogo[nCnt] = NULL;
		}
	}

	//スプリントサウンド
	SprintSound();
}

//=============================================================================
// スプリントのサウンド処理
//=============================================================================
void CGame::SprintSound(void)
{
	CSound *pSound = CManager::GetSound();
	//フラグの回収(関数)
	if (m_bSprintFlag == true)
	{
		//サウンドを鳴らす
		pSound->PlaySound(CSound::SOUND_LABEL_SE030);
		m_bSprintFlag = false;
	}
}

//=============================================================================
// スプリントのサウンドへのフラグ処理
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
				//サウンドを止める
				//pSound->StopSound(CSound::SOUND_LABEL_SE030);
			}
		}
	}
}

//=============================================================================
// イベント用の地面を隠す処理
//=============================================================================
void CGame::HideEventField(void)
{
	if (m_pFieldManager == NULL) { return; }
	m_pFieldManager->GetFieldRender(CFieldManager::FIELD_TYPE_EVENT)->SetDisp(false);
}

//=============================================================================
// エリアボーナスイベントの更新処理
//=============================================================================
void CGame::UpdateAreaBonusEvent(void)
{
	// カウンターを増加
	m_nAreaBonusEventCounter++;
	if (m_nAreaBonusEventCounter >= AREABONUSEVENT_TIME)
	{// 経過時間がある程度貯まった
		m_nAreaBonusUICounter = 0;
		FinishAreaBonusEvent();
	}
}

//=============================================================================
// エリアボーナスイベントUIの更新処理
//=============================================================================
void CGame::UpdateAreaBonusUI(void)
{
	// カウンター増加
	m_nAreaBonusUICounter++;

	// カウンターが一定値まで貯まるまで背景を動かす
	if (m_nAreaBonusUICounter <= AREABONUSEVENT_BG_MOVETIME)
	{
		// 上側
		D3DXVECTOR3 pos = INITIALIZE_VECTOR3;
		if (m_pAreaBonusBgUp != NULL)
		{
			pos = m_pAreaBonusBgUp->GetPos();
			pos.y += AREABONUSEVENT_BG_MOVE;
			m_pAreaBonusBgUp->SetPos(pos);
			m_pAreaBonusBgUp->SetVtxBuffPos();
		}

		// 下側
		if (m_pAreaBonusBgDown != NULL)
		{
			pos = m_pAreaBonusBgDown->GetPos();
			pos.y -= AREABONUSEVENT_BG_MOVE;
			m_pAreaBonusBgDown->SetPos(pos);
			m_pAreaBonusBgDown->SetVtxBuffPos();
		}
		return;
	}

	// 透明度を下げる
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
// エリアボーナスイベント用の地面の更新処理(開始時)
//=============================================================================
void CGame::UpdateAreaBonusField_Start(void)
{
	if (m_nAreaBonusUICounter <= AREABONUSEVENT_BG_MOVETIME) { return; }

	// 地面クラスを取得
	if (m_pFieldManager == NULL) { return; }
	CField *pField = m_pFieldManager->GetEventField();
	if (pField == NULL) { return; }

	// 透明度を上げる
	D3DXCOLOR col = pField->GetCol();
	col.a += AREABONUSEVENT_FIELD_ALPHA;
	if (col.a >= 1.0f)
	{
		col.a = 1.0f;
	}
	pField->SetColor(col);
}

//=============================================================================
// エリアボーナスイベント用の地面の更新処理(終了時)
//=============================================================================
void CGame::UpdateAreaBonusField_Finish(void)
{
	if (m_nAreaBonusUICounter <= AREABONUSEVENT_BG_MOVETIME) { return; }

	// 地面クラスを取得
	if (m_pFieldManager == NULL) { return; }
	CField *pField = m_pFieldManager->GetEventField();
	if (pField == NULL) { return; }

	// 透明度を下げる
	D3DXCOLOR col = pField->GetCol();
	col.a -= AREABONUSEVENT_FIELD_ALPHA;
	if (col.a <= 0.0f)
	{
		col.a = 0.0f;
	}
	pField->SetColor(col);
}

//=============================================================================
// エリアボーナスイベントの描画処理
//=============================================================================
void CGame::DrawAreaBounsEventUI(void)
{
	// 背景(上側)
	if (m_pAreaBonusBgUp != NULL)
	{
		m_pAreaBonusBgUp->Draw();
	}

	// 背景(下側)
	if (m_pAreaBonusBgDown != NULL)
	{
		m_pAreaBonusBgDown->Draw();
	}

	// ロゴ
	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		if (m_pAreaBonusLogo[nCnt] != NULL)
		{
			m_pAreaBonusLogo[nCnt]->Draw();
		}
	}
}

//=============================================================================
// 分断イベントをスタートさせる処理
//=============================================================================
void CGame::StartDivisionEvent(void)
{
	// フラグを設定
	m_bDivisionEventFlag = true;

	// カウンターを初期化
	m_nDivisionEventCounter = 0;
	m_nDivisionUICounter = 0;

	// 分断する方向をランダムで設定
	m_nDivisionDirection = rand() % 2;

	// イベント用カメラを生成
	CreateEventCamera_DivisionStart();

	// 背景を生成
	CreateDivisionBg_Start();

	// ロゴを生成
	CreateDivisionLogo_Start();

	// 分断用コライダーを生成
	CreateDivisionCollider();

	// 分断用の壁を生成
	CreateDivisionWall();

	// キャラクターの位置をずらしておく
	ShiftCharaPosition();

	SprintFlag();
}

//=============================================================================
// 分断イベント(開始時)用カメラを生成する
//=============================================================================
void CGame::CreateEventCamera_DivisionStart(void)
{
	if (m_pEventCamera != NULL) { return; }

	// 描画領域の設定
	D3DVIEWPORT9 viewport;
	viewport.X = 0;
	viewport.Y = 0;
	viewport.Width = SCREEN_WIDTH;
	viewport.Height = SCREEN_HEIGHT;
	viewport.MaxZ = 1.0f;
	viewport.MinZ = 0.0f;

	// 視点位置の設定
	D3DXVECTOR3 posV = INITIALIZE_VECTOR3;
	posV.x = 0.0f;
	posV.y = DIVISIONEVENT_CAMERA_POSVHEIGHT;
	posV.z = DIVISIONEVENT_CAMERA_LENGTH;

	// イベントカメラの生成
	m_pEventCamera = new CEventCamera;
	if (m_pEventCamera != NULL)
	{
		m_pEventCamera->Init(posV, INITIALIZE_VECTOR3,
			NULL, CEventCamera::TYPE_DIVISION_START);
		m_pEventCamera->SetViewport(viewport);
	}
}

//=============================================================================
// 分断イベント(開始時)用背景を生成する
//=============================================================================
void CGame::CreateDivisionBg_Start(void)
{
	// 上側
	if (m_pDivisionBgUp == NULL)
	{
		m_pDivisionBgUp = CIcon2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, -200.0f, 0.0f),
			D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), SCREEN_WIDTH, 300.0f);
	}

	// 下側
	if (m_pDivisionBgDown == NULL)
	{
		m_pDivisionBgDown = CIcon2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT + 200.0f, 0.0f),
			D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), SCREEN_WIDTH, 300.0f);
	}
}

//=============================================================================
// 分断イベント(開始時)用ロゴを生成する
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
// 分断イベント(開始時)用コライダーを生成する
//=============================================================================
void CGame::CreateDivisionCollider(void)
{
	if (m_pDivisionCollider != NULL) { return; }

	float fWidth = 0.0f;
	float fDepth = 0.0f;
	D3DXVECTOR3 pos = INITIALIZE_VECTOR3;
	switch (m_nDivisionDirection)
	{// 分断の方向によって処理わけ
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

	// ボックスコライダーを生成
	m_pDivisionCollider = CBoxCollider::Create(pos, D3DXVECTOR3(1.0f, 1.0f, 1.0f),
		fWidth, 1000.0f, fDepth, true);
}

//=============================================================================
// 分断イベント(開始時)用の壁を生成する
//=============================================================================
void CGame::CreateDivisionWall(void)
{
	if (m_pDivisionWall != NULL) { return; }
	D3DXVECTOR3 pos = INITIALIZE_VECTOR3;
	D3DXVECTOR3 rot = INITIALIZE_VECTOR3;
	switch (m_nDivisionDirection)
	{// 分断の方向によって処理わけ
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

	// 壁を生成
	m_pDivisionWall = CDivisionWall::Create(pos, rot, DIVISIONEVENT_WALL_COL,
		DIVISIONEVENT_WALL_WIDTH, DIVISIONEVENT_WALL_HEIGHT, DIVISIONEVENT_WALL_PRIORITY, 0.0f, 0.0f,
		DIVISIONEVENT_WALL_TEXWIDTH, DIVISIONEVENT_WALL_TEXHEIGHT);

	// テクスチャを割り当てる
	if (m_pDivisionWall == NULL) { return; }
	m_pDivisionWall->BindTexture(GetTexture()[DIVISIONEVENT_WALL_TEXIDX]);
}

//=============================================================================
// キャラクターの位置をずらす処理(コライダーの内側に入るのを防止する)
//=============================================================================
void CGame::ShiftCharaPosition(void)
{
	D3DXVECTOR3 CharaPos = INITIALIZE_VECTOR3;
	for (int nCntChara = 0; nCntChara < m_nNumPlay + m_nEnemyNum; nCntChara++)
	{
		// キャラクターがコライダーの内側にいないか判定
		if (m_pCharacter[nCntChara] == NULL) { continue; }
		CharaPos = m_pCharacter[nCntChara]->GetPos();
		if (CheckInsideCollider(CharaPos) == false) { return; }

		// 内側にいるならコライダーの外側に押し出す
		PushOutColliderOutSide(CharaPos);

		// キャラクターの位置の設定
		m_pCharacter[nCntChara]->SetPos(CharaPos);
	}
}

//=============================================================================
// コライダーの内側にいるか判定する処理
//=============================================================================
bool CGame::CheckInsideCollider(D3DXVECTOR3 pos)
{
	if (m_pDivisionCollider == NULL) { return false; }
	D3DXVECTOR3 ColliderPos = m_pDivisionCollider->GetPos();
	float fColliderWidth = m_pDivisionCollider->GetWidth();
	float fColliderDepth = m_pDivisionCollider->GetDepth();
	if (pos.x + 15.0f >= ColliderPos.x - fColliderWidth && pos.x - 15.0f <= ColliderPos.x + fColliderWidth
		&& pos.z + 15.0f >= ColliderPos.z - fColliderDepth && pos.z - 15.0f <= ColliderPos.z + fColliderDepth)
	{// コライダーの内側にいる
		return true;
	}

	return false;
}

//=============================================================================
// コライダーの外側に押し出す処理
//=============================================================================
void CGame::PushOutColliderOutSide(D3DXVECTOR3 &pos)
{
	if (m_pDivisionCollider == NULL) { return; }
	D3DXVECTOR3 ColliderPos = m_pDivisionCollider->GetPos();
	float fMoveX = 0.0f;
	float fMoveZ = 0.0f;
	switch (m_nDivisionDirection)
	{// 分断の方向によって処理わけ
	case DIVISIONEVENT_VERTICAL_IDX:
		fMoveX += DIVISIONEVENT_COLLIDER_MIN + 2.0f;
		break;
	case DIVISIONEVENT_HORIZON_IDX:
		fMoveZ += DIVISIONEVENT_COLLIDER_MIN + 2.0f;
		break;
	}

	// ランダムでどちら側に押し出すか設定
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
	{// 分断の方向によって処理わけ
	case DIVISIONEVENT_VERTICAL_IDX:
		pos.x = fMoveX + ColliderPos.x;
		break;
	case DIVISIONEVENT_HORIZON_IDX:
		pos.z = fMoveZ + ColliderPos.z;
		break;
	}
}

//=============================================================================
// 分断イベント(終了時)用カメラを生成する
//=============================================================================
void CGame::CreateEventCamera_DivisionFinish(void)
{
	if (m_pEventCamera != NULL) { return; }

	// 描画領域の設定
	D3DVIEWPORT9 viewport;
	viewport.X = 0;
	viewport.Y = 0;
	viewport.Width = SCREEN_WIDTH;
	viewport.Height = SCREEN_HEIGHT;
	viewport.MaxZ = 1.0f;
	viewport.MinZ = 0.0f;

	// 視点位置の設定
	D3DXVECTOR3 posV = INITIALIZE_VECTOR3;
	posV.x = 0.0f;
	posV.y = DIVISIONEVENT_CAMERA_POSVHEIGHT;
	posV.z = DIVISIONEVENT_CAMERA_LENGTH;

	// イベントカメラの生成
	m_pEventCamera = new CEventCamera;
	if (m_pEventCamera != NULL)
	{
		m_pEventCamera->Init(posV, INITIALIZE_VECTOR3,
			NULL, CEventCamera::TYPE_DIVISION_FINISH);
		m_pEventCamera->SetViewport(viewport);
	}
}

//=============================================================================
// 分断イベント(終了時)用背景を生成する
//=============================================================================
void CGame::CreateDivisionBg_Finish(void)
{
	// 上側
	if (m_pDivisionBgUp == NULL)
	{
		m_pDivisionBgUp = CIcon2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, -200.0f, 0.0f),
			D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), SCREEN_WIDTH, 300.0f);
	}

	// 下側
	if (m_pDivisionBgDown == NULL)
	{
		m_pDivisionBgDown = CIcon2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT + 200.0f, 0.0f),
			D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), SCREEN_WIDTH, 300.0f);
	}
}

//=============================================================================
// 分断イベント(終了時)用ロゴを生成する
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
// 分断イベント(終了時)用コライダーを開放する
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
// 分断イベント(終了時)用の壁を開放する
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
// 分断イベントの終了処理
//=============================================================================
void CGame::FinishDivisionEvent(void)
{
	// イベント用カメラを生成
	CreateEventCamera_DivisionFinish();

	// 背景を生成
	CreateDivisionBg_Finish();

	// ロゴを生成
	CreateDivisionLogo_Finish();

	// コライダーを破棄
	ReleaseDivisionCollider();

	//スプリントサウンドのフラグ
	SprintFlag();
}

//=============================================================================
// 分断イベント用のUIを開放する処理
//=============================================================================
void CGame::ReleaseDivisionEventUI(void)
{
	// 背景(上側)
	if (m_pDivisionBgUp != NULL)
	{
		m_pDivisionBgUp->Uninit();
		delete m_pDivisionBgUp;
		m_pDivisionBgUp = NULL;
	}

	// 背景(下側)
	if (m_pDivisionBgDown != NULL)
	{
		m_pDivisionBgDown->Uninit();
		delete m_pDivisionBgDown;
		m_pDivisionBgDown = NULL;
	}

	// ロゴ
	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		if (m_pDivisionLogo[nCnt] != NULL)
		{
			m_pDivisionLogo[nCnt]->Uninit();
			delete m_pDivisionLogo[nCnt];
			m_pDivisionLogo[nCnt] = NULL;
		}
	}

	//スプリントサウンド
	SprintSound();
}

//=============================================================================
// 分断イベントの更新処理
//=============================================================================
void CGame::UpdateDivisionEvent(void)
{
	// カウンターを増加
	m_nDivisionEventCounter++;
	if (m_nDivisionEventCounter >= DIVISIONEVENT_TIME)
	{// 経過時間がある程度貯まった
		m_nDivisionUICounter = 0;
		FinishDivisionEvent();
	}
}

//=============================================================================
// 分断イベント用UIの更新処理
//=============================================================================
void CGame::UpdateDivisionUI(void)
{
	// カウンター増加
	m_nDivisionUICounter++;

	// カウンターが一定値まで貯まるまで背景を動かす
	if (m_nDivisionUICounter <= DIVISIONEVENT_BG_MOVETIME)
	{
		// 上側
		D3DXVECTOR3 pos = INITIALIZE_VECTOR3;
		if (m_pDivisionBgUp != NULL)
		{
			pos = m_pDivisionBgUp->GetPos();
			pos.y += DIVISIONEVENT_BG_MOVE;
			m_pDivisionBgUp->SetPos(pos);
			m_pDivisionBgUp->SetVtxBuffPos();
		}

		// 下側
		if (m_pDivisionBgDown != NULL)
		{
			pos = m_pDivisionBgDown->GetPos();
			pos.y -= DIVISIONEVENT_BG_MOVE;
			m_pDivisionBgDown->SetPos(pos);
			m_pDivisionBgDown->SetVtxBuffPos();
		}
		return;
	}

	// 透明度を下げる
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
// 分断イベント用壁(開始時)の更新処理
//=============================================================================
void CGame::UpdateDivisionWall_Start(void)
{
	if (m_pDivisionWall == NULL) { return; }

	// 透明度を上げる
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
// 分断イベント用壁(終了時)の更新処理
//=============================================================================
void CGame::UpdateDivisionWall_Finish(void)
{
	if (m_pDivisionWall == NULL) { return; }

	// 透明度を下げる
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
// 分断イベントの描画処理
//=============================================================================
void CGame::DrawDivisionEventUI(void)
{
	// 背景(上側)
	if (m_pDivisionBgUp != NULL)
	{
		m_pDivisionBgUp->Draw();
	}

	// 背景(下側)
	if (m_pDivisionBgDown != NULL)
	{
		m_pDivisionBgDown->Draw();
	}

	// ロゴ
	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		if (m_pDivisionLogo[nCnt] != NULL)
		{
			m_pDivisionLogo[nCnt]->Draw();
		}
	}
}