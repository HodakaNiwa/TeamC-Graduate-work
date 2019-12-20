//=============================================================================
//
// 影処理 [shadow.cpp]
// Author : 佐藤安純 Sato_Asumi
//
//=============================================================================
#include "shadow.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"
#include "camera.h"
#include "fieldmanager.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define TEX_NAME	("data/TEXTURE/Effect/effect000.jpg")

//=============================================================================
// 前方宣言
//=============================================================================
LPDIRECT3DTEXTURE9 CShadow::m_pTexture = NULL;

//=============================================================================
// 生成処理
//=============================================================================
CShadow * CShadow::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	//インスタンスの生成
	CShadow * pShadow;
	pShadow = new CShadow;

	//初期化処理
	pShadow->Init();

	//設定処理
	pShadow->Set(pos, size);
	pShadow->BindTexture(m_pTexture);
	pShadow->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f)); 

	return pShadow;
}

//=============================================================================
// コンストラクタ
//=============================================================================
CShadow::CShadow() : CScene3D(3, OBJTYPE_GRANDEFFECT){}

//=============================================================================
// コンストラクタ
//=============================================================================
CShadow::CShadow(int nPriority, CScene::OBJTYPE obj) : CScene3D(nPriority, obj){}

//=============================================================================
// デストラクタ
//=============================================================================
CShadow::~CShadow(){}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CShadow::Init(void)
{
	CScene3D::Init();
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CShadow::Uninit(void)
{
	CScene3D::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CShadow::Update(void)
{
	CScene3D::Update();
}

//=============================================================================
// 描画処理
//=============================================================================
void CShadow::Draw(void)
{
	CGame *pGame = CManager::GetGame();								// ゲームの取得
	CEventCamera *pEveCam = pGame->GetEveCam();						// イベントカメラの取得
	if (pEveCam == NULL)											// イベントカメラが消えていたら
	{
		//デバイスの取得
		LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

		// αブレンディングを減算合成に設定
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

		CScene3D::Draw();

		// αブレンディングを元に戻す
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	}
}

//=============================================================================
// テクスチャの読み込み
//=============================================================================
void CShadow::LoadTex(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//テクスチャ読み込み
	D3DXCreateTextureFromFile(pDevice, TEX_NAME, &m_pTexture);	//爆発跡
}

//=============================================================================
// テクスチャの破棄
//=============================================================================
void CShadow::UnloadTex(void)
{
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}