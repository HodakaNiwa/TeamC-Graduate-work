//*****************************************************************************
//
//     リング描画の処理[ringRender.cpp]
//     Author:Hodaka Niwa
//
//*****************************************************************************
#include "ringRender.h"
#include "ring.h"
#include "manager.h"
#include "renderer.h"
#include "shader.h"
#include "territory.h"
#include "debuglog.h"

//*****************************************************************************
//     マクロ定義
//*****************************************************************************
//#define RINGRENDER_UPDATE_EVERYTIME

#define RINGRENDER_SHADER_FILANAME "data/ringShader.hlsl"
#define RINGRENDER_PRIORITY (8)
#define RINGRENDER_RING_PRIORITY (3)
#define RINGRENDER_RING_MODELIDX (1)

//*****************************************************************************
//     静的メンバ変数宣言
//*****************************************************************************


//=============================================================================
//    コンストラクタ
//=============================================================================
CRingRender::CRingRender(int nPriority, OBJTYPE objType) : CScene(nPriority, objType)
{
	m_pVtxBuff = NULL;
	m_pWorldBuff = NULL;
	m_pColorBuff = NULL;
	m_pIdxBuff = NULL;
	m_pDecl = NULL;
	m_pRingShader = NULL;
	m_pTexture = NULL;
	m_nNumInstance = 0;
	m_bDisp = false;
	m_bLighting = false;
}

//=============================================================================
//    デストラクタ
//=============================================================================
CRingRender::~CRingRender()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CRingRender *CRingRender::Create(void)
{
	CRingRender *pRingRender = NULL;
	if (pRingRender != NULL) { return NULL; }

	// メモリを確保
	pRingRender = new CRingRender(RINGRENDER_RING_PRIORITY + 1);
	if (pRingRender == NULL) { return NULL; }

	// インスタンス数を設定
	int nNumInstance = 0;

	// 全てのリングを検索
	CScene *pScene = GetTop(RINGRENDER_RING_PRIORITY);
	CScene *pSceneNext = NULL;
	while (pScene != NULL)
	{
		pSceneNext = pScene->GetpNext();
		if (pScene->GetObjType() == OBJTYPE_RING)
		{// リングだったら
			nNumInstance++;
		}
		pScene = pSceneNext;
	}

	// 値の設定
	pRingRender->SetNumInstance(nNumInstance);

	// 初期化処理
	if (FAILED(pRingRender->Init()))
	{
		pRingRender->Uninit();
		return NULL;
	}

	// モデルを設定する
	pRingRender->BindModel(CTerritory::GetMesh()[RINGRENDER_RING_MODELIDX], NULL);

	return pRingRender;	// インスタンスのアドレスを返す
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CRingRender::Init(void)
{
	// インスタンス数が無ければ処理しない
	if (m_nNumInstance <= 0) { return E_FAIL; }

	// 描画デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	if (pDevice == NULL) { return E_FAIL; }

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

	// 頂点宣言データの生成
	if (FAILED(MakeDeclaration(pDevice)))
	{
		return E_FAIL;
	}

	// シェーダーの読み込み処理
	if (FAILED(LoadShader()))
	{
		return E_FAIL;
	}

	// ライティングと描画をする設定に
	m_bDisp = true;
	m_bLighting = true;

#ifndef RINGRENDER_UPDATE_EVERYTIME
	// ワールドマトリックスバッファをロックし、ワールドマトリックスデータへのポインタを取得
	CRingRender::WORLDMATRIX *pWorld = NULL;
	m_pWorldBuff->Lock(0, 0, (void**)&pWorld, 0);

	// 頂点カラーバッファをロックし、頂点カラーデータへのポインタを取得
	CRingRender::COLORDATA *pColor = NULL;
	m_pColorBuff->Lock(0, 0, (void**)&pColor, 0);

	// 全てのリングを探してリストに追加する
	CScene *pScene = GetTop(RINGRENDER_RING_PRIORITY);
	CScene *pSceneNext = NULL;
	while (pScene != NULL)
	{
		pSceneNext = pScene->GetpNext();
		if (pScene->GetObjType() == OBJTYPE_RING)
		{// リングだったら
			pColor->col = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
			AddList((CRing*)pScene, pWorld, pColor);
			pWorld++;
			pColor++;
		}
		pScene = pSceneNext;
	}
#endif

	return S_OK;
}

//=============================================================================
//    ワールドマトリックスバッファの生成処理
//=============================================================================
HRESULT CRingRender::MakeWorldBuff(const LPDIRECT3DDEVICE9 pDevice)
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
HRESULT CRingRender::MakeColorBuff(const LPDIRECT3DDEVICE9 pDevice)
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
//    頂点宣言データの作成処理
//=============================================================================
HRESULT CRingRender::MakeDeclaration(const LPDIRECT3DDEVICE9 pDevice)
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
//    シェーダーを読み込む処理
//=============================================================================
HRESULT CRingRender::LoadShader(void)
{
	if (m_pRingShader != NULL) { return E_FAIL; }
	m_pRingShader = CRingShader::Create(RINGRENDER_SHADER_FILANAME);
	if (m_pRingShader == NULL) { return E_FAIL; }

	return S_OK;
}

//=============================================================================
//    表示するモデルを設定する処理
//=============================================================================
void CRingRender::BindModel(const LPD3DXMESH pMesh, const LPDIRECT3DTEXTURE9 pTexture)
{
	// デバイスを取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	if (pDevice == NULL) { return; }

	// 頂点数とポリゴン数を獲得
	m_nNumVertex = pMesh->GetNumVertices();
	m_nNumPolygon = pMesh->GetNumFaces();

	// インデックスバッファを取得
	pMesh->GetIndexBuffer(&m_pIdxBuff);

	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEXDATA) * m_nNumVertex,
		D3DUSAGE_WRITEONLY,
		(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1),
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// 頂点情報の設定
	VERTEXDATA *pVtx;

	// 頂点バッファをロックし,頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点バッファをロック
	BYTE *pVtxBuff;  // 頂点バッファへのポインタ
	pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < m_nNumVertex; nCntVtx++)
	{// すべての頂点情報を抜き出す
	    // 頂点座標
		pVtx[nCntVtx].pos = *(D3DXVECTOR3*)pVtxBuff;
		pVtxBuff += sizeof(D3DXVECTOR3);

		// 法線ベクトル
		pVtx[nCntVtx].nor = pVtx[nCntVtx].pos;
		D3DXVec3Normalize(&pVtx[nCntVtx].nor, &-pVtx[nCntVtx].nor);
		pVtxBuff += sizeof(D3DXVECTOR3);

		// テクスチャ座標
		pVtx[nCntVtx].tex = *(D3DXVECTOR2*)pVtxBuff;
		pVtxBuff += sizeof(D3DXVECTOR2);
	}

	// 頂点バッファをアンロック
	pMesh->UnlockVertexBuffer();

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	// テクスチャを割り当てる
	BindTexture(pTexture);
}

//=============================================================================
//    終了処理
//=============================================================================
void CRingRender::Uninit(void)
{
	// 頂点バッファの開放
	DIRECT_RELEASE(m_pVtxBuff);

	// ワールドマトリックスバッファの開放
	DIRECT_RELEASE(m_pWorldBuff);

	// 頂点カラーバッファの開放
	DIRECT_RELEASE(m_pColorBuff);

	// 頂点宣言データの開放
	DIRECT_RELEASE(m_pDecl);

	// シェーダーの開放
	if (m_pRingShader != NULL)
	{
		m_pRingShader->Uninit();
		delete m_pRingShader;
		m_pRingShader = NULL;
	}

	// 死亡フラグを立てる
	Release();
}

//=============================================================================
//    更新処理
//=============================================================================
void CRingRender::Update(void)
{
#ifdef RINGRENDER_UPDATE_EVERYTIME
	// ワールドマトリックスバッファをロックし、ワールドマトリックスデータへのポインタを取得
	CRingRender::WORLDMATRIX *pWorld = NULL;
	m_pWorldBuff->Lock(0, 0, (void**)&pWorld, 0);

	// 頂点カラーバッファをロックし、頂点カラーデータへのポインタを取得
	CRingRender::COLORDATA *pColor = NULL;
	m_pColorBuff->Lock(0, 0, (void**)&pColor, 0);

	// 全てのリングを探してリストに追加する
	CScene *pScene = GetTop(RINGRENDER_RING_PRIORITY);
	CScene *pSceneNext = NULL;
	while (pScene != NULL)
	{
		pSceneNext = pScene->GetpNext();
		if (pScene->GetObjType() == OBJTYPE_RING)
		{// リングだったら
			AddList((CRing*)pScene, pWorld, pColor);
			pWorld++;
			pColor++;
		}
		pScene = pSceneNext;
	}
#endif
}

//=============================================================================
//    バッファに書き込む処理処理
//=============================================================================
void CRingRender::AddList(CRing *pRing, WORLDMATRIX *pWorld, COLORDATA *pCol)
{
	// ワールドマトリックス情報を書き込む
	AddWorld(pRing, pWorld);

	// 頂点カラー情報を書き込む
	AddColor(pRing, pCol);
}

//=============================================================================
//    ワールドマトリックス情報を書き込む処理
//=============================================================================
void CRingRender::AddWorld(CRing *pRing, WORLDMATRIX *pWorld)
{
	D3DXVECTOR3 pos = INITIALIZE_VECTOR3;
	D3DXVECTOR3 rot = INITIALIZE_VECTOR3;
	D3DXVECTOR3 Scale = INITIALIZE_VECTOR3;
	pos = pRing->GetPos();
	rot = pRing->GetRot();
	Scale = pRing->GetScale();
	float fSinPitch = sinf(rot.x);
	float fCosPitch = cosf(rot.x);
	float fSinYaw = sinf(rot.y);
	float fCosYaw = cosf(rot.y);
	float fSinRoll = sinf(rot.z);
	float fCosRoll = cosf(rot.z);
	pWorld[0].m1[0] = fSinRoll * fSinPitch * fSinYaw + fCosRoll * fCosYaw;
	pWorld[0].m1[1] = fSinRoll * fCosPitch;
	pWorld[0].m1[2] = fSinRoll * fSinPitch * fCosYaw - fCosRoll * fSinYaw;
	pWorld[0].m1[3] = 0.0f;
	pWorld[0].m2[0] = fCosRoll * fSinPitch * fSinYaw - fSinRoll * fCosYaw;
	pWorld[0].m2[1] = fCosRoll * fCosPitch;
	pWorld[0].m2[2] = fCosRoll * fSinPitch * fCosYaw + fSinRoll * fSinYaw;
	pWorld[0].m2[3] = 0.0f;
	pWorld[0].m3[0] = fCosPitch * fSinYaw;
	pWorld[0].m3[1] = -fSinPitch;
	pWorld[0].m3[2] = fCosPitch * fCosYaw;
	pWorld[0].m3[3] = 0.0f;
	pWorld[0].m4[0] = pos.x;
	pWorld[0].m4[1] = pos.y;
	pWorld[0].m4[2] = pos.z;
	pWorld[0].m4[3] = 1.0f;

	// 大きさを反映させる
	pWorld[0].m1[0] *= Scale.x;
	pWorld[0].m1[1] *= Scale.x;
	pWorld[0].m1[2] *= Scale.x;
	pWorld[0].m2[0] *= Scale.y;
	pWorld[0].m2[1] *= Scale.y;
	pWorld[0].m2[2] *= Scale.y;
	pWorld[0].m3[0] *= Scale.z;
	pWorld[0].m3[1] *= Scale.z;
	pWorld[0].m3[2] *= Scale.z;
}

//=============================================================================
//    色情報を書き込む処理
//=============================================================================
void CRingRender::AddColor(CRing *pRing, COLORDATA *pCol)
{
	//pCol[0].col = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
	pCol[0].col = pCol[0].col; //安
}

//=============================================================================
//    描画処理
//=============================================================================
void CRingRender::Draw(void)
{
	if (m_pRingShader == NULL || m_bDisp == false) { return; }

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
	m_pRingShader->SetMtxProjection(mtxProj);
	m_pRingShader->SetMtxView(mtxView);

	// テクスチャの設定
	m_pRingShader->BindTexture(m_pTexture);

	// ライトの設定
	D3DLIGHT9 Light;
	for (int nCntLight = 0; nCntLight < MAXLIGHT; nCntLight++)
	{
		pDevice->GetLight(nCntLight, &Light);
		m_pRingShader->SetLightDir(Light.Direction, nCntLight);
		m_pRingShader->SetLightDiffuse(Light.Diffuse, nCntLight);
	}

	// 法線を正規化する(大きさによって法線が変わるので)
	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);

	// シェーダーに値をセット
	m_pRingShader->SetParamToEffect();

	// 描画開始
	m_pRingShader->Begin();

	// レンダリングパス開始
	int nPassIdx = 0;
	if (m_pTexture == NULL) { nPassIdx = 1; }
	m_pRingShader->BeginPass(nPassIdx);

	// ポリゴンの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_nNumVertex, 0, m_nNumPolygon);

	// レンダリングパス終了
	m_pRingShader->EndPass();

	// 描画終了
	m_pRingShader->End();

	// 後始末
	pDevice->SetVertexDeclaration(NULL);
	pDevice->SetStreamSourceFreq(0, 1);
	pDevice->SetStreamSourceFreq(1, 1);
	pDevice->SetStreamSourceFreq(2, 1);
	pDevice->SetStreamSource(1, NULL, 0, 0);
	pDevice->SetStreamSource(2, NULL, 0, 0);

	// 法線を正規化しない
	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, false);
}