//=============================================================================
//
// リザルト処理 [result.cpp]
// Author : 佐藤　安純
//
//=============================================================================
#include "result.h"
#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "loadText.h"
#include "camera.h"
#include "fieldmanager.h"
#include "charResult.h"
#include "scene2D.h"
#include "moveUI.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define FADE_OUT_TIME		(60  * 5)		// フェードするまでの時間
#define UI_FILENAME			("data\\TEXT\\UI\\result.txt")
#define LOAD_OBJ			("data/TEXT/Stage.txt")
#define LOAD_TERRITORY		("data/TEXT/Territory.txt")
#define FLAG_JAPAN			("data/TEXTURE/Select/NationalFlag/Japan.png")
#define FLAG_RUSSIA			("data/TEXTURE/Select/NationalFlag/Russia.png")
#define FLAG_AMERICA		("data/TEXTURE/Select/NationalFlag/America.png")
#define FLAG_BRAZIL			("data/TEXTURE/Select/NationalFlag/Brazil.png")
#define FLAG_ENGLAND		("data/TEXTURE/Select/NationalFlag/Engrand.png")
#define FLAG_ITALY			("data/TEXTURE/Select/NationalFlag/Italy.png")
#define FLAG_NEWZEALAND		("data/TEXTURE/Select/NationalFlag/Newzealand.png")
#define FLAG_SOUTHAFRICA	("data/TEXTURE/Select/NationalFlag/SouthAfrica.png")
#define PLAYERNAME			("data/TEXTURE/result/PlayerNumber.png")

//画面が白い状態時のマクロ
#define FLAG_WHIGHTSIZE		(D3DXVECTOR3(130.0f, 100.0f, 0.0f))
#define FLAG_AIMSIZE		(D3DXVECTOR3(75.0f, 60.0f, 0.0f))

//ズーム状態のマクロ
#define CHANGE_TIME	(90)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
CResult::TYPE CResult::m_type = TYPE_MAX;

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CResult::CResult(){}

//=============================================================================
// デストラクタ
//=============================================================================
CResult::~CResult(){}

//=============================================================================
// 初期化処理
//=============================================================================
void CResult::Init(void)
{
	//ベースの初期化
	CModeBase::Init();

	//変数の初期化
	m_pCamera = NULL;
	m_pLoadTerritory = NULL;
	m_pLoadTextObj = NULL;
	m_pFieldManager = NULL;
	m_pWinChar = NULL;
	m_pWhiteBack = NULL;
	m_pNatinalFlag = NULL;
	m_pWinerName = NULL;
	m_state = STATE_WHITE;
	m_bWhiteIn = false;

	for (int nCnt = 0; nCnt < CUNTRY; nCnt++)
	{
		m_pCuntryTex[nCnt] = NULL;
	}
	for (int nCnt = 0; nCnt < RESULT_TEX; nCnt++)
	{
		m_pTexture[nCnt] = NULL;
	}

	//テクスチャの読み込み
	LoadTex();

	//地面の生成
	if (m_pFieldManager == NULL)
	{
		m_pFieldManager = new CFieldManager;
		m_pFieldManager->Init();
	}

	if (m_pLoadTextObj == NULL) { m_pLoadTextObj = CLoadTextObject::Create(LOAD_OBJ); }					//オブジェクトの生成
	if (m_pLoadTerritory == NULL) { m_pLoadTerritory = CLoadTextTerritory::Create(LOAD_TERRITORY); }	//テリトリーの読み込み

	//キャラの生成
	m_pWinChar = CCharResult::Create(INITIALIZE_VECTOR3, "data\\TEXT\\ModelLoad\\SPEEDPLAYER_LOAD.txt", "data\\TEXT\\MotionLoad\\SPEEDPLAYER_MOTION.txt");

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

		m_pCamera = new CResultCamera;
		if (m_pWinChar != NULL) { m_pCamera->SetTargetPos(m_pWinChar->GetPos()); }
		m_pCamera->Init();
		m_pCamera->SetViewport(viewport);
		m_pCamera->SetPosV(D3DXVECTOR3(-40.0f, 60.0f, -100.0f));
		m_pCamera->SetPosR(D3DXVECTOR3(-70.0f, 90.0f, 50.0f));
	}

	//白背景の生成
	if (m_pWhiteBack == NULL) { m_pWhiteBack = CScene2D::Create(D3DXVECTOR3(640.0f, 360.0f, 0.0f), D3DXVECTOR3(640.0f, 360.0f, 0.0f), CScene2D::TYPE_LOGO); }

	//国旗の生成
	if (m_pNatinalFlag == NULL) { m_pNatinalFlag = CMoveUI::Create(D3DXVECTOR3(640.0f, 360.0f, 0.0f), D3DXVECTOR3(140.0f, 110.0f, 0.0f), m_pCuntryTex[0]); }

	//初期化
	m_nCounter = 0;
	m_nTime = 0;
}
//=============================================================================
// 終了処理
//=============================================================================
void CResult::Uninit(void)
{
	//ベースの破棄
	CModeBase::Uninit();

	//カメラの破棄
	if (m_pCamera != NULL)
	{
		m_pCamera->Uninit();
		delete m_pCamera;
		m_pCamera = NULL;
	}

	//地面マネージャーの破棄
	if (m_pFieldManager != NULL)
	{
		m_pFieldManager->Uninit();
		delete m_pFieldManager;
		m_pFieldManager = NULL;
	}

	//オブジェクトの破棄
	if (m_pLoadTextObj != NULL)
	{
		m_pLoadTextObj->Uninit();
		delete m_pLoadTextObj;
		m_pLoadTextObj = NULL;
	}

	//テリトリーの破棄
	if (m_pLoadTerritory != NULL)
	{
		m_pLoadTerritory->Uninit();
		delete m_pLoadTerritory;
		m_pLoadTerritory = NULL;
	}

	m_pWinChar = NULL;
	m_pNatinalFlag = NULL;

	//テクスチャの破棄
	UnloadTex();

	//フェード以外を削除
	CScene::ReleaseFade();
}
//=============================================================================
// 更新処理
//=============================================================================
void CResult::Update(void)
{
	//インプットの取得　
	CGamePad	*pGamePad = CManager::GetInputGamePad();
	CInputKeyboard * pKeyboard = CManager::GetInputkeyboard();
	
	if (pKeyboard->GetKeyboardTrigger(DIK_RETURN) == true)
	{
		if (CFade::FADE_OUT != CFade::GetFadeMode())
		{
			//決定音
			CSound::PlaySound(CSound::SOUND_LABEL_SE007);
			CFade::SetFade(CManager::MODE_TITLE);
		}
	}

	//カメラの更新処理
	if (m_pCamera != NULL) { m_pCamera->Update(); }

	//状態ごとの更新処理
	switch (m_state)
	{
	case STATE_WHITE: UpdateWhite(); break;
	case STATE_ZOOM: UpdateZoom();	break;
	case STATE_RANKING: UpdateRanking(); break;
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CResult::Draw(void)
{
	//カメラの設定
	if (m_pCamera != NULL) { m_pCamera->Set(); }

	CScene::DrawAll();
}

//=============================================================================
// 画面が白い状態の更新処理
//=============================================================================
void CResult::UpdateWhite(void)
{
	if (m_pNatinalFlag == NULL) { return; }

	if (!m_bWhiteIn)
	{
		D3DXVECTOR3 Size = m_pNatinalFlag->GetSize();

		if (Size.x == FLAG_WHIGHTSIZE.x)
		{
			m_bWhiteIn = true;
		}
		else
		{
			m_pNatinalFlag->UpdateScaleDown(FLAG_WHIGHTSIZE, D3DXVECTOR2(0.1f, 0.1f));
		}
	}

	if (!m_bWhiteIn) { return; }

	//国旗を左上に移動する
	m_pNatinalFlag->UpdateScaleDown(FLAG_AIMSIZE, D3DXVECTOR2(1.0f, 0.7f));
	m_pNatinalFlag->UpdateMoveDest(D3DXVECTOR3(140.0f, 90.0f, 0.0f), 8.0f);

	if (m_pWhiteBack != NULL)
	{//背景を透明にする
		//死亡フラグ
		bool bDeth = false;

		//色を取得
		D3DXCOLOR col = m_pWhiteBack->GetColor();
		col.a -= 0.05f;

		if (col.a < 0.0f)
		{
			col.a = 0.0f;
			bDeth = true;
		}

		//色を設定する
		m_pWhiteBack->SetColoer(col);

		if (bDeth)
		{//白背景を破棄する
			m_pWhiteBack->Uninit();
			m_pWhiteBack = NULL;
		}
	}

	//カメラをキャラクターを移す状態にする
	if (m_pCamera != NULL) 
	{ 
		if (m_pCamera->GetState() != CResultCamera::STATE_CHAR_ZOOM)
		{
			m_pCamera->SetState(CResultCamera::STATE_CHAR_IN);
		}
		else
		{
			m_state = STATE_ZOOM;	//ズーム状態にする
		}
	}
}

//=============================================================================
// キャラクターズーム時の処理
//=============================================================================
void CResult::UpdateZoom(void)
{
	//勝者名の生成・移動処理
	if (m_pWinerName == NULL)
	{
		m_pWinerName = CMoveUI::Create(D3DXVECTOR3(350.0f, 600.0f, 0.0f), D3DXVECTOR3(300.0f, 100.0f, 0.0f), m_pTexture[0]);
		m_pWinerName->SetTexUV(0.0f, 1.0f, 0.0f, 0.0833f);
	}
	else
	{
		m_pWinerName->UpdateMoveDest(D3DXVECTOR3(300.0f, 600.0f, 0.0f), 1.0f);
	}

	//時間の加算
	m_nCounter++;

	if (CHANGE_TIME < m_nCounter) { m_state = STATE_RANKING; }
}

//=============================================================================
// ランキングを表示する
//=============================================================================
void CResult::UpdateRanking(void)
{
	if (m_pWinerName != NULL) { m_pWinerName->UpdateMoveDest(D3DXVECTOR3(300.0f, 350.0f, 0.0f), 20.0f); }
}

//=============================================================================
// テクスチャの読み込み処理
//=============================================================================
void CResult::LoadTex(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//国旗テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, FLAG_JAPAN, &m_pCuntryTex[0]);		//日本
	D3DXCreateTextureFromFile(pDevice, FLAG_RUSSIA, &m_pCuntryTex[1]);		//ロシア
	D3DXCreateTextureFromFile(pDevice, FLAG_AMERICA, &m_pCuntryTex[2]);		//アメリカ
	D3DXCreateTextureFromFile(pDevice, FLAG_BRAZIL, &m_pCuntryTex[3]);		//ブラジル
	D3DXCreateTextureFromFile(pDevice, FLAG_ENGLAND, &m_pCuntryTex[4]);		//イギリス
	D3DXCreateTextureFromFile(pDevice, FLAG_ITALY, &m_pCuntryTex[5]);		//イタリア
	D3DXCreateTextureFromFile(pDevice, FLAG_NEWZEALAND, &m_pCuntryTex[6]);	//ニュージーランド
	D3DXCreateTextureFromFile(pDevice, FLAG_SOUTHAFRICA, &m_pCuntryTex[7]);	//南アフリカ

	//リザルトテクスチャの生成
	D3DXCreateTextureFromFile(pDevice, PLAYERNAME, &m_pTexture[0]);		//プレイヤーネームの読み込み
}

//=============================================================================
// テクスチャの破棄
//=============================================================================
void CResult::UnloadTex(void)
{
	//国旗のテクスチャの破棄
	for (int nCnt = 0; nCnt < CUNTRY; nCnt++)
	{
		if (m_pCuntryTex[nCnt] != NULL)
		{
			m_pCuntryTex[nCnt]->Release();
			m_pCuntryTex[nCnt] = NULL;
		}
	}

	//リザルトテクスチャの破棄
	for (int nCnt = 0; nCnt < RESULT_TEX; nCnt++)
	{
		if (m_pTexture[nCnt] != NULL)
		{
			m_pTexture[nCnt]->Release();
			m_pTexture[nCnt] = NULL;
		}
	}
}