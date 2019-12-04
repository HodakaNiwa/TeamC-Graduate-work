//*****************************************************************************
//
//     地面描画の処理[fieldRender.cpp]
//     Author:Hodaka Niwa
//
//*****************************************************************************
#include "fieldRender.h"
#include "manager.h"
#include "renderer.h"
#include "shader.h"
#include "debuglog.h"

//*****************************************************************************
//     マクロ定義
//*****************************************************************************


//*****************************************************************************
//     静的メンバ変数宣言
//*****************************************************************************


//=============================================================================
//    コンストラクタ
//=============================================================================
CFieldRender::CFieldRender(int nPriority, OBJTYPE objType)
{
	m_pVtxBuff = NULL;
	m_pWorldBuff = NULL;
	m_pColorBuff = NULL;
	m_pIdxBuff = NULL;
	m_pDecl = NULL;
	m_pFieldShader = NULL;
	m_pTexture = NULL;
	m_nNumInstance = 0;
	m_bDisp = false;
	m_bLighting = false;
}

//=============================================================================
//    デストラクタ
//=============================================================================
CFieldRender::~CFieldRender()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CFieldRender *CFieldRender::Create(int nNumInstance, int nPriority)
{
	CFieldRender *pFieldRender = NULL;
	if (pFieldRender != NULL) { return NULL; }

	// メモリを確保
	pFieldRender = new CFieldRender(nPriority);
	if (pFieldRender == NULL) { return NULL; }

	// 値の設定
	pFieldRender->SetNumInstance(nNumInstance);

	// 初期化処理
	if (FAILED(pFieldRender->Init()))
	{
		pFieldRender->Uninit();
		return NULL;
	}

	return pFieldRender;	// インスタンスのアドレスを返す
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CFieldRender::Init(void)
{
	// 描画デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	if (pDevice == NULL) { return E_FAIL; }

	// 頂点バッファの生成
	if (FAILED(MakeVertex(pDevice)))
	{
		return E_FAIL;
	}

	// ワールドマトリックスバッファの生成
	if (FAILED(MakeWorldBuff(pDevice)))
	{
		return E_FAIL;
	}

	// 頂点カラーバッファの生成
	if (FAILED(MakeColorBuff(pDevice)))
	{
		return E_FAIL;
	}

	// インデックスバッファの生成
	if (FAILED(MakeIndex(pDevice)))
	{
		return E_FAIL;
	}

	// 頂点宣言データの生成
	if (FAILED(MakeDeclaration(pDevice)))
	{
		return E_FAIL;
	}

	// ライティングと描画をする設定に
	m_bDisp = true;
	m_bLighting = true;

	return S_OK;
}

//=============================================================================
//    頂点バッファの生成処理
//=============================================================================
HRESULT CFieldRender::MakeVertex(const LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEXDATA) * 4,
		D3DUSAGE_WRITEONLY,
		(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1),
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);
	if (m_pVtxBuff == NULL) { return E_FAIL; }

	return S_OK;
}

//=============================================================================
//    ワールドマトリックスバッファの生成処理
//=============================================================================
HRESULT CFieldRender::MakeWorldBuff(const LPDIRECT3DDEVICE9 pDevice)
{
	// ワールドマトリックスバッファを生成
	pDevice->CreateVertexBuffer(sizeof(WORLDMATRIX) * m_nNumInstance,
		D3DUSAGE_WRITEONLY,
		0,
		D3DPOOL_MANAGED,
		&m_pWorldBuff,
		NULL);
	if (m_pWorldBuff == NULL) { return E_FAIL; }

	return S_OK;
}

//=============================================================================
//    頂点カラーバッファの生成処理
//=============================================================================
HRESULT CFieldRender::MakeColorBuff(const LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点カラーバッファを生成
	pDevice->CreateVertexBuffer(sizeof(COLORDATA) * m_nNumInstance,
		D3DUSAGE_WRITEONLY,
		0,
		D3DPOOL_MANAGED,
		&m_pColorBuff,
		NULL);
	if (m_pColorBuff == NULL) { return E_FAIL; }

	return S_OK;
}

//=============================================================================
//    インデックスバッファの生成処理
//=============================================================================
HRESULT CFieldRender::MakeIndex(const LPDIRECT3DDEVICE9 pDevice)
{
	// インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * 6,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIdxBuff,
		NULL);
	if (m_pIdxBuff == NULL) { return E_FAIL; }

	WORD *pIdx;       // インデックスデータへのポインタ

	// インデックスバッファをロックし,インデックスデータへのポインタを取得
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	pIdx[0] = 0;
	pIdx[1] = 1;
	pIdx[2] = 2;
	pIdx[3] = 2;
	pIdx[4] = 1;
	pIdx[5] = 3;

	// インデックスバッファをアンロックする
	m_pIdxBuff->Unlock();

	return S_OK;
}

//=============================================================================
//    頂点宣言データの作成処理
//=============================================================================
HRESULT CFieldRender::MakeDeclaration(const LPDIRECT3DDEVICE9 pDevice)
{	// 必要なデータを羅列
	D3DVERTEXELEMENT9 declElems[] =
	{
		// Stream0(繰り返すデータの束)
		{ 0, 0,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },	// 頂点座標
		{ 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 }, 	// 法線ベクトル
		{ 0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },	// UV座標

		// Stream1(ワールド空間上の位置)
		{ 1,  0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1 },	// WORLD 1行目
		{ 1, 16, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 2 },	// WORLD 2行目
		{ 1, 32, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 3 },	// WORLD 3行目
		{ 1, 48, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 4 },	// WORLD 4行目

		// Stream2(頂点カラー)
		{ 2, 0, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },	// 頂点カラー
		D3DDECL_END()
	};

	// 頂点宣言の作成
	if (FAILED(pDevice->CreateVertexDeclaration(declElems, &m_pDecl)))
	{
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CFieldRender::Uninit(void)
{
	// 頂点バッファの開放
	DIRECT_RELEASE(m_pVtxBuff);

	// ワールドマトリックスバッファの開放
	DIRECT_RELEASE(m_pWorldBuff);

	// 頂点カラーバッファの開放
	DIRECT_RELEASE(m_pColorBuff);

	// インデックスバッファの開放
	DIRECT_RELEASE(m_pIdxBuff);

	// 頂点宣言データの開放
	DIRECT_RELEASE(m_pDecl);

	// 死亡フラグを立てる
	Release();
}

//=============================================================================
//    更新処理
//=============================================================================
void CFieldRender::Update(void)
{

}

//=============================================================================
//    描画処理
//=============================================================================
void CFieldRender::Draw(void)
{
	if (m_pFieldShader == NULL || m_bDisp == false) { return; }

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	if (pDevice == NULL) { return; }

	// インスタンシング宣言
	pDevice->SetStreamSourceFreq(0, D3DSTREAMSOURCE_INDEXEDDATA | m_nNumInstance);
	pDevice->SetStreamSourceFreq(1, D3DSTREAMSOURCE_INSTANCEDATA | 1);
	pDevice->SetStreamSourceFreq(2, D3DSTREAMSOURCE_INSTANCEDATA | 1);

	// 頂点宣言データを設定
	pDevice->SetVertexDeclaration(m_pDecl);

	// 頂点バッファをストリームの0番に設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEXDATA));

	// ワールドマトリックスバッファをストリームの1番に設定
	pDevice->SetStreamSource(1, m_pWorldBuff, 0, sizeof(WORLDMATRIX));

	// 頂点カラーバッファをストリームの2番に設定
	pDevice->SetStreamSource(2, m_pColorBuff, 0, sizeof(COLORDATA));

	// インデックスバッファを設定
	pDevice->SetIndices(m_pIdxBuff);

	// プロジェクションマトリックスとビューマトリックスの設定
	D3DXMATRIX mtxView, mtxProj;
	pDevice->GetTransform(D3DTS_PROJECTION, &mtxProj);
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);
	m_pFieldShader->SetMtxProjection(mtxProj);
	m_pFieldShader->SetMtxView(mtxView);

	// テクスチャの設定
	m_pFieldShader->BindTexture(m_pTexture);

	// ライトの設定
	D3DLIGHT9 Light;
	for (int nCntLight = 0; nCntLight < MAXLIGHT; nCntLight++)
	{
		pDevice->GetLight(nCntLight, &Light);
		m_pFieldShader->SetLightDir(Light.Direction, nCntLight);
		m_pFieldShader->SetLightDiffuse(Light.Diffuse, nCntLight);
	}

	// シェーダーに値をセット
	m_pFieldShader->SetParamToEffect();

	// 描画開始
	m_pFieldShader->Begin();

	// レンダリングパス開始
	int nPassIdx = 0;
	if (m_pTexture == NULL) { nPassIdx = 1; }
	if (m_bLighting == false) { nPassIdx += 2; }
	m_pFieldShader->BeginPass(nPassIdx);

	// ポリゴンの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);

	// レンダリングパス終了
	m_pFieldShader->EndPass();

	// 描画終了
	m_pFieldShader->End();

	// 後始末
	pDevice->SetVertexDeclaration(NULL);
	pDevice->SetStreamSourceFreq(0, 1);
	pDevice->SetStreamSourceFreq(1, 1);
	pDevice->SetStreamSourceFreq(2, 1);
	pDevice->SetStreamSource(1, NULL, 0, 0);
	pDevice->SetStreamSource(2, NULL, 0, 0);
}