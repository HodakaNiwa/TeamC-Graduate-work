//=============================================================================
//
// ランキングの処理 [ranking.cpp]
// Author : shun yokomichi
//
//=============================================================================
#include "ranking.h"
#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "loadText.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define FAREM_COUNT	(60 * 5)	// フェードのフレーム数
#define LOAD_UI		("data\\TEXT\\UI\\Ranking.txt")

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CRanking::CRanking(){}

//=============================================================================
// デストラクタ
//=============================================================================
CRanking::~CRanking(){}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CRanking::Init()
{
	//変数の初期化
	m_ppTexture = NULL;

	//UI情報の読み込み
	LoadUI();

	m_nCntFrame = 0;

	return S_OK;
}
//=============================================================================
// 終了処理
//=============================================================================
void CRanking::Uninit(void)
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
void CRanking::Update(void)
{
	//カウンターの加算
	m_nCntFrame++;

	//インプットの取得　
	CGamePad	*pGamePad = CManager::GetInputGamePad();
	CInputKeyboard * pKeyboard = CManager::GetInputkeyboard();

	if (pKeyboard->GetKeyboardTrigger(DIK_RETURN) == true)
	{//ランキングからタイトルへ
	 //フェードが始まったら
		if (CFade::GetFadeMode() != CFade::FADE_OUT)
		{
			CFade::SetFade(CManager::MODE_TITLE);
		}
	}
}
//=============================================================================
// 描画処理
//=============================================================================
void CRanking::Draw(void){}

//=============================================================================
// 読み込み処理
//=============================================================================
void CRanking::LoadUI(void)
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