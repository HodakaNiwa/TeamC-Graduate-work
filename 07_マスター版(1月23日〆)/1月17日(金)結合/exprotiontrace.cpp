//=============================================================================
//
// 爆発跡処理 [exprotiontrace.cpp]
// Author : 佐藤安純 Sato_Asumi
//
//=============================================================================
#include "exprotiontrace.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"
#include "camera.h"
#include "fieldmanager.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define SIZE		(D3DXVECTOR3(80.0f, 0.0f, 80.0f))
#define LIFE		(60 * 4)
#define TEX_NAME	("data/TEXTURE/game/explotion.png")

//=============================================================================
// 前方宣言
//=============================================================================
LPDIRECT3DTEXTURE9 CExprosionTrace::m_pTexture = NULL;

//=============================================================================
// 生成処理
//=============================================================================
CExprosionTrace * CExprosionTrace::Create(const D3DXVECTOR3 pos)
{
	//インスタンスの生成
	CExprosionTrace * pField;
	pField = new CExprosionTrace;

	//初期化処理
	pField->Init();

	//設定処理
	pField->Set(pos, SIZE);
	pField->BindTexture(m_pTexture);
	pField->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f));

	return pField;
}

//=============================================================================
// コンストラクタ
//=============================================================================
CExprosionTrace::CExprosionTrace() : CScene3D(3, OBJTYPE_GRANDEFFECT){}

//=============================================================================
// コンストラクタ
//=============================================================================
CExprosionTrace::CExprosionTrace(int nPriority, CScene::OBJTYPE obj) : CScene3D(nPriority, obj){}

//=============================================================================
// デストラクタ
//=============================================================================
CExprosionTrace::~CExprosionTrace(){}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CExprosionTrace::Init(void)
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
void CExprosionTrace::Uninit(void)
{
	CScene3D::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CExprosionTrace::Update(void)
{
	CScene3D::Update();

	//表示時間を減らす
	m_nLife--;

	if (m_nLife < 0)
	{
		//α値を更新処理
		D3DXCOLOR col = CScene3D::GetCol();
		col.a -= 0.02f;

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
void CExprosionTrace::Draw(void)
{
	CScene3D::Draw();
}

//=============================================================================
// テクスチャの読み込み
//=============================================================================
void CExprosionTrace::LoadTex(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//テクスチャ読み込み
	D3DXCreateTextureFromFile(pDevice, TEX_NAME, &m_pTexture);	//爆発跡
}

//=============================================================================
// テクスチャの破棄
//=============================================================================
void CExprosionTrace::UnloadTex(void)
{
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}