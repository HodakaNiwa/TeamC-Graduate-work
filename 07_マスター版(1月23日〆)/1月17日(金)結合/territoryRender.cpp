//*****************************************************************************
//
//     テリトリー描画の処理[territoryRender.cpp]
//     Author:Hodaka Niwa
//
//*****************************************************************************
#include "territoryRender.h"
#include "manager.h"
#include "renderer.h"
#include "shader.h"
#include "territory.h"
#include "debuglog.h"

//*****************************************************************************
//     マクロ定義
//*****************************************************************************
#define TERRITORYRENDER_SHADER_FILANAME "data/territoryShader.hlsl"
#define TERRITORYRENDER_PRIORITY (8)
#define TERRITORYRENDER_TERRITORY_PRIORITY (3)
#define TERRITORYRENDER_TERRITORY_MODELIDX (0)

//*****************************************************************************
//     静的メンバ変数宣言
//*****************************************************************************


//=============================================================================
//    コンストラクタ
//=============================================================================
CTerritoryRender::CTerritoryRender(int nPriority, OBJTYPE objType) : CScene(nPriority, objType)
{
	m_pVtxBuff = NULL;
	m_pWorldBuff = NULL;
	m_pColorBuff = NULL;
	m_pIdxBuff = NULL;
	m_pDecl = NULL;
	m_pTerritoryShader = NULL;
	m_pTexture = NULL;
	m_nNumInstance = 0;
	m_bDisp = false;
	m_bLighting = false;
}

//=============================================================================
//    デストラクタ
//=============================================================================
CTerritoryRender::~CTerritoryRender()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CTerritoryRender *CTerritoryRender::Create(void)
{
	CTerritoryRender *pTerritoryRender = NULL;
	if (pTerritoryRender != NULL) { return NULL; }

	// メモリを確保
	pTerritoryRender = new CTerritoryRender(TERRITORYRENDER_TERRITORY_PRIORITY + 1);
	if (pTerritoryRender == NULL) { return NULL; }

	// インスタンス数を設定
	int nNumInstance = 0;

	// 全てのテリトリーを検索
	CScene *pScene = GetTop(TERRITORYRENDER_TERRITORY_PRIORITY);
	CScene *pSceneNext = NULL;
	while (pScene != NULL)
	{
		pSceneNext = pScene->GetpNext();
		if (pScene->GetObjType() == OBJTYPE_TERRITORY)
		{// テリトリーだったら
			nNumInstance++;
		}
		pScene = pSceneNext;
	}

	// 値の設定
	pTerritoryRender->SetNumInstance(nNumInstance);

	// 初期化処理
	if (FAILED(pTerritoryRender->Init()))
	{
		pTerritoryRender->Uninit();
		return NULL;
	}

	// モデルを設定する
	pTerritoryRender->BindModel(CTerritory::GetMesh()[TERRITORYRENDER_TERRITORY_MODELIDX], CTerritory::GetTexture()[TERRITORYRENDER_TERRITORY_MODELIDX]);

	return pTerritoryRender;	// インスタンスのアドレスを返す
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CTerritoryRender::Init(void)
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

	return S_OK;
}

//=============================================================================
//    ワールドマトリックスバッファの生成処理
//=============================================================================
HRESULT CTerritoryRender::MakeWorldBuff(const LPDIRECT3DDEVICE9 pDevice)
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
HRESULT CTerritoryRender::MakeColorBuff(const LPDIRECT3DDEVICE9 pDevice)
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
HRESULT CTerritoryRender::MakeDeclaration(const LPDIRECT3DDEVICE9 pDevice)
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
HRESULT CTerritoryRender::LoadShader(void)
{
	if (m_pTerritoryShader != NULL) { return E_FAIL; }
	m_pTerritoryShader = CTerritoryShader::Create(TERRITORYRENDER_SHADER_FILANAME);
	if (m_pTerritoryShader == NULL) { return E_FAIL; }

	return S_OK;
}

//=============================================================================
//    表示するモデルを設定する処理
//=============================================================================
void CTerritoryRender::BindModel(const LPD3DXMESH pMesh, const LPDIRECT3DTEXTURE9 pTexture)
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
void CTerritoryRender::Uninit(void)
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
	if (m_pTerritoryShader != NULL)
	{
		m_pTerritoryShader->Uninit();
		delete m_pTerritoryShader;
		m_pTerritoryShader = NULL;
	}

	// 死亡フラグを立てる
	Release();
}

//=============================================================================
//    更新処理
//=============================================================================
void CTerritoryRender::Update(void)
{
	// ワールドマトリックスバッファをロックし、ワールドマトリックスデータへのポインタを取得
	CTerritoryRender::WORLDMATRIX *pWorld = NULL;
	m_pWorldBuff->Lock(0, 0, (void**)&pWorld, 0);

	// 頂点カラーバッファをロックし、頂点カラーデータへのポインタを取得
	CTerritoryRender::COLORDATA *pColor = NULL;
	m_pColorBuff->Lock(0, 0, (void**)&pColor, 0);

	// 全てのテリトリーを探してリストに追加する
	CScene *pScene = GetTop(TERRITORYRENDER_TERRITORY_PRIORITY);
	CScene *pSceneNext = NULL;
	while (pScene != NULL)
	{
		pSceneNext = pScene->GetpNext();
		if (pScene->GetObjType() == OBJTYPE_TERRITORY)
		{// テリトリーだったら
			AddList((CTerritory*)pScene, pWorld, pColor);
			pWorld++;
			pColor++;
		}
		pScene = pSceneNext;
	}

	// ワールドマトリックスバッファをアンロックする
	m_pWorldBuff->Unlock();

	// 頂点カラーバッファをアンロックする
	m_pColorBuff->Unlock();
}

//=============================================================================
//    バッファに書き込む処理処理
//=============================================================================
void CTerritoryRender::AddList(CTerritory *pTerritory, WORLDMATRIX *pWorld, COLORDATA *pCol)
{
	// ワールドマトリックス情報を書き込む
	AddWorld(pTerritory, pWorld);

	// 頂点カラー情報を書き込む
	AddColor(pTerritory, pCol);
}

//=============================================================================
//    ワールドマトリックス情報を書き込む処理
//=============================================================================
void CTerritoryRender::AddWorld(CTerritory *pTerritory, WORLDMATRIX *pWorld)
{
	D3DXVECTOR3 pos = INITIALIZE_VECTOR3;
	D3DXVECTOR3 rot = INITIALIZE_VECTOR3;
	D3DXVECTOR3 Scale = INITIALIZE_VECTOR3;
	pos = pTerritory->GetPos();
	rot = pTerritory->GetRot();
	Scale = pTerritory->GetScale();
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
void CTerritoryRender::AddColor(CTerritory *pTerritory, COLORDATA *pCol)
{
	pCol[0].col = pTerritory->GetColor();
}

//=============================================================================
//    描画処理
//=============================================================================
void CTerritoryRender::Draw(void)
{
	if (m_pTerritoryShader == NULL || m_bDisp == false) { return; }

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
	m_pTerritoryShader->SetMtxProjection(mtxProj);
	m_pTerritoryShader->SetMtxView(mtxView);

	// テクスチャの設定
	m_pTerritoryShader->BindTexture(m_pTexture);

	// ライトの設定
	D3DLIGHT9 Light;
	for (int nCntLight = 0; nCntLight < MAXLIGHT; nCntLight++)
	{
		pDevice->GetLight(nCntLight, &Light);
		m_pTerritoryShader->SetLightDir(Light.Direction, nCntLight);
		m_pTerritoryShader->SetLightDiffuse(Light.Diffuse, nCntLight);
	}

	// 法線を正規化する(大きさによって法線が変わるので)
	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);

	// シェーダーに値をセット
	m_pTerritoryShader->SetParamToEffect();

	// 描画開始
	m_pTerritoryShader->Begin();

	// レンダリングパス開始
	int nPassIdx = 0;
	if (m_pTexture == NULL) { nPassIdx = 1; }
	m_pTerritoryShader->BeginPass(nPassIdx);

	// ポリゴンの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_nNumVertex, 0, m_nNumPolygon);

	// レンダリングパス終了
	m_pTerritoryShader->EndPass();

	// 描画終了
	m_pTerritoryShader->End();

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