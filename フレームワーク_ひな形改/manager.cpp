//=============================================================================
//
// マネージャー処理 [manager.cpp]
// Author : 佐藤安純　SATOU ASUMI
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
#include "modebase.h"
#include "time.h"
#include"inputmouce.h"
#include "RawMouse.h"
//=============================================================================
// マクロ定義
//=============================================================================
#define TEXT_PLYER			("data/TEXT/TestPlayer2.txt")			//プレイヤーのモーション情報テキスト
#define TEXT_BIRD			("data/TEXT/bird.txt")					//鳥のモーション情報テキスト
#define TEXT_RABIT			("data/TEXT/rabit.txt")					//ウサギのモーション情報テキスト
#define TEXT_BUTTERFLY		("data/TEXT/Butterfly.txt")				//蝶のモーション情報テキスト
#define TEXT_PARTICLE		("data/TEXT//EFFECT/Particle.txt")		//エフェクトの情報テキスト
#define TEXT_2DTEXTURE		("data/TEXT/2DTexture.txt")				//2Dテクスチャの情報テキスト
#define TEXT_STAGEGAME		("data/TEXT/STAGE/Stage.txt")			//ゲームステージの情報テキスト
#define TEXT_STAGETUTORIAL	("data/TEXT/STAGE/TutorialStage.txt")	//チュートリアルステージの情報テキスト
#define RANK_1				(100)									//ランキングの初期スコア
#define RANK_2				(200)
#define RANK_3				(300)
#define RANK_4				(400)
#define RANK_5				(500)
#define RANK_6				(600)									//ランキングの初期スコア
#define RANK_7				(700)
#define RANK_8				(800)



//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
CRenderer * CManager::m_pRenderer = NULL;
CSound * CManager::m_pSound = NULL;
CInputKeyboard * CManager::m_pInputKeyboard = NULL;
CGamePad * CManager::m_pInputGamePad = NULL;
CLight * CManager::m_pLight = NULL;
CInputXPad * CManager::m_pXPad = NULL;
CInputMouse * CManager::m_pInputMouse = NULL;
CRawMouse * CManager::m_pRawMouse = NULL;

//画面
CModeBase * CManager::m_pModeBase = NULL;

CFade * CManager::m_pFade = NULL;

int CManager::m_nScore = 0;
int CManager::m_nRankScore[RANK] = {};
int CManager::m_nMaxConbo = 0;
int CManager::m_nDethButterfly = 0;

//モード
CManager::MODE CManager::m_mode = CManager::MODE_SELECT;

//=============================================================================
// コンストラクタ
//=============================================================================
CManager::CManager(){}

//=============================================================================
// デストラクタ
//=============================================================================
CManager::~CManager(){}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, bool bWindow)
{
	//スコアの設定
	m_nRankScore[0] = RANK_1;
	m_nRankScore[1] = RANK_2;
	m_nRankScore[2] = RANK_3;
	m_nRankScore[3] = RANK_4;
	m_nRankScore[4] = RANK_5;
	m_nRankScore[5] = RANK_6;
	m_nRankScore[6] = RANK_7;
	m_nRankScore[7] = RANK_8;

	//サウンド処理
	if (m_pSound == NULL)
	{
		m_pSound = new CSound;
		m_pSound->InitSound(hWnd);
	}

	//レンダリング処理
	if (m_pRenderer == NULL)
	{
		//レンダリングクラスの生成
		m_pRenderer = new CRenderer;

		if (FAILED(m_pRenderer->Init(hWnd, TRUE))) //FALSEで全画面
		{
			return-1;
		}

		//フェードの生成
		if (m_pFade == NULL)
		{
			m_pFade = new CFade;
			m_pFade->Init();
		}

		SetMode(m_mode);	//画面モードの設定
	}

	//ライトの生成
	if (m_pLight == NULL)
	{
		m_pLight = new CLight;

		if (m_pLight != NULL)
		{
			m_pLight->Init();
		}
	}
	
	//入力処理(キーボード)
	if (m_pInputKeyboard == NULL)
	{
		//インプットクラスの生成
		m_pInputKeyboard = new CInputKeyboard;

		if (FAILED(m_pInputKeyboard->Init(hInstance, hWnd)))
		{
			return S_OK;
		}
	}

	//入力処理(ゲームパッド)
	if (m_pInputGamePad == NULL)
	{
		//インプットクラスの生成
		m_pInputGamePad = new CGamePad;

		if (FAILED(m_pInputGamePad->Init(hInstance, hWnd)))
		{
			return S_OK;
		}
	}

	//入力処理(ゲームパッド(Xインプット))
	if (m_pXPad == NULL)
	{
		//インプットクラスの生成
		m_pXPad = new CInputXPad;
		
		if (FAILED(m_pXPad->Init(hInstance, hWnd)))
		{
			return S_OK;
		}
	}

	//入力処理(マウス)
	if (m_pInputMouse == NULL)
	{
		//マウスクラスの生成
		m_pInputMouse = new CInputMouse;

		if (FAILED(m_pInputMouse->Init(hInstance, hWnd)))
		{
			return S_OK;
		}
	}
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CManager::Uninit(void)
{
	//画面の破棄
	if (m_pModeBase != NULL)
	{
		m_pModeBase->Uninit();
		delete m_pModeBase;
		m_pModeBase = NULL;
	}

	//フェード画面の破棄
	if (m_pFade != NULL)
	{
		m_pFade->Uninit();
		m_pFade = NULL;
	}

	//全てのメモリを解放
	CScene::ReleaseAll();

	//入力処理(キーボード)の破棄
	if (m_pInputKeyboard != NULL)
	{
		m_pInputKeyboard->Uninit();
		delete m_pInputKeyboard;
		m_pInputKeyboard = NULL;
	}

	//入力処理(ゲームパッド)の破棄
	if (m_pInputGamePad != NULL)
	{
		m_pInputGamePad->Uninit();
		delete m_pInputGamePad;
		m_pInputGamePad = NULL;
	}

	//入力処理(ゲームパッド(Xインプット))の破棄
	if (m_pXPad != NULL)
	{
		m_pXPad->Uninit();
		delete m_pXPad;
		m_pXPad = NULL;
	}

	//入力処理(マウス)の破棄
	if (m_pInputMouse != NULL)
	{
		m_pInputMouse->Uninit();
		delete m_pInputMouse;
		m_pInputMouse = NULL;
	}

	//入力処理(ラウマウス)の破棄
	if (m_pRawMouse != NULL)
	{
		m_pRawMouse->Uninit();
		delete m_pRawMouse;
		m_pRawMouse = NULL;
	}

	//ライトの破棄
	if (m_pLight != NULL)
	{ 
		m_pLight->Uninit();
		delete m_pLight;
		m_pLight = NULL;
	}

	//レンダリングの破棄
	if (m_pRenderer != NULL)
	{
		m_pRenderer->Uninit();
		delete m_pRenderer;
		m_pRenderer = NULL;
	}

	//サウンドの破棄
	if (m_pSound != NULL)
	{
		m_pSound->UninitSound();
		delete m_pSound;
		m_pSound = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void CManager::Update(void)
{
	//入力処理(キーボード)
	if (m_pInputKeyboard != NULL) { m_pInputKeyboard->Update(); }

	//ゲームパッドの更新
	if (m_pInputGamePad != NULL) { m_pInputGamePad->Update(); };

	//ゲームパッドの更新(Xインプット)
	if (m_pXPad != NULL) { m_pXPad->Update(); }

	//マウスの更新(マウス)
	if (m_pInputMouse != NULL) { m_pInputMouse->Update(); }

	//レンダリング
	if (m_pRenderer != NULL) { m_pRenderer->Update(); }		

	//ゲーム画面の更新
	if (m_pModeBase != NULL) { m_pModeBase->Update(); }

	if(m_pLight != NULL){ m_pLight->Update(); }				//ライト
}

//=============================================================================
// 描画処理
//=============================================================================
void CManager::Draw(void)
{
	//レンダリング
	if (m_pRenderer != NULL){ m_pRenderer->Draw(m_pModeBase); }
}

//=============================================================================
// モードの設定
//=============================================================================
void CManager::SetMode(MODE mode)
{
	//音の停止
	CSound::StopSound();

	if (m_pModeBase != NULL)
	{
		m_pModeBase->Uninit();
		delete m_pModeBase;
		m_pModeBase = NULL;
	}

	//次の画面モードの設定
	m_mode = mode;

	//画面を生成する
	if (m_pModeBase == NULL)
	{
		switch (m_mode)
		{
		case MODE_TITLE:	//タイトル画面
			CSound::PlaySound(CSound::SOUND_LABEL_BGM000);
			m_pModeBase = new CTitle;
			break;

		case MODE_SELECT:	//セレクト画面
			CSound::PlaySound(CSound::SOUND_LABEL_BGM006);
			m_pModeBase = new CSelect;
			break;

		case MODE_TUTORIAL:	//チュートリアル画面
			CSound::PlaySound(CSound::SOUND_LABEL_BGM005);
			m_pModeBase = new CTutorial;
			break;

		case MODE_GAME:		//ゲーム画面
			CSound::PlaySound(CSound::SOUND_LABEL_BGM001);
			m_pModeBase = new CGame;
			break;

		case MODE_RESULT:	//リザルト画面
			m_pModeBase = new CResult;
			break;

		case MODE_RANKING:	//ランキング画面
			CSound::PlaySound(CSound::SOUND_LABEL_BGM004);
			m_pModeBase = new CRanking;
			break;
		}

		if (m_pModeBase != NULL) { m_pModeBase->Init(); }
	}
}

//=============================================================================
// レンダリングの取得
//=============================================================================
CRenderer * CManager::GetRenderer(void)
{
	return m_pRenderer;
}

//=============================================================================
// 入力処理の取得(キーボード)
//=============================================================================
CInputKeyboard * CManager::GetInputkeyboard(void)
{
	return m_pInputKeyboard;
}

//=============================================================================
// ゲットサウンド
//=============================================================================
CSound * CManager::GetSound(void)
{
	return m_pSound;
}

//=============================================================================
// ラウマウスの生成
//=============================================================================
void CManager::CreateRawMouse(HWND hWnd)
{
	m_pRawMouse = CRawMouse::Create(hWnd);
}
