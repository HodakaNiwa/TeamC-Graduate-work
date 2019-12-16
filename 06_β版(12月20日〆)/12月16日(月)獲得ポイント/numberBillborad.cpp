//=============================================================================
//
// ビルボード処理 [scenebillbord.cpp]
// Author : 佐藤安純 Sato_Asumi
//
//=============================================================================
#include "numberBillborad.h"
#include "manager.h"
#include "renderer.h"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9 CNumberBillborad::m_pTexture = NULL;

//=============================================================================
//　マクロ定義
//=============================================================================
#define TEX_NAME	("data/TEXTURE/number/number_time.png")

//=============================================================================
// 生成処理
//=============================================================================
CNumberBillborad * CNumberBillborad::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	//インスタンスの生成
	CNumberBillborad * pSceneBillboard;
	pSceneBillboard = new CNumberBillborad;

	//初期化処理
	pSceneBillboard->Init();

	//設定処理
	pSceneBillboard->Set(pos, size);

	return pSceneBillboard;
}

//=============================================================================
// コンストラクタ
//=============================================================================
CNumberBillborad::CNumberBillborad(){}

//=============================================================================
// デストラクタ
//=============================================================================
CNumberBillborad::~CNumberBillborad(){}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CNumberBillborad::Init(void)
{
	//変数の初期化
	m_pos = D3DXVECTOR3(0, 0, 0);
	m_rot = D3DXVECTOR3(0, 0, 0);
	m_Color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//レンダリングクラスを取得
	CRenderer * pRenderer = NULL;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	VERTEX_3D * pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//法線の設定
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	//頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//UV値の設定
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
void CNumberBillborad::Uninit(void)
{
	//頂点バッファの破棄
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void CNumberBillborad::Update(void){}

//=============================================================================
// 描画処理
//=============================================================================
void CNumberBillborad::Draw(void)
{
	//レンダリングクラスを取得
	CRenderer * pRenderer = NULL;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	D3DXMATRIX		  mtxRot, mtxTrans;			// 計算用マトリックス
	D3DXMATRIX		  mtxView;					//ビューマトリクス

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//ビューマトリックスを取得
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	//逆行列を設定
	m_mtxWorld._11 = mtxView._11;
	m_mtxWorld._12 = mtxView._21;
	m_mtxWorld._13 = mtxView._31;
	m_mtxWorld._21 = mtxView._12;
	m_mtxWorld._22 = mtxView._22;
	m_mtxWorld._23 = mtxView._32;
	m_mtxWorld._31 = mtxView._13;
	m_mtxWorld._32 = mtxView._23;
	m_mtxWorld._33 = mtxView._33;

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	if (m_pVtxBuff != NULL)
	{
		// 頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		// テクスチャの設定
		pDevice->SetTexture(0, m_pTexture);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
			0,
			2);
	}
}

//=============================================================================
// 設定処理
//=============================================================================
void CNumberBillborad::Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
{
	m_pos = pos;
	m_size = size;

	VERTEX_3D * pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-size.x, size.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(size.x, size.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-size.x, -size.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(size.x, -size.y, 0.0f);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================================================
// サイズの設定
//=============================================================================
void CNumberBillborad::SetSize(D3DXVECTOR3 size)
{
	//サイズの保存
	m_size = size;

	VERTEX_3D * pVtx;		//頂点情報へのポインタ

	if (m_pVtxBuff != NULL)
	{
		//頂点バッファをロックし、頂点データへのポインタを取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-size.x, size.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(size.x, size.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-size.x, -size.y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(size.x, -size.y, 0.0f);

		//頂点バッファをアンロックする
		m_pVtxBuff->Unlock();
	}
}

//=============================================================================
// 座標の設定
//=============================================================================
void CNumberBillborad::SetPos(D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//=============================================================================
// 色の設定
//=============================================================================
void CNumberBillborad::SetColor(D3DXCOLOR color)
{
	VERTEX_3D * pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	m_Color = color;

	//頂点座標の設定
	pVtx[0].col = D3DXCOLOR(color.r, color.g, color.b, color.a);
	pVtx[1].col = D3DXCOLOR(color.r, color.g, color.b, color.a);
	pVtx[2].col = D3DXCOLOR(color.r, color.g, color.b, color.a);
	pVtx[3].col = D3DXCOLOR(color.r, color.g, color.b, color.a);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================================================
// a値の更新
//=============================================================================
void CNumberBillborad::SetColorABillborad(float colA)
{
	//a値の更新
	m_Color.a = colA;

	//色の設定
	SetColor(m_Color);
}

//=============================================================================
// UVの設定
//=============================================================================
void CNumberBillborad::SetUV(D3DXVECTOR2 uvMIn, D3DXVECTOR2 uvMax)
{
	VERTEX_3D * pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].tex = D3DXVECTOR2(uvMIn.x, uvMIn.y);
	pVtx[1].tex = D3DXVECTOR2(uvMax.x, uvMIn.y);
	pVtx[2].tex = D3DXVECTOR2(uvMIn.x, uvMax.y);
	pVtx[3].tex = D3DXVECTOR2(uvMax.x, uvMax.y);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================================================
// テクスチャの読み込み
//=============================================================================
void CNumberBillborad::LoadTex(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//テクスチャ読み込み
	D3DXCreateTextureFromFile(pDevice, TEX_NAME, &m_pTexture);	
}

//=============================================================================
// テクスチャの破棄
//=============================================================================
void CNumberBillborad::UnloadTex()
{
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}