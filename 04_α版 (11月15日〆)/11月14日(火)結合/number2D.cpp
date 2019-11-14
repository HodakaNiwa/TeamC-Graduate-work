//=============================================================================
//
// 数字の処理 [number.cpp]
// Author : 佐藤安純　SATOU ASUMI
//
//=============================================================================
#include "number2D.h"
#include "manager.h"
#include "scene.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define TEXTURE_NAME	("data/TEXTURE/number/number.png")	
#define VECTOR_ZERO		(D3DXVECTOR3(0.0f, 0.0f, 0.0f))		//ベクトルの初期化

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9 CNumber2D::m_pTexture = NULL;

//=============================================================================
// 生成処理
//=============================================================================
CNumber2D * CNumber2D::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, D3DXCOLOR col)
{
	//インスタンスの生成
	CNumber2D * pNumber2D;
	pNumber2D = new CNumber2D;

	//初期化処理
	pNumber2D->Init();

	pNumber2D->Set(pos, size, col);

	return pNumber2D;
}

//=============================================================================
// コンストラクタ
//=============================================================================
CNumber2D::CNumber2D()
{
	m_pVtxBuff	= NULL;				// 頂点バッファへのポインタ
	m_pos		= VECTOR_ZERO;		// 中心座標
	m_Other		= VECTOR_ZERO;		// ４頂点の座標
	m_Size		= VECTOR_ZERO;		// サイズ
	m_HoldPos	= VECTOR_ZERO;		// 座標の保存
	m_bUse = false;					// 使用状態
}

//=============================================================================
// デストラクタ
//=============================================================================
CNumber2D::~CNumber2D(){}

//=============================================================================
// テクスチャの生成
//=============================================================================
HRESULT CNumber2D::Load(void)
{
	//レンダリングクラスを取得
	CRenderer * pRenderer = NULL;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice,
		TEXTURE_NAME,
		&m_pTexture);

	return S_OK;
}

//=============================================================================
// テクスチャの破棄
//=============================================================================
void CNumber2D::Unload(void)
{
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		//m_pTexture = NULL;
	}
}


//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CNumber2D::Init()
{
	//変数の初期化
	m_pos	= VECTOR_ZERO;		// 中心座標
	m_Other = VECTOR_ZERO;		// ４頂点の座標
	m_Size	= VECTOR_ZERO;		// サイズ
	m_bUse = false;				// 使用状態

	//レンダリングクラスを取得
	CRenderer * pRenderer = NULL;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	VERTEX_2D*pVtx;			//頂点情報へのポインタ

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点情報を設定
	pVtx[0].pos = VECTOR_ZERO;
	pVtx[1].pos = VECTOR_ZERO;
	pVtx[2].pos = VECTOR_ZERO;
	pVtx[3].pos = VECTOR_ZERO;

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
	pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CNumber2D::Uninit(void)
{
	// 頂点バッファの破棄
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

}

//=============================================================================
// 更新処理
//=============================================================================
void CNumber2D::Update(void){}

//=============================================================================
// 描画処理
//=============================================================================
void CNumber2D::Draw(void)
{
	//レンダリングクラスを取得
	CRenderer * pRenderer = NULL;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, m_pTexture);

	if (m_bUse == true)
	{
		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}
}

//=============================================================================
// 設定処理
//=============================================================================
void CNumber2D::Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, D3DXCOLOR col)
{
	//変数宣言
	float fAngle = 0.0f;	//角度
	float fRadius = 0.0f;	//辺の長さ

	VERTEX_2D*pVtx;			//頂点情報へのポインタ

	m_pos = pos;			//座標設定
	m_HoldPos = pos;		//座標の保存
	m_Size = size;			//４頂点の座標

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

	//頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(col.r, col.g, col.b, col.a);
	pVtx[1].col = D3DXCOLOR(col.r, col.g, col.b, col.a);
	pVtx[2].col = D3DXCOLOR(col.r, col.g, col.b, col.a);
	pVtx[3].col = D3DXCOLOR(col.r, col.g, col.b, col.a);
	
	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================================================
// 数の設定処理
//=============================================================================
void CNumber2D::SetNumber(const int nNumber)
{
	VERTEX_2D*pVtx;			//頂点情報へのポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f + (0.1f * nNumber), 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + (0.1f * nNumber), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + (0.1f * nNumber), 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + (0.1f * nNumber), 1.0f);

		//頂点バッファをアンロックする
		m_pVtxBuff->Unlock();
	}
}

//=============================================================================
// 使用状態
//=============================================================================
void CNumber2D::SetbUse(bool bUse)
{
	m_bUse = bUse;
}

//=============================================================================
// 移動させる
//=============================================================================
void CNumber2D::SetPos(D3DXVECTOR3 pos)
{
	VERTEX_2D*pVtx;			//頂点情報へのポインタ

	m_pos = pos;			//座標設定

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
// 座標のリセット
//=============================================================================
void CNumber2D::ResetPos(void)
{
	VERTEX_2D*pVtx;			//頂点情報へのポインタ

	m_pos.x = m_HoldPos.x;		//座標設定

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
// 色の設定
//=============================================================================
void CNumber2D::SetColor(D3DXCOLOR col)
{
	VERTEX_2D*pVtx;			//頂点情報へのポインタ

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

//=============================================================================
// サイズの設定
//=============================================================================
void CNumber2D::SetSize(D3DXVECTOR3 size)
{
	//変数宣言
	float fAngle = 0.0f;	//角度
	float fRadius = 0.0f;	//辺の長さ

	m_Size = size;			//サイズの設定

	VERTEX_2D*pVtx;			//頂点情報へのポインタ

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
}