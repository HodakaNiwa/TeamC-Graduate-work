//=============================================================================
//
// マネージャー処理 [manager.cpp]
// Author : 佐藤安純　SATOU ASUMI : 山下敦史 : 小林将兵 : 丹羽保貴
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
#include "result.h"
#include "select.h"
#include "modebase.h"
#include "time.h"
#include "RawMouse.h"
#include "collision.h"
#include "debuglog.h"

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
#define RANK_1				(30)									//ランキングの初期スコア
#define RANK_2				(25)
#define RANK_3				(20)
#define RANK_4				(15)
#define RANK_5				(10)
#define RANK_6				(5)									//ランキングの初期スコア
#define RANK_7				(3)
#define RANK_8				(1)

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
CDebugProc * CManager::m_pDebugProc = NULL;
HWND CManager::m_hWnd = NULL;

//画面
CModeBase * CManager::m_pModeBase = NULL;

CFade * CManager::m_pFade = NULL;

int CManager::m_nScore[MAX_PLAYERNUM] = {};
int CManager::m_nCuntry[MAX_PLAYERNUM] = {};
int CManager::m_nCharType[MAX_PLAYERNUM] = {};
int CManager::m_nRankScore[RANK] = {};
int CManager::m_nRankCuntry[RANK] = {};
int CManager::m_nRankCharType[RANK] = {};

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
	//ウインドウの保存
	m_hWnd = hWnd;

	//スコアの設定
	m_nRankScore[0] = RANK_1;
	m_nRankScore[1] = RANK_2;
	m_nRankScore[2] = RANK_3;
	m_nRankScore[3] = RANK_4;
	m_nRankScore[4] = RANK_5;
	m_nRankScore[5] = RANK_6;
	m_nRankScore[6] = RANK_7;
	m_nRankScore[7] = RANK_8;

	//国番号の設定
	m_nRankCuntry[0] = 7;
	m_nRankCuntry[1] = 6;
	m_nRankCuntry[2] = 5;
	m_nRankCuntry[3] = 4;
	m_nRankCuntry[4] = 3;
	m_nRankCuntry[5] = 2;
	m_nRankCuntry[6] = 1;
	m_nRankCuntry[7] = 0;

	//キャラタイプの設定
	m_nRankCharType[0] = 0;
	m_nRankCharType[1] = 2;
	m_nRankCharType[2] = 2;
	m_nRankCharType[3] = 1;
	m_nRankCharType[4] = 1;
	m_nRankCharType[5] = 2;
	m_nRankCharType[6] = 0;
	m_nRankCharType[7] = 0;

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

		if (FAILED(m_pRenderer->Init(hWnd, bWindow))) //FALSEで全画面
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

	//デバッグログ
	if (m_pDebugProc == NULL)
	{
		m_pDebugProc = new CDebugProc;
		if (m_pDebugProc != NULL)
		{
			m_pDebugProc->Init();
		}
	}
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CManager::Uninit(void)
{
	//デバッグログの破棄
	if (m_pDebugProc != NULL)
	{
		m_pDebugProc->Uninit();
		delete m_pDebugProc;
		m_pDebugProc = NULL;
	}

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

	//入力処理(ゲームパッド(Xインプット))の破棄
	if (m_pXPad != NULL)
	{
		m_pXPad->Uninit();
		delete m_pXPad;
		m_pXPad = NULL;
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

	//ゲームパッドの更新(Xインプット)
	if (m_pXPad != NULL) { m_pXPad->Update(); }

	//レンダリングの更新
	if (m_pRenderer != NULL) { m_pRenderer->Update(); }

	//ゲーム画面の更新
	if (m_pModeBase != NULL) { m_pModeBase->Update(); }

	//ライトの更新
	if(m_pLight != NULL){ m_pLight->Update(); }

	if (m_pInputKeyboard->GetKeyboardTrigger(DIK_F3) == true)
	{
		CCollider::SwicthDraw();
	}
	if (m_pInputKeyboard->GetKeyboardTrigger(DIK_F4) == true)
	{
		CDebugProc::SetDisp(CDebugProc::GetDisp() ? false : true);
	}
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
