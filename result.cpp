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

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define FADE_OUT_TIME			(60  * 5)		// フェードするまでの時間
//#define FILENAME_CLEAR			("data\\TEXTURE\\result\\Clear.png")	//クリア
//#define FILENAME_OVER			("data\\TEXTURE\\result\\over.png")		//オーバー
#define UI_FILENAME				("data\\TEXT\\UI\\result.txt")

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
HRESULT CResult::Init()
{
	//変数の初期化
	m_nCountTime = 0;
	m_ppTexture = NULL;

	//レンダリングクラスを取得
	CRenderer * pRenderer = NULL;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//テクスチャ情報の取得
	CLoadTextTexture * pLoadTextTexture = CLoadTextTexture::Create(UI_FILENAME);		//2Dテクスチャのポインタを取得
	CLoadTextTexture::TEX_INFO TexInfo = pLoadTextTexture->GetTexInfo();	//テクスチャ情報の取得
	m_nMaxTexture = TexInfo.nMaxTex;										//テクスチャの最大数を取得

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

	return S_OK;
}
//=============================================================================
// 終了処理
//=============================================================================
void CResult::Uninit(void)
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

	//フェード以外を削除
	CScene::ReleaseFade();
}
//=============================================================================
// 更新処理
//=============================================================================
void CResult::Update(void)
{
	//カウンターの加算
	m_nCountTime++;
	
	//インプットの取得　
	CGamePad	*pGamePad = CManager::GetInputGamePad();
	CInputKeyboard * pKeyboard = CManager::GetInputkeyboard();

	if (pKeyboard->GetKeyboardTrigger(DIK_RETURN) == true)
	{
		if (CFade::FADE_OUT != CFade::GetFadeMode())
		{
			CFade::SetFade(CManager::MODE_TITLE);
		}
	}
}
//=============================================================================
// 描画処理
//=============================================================================
void CResult::Draw(void){}