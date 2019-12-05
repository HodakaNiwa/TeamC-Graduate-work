//*****************************************************************************
//
//     インスタンシングを利用した観客の処理[audience.cpp]
//     Author:Hodaka Niwa
//
//*****************************************************************************
#include "audience.h"
#include "manager.h"
#include "library.h"
#include "shader.h"

//*****************************************************************************
//     マクロ定義
//*****************************************************************************
#define AUDIENCE_DRAW_UNDER												// 体の下の面を描画するかしないか
#define AUDIENCE_FILENAME         "data/TEXT/audienceData.txt"			// 観客のスクリプトファイル名
#define AUDIENCE_SHADER_FILENAME  "data/audience.hlsl"					// 観客のシェーダーファイル名
#define AUDIENCE_PARTS_FILENAME   "data/MODEL/audience_parts.x"			// 観客のパーツモデルファイル名
#define AUDIECNE_SCALE            (D3DXVECTOR3(0.4f,0.4f,0.4f))			// 観客の大きさ
#define AUDIECNE_COLOR_RIVISION   (1.0f)								// 色の倍率

#define AUDIENCE_BODY_OFFSET      (D3DXVECTOR3(0.0f,0.0f,0.0f))			// 体のオフセット
#define AUDIENCE_BODY_HEIGHT      (40.0f)								// 体の高さ
#define AUDIENCE_BODY_SIZE        (12.0f)								// 体のサイズ

#define AUDIENCE_HEAD_OFFSET      (D3DXVECTOR3(0.0f,50.0f,0.0f))		// 頭のオフセット
#define AUDIENCE_HEAD_SCALE       (D3DXVECTOR3(0.45f,0.45f,0.45f))		// 頭の大きさ
#define AUDIENCE_HEAD_COLOR       (D3DXCOLOR(0.98f, 0.6f, 0.3f, 1.0f))	// 頭の色

#define AUDIENCE_LEFTHAND_OFFSET  (D3DXVECTOR3(-15.0f,26.0f,0.0f))		// 左手のオフセット
#define AUDIENCE_LEFTHAND_SCALE   (D3DXVECTOR3(0.20f,0.20f,0.20f))		// 左手の大きさ
#define AUDIENCE_LEFTHAND_COLOR   (D3DXCOLOR(0.98f, 0.6f, 0.3f, 1.0f))	// 左手の色

#define AUDIENCE_RIGHTHAND_OFFSET (D3DXVECTOR3(15.0f,26.0f,0.0f))		// 右手のオフセット
#define AUDIENCE_RIGHTHAND_SCALE  (D3DXVECTOR3(0.20f,0.20f,0.20f))		// 右手の大きさ
#define AUDIENCE_RIGHTHAND_COLOR  (D3DXCOLOR(0.98f, 0.6f, 0.3f, 1.0f))	// 右手の色

// ファイル読み込み用のパス
#define AUDIENCESET "AUDIENCESET"
#define END_AUDIENCESET "END_AUDIENCESET"
#define POS "POS = "
#define ROT "ROT = "
#define SCALE "SCALE = "
#define COL "COL = "

//*****************************************************************************
//    静的メンバ変数宣言
//*****************************************************************************


//=============================================================================
//    コンストラクタ
//=============================================================================
CAudience::CAudience(int nPriority, OBJTYPE objType) : CScene(nPriority, objType)
{
	m_pWorldBuff = NULL;
	m_pDecl = NULL;
	m_pInsShader = NULL;
	m_nNumInstance = 0;
	for (int nCnt = 0; nCnt < PARTS_MAX; nCnt++)
	{
		m_pVtxBuff[nCnt] = NULL;
		m_pColorBuff[nCnt] = NULL;
		m_pIdxBuff[nCnt] = NULL;
		m_nNumVertex[nCnt] = 0;
		m_nNumIndex[nCnt] = 0;
		m_nNumPolygon[nCnt] = 0;
		D3DXMatrixIdentity(&m_MtxLocal[nCnt]);
	}
}

//=============================================================================
//    デストラクタ
//=============================================================================
CAudience::~CAudience()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CAudience *CAudience::Create(void)
{
	// メモリを確保
	CAudience *pAudience = NULL;
	pAudience = new CAudience;
	if (pAudience == NULL) { return NULL; }

	// 初期化処理
	if (FAILED(pAudience->Init()))
	{
		pAudience->Uninit();
		return NULL;
	}

	return pAudience;
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CAudience::Init(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	if (pDevice == NULL) { return E_FAIL; }

	// 頂点数,インデックス数,ポリゴン数の設定
	// 体
#ifdef AUDIENCE_DRAW_UNDER
	m_nNumVertex[PARTS_BODY] = 9;
	m_nNumIndex[PARTS_BODY] = 30;
	m_nNumPolygon[PARTS_BODY] = 10;
#else
	m_nNumVertex[PARTS_BODY] = 9;
	m_nNumIndex[PARTS_BODY] = 21;
	m_nNumPolygon[PARTS_BODY] = 7;
#endif

	// インスタンシング数を読み込む
	char aStr[256];
	LoadInstanceDataNum(aStr);
	if (m_nNumInstance == 0)
	{
		return E_FAIL;
	}

	// インスタンシング数によってメモリを確保
	D3DXVECTOR3 *pPos;
	pPos = new D3DXVECTOR3[m_nNumInstance];

	D3DXVECTOR3 *pRot;
	pRot = new D3DXVECTOR3[m_nNumInstance];

	D3DXVECTOR3 *pScale;
	pScale = new D3DXVECTOR3[m_nNumInstance];

	D3DXCOLOR *pCol;
	pCol = new D3DXCOLOR[m_nNumInstance];
	for (int nCnt = 0; nCnt < m_nNumInstance; nCnt++)
	{
		pPos[nCnt] = INITIALIZE_VECTOR3;
		pScale[nCnt] = INITIALIZE_VECTOR3;
		pRot[nCnt] = INITIALIZE_VECTOR3;
		pCol[nCnt] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}

	// 値の読み込み
	LoadInstanceData(aStr, pPos, pRot, pScale, pCol);

	// ワールドマトリックスバッファの作成
	MakeWorldBuff(pDevice, pPos, pRot, pScale);

	// 頂点宣言データの作成
	MakeDeclaration(pDevice);

	// シェーダーファイルを読み込む処理
	LoadShader();

	// 体を作成する処理
	MakeBody(pDevice, pCol);

	// 頭を作成する処理
	MakeHead(pDevice);

	// 左手を作成する処理
	MakeLeftHand(pDevice);

	// 右手を作成する処理
	MakeRightHand(pDevice);

	// メモリの開放
	if (pPos != NULL)
	{
		delete[] pPos;
		pPos = NULL;
	}
	if (pRot != NULL)
	{
		delete[] pRot;
		pRot = NULL;
	}
	if (pScale != NULL)
	{
		delete[] pScale;
		pScale = NULL;
	}
	if (pCol != NULL)
	{
		delete[] pCol;
		pCol = NULL;
	}

	return S_OK;
}


//=============================================================================
//    インスタンシング数をファイルから読み込む処理
//=============================================================================
void CAudience::LoadInstanceDataNum(char *pStr)
{
	// ファイルオープン
	CFileLoader *pFileLoader = CFileLoader::Create(AUDIENCE_FILENAME);
	if (pFileLoader == NULL) { return; }

	// スクリプト読み込み開始
	pFileLoader->GetString(pStr);
	if (CFunctionLib::Memcmp(pStr, SCRIPT) == 0)
	{
		while (1)
		{
			pFileLoader->GetString(pStr);
			if (CFunctionLib::Memcmp(pStr, AUDIENCESET) == 0)
			{
				m_nNumInstance++;
			}
			else if (CFunctionLib::Memcmp(pStr, END_SCRIPT) == 0)
			{
				break;
			}
		}
	}


	// メモリの開放
	if (pFileLoader != NULL)
	{
		pFileLoader->Uninit();
		delete pFileLoader;
		pFileLoader = NULL;
	}
}

//=============================================================================
//    インスタンシングデータをファイルから読み込む処理
//=============================================================================
void CAudience::LoadInstanceData(char *pStr, D3DXVECTOR3 *pPos, D3DXVECTOR3 *pRot, D3DXVECTOR3 *pScale, D3DXCOLOR *pCol)
{
	// ファイルオープン
	CFileLoader *pFileLoader = CFileLoader::Create(AUDIENCE_FILENAME);
	if (pFileLoader == NULL) { return; }

	// スクリプト読み込み開始
	int nCntData = 0;
	pFileLoader->GetString(pStr);
	if (CFunctionLib::Memcmp(pStr, SCRIPT) == 0)
	{
		while (1)
		{
			pFileLoader->GetString(pStr);
			if (CFunctionLib::Memcmp(pStr, AUDIENCESET) == 0)
			{
				pRot[nCntData] = INITIALIZE_VECTOR3;
				pScale[nCntData] = AUDIECNE_SCALE;
				while (1)
				{
					pFileLoader->GetString(pStr);
					if (CFunctionLib::Memcmp(pStr, POS) == 0)
					{
						pPos[nCntData] = CFunctionLib::ReadVector3(pStr, POS);
					}
					else if (CFunctionLib::Memcmp(pStr, ROT) == 0)
					{
						pRot[nCntData] = CFunctionLib::ReadVector3(pStr, ROT);
					}
					else if (CFunctionLib::Memcmp(pStr, SCALE) == 0)
					{
						pScale[nCntData] = CFunctionLib::ReadVector3(pStr, SCALE);
					}
					else if (CFunctionLib::Memcmp(pStr, COL) == 0)
					{
						pCol[nCntData] = CFunctionLib::ReadVector4(pStr, COL);
					}
					else if (CFunctionLib::Memcmp(pStr, END_AUDIENCESET) == 0)
					{
						nCntData++;
						break;
					}
				}
			}
			else if (CFunctionLib::Memcmp(pStr, END_SCRIPT) == 0)
			{
				break;
			}
		}
	}

	// メモリの開放
	if (pFileLoader != NULL)
	{
		pFileLoader->Uninit();
		delete pFileLoader;
		pFileLoader = NULL;
	}
}

//=============================================================================
//    ワールドマトリックスバッファを作成する処理
//=============================================================================
void CAudience::MakeWorldBuff(const LPDIRECT3DDEVICE9 pDevice, D3DXVECTOR3 *pPos, D3DXVECTOR3 *pRot, D3DXVECTOR3 *pScale)
{
	// ワールドマトリックスバッファを生成
	pDevice->CreateVertexBuffer(sizeof(WORLDMATRIX) * m_nNumInstance,
		D3DUSAGE_WRITEONLY,
		0,
		D3DPOOL_MANAGED,
		&m_pWorldBuff,
		NULL);

	// ワールドマトリックスバッファをロックし,データへのポインタを取得
	WORLDMATRIX *pWorld;
	m_pWorldBuff->Lock(0, 0, (void**)&pWorld, 0);

	// ワールドマトリックスの設定
	float fSinPitch;
	float fCosPitch;
	float fSinYaw;
	float fCosYaw;
	float fSinRoll;
	float fCosRoll;
	D3DXMATRIX mtxWorld;
	D3DXMatrixIdentity(&mtxWorld);
	for (int nCnt = 0; nCnt < m_nNumInstance; nCnt++, pPos++, pRot++, pScale++)
	{
		// 回転行列を作成
		fSinPitch = sinf(pRot->x);
		fCosPitch = cosf(pRot->x);
		fSinYaw = sinf(pRot->y);
		fCosYaw = cosf(pRot->y);
		fSinRoll = sinf(pRot->z);
		fCosRoll = cosf(pRot->z);
		mtxWorld._11 = fSinRoll * fSinPitch * fSinYaw + fCosRoll * fCosYaw;
		mtxWorld._12 = fSinRoll * fCosPitch;
		mtxWorld._13 = fSinRoll * fSinPitch * fCosYaw - fCosRoll * fSinYaw;
		mtxWorld._21 = fCosRoll * fSinPitch * fSinYaw - fSinRoll * fCosYaw;
		mtxWorld._22 = fCosRoll * fCosPitch;
		mtxWorld._23 = fCosRoll * fSinPitch * fCosYaw + fSinRoll * fSinYaw;
		mtxWorld._31 = fCosPitch * fSinYaw;
		mtxWorld._32 = -fSinPitch;
		mtxWorld._33 = fCosPitch * fCosYaw;

		// 行列を設定
		pWorld[nCnt].m1[0] = mtxWorld._11 * pScale->x;
		pWorld[nCnt].m1[1] = mtxWorld._12 * pScale->x;
		pWorld[nCnt].m1[2] = mtxWorld._13 * pScale->x;
		pWorld[nCnt].m1[3] = mtxWorld._14;
		pWorld[nCnt].m2[0] = mtxWorld._21 * pScale->y;
		pWorld[nCnt].m2[1] = mtxWorld._22 * pScale->y;
		pWorld[nCnt].m2[2] = mtxWorld._23 * pScale->y;
		pWorld[nCnt].m2[3] = mtxWorld._24;
		pWorld[nCnt].m3[0] = mtxWorld._31 * pScale->z;
		pWorld[nCnt].m3[1] = mtxWorld._32 * pScale->z;
		pWorld[nCnt].m3[2] = mtxWorld._33 * pScale->z;
		pWorld[nCnt].m3[3] = mtxWorld._34;
		pWorld[nCnt].m4[0] = pPos->x;
		pWorld[nCnt].m4[1] = pPos->y;
		pWorld[nCnt].m4[2] = pPos->z;
		pWorld[nCnt].m4[3] = mtxWorld._44;
	}

	// ワールドマトリックスバッファをアンロックする
	m_pWorldBuff->Unlock();
}

//=============================================================================
//    頂点宣言データを作成する処理
//=============================================================================
void CAudience::MakeDeclaration(const LPDIRECT3DDEVICE9 pDevice)
{
	// 必要なデータを羅列
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

		// Stream2(体の色)
		{ 2,  0, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },	// 頂点カラー
		D3DDECL_END()
	};

	// 頂点宣言の作成
	pDevice->CreateVertexDeclaration(declElems, &m_pDecl);
}

//=============================================================================
//    シェーダーを読み込む処理
//=============================================================================
void CAudience::LoadShader(void)
{
	if (m_pInsShader != NULL) { return; }
	m_pInsShader = CInstancingShader::Create(AUDIENCE_SHADER_FILENAME);
}

//=============================================================================
//    終了処理
//=============================================================================
void CAudience::Uninit(void)
{
	for (int nCnt = 0; nCnt < PARTS_MAX; nCnt++)
	{
		// 頂点バッファの開放
		DIRECT_RELEASE(m_pVtxBuff[nCnt]);

		// 頂点カラーバッファを開放
		DIRECT_RELEASE(m_pColorBuff[nCnt]);

		// インデックスバッファを開放
		DIRECT_RELEASE(m_pIdxBuff[nCnt]);
	}

	// ワールドマトリックスバッファを開放
	DIRECT_RELEASE(m_pWorldBuff);

	// 頂点宣言データを開放
	DIRECT_RELEASE(m_pDecl);

	// シェーダーの開放
	if (m_pInsShader != NULL)
	{
		m_pInsShader->Uninit();
		delete m_pInsShader;
		m_pInsShader = NULL;
	}

	// 死亡フラグを立てる
	Release();
}

//=============================================================================
//    更新処理
//=============================================================================
void CAudience::Update(void)
{

}

//=============================================================================
//    描画処理
//=============================================================================
void CAudience::Draw(void)
{
	if (m_pInsShader == NULL) { return; }

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	if (pDevice == NULL) { return; }

	// インスタンシング宣言
	pDevice->SetStreamSourceFreq(0, D3DSTREAMSOURCE_INDEXEDDATA | m_nNumInstance);
	pDevice->SetStreamSourceFreq(1, D3DSTREAMSOURCE_INSTANCEDATA | 1);
	pDevice->SetStreamSourceFreq(2, D3DSTREAMSOURCE_INSTANCEDATA | 1);

	// 頂点宣言データを設定
	pDevice->SetVertexDeclaration(m_pDecl);

	// ワールドマトリックスバッファをストリームの1番に設定
	pDevice->SetStreamSource(1, m_pWorldBuff, 0, sizeof(WORLDMATRIX));

	// プロジェクションマトリックスとビューマトリックスの設定
	D3DXMATRIX mtxView, mtxProj;
	pDevice->GetTransform(D3DTS_PROJECTION, &mtxProj);
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);
	m_pInsShader->SetMtxProjection(mtxProj);
	m_pInsShader->SetMtxView(mtxView);

	// テクスチャの設定
	m_pInsShader->BindTexture(NULL);

	// ライトの設定
	D3DLIGHT9 Light;
	pDevice->GetLight(0, &Light);
	m_pInsShader->SetLightDir(Light.Direction);
	m_pInsShader->SetLightDiffuse(Light.Diffuse);
	m_pInsShader->SetLightAmbient(Light.Ambient);

	// シェーダーに値をセット
	m_pInsShader->SetParamToEffect();

	// カリングを外す
	DWORD CullMode;
	pDevice->GetRenderState(D3DRS_CULLMODE, &CullMode);
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	// 描画開始
	m_pInsShader->Begin();

	// レンダリングパスの番号を設定
	int nPassIdx[PARTS_MAX] = {1,2,2,2};

	for (int nCnt = 0; nCnt < PARTS_MAX; nCnt++)
	{
		// ローカル座標におけるワールドマトリックスを設定
		m_pInsShader->SetMtxLocal(m_MtxLocal[nCnt]);

		// 頂点バッファをストリームの0番に設定
		pDevice->SetStreamSource(0, m_pVtxBuff[nCnt], 0, sizeof(VERTEXDATA));

		// 頂点カラーバッファをストリームの2番に設定
		pDevice->SetStreamSource(2, m_pColorBuff[nCnt], 0, sizeof(COLORDATA));

		// インデックスバッファを設定
		pDevice->SetIndices(m_pIdxBuff[nCnt]);

		// シェーダーに値をセット
		m_pInsShader->SetParamToEffect();

		// レンダリングパス開始
		m_pInsShader->BeginPass(nPassIdx[nCnt]);

		// ポリゴンの描画
		pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_nNumIndex[nCnt], 0, m_nNumPolygon[nCnt]);

		// レンダリングパス終了
		m_pInsShader->EndPass();
	}

	// 描画終了
	m_pInsShader->End();

	// 後始末
	pDevice->SetVertexDeclaration(NULL);
	pDevice->SetStreamSourceFreq(0, 1);
	pDevice->SetStreamSourceFreq(1, 1);
	pDevice->SetStreamSourceFreq(2, 1);
	pDevice->SetRenderState(D3DRS_CULLMODE, CullMode);
	pDevice->SetStreamSource(1, NULL, 0, 0);
	pDevice->SetStreamSource(2, NULL, 0, 0);
}


//*****************************************************************************
//     体関連
//*****************************************************************************
//=============================================================================
//    体を作成する処理
//=============================================================================
void CAudience::MakeBody(const LPDIRECT3DDEVICE9 pDevice, D3DXCOLOR *pCol)
{
	// 頂点バッファの作成
	MakeVertexBody(pDevice);

	// 頂点カラーバッファの作成
	MakeColorBuffBody(pDevice, pCol);

	// インデックスバッファの作成
	MakeIndexBody(pDevice);

	// ローカル座標におけるワールドマトリックスを作成する
	MakeMtxLocalBody();
}

//=============================================================================
//    頂点バッファを作成する処理
//=============================================================================
void CAudience::MakeVertexBody(const LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEXDATA) * 9,
		D3DUSAGE_WRITEONLY,
		(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1),
		D3DPOOL_MANAGED,
		&m_pVtxBuff[PARTS_BODY],
		NULL);

	// 頂点情報の設定
	VERTEXDATA *pVtx;

	// 頂点バッファをロックし,頂点データへのポインタを取得
	m_pVtxBuff[PARTS_BODY]->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標
	pVtx[0].pos = D3DXVECTOR3(0.0f, AUDIENCE_BODY_HEIGHT, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(-AUDIENCE_BODY_SIZE, 0.0f, AUDIENCE_BODY_SIZE);
	pVtx[2].pos = D3DXVECTOR3(AUDIENCE_BODY_SIZE, 0.0f, AUDIENCE_BODY_SIZE);
	pVtx[3].pos = D3DXVECTOR3(-AUDIENCE_BODY_SIZE, 0.0f, -AUDIENCE_BODY_SIZE);
	pVtx[4].pos = D3DXVECTOR3(AUDIENCE_BODY_SIZE, 0.0f, -AUDIENCE_BODY_SIZE);
	pVtx[5].pos = D3DXVECTOR3(-AUDIENCE_BODY_SIZE, 0.0f, AUDIENCE_BODY_SIZE);
	pVtx[6].pos = D3DXVECTOR3(AUDIENCE_BODY_SIZE, 0.0f, AUDIENCE_BODY_SIZE);
	pVtx[7].pos = D3DXVECTOR3(-AUDIENCE_BODY_SIZE, 0.0f, -AUDIENCE_BODY_SIZE);
	pVtx[8].pos = D3DXVECTOR3(AUDIENCE_BODY_SIZE, 0.0f, -AUDIENCE_BODY_SIZE);

	// 法線計算用ベクトル
	D3DXVECTOR3 vecVer[12];

	// 外積用にベクトルを作る
	// 奥の面
	vecVer[0] = pVtx[1].pos - pVtx[0].pos;
	vecVer[1] = pVtx[2].pos - pVtx[0].pos;

	// 右の面
	vecVer[2] = pVtx[2].pos - pVtx[0].pos;
	vecVer[3] = pVtx[4].pos - pVtx[0].pos;

	// 手前の面
	vecVer[4] = pVtx[4].pos - pVtx[0].pos;
	vecVer[5] = pVtx[3].pos - pVtx[0].pos;

	// 左の面
	vecVer[6] = pVtx[3].pos - pVtx[0].pos;
	vecVer[7] = pVtx[1].pos - pVtx[0].pos;

	// 下の面(左側)
	vecVer[8] = pVtx[6].pos - pVtx[5].pos;
	vecVer[9] = pVtx[7].pos - pVtx[5].pos;

	// 下の面(右側)
	vecVer[10] = pVtx[7].pos - pVtx[8].pos;
	vecVer[11] = pVtx[6].pos - pVtx[8].pos;

	// 頂点の法線
	D3DXVECTOR3 nor1, nor2, nor3, nor4, nor5, nor6;
	D3DXVec3Cross(&nor1, &vecVer[1], &vecVer[0]);
	D3DXVec3Cross(&nor2, &vecVer[3], &vecVer[2]);
	D3DXVec3Cross(&nor3, &vecVer[5], &vecVer[4]);
	D3DXVec3Cross(&nor4, &vecVer[7], &vecVer[6]);
	D3DXVec3Cross(&nor5, &vecVer[8], &vecVer[9]);
	D3DXVec3Cross(&nor6, &vecVer[10], &vecVer[11]);
	D3DXVec3Normalize(&nor1, &nor1);
	D3DXVec3Normalize(&nor2, &nor2);
	D3DXVec3Normalize(&nor3, &nor3);
	D3DXVec3Normalize(&nor4, &nor4);
	D3DXVec3Normalize(&nor5, &nor5);
	D3DXVec3Normalize(&nor6, &nor6);

	// 法線ベクトル
	pVtx[0].nor = (nor1 + nor2 + nor3 + nor4) / 4;
	pVtx[1].nor = (nor1 + nor4) / 2;
	pVtx[2].nor = (nor1 + nor2) / 2;
	pVtx[3].nor = (nor3 + nor4) / 2;
	pVtx[4].nor = (nor2 + nor3) / 2;
	pVtx[5].nor = nor5;
	pVtx[6].nor = (nor5 + nor6) / 2;
	pVtx[7].nor = (nor5 + nor6) / 2;
	pVtx[8].nor = nor6;

	// テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[3].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[4].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[5].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[6].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[7].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[8].tex = D3DXVECTOR2(0.0f, 0.0f);

	// 頂点バッファをアンロックする
	m_pVtxBuff[PARTS_BODY]->Unlock();
}

//=============================================================================
//    頂点カラーバッファを作成する処理
//=============================================================================
void CAudience::MakeColorBuffBody(const LPDIRECT3DDEVICE9 pDevice, D3DXCOLOR *pCol)
{
	// 頂点カラーバッファを生成
	pDevice->CreateVertexBuffer(sizeof(COLORDATA) * m_nNumInstance,
		D3DUSAGE_WRITEONLY,
		(D3DFVF_DIFFUSE),
		D3DPOOL_MANAGED,
		&m_pColorBuff[PARTS_BODY],
		NULL);

	// 頂点カラー情報の設定
	COLORDATA *pColor;

	// 頂点カラーバッファをロックし,頂点カラーデータへのポインタを取得
	m_pColorBuff[PARTS_BODY]->Lock(0, 0, (void**)&pColor, 0);

	// 頂点カラーの設定
	for (int nCnt = 0; nCnt < m_nNumInstance; nCnt++, pCol++)
	{
		pColor[nCnt].col = D3DXCOLOR(pCol->r, pCol->g, pCol->b, pCol->a);
	}

	// 頂点カラーバッファをアンロックする
	m_pColorBuff[PARTS_BODY]->Unlock();
}

//=============================================================================
//    インデックスバッファを作成する処理
//=============================================================================
void CAudience::MakeIndexBody(const LPDIRECT3DDEVICE9 pDevice)
{
	// インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * m_nNumIndex[PARTS_BODY],
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIdxBuff[PARTS_BODY],
		NULL);

	WORD *pIdx;       // インデックスデータへのポインタ

	// インデックスバッファをロックし,インデックスデータへのポインタを取得
	m_pIdxBuff[PARTS_BODY]->Lock(0, 0, (void**)&pIdx, 0);

	// 奥の面
	pIdx[0] = 0;
	pIdx[1] = 1;
	pIdx[2] = 2;

	// 縮退
	pIdx[3] = 2;
	pIdx[4] = 2;
	pIdx[5] = 0;

	// 右の面
	pIdx[6] = 0;
	pIdx[7] = 2;
	pIdx[8] = 4;

	// 縮退
	pIdx[9] = 4;
	pIdx[10] = 4;
	pIdx[11] = 0;

	// 手前の面
	pIdx[12] = 0;
	pIdx[13] = 4;
	pIdx[14] = 3;

	// 縮退
	pIdx[15] = 3;
	pIdx[16] = 3;
	pIdx[17] = 0;

	// 左の面
	pIdx[18] = 0;
	pIdx[19] = 3;
	pIdx[20] = 1;

#ifdef AUDIENCE_DRAW_UNDER
	// 縮退
	pIdx[21] = 5;
	pIdx[22] = 5;
	pIdx[23] = 5;

	// 下の面(左側)
	pIdx[24] = 5;
	pIdx[25] = 6;
	pIdx[26] = 7;

	// 下の面(右側)
	pIdx[27] = 7;
	pIdx[28] = 6;
	pIdx[29] = 8;
#endif

	// インデックスバッファをアンロックする
	m_pIdxBuff[PARTS_BODY]->Unlock();
}

//=============================================================================
//    ローカル座標におけるワールドマトリックスを作成する処理
//=============================================================================
void CAudience::MakeMtxLocalBody(void)
{
	// オフセット位置を反映
	m_MtxLocal[PARTS_BODY]._41 = AUDIENCE_BODY_OFFSET.x;
	m_MtxLocal[PARTS_BODY]._42 = AUDIENCE_BODY_OFFSET.y;
	m_MtxLocal[PARTS_BODY]._43 = AUDIENCE_BODY_OFFSET.z;
}


//*****************************************************************************
//     頭関連
//*****************************************************************************
//=============================================================================
//    頭を作成する処理
//=============================================================================
void CAudience::MakeHead(const LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点バッファの作成
	MakeVertexHead(pDevice);

	// 頂点カラーバッファの作成
	MakeColorBuffHead(pDevice);

	// ローカル座標におけるワールドマトリックスを作成する
	MakeMtxLocalHead();
}

//=============================================================================
//    頂点バッファを作成する処理
//=============================================================================
void CAudience::MakeVertexHead(const LPDIRECT3DDEVICE9 pDevice)
{
	// 球モデルを読み込み
	LPD3DXMESH pMesh = NULL;
	LPD3DXBUFFER pBuffMat = NULL;
	DWORD nNumMat;
	D3DXLoadMeshFromX(AUDIENCE_PARTS_FILENAME, D3DXMESH_SYSTEMMEM, pDevice, NULL, &pBuffMat, NULL, &nNumMat, &pMesh);
	if (pMesh == NULL || pBuffMat == NULL) { return; }

	// 頂点数とポリゴン数を獲得
	m_nNumVertex[PARTS_HEAD] = pMesh->GetNumVertices();
	m_nNumPolygon[PARTS_HEAD] = pMesh->GetNumFaces();

	// インデックスバッファを取得
	pMesh->GetIndexBuffer(&m_pIdxBuff[PARTS_HEAD]);

	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEXDATA) * m_nNumVertex[PARTS_HEAD],
		D3DUSAGE_WRITEONLY,
		(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1),
		D3DPOOL_MANAGED,
		&m_pVtxBuff[PARTS_HEAD],
		NULL);

	// 頂点情報の設定
	VERTEXDATA *pVtx;

	// 頂点バッファをロックし,頂点データへのポインタを取得
	m_pVtxBuff[PARTS_HEAD]->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点バッファをロック
	BYTE *pVtxBuff;  // 頂点バッファへのポインタ
	pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < m_nNumVertex[PARTS_HEAD]; nCntVtx++)
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
	m_pVtxBuff[PARTS_HEAD]->Unlock();

	// データを開放
	pMesh->Release();
	pBuffMat->Release();
}

//=============================================================================
//    頂点カラーバッファを作成する処理
//=============================================================================
void CAudience::MakeColorBuffHead(const LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点カラーバッファを生成
	pDevice->CreateVertexBuffer(sizeof(COLORDATA) * m_nNumInstance,
		D3DUSAGE_WRITEONLY,
		(D3DFVF_DIFFUSE),
		D3DPOOL_MANAGED,
		&m_pColorBuff[PARTS_HEAD],
		NULL);

	// 頂点カラー情報の設定
	COLORDATA *pColor;

	// 頂点カラーバッファをロックし,頂点カラーデータへのポインタを取得
	m_pColorBuff[PARTS_HEAD]->Lock(0, 0, (void**)&pColor, 0);

	// 頂点カラーの設定
	for (int nCnt = 0; nCnt < m_nNumInstance; nCnt++)
	{
		pColor[nCnt].col = AUDIENCE_HEAD_COLOR / AUDIECNE_COLOR_RIVISION;
	}

	// 頂点カラーバッファをアンロックする
	m_pColorBuff[PARTS_HEAD]->Unlock();
}

//=============================================================================
//    ローカル座標におけるワールドマトリックスを作成する処理
//=============================================================================
void CAudience::MakeMtxLocalHead(void)
{
	// 大きさを反映
	m_MtxLocal[PARTS_HEAD]._11 = m_MtxLocal[PARTS_HEAD]._11 * AUDIENCE_HEAD_SCALE.x;
	m_MtxLocal[PARTS_HEAD]._12 = m_MtxLocal[PARTS_HEAD]._12 * AUDIENCE_HEAD_SCALE.x;
	m_MtxLocal[PARTS_HEAD]._13 = m_MtxLocal[PARTS_HEAD]._13 * AUDIENCE_HEAD_SCALE.x;
	m_MtxLocal[PARTS_HEAD]._21 = m_MtxLocal[PARTS_HEAD]._21 * AUDIENCE_HEAD_SCALE.y;
	m_MtxLocal[PARTS_HEAD]._22 = m_MtxLocal[PARTS_HEAD]._22 * AUDIENCE_HEAD_SCALE.y;
	m_MtxLocal[PARTS_HEAD]._23 = m_MtxLocal[PARTS_HEAD]._23 * AUDIENCE_HEAD_SCALE.y;
	m_MtxLocal[PARTS_HEAD]._31 = m_MtxLocal[PARTS_HEAD]._31 * AUDIENCE_HEAD_SCALE.z;
	m_MtxLocal[PARTS_HEAD]._32 = m_MtxLocal[PARTS_HEAD]._32 * AUDIENCE_HEAD_SCALE.z;
	m_MtxLocal[PARTS_HEAD]._33 = m_MtxLocal[PARTS_HEAD]._33 * AUDIENCE_HEAD_SCALE.z;

	// オフセット位置を反映
	m_MtxLocal[PARTS_HEAD]._41 = AUDIENCE_HEAD_OFFSET.x;
	m_MtxLocal[PARTS_HEAD]._42 = AUDIENCE_HEAD_OFFSET.y;
	m_MtxLocal[PARTS_HEAD]._43 = AUDIENCE_HEAD_OFFSET.z;
}


//*****************************************************************************
//     頭関連
//*****************************************************************************
//=============================================================================
//    頭を作成する処理
//=============================================================================
void CAudience::MakeLeftHand(const LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点バッファの作成
	MakeVertexLeftHand(pDevice);

	// 頂点カラーバッファの作成
	MakeColorBuffLeftHand(pDevice);

	// ローカル座標におけるワールドマトリックスを作成する
	MakeMtxLocalLeftHand();
}

//=============================================================================
//    頂点バッファを作成する処理
//=============================================================================
void CAudience::MakeVertexLeftHand(const LPDIRECT3DDEVICE9 pDevice)
{
	// 球モデルを読み込み
	LPD3DXMESH pMesh = NULL;
	LPD3DXBUFFER pBuffMat = NULL;
	DWORD nNumMat;
	D3DXLoadMeshFromX(AUDIENCE_PARTS_FILENAME, D3DXMESH_SYSTEMMEM, pDevice, NULL, &pBuffMat, NULL, &nNumMat, &pMesh);
	if (pMesh == NULL || pBuffMat == NULL) { return; }

	// 頂点数とポリゴン数を獲得
	m_nNumVertex[PARTS_LEFT_HAND] = pMesh->GetNumVertices();
	m_nNumPolygon[PARTS_LEFT_HAND] = pMesh->GetNumFaces();

	// インデックスバッファを取得
	pMesh->GetIndexBuffer(&m_pIdxBuff[PARTS_LEFT_HAND]);

	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEXDATA) * m_nNumVertex[PARTS_LEFT_HAND],
		D3DUSAGE_WRITEONLY,
		(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1),
		D3DPOOL_MANAGED,
		&m_pVtxBuff[PARTS_LEFT_HAND],
		NULL);

	// 頂点情報の設定
	VERTEXDATA *pVtx;

	// 頂点バッファをロックし,頂点データへのポインタを取得
	m_pVtxBuff[PARTS_LEFT_HAND]->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点バッファをロック
	BYTE *pVtxBuff;  // 頂点バッファへのポインタ
	pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < m_nNumVertex[PARTS_LEFT_HAND]; nCntVtx++)
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
	m_pVtxBuff[PARTS_LEFT_HAND]->Unlock();

	// データを開放
	pMesh->Release();
	pBuffMat->Release();
}

//=============================================================================
//    頂点カラーバッファを作成する処理
//=============================================================================
void CAudience::MakeColorBuffLeftHand(const LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点カラーバッファを生成
	pDevice->CreateVertexBuffer(sizeof(COLORDATA) * m_nNumInstance,
		D3DUSAGE_WRITEONLY,
		(D3DFVF_DIFFUSE),
		D3DPOOL_MANAGED,
		&m_pColorBuff[PARTS_LEFT_HAND],
		NULL);

	// 頂点カラー情報の設定
	COLORDATA *pColor;

	// 頂点カラーバッファをロックし,頂点カラーデータへのポインタを取得
	m_pColorBuff[PARTS_LEFT_HAND]->Lock(0, 0, (void**)&pColor, 0);

	// 頂点カラーの設定
	for (int nCnt = 0; nCnt < m_nNumInstance; nCnt++)
	{
		pColor[nCnt].col = AUDIENCE_LEFTHAND_COLOR / AUDIECNE_COLOR_RIVISION;
	}

	// 頂点カラーバッファをアンロックする
	m_pColorBuff[PARTS_LEFT_HAND]->Unlock();
}

//=============================================================================
//    ローカル座標におけるワールドマトリックスを作成する処理
//=============================================================================
void CAudience::MakeMtxLocalLeftHand(void)
{
	// 大きさを反映
	m_MtxLocal[PARTS_LEFT_HAND]._11 = m_MtxLocal[PARTS_LEFT_HAND]._11 * AUDIENCE_LEFTHAND_SCALE.x;
	m_MtxLocal[PARTS_LEFT_HAND]._12 = m_MtxLocal[PARTS_LEFT_HAND]._12 * AUDIENCE_LEFTHAND_SCALE.x;
	m_MtxLocal[PARTS_LEFT_HAND]._13 = m_MtxLocal[PARTS_LEFT_HAND]._13 * AUDIENCE_LEFTHAND_SCALE.x;
	m_MtxLocal[PARTS_LEFT_HAND]._21 = m_MtxLocal[PARTS_LEFT_HAND]._21 * AUDIENCE_LEFTHAND_SCALE.y;
	m_MtxLocal[PARTS_LEFT_HAND]._22 = m_MtxLocal[PARTS_LEFT_HAND]._22 * AUDIENCE_LEFTHAND_SCALE.y;
	m_MtxLocal[PARTS_LEFT_HAND]._23 = m_MtxLocal[PARTS_LEFT_HAND]._23 * AUDIENCE_LEFTHAND_SCALE.y;
	m_MtxLocal[PARTS_LEFT_HAND]._31 = m_MtxLocal[PARTS_LEFT_HAND]._31 * AUDIENCE_LEFTHAND_SCALE.z;
	m_MtxLocal[PARTS_LEFT_HAND]._32 = m_MtxLocal[PARTS_LEFT_HAND]._32 * AUDIENCE_LEFTHAND_SCALE.z;
	m_MtxLocal[PARTS_LEFT_HAND]._33 = m_MtxLocal[PARTS_LEFT_HAND]._33 * AUDIENCE_LEFTHAND_SCALE.z;

	// オフセット位置を反映
	m_MtxLocal[PARTS_LEFT_HAND]._41 = AUDIENCE_LEFTHAND_OFFSET.x;
	m_MtxLocal[PARTS_LEFT_HAND]._42 = AUDIENCE_LEFTHAND_OFFSET.y;
	m_MtxLocal[PARTS_LEFT_HAND]._43 = AUDIENCE_LEFTHAND_OFFSET.z;
}


//*****************************************************************************
//     右手関連
//*****************************************************************************
//=============================================================================
//    右手を作成する処理
//=============================================================================
void CAudience::MakeRightHand(const LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点バッファの作成
	MakeVertexRightHand(pDevice);

	// 頂点カラーバッファの作成
	MakeColorBuffRightHand(pDevice);

	// ローカル座標におけるワールドマトリックスを作成する
	MakeMtxLocalRightHand();
}

//=============================================================================
//    頂点バッファを作成する処理
//=============================================================================
void CAudience::MakeVertexRightHand(const LPDIRECT3DDEVICE9 pDevice)
{
	// 球モデルを読み込み
	LPD3DXMESH pMesh = NULL;
	LPD3DXBUFFER pBuffMat = NULL;
	DWORD nNumMat;
	D3DXLoadMeshFromX(AUDIENCE_PARTS_FILENAME, D3DXMESH_SYSTEMMEM, pDevice, NULL, &pBuffMat, NULL, &nNumMat, &pMesh);
	if (pMesh == NULL || pBuffMat == NULL) { return; }

	// 頂点数とポリゴン数を獲得
	m_nNumVertex[PARTS_RIGHT_HAND] = pMesh->GetNumVertices();
	m_nNumPolygon[PARTS_RIGHT_HAND] = pMesh->GetNumFaces();

	// インデックスバッファを取得
	pMesh->GetIndexBuffer(&m_pIdxBuff[PARTS_RIGHT_HAND]);

	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEXDATA) * m_nNumVertex[PARTS_RIGHT_HAND],
		D3DUSAGE_WRITEONLY,
		(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1),
		D3DPOOL_MANAGED,
		&m_pVtxBuff[PARTS_RIGHT_HAND],
		NULL);

	// 頂点情報の設定
	VERTEXDATA *pVtx;

	// 頂点バッファをロックし,頂点データへのポインタを取得
	m_pVtxBuff[PARTS_RIGHT_HAND]->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点バッファをロック
	BYTE *pVtxBuff;  // 頂点バッファへのポインタ
	pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < m_nNumVertex[PARTS_RIGHT_HAND]; nCntVtx++)
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
	m_pVtxBuff[PARTS_RIGHT_HAND]->Unlock();

	// データを開放
	pMesh->Release();
	pBuffMat->Release();
}

//=============================================================================
//    頂点カラーバッファを作成する処理
//=============================================================================
void CAudience::MakeColorBuffRightHand(const LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点カラーバッファを生成
	pDevice->CreateVertexBuffer(sizeof(COLORDATA) * m_nNumInstance,
		D3DUSAGE_WRITEONLY,
		(D3DFVF_DIFFUSE),
		D3DPOOL_MANAGED,
		&m_pColorBuff[PARTS_RIGHT_HAND],
		NULL);

	// 頂点カラー情報の設定
	COLORDATA *pColor;

	// 頂点カラーバッファをロックし,頂点カラーデータへのポインタを取得
	m_pColorBuff[PARTS_RIGHT_HAND]->Lock(0, 0, (void**)&pColor, 0);

	// 頂点カラーの設定
	for (int nCnt = 0; nCnt < m_nNumInstance; nCnt++)
	{
		pColor[nCnt].col = AUDIENCE_RIGHTHAND_COLOR / AUDIECNE_COLOR_RIVISION;
	}

	// 頂点カラーバッファをアンロックする
	m_pColorBuff[PARTS_RIGHT_HAND]->Unlock();
}

//=============================================================================
//    ローカル座標におけるワールドマトリックスを作成する処理
//=============================================================================
void CAudience::MakeMtxLocalRightHand(void)
{
	// 大きさを反映
	m_MtxLocal[PARTS_RIGHT_HAND]._11 = m_MtxLocal[PARTS_RIGHT_HAND]._11 * AUDIENCE_RIGHTHAND_SCALE.x;
	m_MtxLocal[PARTS_RIGHT_HAND]._12 = m_MtxLocal[PARTS_RIGHT_HAND]._12 * AUDIENCE_RIGHTHAND_SCALE.x;
	m_MtxLocal[PARTS_RIGHT_HAND]._13 = m_MtxLocal[PARTS_RIGHT_HAND]._13 * AUDIENCE_RIGHTHAND_SCALE.x;
	m_MtxLocal[PARTS_RIGHT_HAND]._21 = m_MtxLocal[PARTS_RIGHT_HAND]._21 * AUDIENCE_RIGHTHAND_SCALE.y;
	m_MtxLocal[PARTS_RIGHT_HAND]._22 = m_MtxLocal[PARTS_RIGHT_HAND]._22 * AUDIENCE_RIGHTHAND_SCALE.y;
	m_MtxLocal[PARTS_RIGHT_HAND]._23 = m_MtxLocal[PARTS_RIGHT_HAND]._23 * AUDIENCE_RIGHTHAND_SCALE.y;
	m_MtxLocal[PARTS_RIGHT_HAND]._31 = m_MtxLocal[PARTS_RIGHT_HAND]._31 * AUDIENCE_RIGHTHAND_SCALE.z;
	m_MtxLocal[PARTS_RIGHT_HAND]._32 = m_MtxLocal[PARTS_RIGHT_HAND]._32 * AUDIENCE_RIGHTHAND_SCALE.z;
	m_MtxLocal[PARTS_RIGHT_HAND]._33 = m_MtxLocal[PARTS_RIGHT_HAND]._33 * AUDIENCE_RIGHTHAND_SCALE.z;

	// オフセット位置を反映
	m_MtxLocal[PARTS_RIGHT_HAND]._41 = AUDIENCE_RIGHTHAND_OFFSET.x;
	m_MtxLocal[PARTS_RIGHT_HAND]._42 = AUDIENCE_RIGHTHAND_OFFSET.y;
	m_MtxLocal[PARTS_RIGHT_HAND]._43 = AUDIENCE_RIGHTHAND_OFFSET.z;
}

//=============================================================================
//
//=============================================================================
void  CAudience::Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
{

}