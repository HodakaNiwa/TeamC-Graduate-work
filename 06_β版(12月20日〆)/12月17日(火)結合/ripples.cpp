//=============================================================================
//
// 波紋処理 [ripples.cpp]
// Author : 佐藤安純 Sato_Asumi
//
//=============================================================================
#include "ripples.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"
#include "camera.h"
#include "fieldmanager.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define SIZE		(D3DXVECTOR3(1.0f, 0.0f, 1.0f))
#define LIFE		(12)
#define TEX_NAME	("data/TEXTURE/game/ripples.png")
#define ADD_SIZE	(2.0f)

//=============================================================================
// 前方宣言
//=============================================================================
LPDIRECT3DTEXTURE9 CRipples::m_pTexture = NULL;

//=============================================================================
// 生成処理
//=============================================================================
CRipples * CRipples::Create(const D3DXVECTOR3 pos)
{
	//インスタンスの生成
	CRipples * pField;
	pField = new CRipples;

	//初期化処理
	pField->Init();

	//設定処理
	pField->Set(pos, SIZE);
	pField->BindTexture(m_pTexture);
	pField->SetColor(D3DXCOLOR(0.0f, 0.9f, 1.0f, 1.0f));

	return pField;
}

//=============================================================================
// コンストラクタ
//=============================================================================
CRipples::CRipples() : CScene3D(10, OBJTYPE_GRANDEFFECT){}

//=============================================================================
// コンストラクタ
//=============================================================================
CRipples::CRipples(int nPriority, CScene::OBJTYPE obj) : CScene3D(nPriority, obj){}

//=============================================================================
// デストラクタ
//=============================================================================
CRipples::~CRipples(){}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CRipples::Init(void)
{
	CScene3D::Init();

	//変数の初期化
	m_nLife = LIFE;		//表示時間
	m_bDeth = false;	//死亡フラグ
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CRipples::Uninit(void)
{
	CScene3D::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CRipples::Update(void)
{
	CScene3D::Update();

	//サイズの更新
	D3DXVECTOR3 Size = CScene3D::GetSize();
	Size.x += ADD_SIZE;
	Size.z += ADD_SIZE;

	//サイズの設定
	CScene3D::Set(CScene3D::GetPos(), Size);

	//表示時間を減らす
	m_nLife--;

	if (m_nLife < 0)
	{
		//α値を更新処理
		D3DXCOLOR col = CScene3D::GetCol();
		col.a -= 0.05f;

		//値の修正
		if (col.a < 0.0f)
		{
			col.a = 0.0f;
			m_bDeth = true;
		}

		//色を設定
		CScene3D::SetColor(col);
	}

	//死亡フラグが有効だったら破棄
	if (m_bDeth) { Uninit(); }
}

//=============================================================================
// 描画処理
//=============================================================================
void CRipples::Draw(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//αブレンディングを加算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	//デフォルトの場合FALSEになっているからTRUEにする
	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

	CScene3D::Draw();

	//αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//デフォルトの場合FALSEになっているからTRUEにする
	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
}

//=============================================================================
// テクスチャの読み込み
//=============================================================================
void CRipples::LoadTex(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//テクスチャ読み込み
	D3DXCreateTextureFromFile(pDevice, TEX_NAME, &m_pTexture);	//爆発跡
}

//=============================================================================
// テクスチャの破棄
//=============================================================================
void CRipples::UnloadTex(void)
{
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}