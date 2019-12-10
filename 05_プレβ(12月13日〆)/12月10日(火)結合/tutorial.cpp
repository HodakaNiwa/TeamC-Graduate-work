//=============================================================================
//
// チュートリアルの処理 [tutorial.cpp]
// Author : 佐藤 安純 & 山下敦
//
//=============================================================================
#include "tutorial.h"
#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "loadText.h"
#include "camera.h"
#include "fieldmanager.h"
#include "loadText.h"
#include "territory.h"
#include "RawMouse.h"
#include "moveUI.h"
#include "loadEffect.h"
#include "tutorialplayer.h"
#include "line.h"
#include "territoryRender.h"
#include "ringRender.h"
#include "mine.h"
#include "effectManager.h"
#include "ring.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define BG_POS	(D3DXVECTOR3(640.0f, 350.0f, 0.0f))
#define BG_SIZE	(D3DXVECTOR3(640.0f, 370.0f, 0.0f))
#define FIRST_POS	(D3DXVECTOR3(1800.0f, 360.0f, 0.0f))
#define MIDDLE_POS	(D3DXVECTOR3(640.0f, 360.0f, 0.0f))
#define SLIDEOUT_POS	(D3DXVECTOR3(-600.0f, 360.0f, 0.0f))
#define MOVE		(50.0f) //50

#define LOGO_SIZE	(D3DXVECTOR3(400.0f, 60.0f, 0.0f))
#define TEXT00_POS	(D3DXVECTOR3(640.0f, 180.0f, 0.0f))
#define TEXT00_SIZE	(D3DXVECTOR3(400.0f, 60.0f, 0.0f))
#define TEXT01_POS	(D3DXVECTOR3(650.0f, 450.0f, 0.0f))
#define TEXT01_SIZE	(D3DXVECTOR3(500.0f, 120.0f, 0.0f))
#define TEXTEND_SIZE (D3DXVECTOR3(500.0f, 160.0f, 0.0f))
#define WINDOW_POS	 (D3DXVECTOR3(640.0f, 350.0f, 0.0f))
#define WINDOW_POS1	 (D3DXVECTOR3(850.0f, 340.0f, 0.0f))
#define WINDOW_SIZE	(D3DXVECTOR3(300.0f, 170.0f, 0.0f))
#define CHANG_COL	(0.025f)			//0.025
#define FADEOUT_TIME (240)				//240
#define GETTERRITORY00_TIME (60 * 7)	//7
#define GETTERRITORY01_TIME (60 * 16)	//16
#define GETTERRITORY02_TIME (60 * 20)	//20
#define DEFFOLT_TIME		(60 * 5)	//5
#define CHAR_TIME		(60 * 6)		//6
#define CHAR_TIME1		(60 * 11)		//11
#define CHAR_TIME2		(60 * 18)		//18


//背景テクスチャのパス
#define TEX_BG_001	("data/TEXTURE/Tutorial/BG/0.png")
#define TEX_BG_002	("data/TEXTURE/Tutorial/BG/1_mine.png")
#define TEX_BG_003	("data/TEXTURE/Tutorial/BG/1_pawer.png")
#define TEX_BG_004	("data/TEXTURE/Tutorial/BG/1_speed.png")
#define TEX_BG_005	("data/TEXTURE/Tutorial/BG/2.png")
#define TEX_BG_006	("data/TEXTURE/Tutorial/BG/3.png")
#define TEX_BG_007	("data/TEXTURE/Tutorial/BG/4.png")
#define TEX_BG_008	("data/TEXTURE/Tutorial/BG/5.png")
#define TEX_BG_009	("data/TEXTURE/Tutorial/BG/6.png")
#define TEX_BG_010	("data/TEXTURE/Tutorial/BG/7.png")

//テキストのテクスチャパス
#define TEX_TURORIAL		("data/TEXTURE/Tutorial/Logo.png")
#define TEX_WIN				("data/TEXTURE/Tutorial/勝利条件.png")
#define TEX_WIN_TEXT1		("data/TEXTURE/Tutorial/テキスト.png")
#define TEX_WIN_TEXT2		("data/TEXTURE/Tutorial/テキスト1.png")
#define TEX_GETTERRITORIY	("data/TEXTURE/Tutorial/領地の取り方.png")
#define TEX_CONTROL			("data/TEXTURE/Tutorial/操作方法.png")
#define TEX_CHARACTER		("data/TEXTURE/Tutorial/選手紹介.png")
#define TEX_END				("data/TEXTURE/Tutorial/終了.png")

//読み込むテキストファイル名
#define LOAD_TERRITORY	("data/TEXT/Territory.txt")
#define LOAD_EFFECT	("data/TEXT/EFFECT/Particle.txt")
#define EFFECTMANAGER_FILENAME ("data/TEXT/EFFECT/ringEffect.txt")

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CTutorial::CTutorial(){}

//=============================================================================
// デストラクタ
//=============================================================================
CTutorial::~CTutorial(){}

//=============================================================================
// 初期化処理
//=============================================================================
void CTutorial::Init(void)
{
	//ベースの初期化
	CModeBase::Init();

	CSound *pSound = CManager::GetSound();
	pSound->PlaySound(CSound::SOUND_LABEL_BGM005);

	//変数の初期化
	m_pBG = NULL;
	m_pTitle = NULL;
	m_pText00 = NULL;
	m_pText01 = NULL;
	m_pWindow = NULL;
	m_pLoadTerritory = NULL;
	m_pCamera = NULL;
	m_pFieldManager = NULL;
	m_pLoadEffect = NULL;
	m_pCharTutorial = NULL;
	m_pTerritoryRender = NULL;
	m_pEffectManager = NULL;
	m_pCharDamege = NULL;
	m_state = STATE_FIRST;
	m_nCountTime = 0;
	m_bMoveSlideTitle = false;
	m_bSlideOut = false;

	//テクスチャの初期化
	for (int nCnt = 0; nCnt < TEX_TUTORIAL_BG; nCnt++)
	{
		m_pTextureBG[nCnt] = NULL;
	}

	//テクスチャの初期化
	for (int nCnt = 0; nCnt < TEX_TUTORIAL; nCnt++)
	{
		m_pTextureText[nCnt] = NULL;
	}

	
	LoadTex();						//テクスチャの読み込み
	CreateMultRenderingTex();		//マルチレンダリング用テクスチャの生成
	SetObjTypeList();				//オブジェクトタイプのリスト設定
	CMine::Load();					//地雷の読み込み

	//エフェクトの読み込み
	if (m_pLoadEffect == NULL)
	{
		m_pLoadEffect = CLoadTextEffect::Create(LOAD_EFFECT);
		CParticle::LoadTexture();
		CEffectTool::LoadEffect();
	}

#if _DEBUG
	//カメラの生成
	if (m_pCamera == NULL)
	{
		D3DVIEWPORT9 viewport;
		viewport.X = 0;		//ビューポートの左上X座標
		viewport.Y = 0;		//ビューポートの左上Y座標
		viewport.Width = SCREEN_WIDTH;	//幅
		viewport.Height = SCREEN_HEIGHT;//高さ
		viewport.MaxZ = 1.0f;
		viewport.MinZ = 0.0f;

		m_pCamera = new CCamera;
		m_pCamera->Init();
		m_pCamera->SetViewport(viewport);
		m_pCamera->SetPosV(D3DXVECTOR3(0.0f, 500.0f, -180.0f));
	}
#endif

	//フィールドの生成
	if (m_pFieldManager == NULL)
	{
		m_pFieldManager = new CFieldManager;
		m_pFieldManager->Init();
	}

	//リングの総数を初期化　
	CRing::ResetMaxRing();

	//テリトリーの生成
	if (m_pLoadTerritory == NULL) { m_pLoadTerritory = CLoadTextTerritory::Create(LOAD_TERRITORY); }	//テリトリー

	//キャラクターの生成
	if (m_pCharTutorial == NULL)
	{
		m_pCharTutorial = CTutorialPlayer::Create(D3DXVECTOR3(10.0f, 0.0f, 0.0f), "data\\TEXT\\ModelLoad\\SPEEDPLAYER_LOAD.txt", "data\\TEXT\\MotionLoad\\SPEEDPLAYER_MOTION.txt", 0, CCharacter::TYPE_JAPAN, CCharacter::CHARCTERTYPE_SPEED);
	}

	// リング描画クラスを生成
	m_pRingRender = CRingRender::Create();

	// テリトリー描画クラスを生成
	m_pTerritoryRender = CTerritoryRender::Create();

	// エフェクト管轄クラスを生成
	m_pEffectManager = CEffectManager::Create(EFFECTMANAGER_FILENAME);
}

//=============================================================================
// 終了処理
//=============================================================================
void CTutorial::Uninit(void)
{
	//ベースの破棄
	CModeBase::Uninit();

	//テクスチャの破棄
	if (m_pTextureMT != NULL)
	{
		m_pTextureMT->Release();
		m_pTextureMT = NULL;
	}

	//レンダラーの破棄
	if (m_pRenderMT != NULL)
	{
		m_pRenderMT->Release();
		m_pRenderMT = NULL;
	}

	//バッファの破棄
	if (m_pBuffMT != NULL)
	{
		m_pBuffMT->Release();
		m_pBuffMT = NULL;
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

	//フィールドマネージャーの破棄
	if (m_pFieldManager != NULL)
	{
		m_pFieldManager->Uninit();
		delete m_pFieldManager;
		m_pFieldManager = NULL;
	}

	if (m_pCamera != NULL)
	{//カメラの破棄
		m_pCamera->Uninit();
		delete m_pCamera;
		m_pCamera = NULL;
	}

	//テクスチャの破棄
	UnloadTex();
	CMine::UnLoad();

	if (m_pTerritoryRender != NULL)
	{//テリトリーレンダラーの破棄
		m_pTerritoryRender = NULL;
	}

	if (m_pRingRender != NULL)
	{//リングレンダラーの破棄
		m_pRingRender = NULL;
	}

	// エフェクト管轄クラスの破棄
	if (m_pEffectManager != NULL)
	{
		m_pEffectManager->Uninit();
		delete m_pEffectManager;
		m_pEffectManager = NULL;
	}

	//フェード以外削除
	CScene::ReleaseFade();
}

//=============================================================================
// 更新処理
//=============================================================================
void CTutorial::Update(void)
{
	//インプットの取得　
	CInputKeyboard * pKeyboard = CManager::GetInputkeyboard();
	CInputXPad * pXPad = CManager::GetXPad();
	CRawMouse *pRawMouse = CManager::GetRawMouse();					//RawMouseの取得
	
	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		if (pKeyboard->GetKeyboardTrigger(DIK_RETURN) == true || pXPad->GetTrigger(XINPUT_GAMEPAD_A, nCnt) == true ||
			pRawMouse->GetTrigger(nCnt, CRawMouse::RIMS_BUTTON_LEFT) == true)
		{
			if (CFade::GetFadeMode() == CFade::FADE_NONE) { CFade::SetFade(CManager::MODE_TITLE); }
		}
	}

	//状態ごとの更新処理
	switch (m_state)
	{
	case STATE_FIRST: UpdateFirst(); break;					 //冒頭
	case STATE_WINCONDITIONS: UpdateWin(); break;			 //勝利条件
	case STATE_GETTERRITORY1: 								 //テリトリーの取得１
	case STATE_GETTERRITORY2: UpdateGetTerritory(); break;	 //テリトリーの取得2
	case STATE_CUTLINE: UpdateCutLine(); break;				 //ラインを切る
	case STATE_POINT: UpdateGetPoint(); break;				 //ポイント
	case STATE_CONTROLER: UpdateControler(); break;			 //コントローラー
	case STATE_CHARACTER: UpdateCharacter(); break;			 //キャラクター
	case STATE_END: UpdateEnd(); break;
	}

	//フィールドマネージャーの更新
	if (m_pFieldManager != NULL) { m_pFieldManager->Update(); }

	//カメラの更新処理
	if (m_pCamera != NULL) { m_pCamera->Update(); }

	//テリトリーレンダラーの更新
	if (m_pTerritoryRender != NULL) { m_pTerritoryRender->Update(); }

	//リングレンダラーの更新
	if (m_pRingRender != NULL) { m_pRingRender->Update(); }
}

//=============================================================================
// 描画処理
//=============================================================================
void CTutorial::Draw(void)
{
	if (m_pCamera != NULL) { m_pCamera->Set(); }
	DrawMultRendering();
	CScene::DrawAll();
}

//=============================================================================
// 暴投状態の更新処理
//=============================================================================
void CTutorial::UpdateFirst(void)
{
	if (m_pBG == NULL)
	{//背景の生成
		m_pBG = CMoveUI::Create(BG_POS, BG_SIZE, m_pTextureBG[0]);
	}

	if (m_pTitle == NULL)
	{//チュートリアルロゴの生成
		m_pTitle = CMoveUI::Create(FIRST_POS, LOGO_SIZE, m_pTextureText[0]);
	}
	else
	{//中心まで移動
		MoveTitle(STATE_WINCONDITIONS);
	}
}

//=============================================================================
// 勝利条件の更新処理
//=============================================================================
void CTutorial::UpdateWin(void)
{
	//変数宣言
	bool bFadeOut = false;

	if (m_pTitle == NULL)
	{//チュートリアルロゴの生成
		m_pTitle = CMoveUI::Create(FIRST_POS, LOGO_SIZE, m_pTextureText[1]);
	}
	else
	{
		m_bSlideOut = MoveSlideTitle();
	}

	if (m_bSlideOut)
	{
		//タイトルロゴの破棄
		DeleteTitle();

		//時間の加算
		m_nCountTime++;

		if (m_pText00 == NULL)
		{//テキストの生成
			m_pText00 = CMoveUI::Create(TEXT00_POS, TEXT00_SIZE, m_pTextureText[3]);
			m_pText00->SetColoer(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
		}
		else
		{
			bFadeOut = TextFadeInOut(m_pText00, FADEOUT_TIME, m_nCountTime);
		}

		if (m_nCountTime > 80)
		{//テキスト生成
			if (m_pText01 == NULL)
			{
				m_pText01 = CMoveUI::Create(TEXT01_POS, TEXT01_SIZE, m_pTextureText[2]);
				m_pText01->SetColoer(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
			}
			else
			{
				bFadeOut = TextFadeInOut(m_pText01, FADEOUT_TIME, m_nCountTime);
			}
		}

		if (bFadeOut)
		{//フェードアウト状態になったら
			m_nCountTime = 0;
			m_bSlideOut = false;

			if (m_pText00 != NULL)
			{//テキストの破棄
				m_pText00->Uninit();
				m_pText00 = NULL;
			}

			if (m_pText01 != NULL)
			{//テキストの破棄
				m_pText01->Uninit();
				m_pText01 = NULL;
			}

			//テリトリーの取得状態にする
			m_state = STATE_GETTERRITORY1;
		}
	}
}

//=============================================================================
// テリトリーの取得条件の更新処理
//=============================================================================
void CTutorial::UpdateGetTerritory(void)
{
	if (m_pTitle == NULL)
	{
		m_pTitle = CMoveUI::Create(FIRST_POS, LOGO_SIZE, m_pTextureText[4]);
	}
	else
	{
		m_bSlideOut = MoveSlideTitle();
	}

	if (m_bSlideOut)
	{
		//タイトルロゴの破棄
		DeleteTitle();

		//時間の加算
		m_nCountTime++;

		if (m_nCountTime >= GETTERRITORY01_TIME)
		{//ラインを切る状態にする
			if (m_nCountTime == GETTERRITORY01_TIME)
			{//移動していない状態にする
				if (m_pCharTutorial != NULL)
				{//プレイヤーを破棄
					CTutorialPlayer * pTutorialPlayer = (CTutorialPlayer *)m_pCharTutorial;
					pTutorialPlayer->SetMoveEnd(false);
					pTutorialPlayer->ResetList();
					pTutorialPlayer->SetPos(D3DXVECTOR3(-360.0f, 0.0f, 20.0f));
					pTutorialPlayer->SetRot(INITIALIZE_VECTOR3);
					CTerritory::ResetColorTerritory();		//テリトリーを初期化する

					//ラインの生成
					CLine::Create(D3DXVECTOR3(-220.0f, 0.0f, -20.0f), D3DXVECTOR3(-250.0f, 0.0f, 150.0f), 1, 0);
					CLine::Create(D3DXVECTOR3(-250.0f, 0.0f, 150.0f), D3DXVECTOR3(-220.0f, 0.0f, 320.0f), 1, 0);
					CLine::Create(D3DXVECTOR3(-220.0f, 0.0f, 320.0f), D3DXVECTOR3(100.0f, 0.0f, 320.0f), 1, 0);
				}

				if (m_pCharTutorial != NULL)
				{//キャラクターを移動状態にする
					CTutorialPlayer * pTutorialPlayer = (CTutorialPlayer *)m_pCharTutorial;
					if (pTutorialPlayer->GetMoveEnd() == false) { pTutorialPlayer->SetMovePlayer(true, 2); }
				}
			}

			m_pBG->BindTexture(m_pTextureBG[6]);
			m_state = STATE_CUTLINE;
			m_nCountTime = 0;
			m_bSlideOut = false;
		}
		else if (m_nCountTime >= GETTERRITORY00_TIME)
		{//BGテクスチャの入替え

			if (m_nCountTime == GETTERRITORY00_TIME)
			{//移動していない状態にする
				if (m_pCharTutorial != NULL)
				{//プレイヤーを破棄
					CTutorialPlayer * pTutorialPlayer = (CTutorialPlayer *)m_pCharTutorial;
					pTutorialPlayer->SetMoveEnd(false);
					pTutorialPlayer->SetPos(D3DXVECTOR3(0.0f, 0.0f, -30.0f));
					pTutorialPlayer->SetRot(INITIALIZE_VECTOR3);
					pTutorialPlayer->ResetList();
					CTerritory::ResetColorTerritory();		//テリトリーを初期化する
				}
			}

			if (m_pCharTutorial != NULL)
			{//キャラクターを移動状態にする
				CTutorialPlayer * pTutorialPlayer = (CTutorialPlayer *)m_pCharTutorial;
				if (pTutorialPlayer->GetMoveEnd() == false) { pTutorialPlayer->SetMovePlayer(true, 1); }
			}

			m_pBG->BindTexture(m_pTextureBG[5]);
		}
		else if (m_nCountTime > 0)
		{//BGテクスチャの入替え
			CreateMultiWindow(WINDOW_POS, WINDOW_SIZE);

			if (m_pCharTutorial != NULL)
			{//キャラクターを移動状態にする
				CTutorialPlayer * pTutorialPlayer = (CTutorialPlayer *)m_pCharTutorial;
				if (pTutorialPlayer->GetMoveEnd() == false){ pTutorialPlayer->SetMovePlayer(true, 0); }
			}

			m_pBG->BindTexture(m_pTextureBG[4]);
		}
	}
}

//=============================================================================
// ラインを切る状態の更新処理
//=============================================================================
void CTutorial::UpdateCutLine(void)
{
	//時間の加算
	m_nCountTime++;

	if (m_nCountTime > /*DEFFOLT_TIME*/ 300)
	{
		m_pBG->BindTexture(m_pTextureBG[7]);
		m_state = STATE_POINT;
		m_nCountTime = 0;
		DeleteMultiWindow();
		DeleteCharacter();
	}
}

//=============================================================================
// ポイントの説明の更新処理
//=============================================================================
void CTutorial::UpdateGetPoint(void)
{
	//時間の加算
	m_nCountTime++;

	if (m_nCountTime > DEFFOLT_TIME)
	{
		m_pBG->BindTexture(m_pTextureBG[0]);
		m_state = STATE_CONTROLER;
		m_nCountTime = 0;
	}
}

//=============================================================================
// コントローラーの更新処理
//=============================================================================
void CTutorial::UpdateControler(void)
{
	if (m_pTitle == NULL)
	{
		m_pTitle = CMoveUI::Create(FIRST_POS, LOGO_SIZE, m_pTextureText[5]);
	}
	else
	{
		m_bSlideOut = MoveSlideTitle();
	}

	if (m_bSlideOut)
	{
		//タイトルの破棄
		DeleteTitle();

		//時間の加算
		m_nCountTime++;

		if (m_nCountTime > DEFFOLT_TIME)
		{
			m_pBG->BindTexture(m_pTextureBG[0]);
			m_nCountTime = 0;
			m_bSlideOut = false;
			m_state = STATE_CHARACTER;
		}
		else if (m_nCountTime > 0)
		{
			m_pBG->BindTexture(m_pTextureBG[8]);
		}
	}
}

//=============================================================================
// キャラクター情報の更新処理
//=============================================================================
void CTutorial::UpdateCharacter(void)
{
	if (m_pTitle == NULL)
	{
		m_pTitle = CMoveUI::Create(FIRST_POS, LOGO_SIZE, m_pTextureText[6]);
	}
	else
	{
		m_bSlideOut = MoveSlideTitle();
	}

	if (m_bSlideOut)
	{
		//タイトルロゴの破棄
		DeleteTitle();

		//時間の加算
		m_nCountTime++;

		if (m_nCountTime > CHAR_TIME2)
		{
			m_pBG->BindTexture(m_pTextureBG[0]);
			m_nCountTime = 0;
			m_bSlideOut = false;
			m_state = STATE_END;
			DeleteMultiWindow();
		}
		else if (m_nCountTime >= CHAR_TIME1)
		{
			if (m_nCountTime == CHAR_TIME1) { DeleteCharacter(); }

			if (m_pCharTutorial == NULL)
			{//スピード型のの生成
				m_pCharTutorial = CTutorialSpeedPlayer::Create(INITIALIZE_VECTOR3, 0, 0);
			}

			m_pBG->BindTexture(m_pTextureBG[3]);
		}
		else if (m_nCountTime >= CHAR_TIME)
		{
			if (m_nCountTime == CHAR_TIME) { DeleteCharacter(); }

			if (m_pCharTutorial == NULL)
			{//パワー型のの生成
				m_pCharTutorial = CTutorialPowerPlayer::Create(D3DXVECTOR3(0.0f, 0.0f, 500.0f), 0, 0);
			}

			if (m_pCharDamege == NULL)
			{//ダメージキャラの生成
				m_pCharDamege = CTutorialSpeedPlayer::Create(D3DXVECTOR3(600.0f, 0.0f, 150.0f), 1, 2);
				CTutorialPlayer * pPlayer = (CTutorialPlayer *)m_pCharDamege;
				pPlayer->SetMovePlayer(true, 7);	//移動ルートの設定
			}
			m_pBG->BindTexture(m_pTextureBG[2]);
		}
		else if (m_nCountTime > 0)
		{
			if (m_pCharTutorial == NULL)
			{//地雷型の生成
				m_pCharTutorial = CTutorialTechniquePlayer::Create(D3DXVECTOR3(-40.0f, 0.0f, 0.0f), 0, 0);
			}

			if (m_pCharDamege == NULL)
			{//ダメージキャラの生成
				m_pCharDamege = CTutorialSpeedPlayer::Create(D3DXVECTOR3(250.0f, 0.0f, 600.0f), 1, 2);
				CTutorialPlayer * pPlayer = (CTutorialPlayer *)m_pCharDamege;
				pPlayer->SetMovePlayer(true, 5);	//移動ルートの設定
			}

			CreateMultiWindow(WINDOW_POS1, WINDOW_SIZE);
			m_pBG->BindTexture(m_pTextureBG[1]);
		}
	}
}

//=============================================================================
// 終了状態の更新処理
//=============================================================================
void CTutorial::UpdateEnd(void)
{
	//変数宣言
	bool bFadeOut = false;

	if (m_pTitle == NULL)
	{
		m_pTitle = CMoveUI::Create(MIDDLE_POS, TEXTEND_SIZE, m_pTextureText[7]);
		m_pTitle->SetColoer(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
	}
	else
	{
		//時間の加算
		m_nCountTime++;

		bFadeOut = TextFadeInOut(m_pTitle, FADEOUT_TIME, m_nCountTime);

		if (bFadeOut)
		{//フェードアウトする
			if (CFade::GetFadeMode() != CFade::FADE_OUT) { CFade::SetFade(CManager::MODE_TITLE); }
		}
	}
}

//=============================================================================
// タイトルロゴの破棄
//=============================================================================
void CTutorial::DeleteTitle(void)
{
	if (m_pTitle != NULL)
	{//タイトルロゴの破棄
		m_pTitle->Uninit();
		m_pTitle = NULL;
	}
}

//=============================================================================
//	タイトルロゴの移動
//=============================================================================
void CTutorial::MoveTitle(STATE NextState)
{
	//変数宣言
	D3DXCOLOR col;

	m_pTitle->UpdateMoveDest(MIDDLE_POS, MOVE);

	if (m_pTitle->GetPos().x == MIDDLE_POS.x)
	{//同じ位置になったら半透明にする
		m_nCountTime++;

		if (m_nCountTime > 60)
		{
			col = m_pTitle->GetColor();
			col.a -= CHANG_COL;

			if (col.a < 0.0f)
			{//色の修正
				col.a = 0.0f;
				m_nCountTime = 0;		//タイマーの初期化
				m_state = NextState;	//勝利条件状態にする
			}

			//色を設定する
			m_pTitle->SetColoer(col);
		}
	}

	//タイトルロゴの破棄
	if (m_state == NextState) { DeleteTitle(); }
}

//=============================================================================
// タイトルロゴ　スライド用
//=============================================================================
bool CTutorial::MoveSlideTitle(void)
{
	//変数宣言
	bool bSetPotision = false;

	if (!m_bMoveSlideTitle)
	{
		//中心まで移動
		m_pTitle->UpdateMoveDest(MIDDLE_POS, MOVE);

		if (m_pTitle->GetPos().x == MIDDLE_POS.x)
		{//移動フラグを有効にする
			m_bMoveSlideTitle = true;
		}
	}
	else
	{
		m_nCountTime++;

		if (m_nCountTime > 60)
		{//画面外へ移動
			m_pTitle->UpdateMoveDest(SLIDEOUT_POS, MOVE);

			if (m_pTitle->GetPos().x == SLIDEOUT_POS.x)
			{
				m_nCountTime = 0;
				m_bMoveSlideTitle = false;
				bSetPotision = true;
			}
		}
	}

	return bSetPotision;
}

//=============================================================================
// テキストのフェードインアウトの処理
//=============================================================================
bool CTutorial::TextFadeInOut(CMoveUI * pText, int nFadeOutTime, int nCountTime)
{
	//変数宣言
	D3DXCOLOR col = INITIALIZE_VECTOR4;
	bool bFadeOut = false;

	if (pText != NULL)
	{
		//色の取得
		col = pText->GetColor();

		if (nFadeOutTime > nCountTime)
		{//フェードイン処理
			col.a += CHANG_COL;

			if (col.a > 1.0f) { col.a = 1.0f; }
		}
		else
		{//フェードアウト処理
			col.a -= CHANG_COL;

			if (col.a < 0.0f)
			{
				col.a = 0.0f;
				bFadeOut = true;
			}
		}

		//色の設定
		pText->SetColoer(col);
	}

	return bFadeOut;
}

//=============================================================================
// ウインドウの生成
//=============================================================================
void CTutorial::CreateMultiWindow(D3DXVECTOR3 Pos, D3DXVECTOR3 Size)
{
	if (m_pWindow == NULL)
	{
		m_pWindow = CMoveUI::Create(Pos, Size, m_pTextureMT);
	}
}

//=============================================================================
// ウインドウの破棄
//=============================================================================
void  CTutorial::DeleteMultiWindow(void)
{
	if (m_pWindow != NULL)
	{
		m_pWindow->Uninit();
		m_pWindow = NULL;
	}
}

//=============================================================================
//	キャラクターの破棄
//=============================================================================
void CTutorial::DeleteCharacter(void)
{
	//チュートリアルキャラの破棄
	if (m_pCharTutorial != NULL)
	{
		m_pCharTutorial->ResetList();	//テリトリーの初期化
		m_pCharTutorial->UninitLine();	//ラインの初期化
		m_pCharTutorial->Uninit();
		m_pCharTutorial = NULL;
	}

	//ダメージキャラの破棄
	if (m_pCharDamege != NULL)
	{
		m_pCharDamege->ResetList();	//テリトリーの初期化
		m_pCharDamege->UninitLine();	//ラインの初期化
		m_pCharDamege->Uninit();
		m_pCharDamege = NULL;
	}
}

//=============================================================================
// マルチレンダリング用のテクスチャを生成する
//=============================================================================
void CTutorial::CreateMultRenderingTex(void)
{
	//変数宣言
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	LPDIRECT3DSURFACE9 pRenderOrg = NULL;
	LPDIRECT3DSURFACE9 pBuffOrg = NULL;
	m_pTextureMT = NULL;
	m_pBuffMT = NULL;
	m_pRenderMT = NULL;

	// Textureの作成
	pDevice->CreateTexture(SCREEN_WIDTH, SCREEN_HEIGHT, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &m_pTextureMT, NULL);

	// レンダラーの取得
	m_pTextureMT->GetSurfaceLevel(0, &m_pRenderMT);

	// バッファの生成
	pDevice->CreateDepthStencilSurface(SCREEN_WIDTH, SCREEN_HEIGHT, D3DFMT_D16, D3DMULTISAMPLE_NONE, 0, TRUE, &m_pBuffMT, NULL);

	pDevice->GetRenderTarget(0, &pRenderOrg);		// 現在のレンダラーターゲットを保存
	pDevice->GetDepthStencilSurface(&pBuffOrg);		// 現在のバッファを保存
	pDevice->SetRenderTarget(0, m_pRenderMT);		// レンダラーの設定
	pDevice->SetDepthStencilSurface(m_pBuffMT);		// バッファの設定
	pDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	// 元に戻す
	pDevice->SetRenderTarget(0, pRenderOrg);		// レンダラー
	pDevice->SetDepthStencilSurface(pBuffOrg);		// バッファ

	// ビューポートの初期化
	m_viewportMT.X = 0;						// Xの設定
	m_viewportMT.Y = 0;						// Yの設定
	m_viewportMT.Width = SCREEN_WIDTH;		// 幅の設定
	m_viewportMT.Height = SCREEN_HEIGHT;	// 高さの設定
	m_viewportMT.MinZ = 0.0f;				// 手前
	m_viewportMT.MaxZ = 1.0f;				// 奥

	//視点、注視点の設定
	m_PosV = D3DXVECTOR3(-50.0f, 575.0f, 75.0f);
	m_PosR = D3DXVECTOR3(-50.0f, 0.0f, 140.0f);
}

//=============================================================================
// マルチレンダリング用の描画処理
//=============================================================================
void CTutorial::DrawMultRendering(void)
{
	//変数宣言
	PDIRECT3DSURFACE9 pRenderOrg = NULL;	//レンダラーの保存用変数
	LPDIRECT3DSURFACE9 pBuffOrg = NULL;	//バッファの保存用変数
	D3DVIEWPORT9 viewportOrg;
	D3DXMATRIX  mtxProjectionOrg;	//プロジェクションマトリックス
	D3DXMATRIX  mtxViewOrg;			//ビューマトリクス
	D3DXMATRIX  mtxProjection;		//プロジェクションマトリックス
	D3DXMATRIX  mtxView;			//ビューマトリクス

	//デバイスの取得
	CRenderer * pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//現在の状態を保存する
	pDevice->GetViewport(&viewportOrg);							//ビューポートの取得
	pDevice->GetTransform(D3DTS_VIEW, &mtxViewOrg);				//ビューマトリックスの取得
	pDevice->GetTransform(D3DTS_PROJECTION, &mtxProjectionOrg);	//プロジェクションマトリックスの取得
	pDevice->GetRenderTarget(0, &pRenderOrg);
	pDevice->GetDepthStencilSurface(&pBuffOrg);

	//設定処理
	pDevice->SetRenderTarget(0, m_pRenderMT);
	pDevice->SetDepthStencilSurface(m_pBuffMT);

	//ターゲット・Zバッファのクリア
	pDevice->Clear(0,
		NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	pDevice->SetViewport(&m_viewportMT);	//ビューポートの設定

	D3DXMatrixIdentity(&mtxView);			//ビューマトリックスの初期化

											// ビューマトリックスの作成
	D3DXMatrixLookAtLH(&mtxView, &m_PosV, &m_PosR, &D3DXVECTOR3(0.0f, 1.0f, 0.0f));

	// ビューマトリックスを設定
	pDevice->SetTransform(D3DTS_VIEW, &mtxView);

	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&mtxProjection);

	// プロジェクションマトリックスを作成
	D3DXMatrixPerspectiveFovLH(&mtxProjection,
		D3DXToRadian(45.0f),
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
		10.0f,
		2500.0f);										//カメラの描画範囲

	// プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &mtxProjection);

	//ここで描画する
	for (int nCntLayer = 0; nCntLayer < MAX_LAYER; nCntLayer++)
	{
		CScene * pSceneTop = CScene::GetTop(nCntLayer); //オブジェクトポインタの先頭を取得

		if (pSceneTop != NULL)
		{
			//トップの位置を保存する
			CScene * pScene = pSceneTop;

			while (pScene != NULL)
			{
				CScene * pSceneNext = pScene->GetpNext();	//次のオブジェクトのポインタを保存する
				CScene::OBJTYPE ObjType = pScene->GetObjType();

				for (int nCntObjType = 0; nCntObjType < MAX_OBJTYPE; nCntObjType++)
				{
					if (ObjType == m_ObjTypeList[nCntObjType])
					{//リスト内に同一のオブジェクトが合ったら描画する
						pScene->Draw();
						break;
					}
				}
				pScene = pSceneNext;					//ポインタを進める
			}
		}
	}

	//デフォルトに戻す
	pDevice->SetViewport(&viewportOrg);							//ビューポートの取得
	pDevice->SetTransform(D3DTS_VIEW, &mtxViewOrg);				//ビューマトリックスの取得
	pDevice->SetTransform(D3DTS_PROJECTION, &mtxProjectionOrg);	//プロジェクションマトリックスの取得
	pDevice->SetRenderTarget(0, pRenderOrg);
	pDevice->SetDepthStencilSurface(pBuffOrg);
}

//=============================================================================
// オブジェクトタイプのリスト
//=============================================================================
void CTutorial::SetObjTypeList(void)
{
	m_ObjTypeList[0] = CScene::OBJTYPE_TERRITORY;
	m_ObjTypeList[1] = CScene::OBJTYPE_FIELDRENDER;
	m_ObjTypeList[2] = CScene::OBJTYPE_FLOR;
	m_ObjTypeList[3] = CScene::OBJTYPE_RING;
	m_ObjTypeList[4] = CScene::OBJTYPE_PARTICLE;
	m_ObjTypeList[5] = CScene::OBJTYPE_TRIANGLE;
	m_ObjTypeList[6] = CScene::OBJTYPE_LINE;
	m_ObjTypeList[7] = CScene::OBJTYPE_PLAYER;
	m_ObjTypeList[8] = CScene::OBJTYPE_TERRITORYRENDER;
	m_ObjTypeList[9] = CScene::OBJTYPE_RINGRENDER;
	m_ObjTypeList[10] = CScene::OBJTYPE_MINE;
	m_ObjTypeList[11] = CScene::OBJTYPE_RINGEMITTER;
	m_ObjTypeList[11] = CScene::OBJTYPE_RINGEFFECT;
}

//=============================================================================
// テクスチャの読み込み
//=============================================================================
void CTutorial::LoadTex(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//背景テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, TEX_BG_001, &m_pTextureBG[0]);
	D3DXCreateTextureFromFile(pDevice, TEX_BG_002, &m_pTextureBG[1]);
	D3DXCreateTextureFromFile(pDevice, TEX_BG_003, &m_pTextureBG[2]);
	D3DXCreateTextureFromFile(pDevice, TEX_BG_004, &m_pTextureBG[3]);
	D3DXCreateTextureFromFile(pDevice, TEX_BG_005, &m_pTextureBG[4]);
	D3DXCreateTextureFromFile(pDevice, TEX_BG_006, &m_pTextureBG[5]);
	D3DXCreateTextureFromFile(pDevice, TEX_BG_007, &m_pTextureBG[6]);
	D3DXCreateTextureFromFile(pDevice, TEX_BG_008, &m_pTextureBG[7]);
	D3DXCreateTextureFromFile(pDevice, TEX_BG_009, &m_pTextureBG[8]);
	D3DXCreateTextureFromFile(pDevice, TEX_BG_010, &m_pTextureBG[9]);

	//テキストテクスチャンの読み込み
	D3DXCreateTextureFromFile(pDevice, TEX_TURORIAL, &m_pTextureText[0]);
	D3DXCreateTextureFromFile(pDevice, TEX_WIN, &m_pTextureText[1]);
	D3DXCreateTextureFromFile(pDevice, TEX_WIN_TEXT1, &m_pTextureText[2]);
	D3DXCreateTextureFromFile(pDevice, TEX_WIN_TEXT2, &m_pTextureText[3]);
	D3DXCreateTextureFromFile(pDevice, TEX_GETTERRITORIY, &m_pTextureText[4]);
	D3DXCreateTextureFromFile(pDevice, TEX_CONTROL, &m_pTextureText[5]);
	D3DXCreateTextureFromFile(pDevice, TEX_CHARACTER, &m_pTextureText[6]);
	D3DXCreateTextureFromFile(pDevice, TEX_END, &m_pTextureText[7]);
}

//=============================================================================
// テクスチャの破棄
//=============================================================================
void CTutorial::UnloadTex(void)
{
	for (int nCnt = 0; nCnt < TEX_TUTORIAL_BG; nCnt++)
	{//背景のテクスチャの破棄
		if (m_pTextureBG[nCnt] != NULL)
		{
			m_pTextureBG[nCnt]->Release();
			m_pTextureBG[nCnt] = NULL;
		}
	}

	for (int nCnt = 0; nCnt < TEX_TUTORIAL; nCnt++)
	{//テキストのテクスチャの破棄
		if (m_pTextureText[nCnt] != NULL)
		{
			m_pTextureText[nCnt]->Release();
			m_pTextureText[nCnt] = NULL;
		}
	}
}