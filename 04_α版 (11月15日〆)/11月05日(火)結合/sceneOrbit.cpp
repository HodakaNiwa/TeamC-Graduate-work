//=============================================================================
//
// 軌跡処理 [sceneOrbit.cpp]
// Author : 佐藤安純 Sato_Asumi
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "sceneOrbit.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "LoadText.h"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9 * CSceneOrbit::m_pTexture = NULL;
CSceneOrbit::ORBIT_INFO * CSceneOrbit::m_pOrbitInfo = NULL;
int CSceneOrbit::m_nMaxTexture = 0;
CLoadTextOrbit * CSceneOrbit::m_pLoadText = NULL;

//=============================================================================
// 生成処理
//=============================================================================
CSceneOrbit * CSceneOrbit::Create(TYPE type, D3DXVECTOR3 pos)
{
	//インスタンスの生成
	CSceneOrbit * pSceneOrbit;
	pSceneOrbit = new CSceneOrbit;

	//初期化処理
	pSceneOrbit->Init();		//初期化処理
	pSceneOrbit->SetType(type);	//タイプの設定

	//設定処理
	pSceneOrbit->Set(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	return pSceneOrbit;
}

//=============================================================================
// テクスチャ設定
//=============================================================================
void CSceneOrbit::Load(void)
{
	if (m_pOrbitInfo == NULL)
	{
		m_pLoadText = CLoadTextOrbit::Create();
		m_pOrbitInfo = m_pLoadText->GetOrbit();
	}

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	CLoadTextOrbit::TEX_INFO TexInfo = m_pLoadText->GetTexInfo();		//奇跡のテクスチャ情報の取得
	m_nMaxTexture = TexInfo.nMaxTex;									//テクスチャの最大数を取得

	if (m_pTexture == NULL) { m_pTexture = new LPDIRECT3DTEXTURE9[m_nMaxTexture]; }

	for (int nCntTex = 0; nCntTex < m_nMaxTexture; nCntTex++)
	{
		// テクスチャの生成
		D3DXCreateTextureFromFile(pDevice,
			TexInfo.ppFileName[nCntTex],
			&m_pTexture[nCntTex]);
	}
}

//=============================================================================
// テクスチャ破棄
//=============================================================================
void CSceneOrbit::Unload(void)
{
	//テクスチャの破棄
	for (int nCntTex = 0; nCntTex < m_nMaxTexture; nCntTex++)
	{
		if (m_pTexture[nCntTex] != NULL)
		{
			m_pTexture[nCntTex]->Release();
			m_pTexture[nCntTex] = NULL;
		}
	}

	//ポインタの初期化
	if (m_pTexture != NULL)
	{
		delete[] m_pTexture;
 		m_pTexture = NULL;
	}

	//軌跡のポインタ情報の破棄
	if (m_pOrbitInfo != NULL)
	{
		m_pOrbitInfo = NULL;
	}

	//テキストの破棄
	if (m_pLoadText != NULL)
	{
		m_pLoadText->Uninit();
		delete m_pLoadText;
		m_pLoadText = NULL;
	}
}

//=============================================================================
// コンストラクタ
//=============================================================================
CSceneOrbit::CSceneOrbit() : CScene(4, OBJTYPE_ORBIT)
{
	m_pVtxBuff = NULL;				//頂点バッファ
}

//=============================================================================
// デストラクタ
//=============================================================================
CSceneOrbit::~CSceneOrbit(){}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CSceneOrbit::Init(void)
{
	//変数の初期化
	m_pVtxBuff = NULL;			//頂点バッファ
	m_pMtxParent = NULL;		//マトリックスの始点のポインタ
	m_pMtxParentEnd = NULL;		//マトリックスの終点ポインタ
	m_aPosVertex = NULL;		//保存用頂点情報
	m_type = TYPE_MAX;			//タイプ情報
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CSceneOrbit::Uninit(void)
{
	//保存用頂点の破棄
	if (m_aPosVertex != NULL)
	{
		delete[] m_aPosVertex;
		m_aPosVertex = NULL;
	}

	//頂点バッファの破棄
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	//死亡フラグを立てる
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CSceneOrbit::Update(void)
{
	VERTEX_3D * pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntPrimitiv = 0; nCntPrimitiv < m_pOrbitInfo[m_type].nPrimitiv; nCntPrimitiv++)
		{
			pVtx[0].tex.x += m_pOrbitInfo[m_type].fAnim;

			//該当の位置まで進める
			pVtx++;
		}

		//頂点バッファをアンロックする
		m_pVtxBuff->Unlock();
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CSceneOrbit::Draw(void)
{
	//変数宣言
	D3DXVECTOR3 pos;
	D3DXVECTOR3 pos1 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 pos2 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//レンダリングクラスを取得
	CRenderer * pRenderer = NULL;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	D3DXMATRIX		  mtxRot, mtxTrans, mtxOfset, mtxOfset1;			// 計算用マトリックス

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	D3DXMATRIX mtx = *m_pMtxParent;

	D3DXVec3TransformCoord(&pos1, &m_pOrbitInfo[m_type].pos[0], m_pMtxParent);
	D3DXVec3TransformCoord(&pos2, &m_pOrbitInfo[m_type].pos[1], m_pMtxParentEnd);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//頂点をずらす
	for (int nCntPos = m_pOrbitInfo[m_type].nPrimitiv / 2 - 2; nCntPos >= 0; nCntPos--)
	{
		m_aPosVertex[nCntPos * 2 + 2] = m_aPosVertex[nCntPos * 2];
		m_aPosVertex[nCntPos * 2 + 3] = m_aPosVertex[nCntPos * 2 + 1];
	}

	//原点を取得する
	m_aPosVertex[0] = pos1;
	m_aPosVertex[1] = pos2;

	VERTEX_3D * pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntPrimitiv = 0; nCntPrimitiv < m_pOrbitInfo[m_type].nPrimitiv; nCntPrimitiv++)
	{
		pVtx[0].pos = m_aPosVertex[nCntPrimitiv];

		//該当の位置まで進める
		pVtx++;
	}

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	if (m_type == TYPE_STAR)
	{
		// Zテスト処理を無効にする
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	}

	//ライトを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);	//裏面をカリングする

	// αブレンディングを加算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// テクスチャの設定
	pDevice->SetTexture(0, m_pTexture[m_pOrbitInfo[m_type].nTex]);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
						   0,
						   m_pOrbitInfo[m_type].nPorigon);

	// αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);	//裏面をカリングする

	//ライトを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	if (m_type == TYPE_STAR)
	{
		// Zテスト処理をデフォルトにする
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	}
}

//=============================================================================
// 設定処理
//=============================================================================
void CSceneOrbit::Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
{
	//変数宣言
	float fWidthSize = 0;		//横幅のサイズ
	float fHightSize = 0;		//縦幅のサイズ
	D3DXVECTOR3 Vec0[2] = {};	//ベクトル0
	D3DXVECTOR3 Vec1[2] = {};	//ベクトル1
	int nCntPos = 0;				//読み込んだ座標配列を進める
	m_pos = pos;

	//保存用頂点の生成
	if (m_aPosVertex == NULL)
	{
		m_aPosVertex = new D3DXVECTOR3[m_pOrbitInfo[m_type].nPrimitiv];
	}

	//ポリゴン用の法線
	D3DXVECTOR3 nor[8000] = {};	//ポリゴンの法線ベクトル
	int nCntNor = 0;			//法線の配列を進める

	//レンダリングクラスを取得
	CRenderer * pRenderer = NULL;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_pOrbitInfo[m_type].nPrimitiv,	//確保する頂点バッファの数
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	VERTEX_3D * pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntPrimitiv = 0; nCntPrimitiv < m_pOrbitInfo[m_type].nPrimitiv / 2; nCntPrimitiv++)
	{
	    //頂点座標の設定
		pVtx[0].pos = m_pOrbitInfo[m_type].pos[0] + m_pos;
		pVtx[1].pos = m_pOrbitInfo[m_type].pos[1] + m_pos;

		//座標を保存する
		m_aPosVertex[nCntPos] = pVtx[0].pos;
		m_aPosVertex[nCntPos + 1] = pVtx[1].pos;

		//法線の設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//色の設定
		pVtx[0].col = m_pOrbitInfo[m_type].color;
		pVtx[1].col = m_pOrbitInfo[m_type].color;

		//テクスチャの設定
		pVtx[0].tex = D3DXVECTOR2(0.0f + (nCntPrimitiv * m_pOrbitInfo[m_type].fUV), 1.0f);
		pVtx[1].tex = D3DXVECTOR2(0.0f + (nCntPrimitiv * m_pOrbitInfo[m_type].fUV), 0.0f);

		//該当の位置まで進める
		pVtx += 2;
		nCntPos += 2;		//配列のカウンターを進める
	}

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================================================
// 色の設定
//=============================================================================
void CSceneOrbit::SetColor(D3DXCOLOR col)
{
	VERTEX_3D * pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntPrimitiv = 0; nCntPrimitiv < m_pOrbitInfo[m_type].nPrimitiv / 2; nCntPrimitiv++)
	{
		//色の設定
		pVtx[0].col = col;
		pVtx[1].col = col;

		//該当の位置まで進める
		pVtx += 2;
	}

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}