//*****************************************************************************
//
//     画面エフェクトの処理[screenEffect.cpp]
//     Author:Hodaka Niwa
//
//*****************************************************************************
#include "screenEffect.h"
#include "manager.h"
#include "renderer.h"

//*****************************************************************************
//     マクロ定義
//*****************************************************************************
#define SCREENEFFECT_PRIORITY  (8)		// 画面エフェクトの処理優先順位
#define SCREENEFFECT_ALPHA_CUT (0.01f)	// 画面エフェクトの透明度をどれくらい削るか

//*****************************************************************************
//     静的メンバ変数宣言
//*****************************************************************************


//=============================================================================
//    コンストラクタ
//=============================================================================
CScreenEffect::CScreenEffect(int nPriority, OBJTYPE objType) : CScene2D(nPriority, objType)
{

}

//=============================================================================
//    デストラクタ
//=============================================================================
CScreenEffect::~CScreenEffect()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CScreenEffect *CScreenEffect::Create(D3DXVECTOR3 pos, D3DXCOLOR col, D3DXVECTOR3 size)
{
	// メモリを確保
	CScreenEffect *pScreenEffect = NULL;
	pScreenEffect = new CScreenEffect(SCREENEFFECT_PRIORITY);
	if (pScreenEffect == NULL) { return NULL; }

	// 初期化処理
	if (FAILED(pScreenEffect->Init()))
	{
		pScreenEffect->Uninit();
		return NULL;
	}

	// ポリゴンの設定
	pScreenEffect->Set(pos, size);
	pScreenEffect->SetColoer(col);

	return pScreenEffect;	// インスタンスのアドレスを返す
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CScreenEffect::Init(void)
{
	// 共通の初期化処理
	if (FAILED(CScene2D::Init()))
	{
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CScreenEffect::Uninit(void)
{
	CScene2D::Uninit();
}

//=============================================================================
//    更新処理
//=============================================================================
void CScreenEffect::Update(void)
{
	// 透明度を上げる
	AlphaAdd();
}

//=============================================================================
//    透明度を上げる処理処理
//=============================================================================
void CScreenEffect::AlphaAdd(void)
{
	// 色の取得
	D3DXCOLOR col = GetColor();

	// 透明度を増加
	col.a -= SCREENEFFECT_ALPHA_CUT;

	// 色の設定
	SetColoer(col);

	// 死亡判定
	if (col.a <= 0.0f)
	{
		Uninit();
	}
}

//=============================================================================
//    描画処理
//=============================================================================
void CScreenEffect::Draw(void)
{
	// レンダリングクラス型のポインタ
	CRenderer *pRenderer = CManager::GetRenderer();
	if (pRenderer == NULL) { return; }

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	if (pDevice == NULL) { return; }

	bool bChange = false;
	if (GetColor().r == 0.0f && GetColor().b == 0.0f && GetColor().g == 0.0f)
	{
		// αブレンディングを減算合成に設定
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		SetColoer(D3DXCOLOR(1.0f, 1.0f, 1.0f, GetColor().a));
		bChange = true;
	}
	else
	{
		// αブレンディングを加算合成に設定
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	}

	// 共通の描画処理
	CScene2D::Draw();

	// αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	if (bChange == false) { return; }
	SetColoer(D3DXCOLOR(0.0f, 0.0f, 0.0f, GetColor().a));
}