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
#define RANK_1				(2000)									//ランキングの初期スコア
#define RANK_2				(1500)
#define RANK_3				(1000)
#define RANK_4				(800)
#define RANK_5				(500)

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
CRenderer * CManager::m_pRenderer = NULL;
CSound * CManager::m_pSound = NULL;
CInputKeyboard * CManager::m_pInputKeyboard = NULL;
CGamePad * CManager::m_pInputGamePad = NULL;
CLight * CManager::m_pLight = NULL;

//画面
CTitle * CManager::m_pTitle = NULL;
CSelect * CManager::m_pSelect = NULL;
CTutorial * CManager::m_pTutorial = NULL;
CGame * CManager::m_pGame = NULL;
CResult * CManager::m_pResult = NULL;
CRanking * CManager::m_pRanking = NULL;

CFade * CManager::m_pFade = NULL;

int CManager::m_nScore = 0;
int CManager::m_nRankScore[RANK] = {};
int CManager::m_nMaxConbo = 0;
int CManager::m_nDethButterfly = 0;

//モード
CManager::MODE CManager::m_mode = CManager::MODE_TITLE;

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

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CManager::Uninit(void)
{
	//各画面の破棄
	if (m_pTitle != NULL)		//タイトル画面
	{
		m_pTitle->Uninit();
		delete m_pTitle;
		m_pTitle = NULL;
	}
	if (m_pSelect != NULL)		//セレクト画面
	{
		m_pSelect->Uninit();
		delete m_pSelect;
		m_pSelect = NULL;
	}
	if (m_pTutorial != NULL)	//チュートリアル画面
	{
		m_pTutorial->Uninit();
		delete m_pTutorial;
		m_pTutorial = NULL;
	}
	if (m_pGame != NULL)		//ゲーム画面
	{
		m_pGame->Uninit();
		delete m_pGame;
		m_pGame = NULL;
	}
	if (m_pResult != NULL)		//リザルト画面
	{
		m_pResult->Uninit();
		delete m_pResult;
		m_pResult = NULL;
	}
	if (m_pRanking != NULL)		//ランキング画面
	{
		m_pRanking->Uninit();
		delete m_pRanking;
		m_pRanking = NULL;
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

	switch (m_mode)
	{
	case MODE_TITLE:	
		if (m_pTitle != NULL) { m_pTitle->Update(); }			//タイトル画面
		break;

	case MODE_SELECT:
		if (m_pSelect != NULL) { m_pSelect->Update(); }			//セレクト画面
		break;

	case MODE_TUTORIAL:
		if (m_pTutorial != NULL) { m_pTutorial->Update(); }		//チュートリアル画面
		break;

	case MODE_GAME:		
		if (m_pGame != NULL) { m_pGame->Update(); }				//ゲーム画面
		break;

	case MODE_RESULT:											//リザルト画面
		if (m_pResult != NULL) { m_pResult->Update(); }
		break;

	case MODE_RANKING:	
		if (m_pRanking != NULL) { m_pRanking->Update(); }		//ランキング画面
		break;
	}
	
	if (m_pRenderer != NULL) { m_pRenderer->Update(); }		//レンダリング
	if(m_pLight != NULL){ m_pLight->Update(); }				//ライト
}

//=============================================================================
// 描画処理
//=============================================================================
void CManager::Draw(void)
{
	switch (m_mode)
	{
	case MODE_TITLE:	
		if (m_pTitle != NULL) { m_pTitle->Draw(); }			//タイトル画面
		break;

	case MODE_SELECT:
		if (m_pSelect != NULL) { m_pSelect->Draw(); }		//セレクト画面

	case MODE_TUTORIAL:
		if (m_pTutorial != NULL) { m_pTutorial->Draw(); }	//チュートリアル画面
		break;

	case MODE_GAME:		
		if (m_pGame != NULL) { m_pGame->Draw(); }			//ゲーム画面
		break;

	case MODE_RESULT:
		if (m_pResult != NULL) { m_pResult->Draw(); }		//リザルト画面
		break;

	case MODE_RANKING:	
		if (m_pRanking != NULL) { m_pRanking->Draw(); }		//ランキング画面
		break;
	}

	//レンダリング
	if (m_pRenderer != NULL){ m_pRenderer->Draw(); }
}

//=============================================================================
// モードの設定
//=============================================================================
void CManager::SetMode(MODE mode)
{
	//画面を消す
	switch (m_mode)
	{
	case MODE_TITLE:	//タイトル画面
		if (m_pTitle != NULL)
		{
			CSound::StopSound(CSound::SOUND_LABEL_BGM000);
			m_pTitle->Uninit();
			delete m_pTitle;
			m_pTitle = NULL;
		}
		break;

	case MODE_SELECT:	//セレクト画面
		if (m_pSelect != NULL)
		{
			CSound::StopSound(CSound::SOUND_LABEL_BGM005);
			m_pSelect->Uninit();
			delete m_pSelect;
			m_pSelect = NULL;
		}
		break;

	case MODE_TUTORIAL:	//チュートリアル画面
		if (m_pTutorial != NULL)
		{
			CSound::StopSound(CSound::SOUND_LABEL_BGM005);
			m_pTutorial->Uninit();
			delete m_pTutorial;
			m_pTutorial = NULL;
		}
		break;

	case MODE_GAME:		//ゲーム画面
		if (m_pGame != NULL)
		{
			m_pGame->Uninit();
			delete m_pGame;
			m_pGame = NULL;
		}
		break;

	case MODE_RESULT:
		if (m_pResult != NULL)		//リザルト画面
		{
			m_pResult->Uninit();
			delete m_pResult;
			m_pResult = NULL;
		}
		break;

	case MODE_RANKING:	//ランキング画面
		if (m_pRanking != NULL)
		{
			CSound::StopSound(CSound::SOUND_LABEL_BGM004);
			m_pRanking->Uninit();
			delete m_pRanking;
			m_pRanking = NULL;
		}
		break;
	}

	//次の画面モードの設定
	m_mode = mode;

	//画面を生成する
	switch (m_mode)
	{
	case MODE_TITLE:	//タイトル画面
		if (m_pTitle == NULL)
		{
			CSound::PlaySound(CSound::SOUND_LABEL_BGM000);
			m_pTitle = new CTitle;
			m_pTitle->Init();
		}
		break;

	case MODE_SELECT:
		if (m_pSelect == NULL)
		{
			CSound::PlaySound(CSound::SOUND_LABEL_BGM005);
			m_pSelect = new CSelect;
			m_pSelect->Init();
		}
		break;

	case MODE_TUTORIAL:	//チュートリアル画面
		if (m_pTutorial == NULL)
		{
			CSound::PlaySound(CSound::SOUND_LABEL_BGM005);
			m_pTutorial = new CTutorial;
			m_pTutorial->Init();
		}
		break;

	case MODE_GAME:		//ゲーム画面
		if (m_pGame == NULL)
		{
			CSound::PlaySound(CSound::SOUND_LABEL_BGM001);
			m_pGame = new CGame;
			m_pGame->Init();
		}
		break;

	case MODE_RESULT:	//リザルト画面
		if (m_pResult == NULL)
		{
			CSound::PlaySound(CSound::SOUND_LABEL_BGM001);
			m_pResult = new CResult;
			m_pResult->Init();
		}
		break;

	case MODE_RANKING:	//ランキング画面
		if (m_pRanking == NULL)
		{
			CSound::PlaySound(CSound::SOUND_LABEL_BGM004);
			m_pRanking = new CRanking;
			m_pRanking->Init();
		}
		break;
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