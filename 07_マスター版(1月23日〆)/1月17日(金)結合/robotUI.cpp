//====================================================================================
//
// 第9戦力UI処理	[robotUI.cpp]
// Author;荒谷由朗
//
//====================================================================================
#include "robotUI.h"
#include "game.h"
#include "manager.h"
#include "renderer.h"
#include "character.h"
#include "library.h"
#include "debuglog.h"
#include "scene.h"

//*****************************************************************************
//     マクロ定義
//*****************************************************************************
#define WARNING_TEX_FILENAME_0 "data/TEXTURE/game/Warning000.png"	// 危険警告
#define WARNING_TEX_FILENAME_1 "data/TEXTURE/game/Robot_start.png"	// 出現テロップ

// 危険警告
#define ROBOTUI_WARNING_POS		(D3DXVECTOR3(SCREEN_WIDTH/2,70.0f,1.0f))	// 座標
#define ROBOTUI_WARNING_COL		(D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))			// 色
#define ROBOTUI_WARNING_WIDTH	(SCREEN_WIDTH/2)							// 幅
#define ROBOTUI_WARNING_HEIGHT	(40.0f)										// 高さ

// 出現テロップ
#define ROBOTUI_TELOP_POS		(D3DXVECTOR3(SCREEN_WIDTH/2,600.0f,1.0f))	// 座標
#define ROBOTUI_TELOP_COL		(D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))			// 色
#define ROBOTUI_TELOP_WIDTH		(SCREEN_WIDTH/2)							// 幅
#define ROBOTUI_TELOP_HEIGHT	(100.0f)									// 高さ

//*****************************************************************************
//     静的メンバ変数宣言
//*****************************************************************************
LPDIRECT3DTEXTURE9 CRobotUI::m_apTexture[ROBOTUI_TEXTURE_MAX] = {};

//*****************************************************************************
//     CRobotUIの処理
//*****************************************************************************
//=============================================================================
//    コンストラクタ
//=============================================================================
CRobotUI::CRobotUI()
{
	m_pRobotWarning = NULL;
	m_pRobotTelop	= NULL;
}

//=============================================================================
//    デストラクタ
//=============================================================================
CRobotUI::~CRobotUI()
{
}

//=============================================================================
//    生成処理
//=============================================================================
CRobotUI *CRobotUI::Create(void)
{
	CRobotUI *pRobotUI = NULL;
	if (pRobotUI != NULL) { return NULL; }

	// メモリを確保
	pRobotUI = new CRobotUI;
	if (pRobotUI == NULL) { return NULL; }

	// 初期化処理
	if (FAILED(pRobotUI->Init()))
	{
	}

	return pRobotUI;	// インスタンスのアドレスを返す
}

//=============================================================================
//    テクスチャの読み込み処理
//=============================================================================
HRESULT CRobotUI::Load(void)
{
	char aFileName[ROBOTUI_TEXTURE_MAX][256]
	{
		WARNING_TEX_FILENAME_0,
		WARNING_TEX_FILENAME_1,
	};

	// レンダリングクラス型のポインタ
	CRenderer *pRenderer = CManager::GetRenderer();
	if (pRenderer == NULL) { return E_FAIL; }

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	if (pDevice == NULL) { return E_FAIL; }

	// テクスチャの読み込み
	for (int nCntTex = 0; nCntTex < ROBOTUI_TEXTURE_MAX; nCntTex++)
	{
		D3DXCreateTextureFromFile(pDevice, aFileName[nCntTex], &m_apTexture[nCntTex]);
	}

	return S_OK;
}

//=============================================================================
//    テクスチャの開放処理
//=============================================================================
void CRobotUI::UnLoad(void)
{
	for (int nCntTex = 0; nCntTex < ROBOTUI_TEXTURE_MAX; nCntTex++)
	{
		if (m_apTexture[nCntTex] != NULL)
		{
			m_apTexture[nCntTex]->Release();
			m_apTexture[nCntTex] = NULL;
		}
	}
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CRobotUI::Init(void)
{
	// 各生成処理
	CreateWarning();	// 危険警告
	CreateTelop();		// 出現テロップ

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CRobotUI::Uninit(void)
{
	// 各終了処理
	ReleaseWarning();	// 危険警告を開放
	ReleaseTelop();		// 出現テロップを開放
}

//=============================================================================
//    更新処理
//=============================================================================
void CRobotUI::Update(void)
{
	// 各更新処理
	UpdateWarning();	// 危険警告
	UpdateTelop();		// 出現テロップ
}

//=============================================================================
//    描画処理
//=============================================================================
void CRobotUI::Draw(void)
{
	if (m_pRobotWarning != NULL)	{ m_pRobotWarning->Draw(); }
	if (m_pRobotTelop != NULL)		{ m_pRobotTelop->Draw(); }
}

//=============================================================================
//    危険警告を開放する処理
//=============================================================================
void CRobotUI::CreateWarning(void)
{
	m_pRobotWarning = CRobotUI2D::Create(ROBOTUI_WARNING_POS, ROBOTUI_WARNING_COL, m_apTexture[0],
		(float)ROBOTUI_WARNING_WIDTH, ROBOTUI_WARNING_HEIGHT);
	if (m_pRobotWarning == NULL) { return; }
}

//=============================================================================
//    危険警告を開放する処理
//=============================================================================
void CRobotUI::ReleaseWarning(void)
{
	if (m_pRobotWarning != NULL)
	{
		m_pRobotWarning->Uninit();
		m_pRobotWarning = NULL;
	}
}

//=============================================================================
//    危険警告の動き
//=============================================================================
void CRobotUI::UpdateWarning(void)
{
	if (m_pRobotWarning != NULL)
	{// 危険警告のNULLチェック
		CGame *pGame = CManager::GetGame();		// ゲームの取得

		// 各情報の取得
		float fTexU		= m_pRobotWarning->GetTexU() + 0.001f;
		float fTexV		= m_pRobotWarning->GetTexV();
		float fWidth	= m_pRobotWarning->GetTexWidth();
		float fHeight	= m_pRobotWarning->GetTexHeight();

		m_pRobotWarning->SetTexU(fTexU);
		m_pRobotWarning->SetTexUV(fTexU, fTexU + fWidth, fTexV, fTexV + fHeight);

		bool bEveCamFlag = pGame->GetEVeCamFlag();	// イベントカメラの死亡フラグを取得

		if (bEveCamFlag == true)
		{// イベントカメラが消えていたら消す
			bEveCamFlag = false;	// イベントカメラが消えている状態に
			Uninit();				// 終了処理
		}
	}
}

//=============================================================================
//    危険警告を開放する処理
//=============================================================================
void CRobotUI::CreateTelop(void)
{
	m_pRobotTelop = CRobotUI2D::Create(ROBOTUI_TELOP_POS, ROBOTUI_TELOP_COL, m_apTexture[1],
		(float)ROBOTUI_TELOP_WIDTH, ROBOTUI_TELOP_HEIGHT);
	if (m_pRobotTelop == NULL) { return; }
}

//=============================================================================
//    危険警告を開放する処理
//=============================================================================
void CRobotUI::ReleaseTelop(void)
{
	if (m_pRobotTelop != NULL)
	{
		m_pRobotTelop->Uninit();
		m_pRobotTelop = NULL;
	}
}

//=============================================================================
//    危険警告の動き
//=============================================================================
void CRobotUI::UpdateTelop(void)
{
	if (m_pRobotTelop != NULL)
	{// 出現テロップのNULLチェック

	}
}

//*****************************************************************************
//     CRobotUI2Dの処理
//*****************************************************************************
//=============================================================================
//    コンストラクタ
//=============================================================================
CRobotUI2D::CRobotUI2D(): CScene2D(10, CScene::OBJTYPE_UI)
{
}

//=============================================================================
//    デストラクタ
//=============================================================================
CRobotUI2D::~CRobotUI2D()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CRobotUI2D *CRobotUI2D::Create(D3DXVECTOR3 pos, D3DXCOLOR col, LPDIRECT3DTEXTURE9 pTexture, float fWidth, float fHeight, float fTexU, float fTexV, float fTexWidth, float fTexHeight, float fRot)
{
	// メモリを確保
	CRobotUI2D *pRobotUI2D = NULL;
	pRobotUI2D = new CRobotUI2D;
	if (pRobotUI2D == NULL) { return NULL; }

	// 初期化処理
	pRobotUI2D->Init();
	pRobotUI2D->Set(pos, D3DXVECTOR3(fWidth, fHeight, 0.0f));
	pRobotUI2D->CScene2D::BindTexture(pTexture);

	// 各種値の設定
	pRobotUI2D->SetPos(pos);              // ポリゴンの座標
	pRobotUI2D->SetRot(fRot);             // ポリゴンの向き
	pRobotUI2D->SetCol(col);              // ポリゴンの色
	pRobotUI2D->SetWidth(fWidth);         // ポリゴンの幅
	pRobotUI2D->SetHeight(fHeight);       // ポリゴンの高さ
	pRobotUI2D->SetTexU(fTexU);           // ポリゴンの左上テクスチャU座標
	pRobotUI2D->SetTexV(fTexV);           // ポリゴンの左上テクスチャV座標
	pRobotUI2D->SetTexWidth(fTexWidth);   // ポリゴンのテクスチャ座標の横幅
	pRobotUI2D->SetTexHeight(fTexHeight); // ポリゴンのテクスチャ座標の縦幅
	pRobotUI2D->BindTexture(pTexture);

	return pRobotUI2D;		// インスタンスのアドレスを返す
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CRobotUI2D::Init(void)
{
	CScene2D::Init();

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CRobotUI2D::Uninit(void)
{
	CScene2D::Uninit();
	// 死亡フラグを立てる
	Release();
}

//=============================================================================
//    更新処理
//=============================================================================
void CRobotUI2D::Update(void)
{

}

//=============================================================================
//    描画処理
//=============================================================================
void CRobotUI2D::Draw(void)
{
	CScene2D::Draw();
}
