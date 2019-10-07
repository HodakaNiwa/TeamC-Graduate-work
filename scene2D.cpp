//=============================================================================
//
// 2Dポリゴンの処理 [scene2D.cpp]
// Author : 佐藤安純　SATOU ASUMI
//
//=============================================================================
#include "scene2D.h"
#include "manager.h"
#include "input.h"
#include "renderer.h"
#include "scene.h"
#include "input.h"
#include "logo.h"
#include "bottun.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define VECTOR_ZERO		(D3DXVECTOR3(0.0f, 0.0f, 0.0f))	//ベクトルの初期化

//=============================================================================
// 生成処理
//=============================================================================
CScene2D * CScene2D::Create(const D3DXVECTOR3 pos, D3DXVECTOR3 size, TYPE type)
{
	//インスタンスの生成
	CScene2D * pScene2D = NULL;

	if (type == TYPE_LOGO)
	{
		pScene2D = new CLogo(6, CScene::OBJTYPE_2DPOLYGON);
	}
	else if (type == TYPE_BOTTUN)
	{
		pScene2D = new CBottun(6, CScene::OBJTYPE_2DPOLYGON);
	}
	else
	{
		pScene2D = new CScene2D;
	}
	
	//初期化処理
	pScene2D->Init();

	//設定処理
	pScene2D->Set(pos, size);	//位置、サイズの設定
	return pScene2D;
}

//=============================================================================
// コンストラクタ
//=============================================================================
CScene2D::CScene2D(int nPriority, CScene::OBJTYPE type) : CScene(nPriority, type)
{
	m_pTexture = NULL;		// テクスチャへのポインタ
	m_pVtxBuff = NULL;		// 頂点バッファへのポインタ
	m_pos = VECTOR_ZERO;	// ポリゴンの位置
	m_Other = VECTOR_ZERO;	// ポリゴンの対角線
	m_Size = VECTOR_ZERO;	// ポリゴンのサイズ
	m_bUse = false;			// 使用しているかどうか
}

//=============================================================================
// コンストラクタ
//=============================================================================
CScene2D::CScene2D() : CScene(6, CScene::OBJTYPE_2DPOLYGON)
{
	m_pTexture = NULL;		// テクスチャへのポインタ
	m_pVtxBuff = NULL;		// 頂点バッファへのポインタ
	m_pos = VECTOR_ZERO;	// ポリゴンの位置
	m_Other = VECTOR_ZERO;	// ポリゴンの対角線
	m_Size = VECTOR_ZERO;	// ポリゴンのサイズ
	m_bUse = false;			// 使用しているかどうか
}

//=============================================================================
// デストラクタ
//=============================================================================
CScene2D::~CScene2D(){}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CScene2D::Init(void)
{
	//変数宣言
	float fAngle = 0.0f;	//角度
	float fRadius = 0.0f;	//辺の長さ
	m_pTexture = NULL;

	//レンダリングクラスを取得
	CRenderer * pRenderer = NULL;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	VERTEX_2D*pVtx;			//頂点情報へのポインタ

	// ポリゴンの位置を設定
	m_pos	= VECTOR_ZERO;							// ポリゴンの位置
	m_Size	= VECTOR_ZERO;							// ポリゴンのサイズ
	m_Other = VECTOR_ZERO;							// ポリゴンの４頂点
	m_Tex	= D3DXVECTOR2(0.0f, 0.0f);				// テクスチャのUV値
	m_bUse	= false;								// 使用しているかどうか
	m_Color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	//色の設定

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	//ポリゴンの対角線の角度求める
	fAngle = (float)atan2(m_Size.x, m_Size.y);

	//対角線の長さを求める
	fRadius = sqrtf((m_Size.x * m_Size.x) + (m_Size.y * m_Size.y));

	//ポリゴンの長さを保存する
	m_Other.x = sinf(fAngle) * fRadius;
	m_Other.y = cosf(fAngle) * fRadius;

	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点情報を設定
	pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_Other.x, m_pos.y - m_Other.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_Other.x, m_pos.y - m_Other.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_Other.x, m_pos.y + m_Other.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_Other.x, m_pos.y + m_Other.y, 0.0f);

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CScene2D::Uninit()
{
	// 頂点バッファの破棄
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	//テクスチャの破棄
	if (m_pTexture != NULL)
	{
		m_pTexture = NULL;
	}

	//死亡フラグを立てる
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CScene2D::Update(){}

//=============================================================================
// 描画処理
//=============================================================================
void CScene2D::Draw()
{
	//レンダリングクラスを取得
	CRenderer * pRenderer = NULL;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	if (m_pVtxBuff != NULL)
	{
		//頂点バッファをデバイスのデータストリームに設定
		pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);
		//テクスチャの設定
		pDevice->SetTexture(0, m_pTexture);

		if (m_bUse == true)
		{
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2); //ポリゴンの描画
		}
	}
}

//=============================================================================
// 設定処理
//=============================================================================
void CScene2D :: Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
{
	//変数宣言
	float fAngle = 0.0f;	//角度
	float fRadius = 0.0f;	//辺の長さ

	if (m_bUse == false)
	{
		VERTEX_2D*pVtx;		//頂点情報へのポインタ

		m_pos = pos;	//座標設定
		m_Size = size;	//サイズの設定

		//ポリゴンの対角線の角度求める
		fAngle = (float)atan2(m_Size.x, m_Size.y);

		//対角線の長さを求める
		fRadius = sqrtf((m_Size.x * m_Size.x) + (m_Size.y * m_Size.y));

		//ポリゴンの長さを保存する
		m_Other.x = sinf(fAngle) * fRadius;
		m_Other.y = cosf(fAngle) * fRadius;

		//頂点バッファをロックし、頂点データへのポインタを取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点情報を設定
		pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_Other.x, m_pos.y - m_Other.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_Other.x, m_pos.y - m_Other.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_Other.x, m_pos.y + m_Other.y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_Other.x, m_pos.y + m_Other.y, 0.0f);

		//頂点バッファをアンロックする
		m_pVtxBuff->Unlock();

		m_bUse = true;	//使用している状態にする
	}
}

//=============================================================================
// 共有テクスチャの割り当て
//=============================================================================
void CScene2D::BindTexture(const LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}

//=============================================================================
// テクスチャのUV値の取得
//=============================================================================
D3DXVECTOR2 CScene2D::GetTex(void)
{
	return m_Tex;
}

//=============================================================================
// テクスチャのUV値の設定
//=============================================================================
void CScene2D::SetTex(float uv, int nPattrn)
{
	VERTEX_2D*pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(uv * nPattrn, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(uv * nPattrn + uv, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(uv * nPattrn, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(uv * nPattrn + uv, 1.0f);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================================================
// テクスチャのUV値の設定
//=============================================================================
void CScene2D::SetTex2(float uv, float uv2, int nPattrn, int Width)
{
	VERTEX_2D*pVtx;		//頂点情報へのポインタ

						//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(uv * (nPattrn % Width), uv2 * (nPattrn / Width));
	pVtx[1].tex = D3DXVECTOR2(uv * (nPattrn % Width) + uv, uv2 * (nPattrn / Width));
	pVtx[2].tex = D3DXVECTOR2(uv * (nPattrn % Width), uv2 * (nPattrn / Width) + uv2);
	pVtx[3].tex = D3DXVECTOR2(uv * (nPattrn % Width) + uv, uv2 * (nPattrn / Width) + uv2);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================================================
// スクロール処理
//=============================================================================
void CScene2D::SetTexMove(const float uv)
{
	VERTEX_2D*pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f + uv);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f + uv);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f + uv);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f + uv);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================================================
// スクロール処理
//=============================================================================
void CScene2D::SetTexMove2(const float uv)
{
	VERTEX_2D*pVtx;		//頂点情報へのポインタ

						//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f + uv, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f + uv, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f + uv, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f + uv, 1.0f);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================================================
// UV値の設定
//=============================================================================
void CScene2D::SetTexUV(float u, float u2, float v, float v2)
{
	VERTEX_2D*pVtx;		//頂点情報へのポインタ

						//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(u, v);
	pVtx[1].tex = D3DXVECTOR2(u2, v);
	pVtx[2].tex = D3DXVECTOR2(u, v2);
	pVtx[3].tex = D3DXVECTOR2(u2, v2);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================================================
// 座標の取得処理
//=============================================================================
D3DXVECTOR3 CScene2D::GetPos(void)
{
	return m_pos;
}

//=============================================================================
// ４頂点座標の取得処理
//=============================================================================
D3DXVECTOR3 CScene2D::GetOtherPos(void)
{
	return m_Other;
}

//=============================================================================
// 座標設定処理
//=============================================================================
void CScene2D::SetPos(const D3DXVECTOR3 pos, const D3DXVECTOR3 Otherpos)
{
	VERTEX_2D*pVtx;		//頂点情報へのポインタ

	//数値の更新
	m_pos = pos;
	m_Other = Otherpos;

	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点情報を設定
	pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_Other.x, m_pos.y - m_Other.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_Other.x, m_pos.y - m_Other.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_Other.x, m_pos.y + m_Other.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_Other.x, m_pos.y + m_Other.y, 0.0f);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================================================
// 回転ポリゴンの座標設定処理
//=============================================================================
void CScene2D::SetRotPos(const D3DXVECTOR3 pos, const float fAngle, const float fRadius)
{
	m_pos = pos;			//中心座標

	VERTEX_2D*pVtx;			//頂点情報へのポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点情報を設定
	pVtx[0].pos = D3DXVECTOR3(sinf(fAngle + D3DX_PI) * fRadius + m_pos.x, cosf(fAngle + D3DX_PI) * fRadius + m_pos.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(sinf(fAngle + D3DX_PI * 0.5f) * fRadius + m_pos.x, cosf(fAngle + D3DX_PI * 0.5f) * fRadius + m_pos.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(sinf(fAngle + D3DX_PI * -0.5f) * fRadius + m_pos.x, cosf(fAngle + D3DX_PI * -0.5f) * fRadius + m_pos.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(sinf(fAngle) * fRadius + m_pos.x, cosf(fAngle) * fRadius + m_pos.y, 0.0f);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================================================
// サイズの取得処理
//=============================================================================
D3DXVECTOR3 CScene2D::GetSize(void)
{
	return m_Size;
}

//=============================================================================
// サイズの設定処理
//=============================================================================
void CScene2D::SetSize(const D3DXVECTOR3 size)
{
	m_Size = size;

	VERTEX_2D*pVtx;		//頂点情報へのポイン

	//ポリゴンの対角線の角度求める
	float fAngle = (float)atan2(m_Size.x, m_Size.y);

	//対角線の長さを求める
	float fRadius = sqrtf((m_Size.x * m_Size.x) + (m_Size.y * m_Size.y));

	//ポリゴンの長さを保存する
	m_Other.x = sinf(fAngle) * fRadius;
	m_Other.y = cosf(fAngle) * fRadius;

	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点情報を設定
	pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_Other.x, m_pos.y - m_Other.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_Other.x, m_pos.y - m_Other.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_Other.x, m_pos.y + m_Other.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_Other.x, m_pos.y + m_Other.y, 0.0f);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================================================
// 使用状態処理
//=============================================================================
bool CScene2D::GetUse(void)
{
	return m_bUse;
}

//=============================================================================
// サイズの設定処理
//=============================================================================
void CScene2D::SetUse(const bool bUse)
{
	m_bUse = bUse;
}

//=============================================================================
// 色の設定処理
//=============================================================================
void CScene2D::SetColoer(const D3DXCOLOR col)
{
	m_Color = col;		//色の設定
	VERTEX_2D*pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(col.r, col.g, col.b, col.a);
	pVtx[1].col = D3DXCOLOR(col.r, col.g, col.b, col.a);
	pVtx[2].col = D3DXCOLOR(col.r, col.g, col.b, col.a);
	pVtx[3].col = D3DXCOLOR(col.r, col.g, col.b, col.a);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}