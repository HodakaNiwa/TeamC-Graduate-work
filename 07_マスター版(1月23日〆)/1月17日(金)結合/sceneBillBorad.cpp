//=============================================================================
//
// ビルボード処理 [scenebillbord.cpp]
// Author : 佐藤安純 Sato_Asumi
//
//=============================================================================
#include "sceneBillborad.h"
#include "manager.h"
#include "renderer.h"

//=============================================================================
// 生成処理
//=============================================================================
CSceneBillborad * CSceneBillborad::Create(const D3DXVECTOR3 pos)
{
	//インスタンスの生成
	CSceneBillborad * pSceneBillboard;
	pSceneBillboard = new CSceneBillborad;

	//初期化処理
	pSceneBillboard->Init();

	//設定処理
	pSceneBillboard->Set(pos, D3DXVECTOR3(25.0f, 25.0f, 0.0f));

	return pSceneBillboard;
}

//=============================================================================
// コンストラクタ
//=============================================================================
CSceneBillborad::CSceneBillborad() : CScene(3, OBJTYPE_BILLBORAD)
{
}

//=============================================================================
// コンストラクタ
//=============================================================================
CSceneBillborad::CSceneBillborad(int nPriority, OBJTYPE ObjType) : CScene(nPriority, ObjType)
{
	//変数の初期化
	m_pTexture = NULL;
	m_pVtxBuff = NULL;
	m_pos = INITIALIZE_VECTOR3;
	m_rot = INITIALIZE_VECTOR3;
	m_size = INITIALIZE_VECTOR3;
	m_Color = INITIALIZE_VECTOR4;
	D3DXMatrixIdentity(&m_mtxWorld);
}

//=============================================================================
// デストラクタ
//=============================================================================
CSceneBillborad::~CSceneBillborad()
{
}

//=============================================================================
// テクスチャの割り当て
//=============================================================================
void CSceneBillborad::BindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CSceneBillborad::Init(void)
{
	//変数の初期化
	m_pTexture = NULL;
	m_pVtxBuff = NULL;
	m_pos = INITIALIZE_VECTOR3;
	m_rot = INITIALIZE_VECTOR3;
	m_size = INITIALIZE_VECTOR3;
	m_Color = INITIALIZE_VECTOR4;
	D3DXMatrixIdentity(&m_mtxWorld);

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
void CSceneBillborad::Uninit(void)
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
void CSceneBillborad::Update(void){}

//=============================================================================
// 描画処理
//=============================================================================
void CSceneBillborad::Draw(void)
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

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//ライティングを無効にする 安
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//デフォルトの場合FALSEになっているからTRUEにする　安
	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

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

	//ライティングを有効にする　安
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	//デフォルトの場合FALSEになっているからTRUEにする　安
	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
}

//=============================================================================
// 設定処理
//=============================================================================
void CSceneBillborad::Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
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
void CSceneBillborad::SetSize(D3DXVECTOR3 size)
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
void CSceneBillborad::SetPos(D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//=============================================================================
// 色の設定
//=============================================================================
void CSceneBillborad::SetColor(D3DXCOLOR color)
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
void CSceneBillborad::SetColorABillborad(float colA)
{
	//a値の更新
	m_Color.a = colA;

	//色の設定
	SetColor(m_Color);
}

//=============================================================================
// UVの設定
//=============================================================================
void CSceneBillborad::SetUV(D3DXVECTOR2 uvMIn, D3DXVECTOR2 uvMax)
{
	VERTEX_3D * pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].tex = D3DXVECTOR2(uvMIn.x, uvMax.y);
	pVtx[1].tex = D3DXVECTOR2(uvMax.x, uvMax.y);
	pVtx[2].tex = D3DXVECTOR2(uvMIn.x, uvMIn.y);
	pVtx[3].tex = D3DXVECTOR2(uvMax.x, uvMIn.y);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}