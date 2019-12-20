//=============================================================================
//
// 三角形ポリゴン処理 [sceneTriangle3D.cpp]
// Author : 佐藤安純 Sato_Asumi
//
//=============================================================================
#include "sceneTriangle3D.h"
#include "manager.h"
#include "renderer.h"
#include "Player.h"
#include "character.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define TEXTURE_NAME00	("data//TEXTURE//grand.jpg")
#define MAX_POS			(4)

//=============================================================================
// 生成処理
//=============================================================================
CSceneTriangle3D * CSceneTriangle3D::Create(D3DXVECTOR3 Pos1, D3DXVECTOR3 Pos2, D3DXVECTOR3 Pos3)
{
	//インスタンスの生成
	CSceneTriangle3D * pScene3D;
	pScene3D = new CSceneTriangle3D;

	//初期化処理
	pScene3D->Init();
	pScene3D->SetPos(Pos1, Pos2, Pos3);
	return pScene3D;
}

//=============================================================================
// コンストラクタ
//=============================================================================
CSceneTriangle3D::CSceneTriangle3D() : CScene(4, OBJTYPE_FLOR)
{
	m_pVtxBuff = NULL;
}

//=============================================================================
// デストラクタ
//=============================================================================
CSceneTriangle3D::~CSceneTriangle3D(){}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CSceneTriangle3D::Init(void)
{
	//変数宣言
	m_pVtxBuff = NULL;
	m_nLife = LINE_TIME;
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//レンダリングクラスを取得
	CRenderer * pRenderer = NULL;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 3,
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
	pVtx[1].pos = D3DXVECTOR3(200.0f, 0.0f, -100.0f);
	pVtx[2].pos = D3DXVECTOR3(-200.0f, 0.0f, -100.0f);

	pVtx[0].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);

	//頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//UV値の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CSceneTriangle3D::Uninit(void)
{
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
void CSceneTriangle3D::Update(void)
{
	//変数宣言
	bool bDeth = false;	//死亡フラグ

	//表示時間を減らす
	m_nLife--;

	if (m_nLife < 0)
	{//表示時間が0以下になったら
		m_col.a -= 0.05f;

		if (m_col.a < 0.0f) 
		{
			m_col.a = 0.0f;
			bDeth = true;	//死亡フラグを有効にする
		}

		//色の設定
		SetColor(m_col);
	}

	//破棄する
	if (bDeth) { Uninit(); }
}

//=============================================================================
// 描画処理
//=============================================================================
void CSceneTriangle3D::Draw(void)
{
	//レンダリングクラスを取得
	CRenderer * pRenderer = NULL;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	D3DXMATRIX		  mtxRot, mtxTrans;			// 計算用マトリックス

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, 0.0f, 0.0f, 0.0f);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, 0.0f, 5.0f, 0.0f);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//ライトを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	if (m_pVtxBuff != NULL)
	{
		// 頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		// テクスチャの設定
		pDevice->SetTexture(0, NULL);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
			0,
			1);
	}

	//デフォルトに戻す
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

//=============================================================================
// 色の設定
//=============================================================================
void CSceneTriangle3D::SetColor(D3DXCOLOR col)
{
	//色の保存
	m_col = col;

	VERTEX_3D * pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================================================
// 位置設定
//=============================================================================
void CSceneTriangle3D::SetPos(D3DXVECTOR3 Pos1, D3DXVECTOR3 Pos2, D3DXVECTOR3 Pos3)
{
	VERTEX_3D * pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = Pos1;
	pVtx[1].pos = Pos2;
	pVtx[2].pos = Pos3;

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================================================
// 設定処理
//=============================================================================
void CSceneTriangle3D::Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size){}