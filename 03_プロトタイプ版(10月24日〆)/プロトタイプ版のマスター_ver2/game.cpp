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

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define LOAD_UI		("data\\TEXT\\UI\\Game.txt")
#define VECTOR_ZERO	(D3DXVECTOR3(0.0f, 0.0f, 0.0f))
#define LOAD_OBJECT		("data/TEXT/Stage.txt")
#define LOAD_TERRITORY	("data/TEXT/Territory.txt")
#define LOAD_EFFECT	("data/TEXT/EFFECT/Particle.txt")

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
CPause *CGame::m_pPause = NULL;
bool CGame::m_bPause = false;
bool CGame::m_bDebugCamera = false;
CGame::GAMESTATE CGame::m_gameState = GAMESTATE_NONE;
CSceneMeshFiled * CGame::m_pMeshFiled = NULL;
CLoadTextEffect * CGame::m_pLoadEffect = NULL;

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

	//テクスチャの読み込み
	CNumber2D::Load();
	CTimerLogo::Load();
	//奇跡の読み込み
	CSceneOrbit::Load();

	//エフェクトの読み込み
	if (m_pLoadEffect == NULL)
	{
		m_pLoadEffect = CLoadTextEffect::Create(LOAD_EFFECT);
		CParticle::LoadTexture();
		CEffectTool::LoadEffect();
	}

	//カウントダウン状態
	CManager::SetGameState(CManager::GAMESTATE_FIRSTCOUNTDOWN);

	//地面の生成
	if (m_pMeshFiled == NULL) { m_pMeshFiled = CSceneMeshFiled::Create(VECTOR_ZERO); }
	
	//プレイヤーの生成
	m_pPlayer[0] = CPlayer::Create(D3DXVECTOR3(-50.0f, 0.0f, 0.0f), 0,  "data\\TEXT\\ModelLoad\\PLAYER_LOAD.txt", "data\\TEXT\\MotionLoad\\PLAYER_MOTION.txt",CPlayer::TYPE_AMERICA);
	m_pPlayer[1] = CPlayer::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 1, "data\\TEXT\\ModelLoad\\PLAYER_LOAD.txt", "data\\TEXT\\MotionLoad\\PLAYER_MOTION.txt", CPlayer::TYPE_BRAZIL);
	m_pPlayer[2] = CPlayer::Create(D3DXVECTOR3(50.0f, 0.0f, 0.0f), 2, "data\\TEXT\\ModelLoad\\PLAYER_LOAD.txt", "data\\TEXT\\MotionLoad\\PLAYER_MOTION.txt", CPlayer::TYPE_ENGLAND);
	m_pPlayer[3] = CPlayer::Create(D3DXVECTOR3(100.0f, 0.0f, 0.0f), 3, "data\\TEXT\\ModelLoad\\PLAYER_LOAD.txt", "data\\TEXT\\MotionLoad\\PLAYER_MOTION.txt", CPlayer::TYPE_ITALY);

	//	---<<小林が追加しました>>★	★	★	★	★	★	★	★	★	★
	m_pEnemy[0] = CEnemy::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), "data\\TEXT\\ModelLoad\\ENEMY_MODEL.txt", "data\\TEXT\\MotionLoad\\ENEMY_MOTION.txt");
	//	★	★	★	★	★	★	★	★	★	★	★	★	★	★	★	★

	//カメラの生成(1P:左上)
	if (m_pGameCamera[0] == NULL)
	{
		D3DVIEWPORT9 viewport;
		viewport.X = 5;							//ビューポートの左上X座標
		viewport.Y = 5;							//ビューポートの左上Y座標
		viewport.Width = 1260 / 2;		//幅
		viewport.Height = 700 / 2;	//高さ
		viewport.MaxZ = 1.0f;
		viewport.MinZ = 0.0f;
		m_pGameCamera[0] = new CGameCamera;
		m_pGameCamera[0]->Init(m_pPlayer[0]);
		m_pGameCamera[0]->SetViewport(viewport);
	}

	////カメラの生成(2P:右上)
	if (m_pGameCamera[1] == NULL)
	{
		D3DVIEWPORT9 viewport;
		viewport.X = (SCREEN_WIDTH + 8) / 2;			//ビューポートの左上X座標
		viewport.Y = 5;							//ビューポートの左上Y座標
		viewport.Width = 1260 / 2;		//幅
		viewport.Height = 700 / 2;	//高さ
		viewport.MaxZ = 1.0f;
		viewport.MinZ = 0.0f;
		m_pGameCamera[1] = new CGameCamera;
		m_pGameCamera[1]->Init(m_pPlayer[1]);
		m_pGameCamera[1]->SetViewport(viewport);
	}

	//カメラの生成(3P:左下)
	if (m_pGameCamera[2] == NULL)
	{
		D3DVIEWPORT9 viewport;
		viewport.X = 5;						//ビューポートの左上X座標
		viewport.Y = (SCREEN_HEIGHT + 5) / 2;		//ビューポートの左上Y座標
		viewport.Width = 1260 / 2;	//幅
		viewport.Height = 700 / 2;//高さ
		viewport.MaxZ = 1.0f;
		viewport.MinZ = 0.0f;

		m_pGameCamera[2] = new CGameCamera;
		m_pGameCamera[2]->Init(m_pPlayer[2]);
		m_pGameCamera[2]->SetViewport(viewport);
	}

	//カメラの生成(4P:右下)
	if (m_pGameCamera[3] == NULL)
	{
		D3DVIEWPORT9 viewport;
		viewport.X = (SCREEN_WIDTH + 8) / 2;		//ビューポートの左上X座標
		viewport.Y = (SCREEN_HEIGHT + 5) / 2;		//ビューポートの左上Y座標
		viewport.Width = 1260 / 2;	//幅
		viewport.Height = 700 / 2;//高さ
		viewport.MaxZ = 1.0f;
		viewport.MinZ = 0.0f;

		m_pGameCamera[3] = new CGameCamera;
		m_pGameCamera[3]->Init(m_pPlayer[3]);
		m_pGameCamera[3]->SetViewport(viewport);
	}

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
		m_pDebugCamera->SetPosV(D3DXVECTOR3(14.0f, 370.0f, -304.0f));
		m_pDebugCamera->SetPosV(D3DXVECTOR3(14.0f, 255.0f, -204.0f));
	}

	//読み込み処理
	if (m_pLoadObject == NULL) { m_pLoadObject = CLoadTextObject::Create(LOAD_OBJECT); }				//オブジェクト
	if (m_pLoadTerritory == NULL) { m_pLoadTerritory = CLoadTextTerritory::Create(LOAD_TERRITORY); }	//テリトリー

	//ミニマップの生成
	//CMiniMap::Create();

	//カウントダウンタイマーの生成
	CTimer::Create(CTimer::TYPE_COUNTER, 7, D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXVECTOR3(50.0f, 50.0f, 0.0f));

	//ゲームタイマーの生成（ラスト5秒で描画される）
	CTimer::Create(CTimer::TYPE_GAME, 7, D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXVECTOR3(50.0f, 50.0f, 0.0f));

	// UIを読み込む処理
	LoadUI(LOAD_UI);
}

//=============================================================================
// 終了処理
//=============================================================================
void CGame::Uninit(void)
{	
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
	for (int nCnt = 0; nCnt < MAX_NUMPLAYER; nCnt++)
	{
		if (m_pGameCamera[nCnt] != NULL)
		{
			m_pGameCamera[nCnt]->Uninit();
			delete m_pGameCamera[nCnt];
			m_pGameCamera[nCnt] = NULL;
		}
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

	//テクスチャの破棄
	CNumber2D::Unload();
	CTimerLogo::Unload();
	//奇跡の破棄
	CSceneOrbit::Unload();

	//ベースの破棄
	CModeBase::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CGame::Update(void)
{
	//CLoadEffect::Create(0, D3DXVECTOR3(-50.0f, 0.0f, 0.0f), 4);


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
		for (int nCnt = 0; nCnt < MAX_NUMPLAYER; nCnt++)
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

	//デバックキー
	if (pCInputKeyBoard->GetKeyboardTrigger(DIK_1) == true) { CFade::SetFade(CManager::MODE_RESULT); }
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
		//カメラの設定
		if (m_pGameCamera[0] != NULL) { m_pGameCamera[0]->Set(); }
		CManager::GetRenderer()->GetDevice()->Clear(0,
			NULL,
			(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL),
			D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);
		//すべて描画する(左上)
		CScene::DrawAll();

		//カメラの設定
		if (m_pGameCamera[1] != NULL) { m_pGameCamera[1]->Set(); }
		//すべて描画する（右上）
		CManager::GetRenderer()->GetDevice()->Clear(0,
			NULL,
			(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL),
			D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);
		CScene::DrawAll();

		//カメラの設定
		if (m_pGameCamera[2] != NULL) { m_pGameCamera[2]->Set(); }
		CManager::GetRenderer()->GetDevice()->Clear(0,
			NULL,
			(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL),
			D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);
		//すべて描画する（左下）
		CScene::DrawAll();

		//カメラの設定
		if (m_pGameCamera[3] != NULL) { m_pGameCamera[3]->Set(); }
		CManager::GetRenderer()->GetDevice()->Clear(0,
			NULL,
			(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL),
			D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);
		//すべて描画する（右下）
		CScene::DrawAll();
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

	for (int nCntLayer = 0; nCntLayer < MAX_LAYER; nCntLayer++)
	{
			//トップの位置を保存する
			CScene * pScene = CScene::GetTop(nCntLayer);

			while (pScene != NULL)
			{
				CScene * pSceneNext = pScene->GetpNext();	//次のオブジェクトのポインタを保存する

				if (pScene->GetObjType() == CScene2D::OBJTYPE_2DPOLYGON)
				{
					pScene->Draw();
				}

				pScene = pSceneNext;					//ポインタを進める
			}
	}
	// 描画領域を戻す
	pDevice->SetViewport(&ViewPortDef);
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