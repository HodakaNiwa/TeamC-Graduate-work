//=============================================================================
//
// ゲームの処理 [game.cpp]
// Author : 佐藤安純
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
// マクロ定義
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

// 回転カメラ用(3人プレイ時のみ適用する)
#define ROTATIONCAMERA_POSVHEIGHT (500.0f)
#define ROTATIONCAMERA_POSR       (D3DXVECTOR3(0.0f,0.0f,0.0f))
#define ROTATIONCAMERA_ROTSPEED   (0.003f)
#define ROTATIONCAMERA_LENGTH     (700.0f)

// プレイヤー配置用
#define PLAYER_SPAWN_LENGTH_X   (1100.0f)
#define PLAYER_SPAWN_LENGTH_Z   (1090.0f)
#define PLAYER_SPAWN_RIVISION_X (-90.0f)
#define PLAYER_SPAWN_RIVISION_Z (170.0f)

// マップの見えない壁配置用
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

// エフェクトデータのファイル名
#define EFFECTMANAGER_FILENAME "data/TEXT/EFFECT/ringEffect.txt"

//*****************************************************************************
// 静的メンバ変数
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
// デフォルトコンストラクタ
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
		m_nPlayerType[nCnt] = 0;	//プレイヤーのタイプ
		m_nCountry[nCnt] = 0;		//国
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
	m_bPause = false;					//ポーズを初期化
	m_pLoadObject = NULL;
	m_pLoadTerritory = NULL;
	m_pDebugCamera = NULL;				//デバックカメラ
	m_pFieldManager = NULL;
	m_nSpeedNo = 0;
	m_nPowerNo = 0;
	m_nSweeperNo = 0;
	m_bTypeMax = false;
	m_pEffectManager = NULL;
	m_pEmitter = NULL;

	//テクスチャの読み込み
	CNumber2D::Load();
	CTimerLogo::Load();
	CSkilicon::Load();
	CEffect3D::Load();

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
	}

#else
	//カウントダウン状態
	CGame::SetGameState(CGame::GAMESTATE_NORMAL);
	//CGame::SetGameState(CGame::GAMESTATE_FIRSTCOUNTDOWN);

	//カウントダウンタイマーの生成
	//CTimer::Create(CTimer::TYPE_COUNTER, 7, D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXVECTOR3(50.0f, 80.0f, 0.0f));

	//ゲームタイマーの生成（ラスト5秒で描画される）
	//CTimer::Create(CTimer::TYPE_GAME, 7, D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXVECTOR3(50.0f, 80.0f, 0.0f));

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

	//読み込み処理
	if (m_pLoadObject == NULL) { m_pLoadObject = CLoadTextObject::Create(LOAD_OBJECT); }				//オブジェクト
	if (m_pLoadTerritory == NULL) { m_pLoadTerritory = CLoadTextTerritory::Create(LOAD_TERRITORY); }	//テリトリー

	//テリトリーのエリア分け処理
	CTerritory::SetErea();

	//	---<<小林が追加しました>>★	★	★	★	★	★	★	★	★	★
	for (int nCntEnemy = m_nNumPlay; nCntEnemy < ENEMY_MAX;nCntEnemy++)
	{
		//	スピード
		if (m_nSpeedNo < 3 - (int)m_bTypeMax)
		{
			m_nSpeedNo += 1;
			m_pCharacter[nCntEnemy] = CTypeSpeed::Create(nCntEnemy, m_nSpeedNo,CEnemy::TYPE_BRAZIL, CCharacter::CHARCTERTYPE_SPEED,
				InitPos[nCntEnemy],"data\\TEXT\\ModelLoad\\SPEEDPLAYER_LOAD.txt", "data\\TEXT\\MotionLoad\\SPEEDPLAYER_MOTION.txt");
		}
		//	脳筋
		else if (m_nPowerNo < 3 - (int)m_bTypeMax)
		{
			m_nPowerNo += 1;
			m_pCharacter[nCntEnemy] = CTypePower::Create(nCntEnemy, CEnemy::TYPE_BRAZIL, CCharacter::CHARCTERTYPE_POWER,
				InitPos[nCntEnemy], "data\\TEXT\\ModelLoad\\POWERPLAYER_LOAD.txt", "data\\TEXT\\MotionLoad\\POWERPLAYER_MOTION.txt");
		}
		//	地雷
		else if (m_nSweeperNo < 3)
		{
			m_nSweeperNo += 1;
			m_pCharacter[nCntEnemy] = CTypeSweeper::Create(nCntEnemy, CEnemy::TYPE_BRAZIL, CCharacter::CHARCTERTYPE_TECHNIQUE,
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
	CRingRender::Create();

	// テリトリー描画クラスを生成
	CTerritoryRender::Create();
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
		m_nControllerType[nCnt] = 1;		//コントローラーのタイプを取得
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
// UIを生成する処理
//=============================================================================
void CGame::CreateUI(void)
{
	//セレクトの情報を取得
	CSelect *pSelect = CManager::GetSelect();

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
			m_pUI->CreateSkilicon(m_pCharacter[nCnt], nCnt, 3 * 60);

		}
		else if (m_nPlayerType[nCnt] == 2)
		{//地雷型プレイヤーの生成
			m_pUI->CreateSkilicon(m_pCharacter[nCnt], nCnt, 1 * 60);

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

		// 位置を決定
		//if (m_pCharacter[nSetCharaNum] != NULL)
		//{
		//}
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
	//ポーズ削除
	if (m_pPause != NULL)
	{
		m_pPause->Uninit();
		m_pPause = NULL;
	}

	//カメラの破棄
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
	m_pMeshFiled = NULL;
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

	if (m_pEmitter != NULL)
	{
		m_pEmitter = NULL;
	}
	//テクスチャの破棄
	CNumber2D::Unload();
	CTimerLogo::Unload();
	CSkilicon::UnLoad();
	CEffect3D::UnLoad();

	//アウトライン破棄
	CModel::ShaderUnLoad();

	//奇跡の破棄
	CSceneOrbit::Unload();

	//地雷の破棄
	CMine::UnLoad();

	//メッシュフィールドの破棄
	if (m_pFieldManager != NULL)
	{
		m_pFieldManager->Uninit();
		delete m_pFieldManager;
		m_pFieldManager = NULL;
	}

	// UIの開放
	ReleaseUI();

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

	if (m_pPause == false)
	{//開く音
		//現在の状態を保存
		m_NowGameState = GetGameState();

		switch (m_NowGameState)
		{
		case GAMESTATE_CLEAR:	//ゲームをクリアした状態
			m_nCounterGameState++;

			if (m_nCounterGameState >= 60)
			{//画面（モード）の設定
				if (CFade::GetFadeMode() != CFade::FADE_OUT)
				{
					CFade::SetFade(CManager::MODE_RESULT);
				}
			}
			break;
		case GAMESTATE_OVER:	//ゲームで負けたときの状態
			m_nCounterGameState++;

			if (m_nCounterGameState >= 60)
			{//画面（モード）の設定
				CFade::SetFade(CManager::MODE_RESULT);
			}
			break;
		}
	}

	//ポーズの処理
	if (pCInputKeyBoard->GetKeyboardTrigger(DIK_P) == true)
	{//Pキーが押されたら
		int nType = 0;
		m_bPause = m_bPause ? false : true;

		switch (m_bPause)
		{
		case true:
			if (m_pPause == NULL)
			{
				//ポーズの生成
				m_pPause = CPause::Create();
			}
			break;
		case false:
			if (m_pPause != NULL)
			{
				//ポーズを削除
				m_pPause->Uninit();
				m_pPause = NULL;
			}
		}
	}

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
	{
		if (m_pDebugCamera != NULL) { m_pDebugCamera->Update(); }
	}

	CInputMouse *pInputMouse = CManager::GetInputMouse();

	if (m_pFieldManager != NULL) { m_pFieldManager->Update(); }

	//if (pInputMouse->GetPress(pInputMouse->BUTTON_LEFT) == true)
	//{
	//	CFade::SetFade(CManager::MODE_RESULT);
	//}

	// UIの更新処理
	UpdateUI();

	//デバックキー
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
// ポーズを押したかどうか
//=============================================================================
void CGame::SetPause(bool bPause)
{
	m_bPause = bPause;

	switch (m_bPause)
	{
	case true:
		if (m_pPause == NULL)
		{
			//ポーズの生成
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