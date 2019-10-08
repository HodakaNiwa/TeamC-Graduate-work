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
#include "time.h"
#include "camera.h"
#include "sceneMeshfiled.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define LOAD_UI		("data\\TEXT\\UI\\Game.txt")
#define VECTOR_ZERO	(D3DXVECTOR3(0.0f, 0.0f, 0.0f))

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
CPause *CGame::m_pPause = NULL;
bool CGame::m_bPause = false;
CGame::GAMESTATE CGame::m_gameState = GAMESTATE_NONE;
CSceneMeshFiled * CGame::m_pMeshFiled = NULL;

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CGame::CGame()
{
	m_gameState = GAMESTATE_NONE;
	m_nCounterGameState = 0;
	m_NowGameState = GAMESTATE_NONE;
	m_pGameCamera = NULL;
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

	m_bPause = false;					//ポーズを初期化

	//地面の生成
	if (m_pMeshFiled == NULL) { m_pMeshFiled = CSceneMeshFiled::Create(VECTOR_ZERO); }
	
	//カメラの生成
	if (m_pGameCamera == NULL)
	{
		m_pGameCamera = new CGameCamera;
		m_pGameCamera->Init();
	}

	//UIの読み込み処理
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
	if (m_pGameCamera != NULL)
	{
		m_pGameCamera->Uninit();
		delete m_pGameCamera;
		m_pGameCamera = NULL;
	}

	//変数の初期化
	m_pMeshFiled = NULL;

	//ベースの破棄
	CModeBase::Uninit();

	//フェード以外削除
	CScene::ReleaseFade();
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

	//デバックキー
	if (pCInputKeyBoard->GetKeyboardTrigger(DIK_1) == true) { CFade::SetFade(CManager::MODE_RESULT); }

	//カメラの更新処理
	if (m_pGameCamera != NULL) { m_pGameCamera->Update(); }
}

//=============================================================================
// 描画処理
//=============================================================================
void CGame::Draw(void)
{
	//カメラの設定
	if (m_pGameCamera != NULL) { m_pGameCamera->Set(); }

	//すべて病臥する
	CScene::DrawAll();
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