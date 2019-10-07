//=============================================================================
//
// タイトルの処理 [title.cpp]
// Author : shun yokomichi
//
//=============================================================================
#include "title.h"
#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "ranking.h"
#include "loadText.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define FADE_TIME	(60 * 5)
#define LOAD_UI		("data\\TEXT\\UI\\Title.txt")

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CTitle::CTitle(){}

//=============================================================================
// デストラクタ
//=============================================================================
CTitle::~CTitle(){}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CTitle::Init()
{
	//	変数の初期化
	m_nTitleCounter = 0;
	m_ppTexture = NULL;

	//UI情報の読み込み処理
	LoadUI();

	return S_OK;
}
//=============================================================================
// 終了処理
//=============================================================================
void CTitle::Uninit(void)
{
	//テクスチャの破棄
	for (int nCntTex = 0; nCntTex < m_nMaxTexture; nCntTex++)
	{
		if (m_ppTexture[nCntTex] != NULL)
		{
			m_ppTexture[nCntTex]->Release();
			m_ppTexture[nCntTex] = NULL;
		}
	}

	//テクスチャポインタの破棄
	if (m_ppTexture != NULL)
	{
		delete[] m_ppTexture;
		m_ppTexture = NULL;
	}

	//フェード以外削除
	CScene::ReleaseFade();
}

//=============================================================================
// 更新処理
//=============================================================================
void CTitle::Update(void)
{
	//サウンドの情報
	CSound *pSound = CManager::GetSound();

	//	タイトルのカウンター加算
	m_nTitleCounter++;

	//インプットの取得　
	CGamePad	*pGamePad = CManager::GetInputGamePad();
	CInputKeyboard * pKeyboard = CManager::GetInputkeyboard();

	if (pKeyboard->GetKeyboardTrigger(DIK_RETURN) == true)
	{
		if (CFade::GetFadeMode() != CFade::FADE_OUT)
		{
			CFade::SetFade(CManager::MODE_TUTORIAL);
		}
	}
	if ((m_nTitleCounter > FADE_TIME))
	{
		if (CFade::GetFadeMode() != CFade::FADE_OUT)
		{
			CFade::SetFade(CManager::MODE_RANKING);
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CTitle::Draw(void){}

//=============================================================================
// 読み込み処理
//=============================================================================
void CTitle::LoadUI(void)
{
	//レンダリングクラスを取得
	CRenderer * pRenderer = NULL;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//テクスチャ情報の取得
	CLoadTextTexture * pLoadTextTexture = CLoadTextTexture::Create(LOAD_UI);	//2Dテクスチャのポインタを取得
	CLoadTextTexture::TEX_INFO TexInfo = pLoadTextTexture->GetTexInfo();		//テクスチャ情報の取得
	m_nMaxTexture = TexInfo.nMaxTex;											//テクスチャの最大数を取得

	//テクスチャをサイズ分ヒープ領域からメモリを動的確保する
	if (m_ppTexture == NULL)
	{
		m_ppTexture = new LPDIRECT3DTEXTURE9[m_nMaxTexture];
	}

	// テクスチャの生成
	for (int nCntTex = 0; nCntTex < m_nMaxTexture; nCntTex++)
	{
		D3DXCreateTextureFromFile(pDevice,
			TexInfo.ppFileName[nCntTex],
			&m_ppTexture[nCntTex]);
	}

	//UI情報の取得
	int nMaxPolygon = pLoadTextTexture->GetMaxPolygon();

	for (int nCnt = 0; nCnt < nMaxPolygon; nCnt++)
	{
		CLoadTextTexture::POLYGON_INFO PolygonInfo = pLoadTextTexture->GetPolygonInfo(nCnt);

		//UIの生成
		CScene2D * pScene2D = CScene2D::Create(PolygonInfo.pos, PolygonInfo.size, PolygonInfo.type);
		pScene2D->BindTexture(m_ppTexture[PolygonInfo.nTex]);
	}

	//テキスト情報を破棄する
	if (pLoadTextTexture != NULL)
	{
		pLoadTextTexture->Uninit();
		delete pLoadTextTexture;
	}
}