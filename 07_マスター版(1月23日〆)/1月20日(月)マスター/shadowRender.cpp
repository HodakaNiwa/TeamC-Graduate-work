//*****************************************************************************
//
//     影描画の処理[shadowRender.cpp]
//     Author:Hodaka Niwa
//
//*****************************************************************************
#include "shadowRender.h"
#include "shadow.h"
#include "manager.h"
#include "renderer.h"
#include "shader.h"
#include "camera.h"
#include "game.h"
#include "debuglog.h"

//*****************************************************************************
//     マクロ定義
//*****************************************************************************
#define SHADOWRENDER_SHADER_FILANAME "data/ringShader.hlsl"
#define SHADOWRENDER_PRIORITY (8)
#define SHADOWRENDER_SHADOW_PRIORITY (3)

//*****************************************************************************
//     静的メンバ変数宣言
//*****************************************************************************


//=============================================================================
//    コンストラクタ
//=============================================================================
CShadowRender::CShadowRender(int nPriority, OBJTYPE objType) : CScene(nPriority, objType)
{
	m_pVtxBuff = NULL;
	m_pWorldBuff = NULL;
	m_pColorBuff = NULL;
	m_pSizeBuff = NULL;
	m_pIdxBuff = NULL;
	m_pDecl = NULL;
	m_pShadowShader = NULL;
	m_pTexture = NULL;
	m_nNumInstance = 0;
	m_bDisp = false;
	m_bLighting = false;
}

//=============================================================================
//    デストラクタ
//=============================================================================
CShadowRender::~CShadowRender()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CShadowRender *CShadowRender::Create(void)
{
	CShadowRender *pShadowRender = NULL;
	if (pShadowRender != NULL) { return NULL; }

	// メモリを確保
	pShadowRender = new CShadowRender(SHADOWRENDER_SHADOW_PRIORITY + 1);
	if (pShadowRender == NULL) { return NULL; }

	// インスタンス数を設定
	int nNumInstance = 0;

	// 全てのリングを検索
	CScene *pScene = GetTop(SHADOWRENDER_SHADOW_PRIORITY);
	CScene *pSceneNext = NULL;
	while (pScene != NULL)
	{
		pSceneNext = pScene->GetpNext();
		if (pScene->GetObjType() == OBJTYPE_SHADOW)
		{// 影だったら
			nNumInstance++;
		}
		pScene = pSceneNext;
	}

	// 値の設定
	pShadowRender->SetNumInstance(nNumInstance);

	// 初期化処理
	if (FAILED(pShadowRender->Init()))
	{
		pShadowRender->Uninit();
		return NULL;
	}

	// テクスチャを設定する
	pShadowRender->BindTexture(CShadow::GetTexture());

	return pShadowRender;	// インスタンスのアドレスを返す
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CShadowRender::Init(void)
{
	// インスタンス数が無ければ処理しない
	if (m_nNumInstance <= 0) { return E_FAIL; }

	// 描画デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	if (pDevice == NULL) { return E_FAIL; }

	// 頂点バッファの生成
	if (FAILED(MakeVertexBuff(pDevice)))
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

	// サイズバッファの生成
	if (FAILED(MakeSizeBuff(pDevice)))
	{
		return E_FAIL;
	}

	// インデックスバッファの生成
	if (FAILED(MakeIdxBuff(pDevice)))
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
//    頂点バッファの生成処理
//=============================================================================
HRESULT CShadowRender::MakeVertexBuff(const LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEXDATA) * 4,
		D3DUSAGE_WRITEONLY,
		0,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);
	if (m_pVtxBuff == NULL) { return E_FAIL; }

	// 頂点バッファをロックし,頂点データへのポインタを取得
	VERTEXDATA *pVtx;
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標
	pVtx[0].pos = D3DXVECTOR3(-1.0f, 0.0f, 1.0f);
	pVtx[1].pos = D3DXVECTOR3(1.0f, 0.0f, 1.0f);
	pVtx[2].pos = D3DXVECTOR3(-1.0f, 0.0f, -1.0f);
	pVtx[3].pos = D3DXVECTOR3(1.0f, 0.0f, -1.0f);

	// 法線ベクトル
	pVtx[0].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);

	// テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点バッファをアンロックする
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	return S_OK;
}

//=============================================================================
//    ワールドマトリックスバッファの生成処理
//=============================================================================
HRESULT CShadowRender::MakeWorldBuff(const LPDIRECT3DDEVICE9 pDevice)
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
HRESULT CShadowRender::MakeColorBuff(const LPDIRECT3DDEVICE9 pDevice)
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
//    サイズバッファの生成処理
//=============================================================================
HRESULT CShadowRender::MakeSizeBuff(const LPDIRECT3DDEVICE9 pDevice)
{
	// サイズバッファを生成
	pDevice->CreateVertexBuffer(sizeof(SIZEDATA) * m_nNumInstance,
		D3DUSAGE_WRITEONLY,
		0,
		D3DPOOL_MANAGED,
		&m_pSizeBuff,
		NULL);
	if (m_pSizeBuff == NULL) { return E_FAIL; }

	return S_OK;
}

//=============================================================================
//    インデックスバッファの生成処理
//=============================================================================
HRESULT CShadowRender::MakeIdxBuff(const LPDIRECT3DDEVICE9 pDevice)
{
	if (m_pIdxBuff != NULL) { return E_FAIL; }

	// インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * 6,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIdxBuff,
		NULL);
	if (m_pIdxBuff == NULL) { return E_FAIL; }

	WORD *pIdx;  // インデックスデータへのポインタ

	// インデックスバッファをロックし,インデックスデータへのポインタを取得
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	// 左側
	pIdx[0] = 0;
	pIdx[1] = 1;
	pIdx[2] = 2;

	// 右側
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
HRESULT CShadowRender::MakeDeclaration(const LPDIRECT3DDEVICE9 pDevice)
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

		// Stream3(サイズ)
		{ 3, 0, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 5 },	// サイズ
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
HRESULT CShadowRender::LoadShader(void)
{
	if (m_pShadowShader != NULL) { return E_FAIL; }
	m_pShadowShader = CShadowShader::Create(SHADOWRENDER_SHADER_FILANAME);
	if (m_pShadowShader == NULL) { return E_FAIL; }

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CShadowRender::Uninit(void)
{
	// 頂点バッファの開放
	DIRECT_RELEASE(m_pVtxBuff);

	// ワールドマトリックスバッファの開放
	DIRECT_RELEASE(m_pWorldBuff);

	// 頂点カラーバッファの開放
	DIRECT_RELEASE(m_pColorBuff);

	// サイズバッファの開放
	DIRECT_RELEASE(m_pSizeBuff);

	// インデックスバッファの開放
	DIRECT_RELEASE(m_pIdxBuff);

	// 頂点宣言データの開放
	DIRECT_RELEASE(m_pDecl);

	// シェーダーの開放
	if (m_pShadowShader != NULL)
	{
		m_pShadowShader->Uninit();
		delete m_pShadowShader;
		m_pShadowShader = NULL;
	}

	// 死亡フラグを立てる
	Release();
}

//=============================================================================
//    更新処理
//=============================================================================
void CShadowRender::Update(void)
{
	// インスタンス数を初期化
	m_nNumInstance = 0;

	// ワールドマトリックスバッファの開放
	DIRECT_RELEASE(m_pWorldBuff);

	// 頂点カラーバッファの開放
	DIRECT_RELEASE(m_pColorBuff);

	// サイズバッファの開放
	DIRECT_RELEASE(m_pSizeBuff);

	// 全てのリングを検索
	CScene *pScene = GetTop(SHADOWRENDER_SHADOW_PRIORITY);
	CScene *pSceneNext = NULL;
	while (pScene != NULL)
	{
		pSceneNext = pScene->GetpNext();
		if (pScene->GetObjType() == OBJTYPE_SHADOW)
		{// 影だったら
			m_nNumInstance++;
		}
		pScene = pSceneNext;
	}

	// 描画デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	if (pDevice == NULL) { return; }

	// ワールドマトリックスバッファの生成
	if (FAILED(MakeWorldBuff(pDevice)))
	{
		return;
	}

	// 頂点カラーバッファの生成
	if (FAILED(MakeColorBuff(pDevice)))
	{
		return;
	}

	// サイズバッファの生成
	if (FAILED(MakeSizeBuff(pDevice)))
	{
		return;
	}

	// ワールドマトリックスバッファをロックし、ワールドマトリックスデータへのポインタを取得
	CShadowRender::WORLDMATRIX *pWorld = NULL;
	m_pWorldBuff->Lock(0, 0, (void**)&pWorld, 0);

	// 頂点カラーバッファをロックし、頂点カラーデータへのポインタを取得
	CShadowRender::COLORDATA *pColor = NULL;
	m_pColorBuff->Lock(0, 0, (void**)&pColor, 0);

	// サイズバッファをロックし、サイズデータへのポインタを取得
	CShadowRender::SIZEDATA *pSize = NULL;
	m_pSizeBuff->Lock(0, 0, (void**)&pSize, 0);

	// 全てのリングを探してリストに追加する
	pScene = GetTop(SHADOWRENDER_SHADOW_PRIORITY);
	pSceneNext = NULL;
	while (pScene != NULL)
	{
		pSceneNext = pScene->GetpNext();
		if (pScene->GetObjType() == OBJTYPE_SHADOW)
		{// 影だったら
			AddList((CShadow*)pScene, pWorld, pColor, pSize);
			pWorld++;
			pColor++;
			pSize++;
		}
		pScene = pSceneNext;
	}

	// ワールドマトリックスバッファをアンロックする
	m_pWorldBuff->Unlock();

	// 頂点カラーバッファをアンロックする
	m_pColorBuff->Unlock();

	// サイズバッファをアンロックする
	m_pSizeBuff->Unlock();
}

//=============================================================================
//    バッファに書き込む処理処理
//=============================================================================
void CShadowRender::AddList(CShadow *pShadow, WORLDMATRIX *pWorld, COLORDATA *pCol, SIZEDATA *pSize)
{
	// ワールドマトリックス情報を書き込む
	AddWorld(pShadow, pWorld);

	// 頂点カラー情報を書き込む
	AddColor(pShadow, pCol);

	// サイズ情報を書き込む
	AddSize(pShadow, pSize);
}

//=============================================================================
//    ワールドマトリックス情報を書き込む処理
//=============================================================================
void CShadowRender::AddWorld(CShadow *pShadow, WORLDMATRIX *pWorld)
{
	D3DXVECTOR3 pos = INITIALIZE_VECTOR3;
	D3DXVECTOR3 rot = INITIALIZE_VECTOR3;
	D3DXVECTOR3 Scale = pShadow->GetSize();
	Scale.y = 1.0f;
	pos = pShadow->GetPos();
	rot = pShadow->GetRot();
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
void CShadowRender::AddColor(CShadow *pShadow, COLORDATA *pCol)
{
	pCol[0].col = pShadow->GetCol();
}

//=============================================================================
//    サイズ情報を書き込む処理
//=============================================================================
void CShadowRender::AddSize(CShadow *pShadow, SIZEDATA *pSize)
{
	pSize[0].fWidth = pShadow->GetSize().x;
	pSize[0].fDepth = pShadow->GetSize().z;
}

//=============================================================================
//    描画処理
//=============================================================================
void CShadowRender::Draw(void)
{
	if (m_pShadowShader == NULL || m_bDisp == false) { return; }
	CGame *pGame = CManager::GetGame();				// ゲームの取得
	CEventCamera *pEveCam = pGame->GetEveCam();		// イベントカメラの取得
	if (pEveCam != NULL)
	{// イベント中は描画しない
		return;
	}

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	if (pDevice == NULL) { return; }

	// インスタンシング宣言
	pDevice->SetStreamSourceFreq(0, D3DSTREAMSOURCE_INDEXEDDATA | m_nNumInstance);
	pDevice->SetStreamSourceFreq(1, D3DSTREAMSOURCE_INSTANCEDATA | 1);
	pDevice->SetStreamSourceFreq(2, D3DSTREAMSOURCE_INSTANCEDATA | 1);
	pDevice->SetStreamSourceFreq(3, D3DSTREAMSOURCE_INSTANCEDATA | 1);

	// 頂点宣言データを設定
	pDevice->SetVertexDeclaration(m_pDecl);

	// 頂点バッファをストリームの0番に設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEXDATA));

	// ワールドマトリックスバッファをストリームの1番に設定
	pDevice->SetStreamSource(1, m_pWorldBuff, 0, sizeof(WORLDMATRIX));

	// 頂点カラーバッファをストリームの2番に設定
	pDevice->SetStreamSource(2, m_pColorBuff, 0, sizeof(COLORDATA));

	// サイズバッファをストリームの3番に設定
	pDevice->SetStreamSource(3, m_pSizeBuff, 0, sizeof(SIZEDATA));

	// インデックスバッファを設定
	pDevice->SetIndices(m_pIdxBuff);

	// プロジェクションマトリックスとビューマトリックスの設定
	D3DXMATRIX mtxView, mtxProj;
	pDevice->GetTransform(D3DTS_PROJECTION, &mtxProj);
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);
	m_pShadowShader->SetMtxProjection(mtxProj);
	m_pShadowShader->SetMtxView(mtxView);

	// テクスチャの設定
	m_pShadowShader->BindTexture(m_pTexture);

	// ライトの設定
	D3DLIGHT9 Light;
	for (int nCntLight = 0; nCntLight < MAXLIGHT; nCntLight++)
	{
		pDevice->GetLight(nCntLight, &Light);
		m_pShadowShader->SetLightDir(Light.Direction, nCntLight);
		m_pShadowShader->SetLightDiffuse(Light.Diffuse, nCntLight);
	}

	// シェーダーに値をセット
	m_pShadowShader->SetParamToEffect();

	// 描画開始
	m_pShadowShader->Begin();

	// レンダリングパス開始
	int nPassIdx = 0;
	if (m_pTexture == NULL) { nPassIdx = 1; }
	m_pShadowShader->BeginPass(nPassIdx);

	// αブレンディングを減算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// ポリゴンの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);

	// αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// レンダリングパス終了
	m_pShadowShader->EndPass();

	// 描画終了
	m_pShadowShader->End();

	// 後始末
	pDevice->SetVertexDeclaration(NULL);
	pDevice->SetStreamSourceFreq(0, 1);
	pDevice->SetStreamSourceFreq(1, 1);
	pDevice->SetStreamSourceFreq(2, 1);
	pDevice->SetStreamSourceFreq(3, 1);
	pDevice->SetStreamSource(1, NULL, 0, 0);
	pDevice->SetStreamSource(2, NULL, 0, 0);
	pDevice->SetStreamSource(3, NULL, 0, 0);
}