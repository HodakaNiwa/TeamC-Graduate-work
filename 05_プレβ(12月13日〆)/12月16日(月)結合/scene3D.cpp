//=============================================================================
//
// 3Dポリゴン処理 [scene3D.cpp]
// Author : 佐藤安純 Sato_Asumi
//
//=============================================================================
#include "scene3D.h"
#include "manager.h"
#include "renderer.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define TEXTURE_NAME00	("data//TEXTURE//grand.jpg")
#define MAX_POS			(4)

//=============================================================================
// 生成処理
//=============================================================================
CScene3D * CScene3D::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
{
	//インスタンスの生成
	CScene3D * pScene3D;
	pScene3D = new CScene3D;

	//初期化処理
	pScene3D->Init();

	//設定処理
	pScene3D->Set(pos, size);

	return pScene3D;
}

//=============================================================================
// コンストラクタ
//=============================================================================
CScene3D::CScene3D() : CScene(3, OBJTYPE_FLOR)
{
	m_pTexture = NULL;
	m_pVtxBuff = NULL;
	m_pos = D3DXVECTOR3(0, 0, 0);
	m_rot = D3DXVECTOR3(0, 0, 0);
	m_col = INITIALIZE_VECTOR4;
}

//=============================================================================
// コンストラクタ
//=============================================================================
CScene3D::CScene3D(int nPriority, CScene::OBJTYPE obj) : CScene(nPriority, obj)
{
}

//=============================================================================
// デストラクタ
//=============================================================================
CScene3D::~CScene3D()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CScene3D::Init(void)
{
	//変数宣言
	m_pTexture = NULL;
	m_pVtxBuff = NULL;
	m_pos = D3DXVECTOR3(0, 0, 0);
	m_rot = D3DXVECTOR3(0, 0, 0);
	m_Division = D3DXVECTOR2(1, 1);

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
	pVtx[0].pos = D3DXVECTOR3(-50.0f, 50.0f, 50.0f);
	pVtx[1].pos = D3DXVECTOR3(50.0f, 0.0f, 50.0f);
	pVtx[2].pos = D3DXVECTOR3(-50.0f, 50.0f, -50.0f);
	pVtx[3].pos = D3DXVECTOR3(50.0f, 0.0f, -50.0f);

	pVtx[0].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);

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

	// 色の情報を保存
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CScene3D::Uninit(void)
{
	//頂点バッファの破棄
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
void CScene3D::Update(void)
{
}

//=============================================================================
// 描画処理
//=============================================================================
void CScene3D::Draw(void)
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
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

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
void CScene3D::Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
{
	m_pos = pos;
	m_size = size;

	//変数宣言
	D3DXVECTOR3 Vec0[2] = {};
	D3DXVECTOR3 Vec1[2] = {};

	VERTEX_3D * pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-size.x, 0.0f, size.z);
	pVtx[1].pos = D3DXVECTOR3(size.x, 0.0f, size.z);
	pVtx[2].pos = D3DXVECTOR3(-size.x, 0.0f, -size.z);
	pVtx[3].pos = D3DXVECTOR3(size.x, 0.0f, -size.z);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}


//=============================================================================
// 色の設定
//=============================================================================
void CScene3D::SetColor(D3DXCOLOR col)
{
	VERTEX_3D * pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	// 頂点カラー情報を保存
	m_col = col;
}

//=============================================================================
// テクスチャの更新
//=============================================================================
void CScene3D::SetMoveTex(float fMoveX, float fMoveY)
{
	VERTEX_3D * pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f * m_Division.x + fMoveX, 0.0f * m_Division.y + fMoveY);
	pVtx[1].tex = D3DXVECTOR2(1.0f * m_Division.x + fMoveX, 0.0f * m_Division.y + fMoveY);
	pVtx[2].tex = D3DXVECTOR2(0.0f * m_Division.x + fMoveX, 1.0f * m_Division.y + fMoveY);
	pVtx[3].tex = D3DXVECTOR2(1.0f * m_Division.x + fMoveX, 1.0f * m_Division.y + fMoveY);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================================================
// テクスチャの分割数の設定
//=============================================================================
void CScene3D::SetDivision(D3DXVECTOR2 Division)
{
	m_Division = Division;	//分割数の保存

	VERTEX_3D * pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f * Division.x, 0.0f * Division.y);
	pVtx[1].tex = D3DXVECTOR2(1.0f * Division.x, 0.0f * Division.y);
	pVtx[2].tex = D3DXVECTOR2(0.0f * Division.x, 1.0f * Division.y);
	pVtx[3].tex = D3DXVECTOR2(1.0f * Division.x, 1.0f * Division.y);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================================================
//　UVの分割数
//=============================================================================
void CScene3D::SetTexUV(float fWhidth, float fHight, float fWhidth2, float fHight2)
{//安
	VERTEX_3D * pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].tex = D3DXVECTOR2(fWhidth, fHight);
	pVtx[1].tex = D3DXVECTOR2(fWhidth2, fHight);
	pVtx[2].tex = D3DXVECTOR2(fWhidth, fHight2);
	pVtx[3].tex = D3DXVECTOR2(fWhidth2, fHight2);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}