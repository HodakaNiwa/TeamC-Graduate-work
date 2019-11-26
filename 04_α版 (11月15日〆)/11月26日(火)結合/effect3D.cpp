//*****************************************************************************
//
//     エフェクト(3D)の処理[effect3D.cpp]
//     Author:Hodaka Niwa
//
//*****************************************************************************
#include "effect3D.h"
#include "manager.h"
#include "renderer.h"
#include "library.h"

//*****************************************************************************
//     マクロ定義
//*****************************************************************************
#define EFFECT3D_TEX_FILENAME "data/TEXTURE/Effect/effect000.jpg"

//*****************************************************************************
//    静的メンバ変数宣言
//*****************************************************************************
LPDIRECT3DTEXTURE9 CEffect3D::m_pTexture = NULL;// テクスチャへのポインタ

//=============================================================================
//    コンストラクタ
//=============================================================================
CEffect3D::CEffect3D(int nPriority, OBJTYPE objType) : CScene(nPriority, objType)
{

}

//=============================================================================
//    デストラクタ
//=============================================================================
CEffect3D::~CEffect3D()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CEffect3D *CEffect3D::Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, int nLife, float fCutAlpha, float fCutLength, bool bLighting, int nPriority, float fRot, float fTexU, float fTexV, float fTexWidth, float fTexHeight)
{
	// メモリの確保
	CEffect3D *pEffect3D = NULL;
	pEffect3D = new CEffect3D(nPriority);
	if (pEffect3D == NULL) { return NULL; }

	// 変数の初期化
	pEffect3D->ClearVariable();

	// 各種値の代入
	pEffect3D->SetPos(pos);					// ポリゴンの座標
	pEffect3D->SetCol(col);					// ポリゴンの色
	pEffect3D->SetRot(fRot);				// ポリゴンの向き
	pEffect3D->SetWidth(fWidth);			// ポリゴンの幅
	pEffect3D->SetHeight(fHeight);			// ポリゴンの高さ
	pEffect3D->SetTexU(fTexU);				// ポリゴンの左上テクスチャU座標
	pEffect3D->SetTexV(fTexV);				// ポリゴンの左上テクスチャV座標
	pEffect3D->SetTexWidth(fTexWidth);		// ポリゴンのテクスチャ座標の横幅
	pEffect3D->SetTexHeight(fTexHeight);	// ポリゴンのテクスチャ座標の縦幅
	pEffect3D->SetLighting(bLighting);		// ライティングするかしないか
	pEffect3D->SetLife(nLife);				// 寿命
	pEffect3D->SetCutAlpha(fCutAlpha);		// 透明度を削る量
	pEffect3D->SetCutLength(fCutLength);	// 大きさを削る量

	// 初期化処理
	if (FAILED(pEffect3D->Init()))
	{
		pEffect3D->Uninit();
		return NULL;
	}

	return pEffect3D;
}

//=============================================================================
//    描画処理
//=============================================================================
HRESULT CEffect3D::Load(void)
{
	if (m_pTexture != NULL) { return E_FAIL; }
	// テクスチャの読み込み
	if (FAILED(D3DXCreateTextureFromFile(CManager::GetRenderer()->GetDevice(), EFFECT3D_TEX_FILENAME, &m_pTexture)))
	{
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
//    描画処理
//=============================================================================
void CEffect3D::UnLoad(void)
{
	DIRECT_RELEASE(m_pTexture);
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CEffect3D::Init(void)
{
	// 描画デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	if (pDevice == NULL) { return E_FAIL; }

	// 頂点バッファの作成
	MakeVertex(pDevice);

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CEffect3D::Uninit(void)
{
	// 頂点バッファの破棄
	DIRECT_RELEASE(m_pVtxBuff);

	// リリース処理
	CScene::Release();
}

//=============================================================================
//    更新処理
//=============================================================================
void CEffect3D::Update(void)
{
	// 透明度を削る
	m_Col.a -= m_fCutAlpha;

	// 大きさを削る
	m_fLength -= m_fCutLength;

	// 頂点バッファに値設定
	SetValue();

	// 寿命を削る
	m_nLife--;

	// 死亡判定
	if (m_nLife <= 0 || m_Col.a <= 0.0f || m_fLength <= 0.0f)
	{
		Uninit();
	}
}

//=============================================================================
//    頂点バッファに値を設定する処理
//=============================================================================
void CEffect3D::SetValue(void)
{
	// 頂点情報の設定
	VERTEX_3D *pVtx;

	// 頂点バッファをロックし,頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標
	pVtx[0].pos = D3DXVECTOR3(-sinf(m_fAngle) * m_fLength, cosf(m_fAngle) * m_fLength, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(sinf(m_fAngle) * m_fLength, cosf(m_fAngle) * m_fLength, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-sinf(m_fAngle) * m_fLength, -cosf(m_fAngle) * m_fLength, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(sinf(m_fAngle) * m_fLength, -cosf(m_fAngle) * m_fLength, 0.0f);

	// 頂点カラー
	pVtx[0].col = m_Col;
	pVtx[1].col = m_Col;
	pVtx[2].col = m_Col;
	pVtx[3].col = m_Col;

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================================================
//    描画処理
//=============================================================================
void CEffect3D::Draw(void)
{
	// 描画デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	if (pDevice == NULL) { return; }

	// トランスフォーム情報を計算しデバイスに設定
	CalcNotScaleTransform(pDevice);
	SetTransformToDevice(pDevice);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, m_pTexture);

	// ライティングの設定
	DWORD Lighting;
	pDevice->GetRenderState(D3DRS_LIGHTING, &Lighting);
	if (m_bLighting == false)
	{// ライティングを外す
		pDevice->SetRenderState(D3DRS_LIGHTING, false);
	}

	// αブレンディングを加算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// Zバッファに書き込まない設定に
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	// Zバッファに書き込む設定に
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	// αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// ライティングを戻す
	pDevice->SetRenderState(D3DRS_LIGHTING, Lighting);
}

//=============================================================================
//    トランスフォーム情報(大きさを含めない)を計算する
//=============================================================================
void CEffect3D::CalcNotScaleTransform(const LPDIRECT3DDEVICE9 pDevice)
{
	D3DXMATRIX mtxView, mtxTrans; // 計算用マトリックス

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_MtxWorld);

	// カメラの向きを取得
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	// ワールドマトリックスに向きを設定(カメラの逆行列を入れることでカメラの正面に向きを補正)
	m_MtxWorld._11 = mtxView._11;
	m_MtxWorld._12 = mtxView._21;
	m_MtxWorld._13 = mtxView._31;
	m_MtxWorld._21 = mtxView._12;
	m_MtxWorld._22 = mtxView._22;
	m_MtxWorld._23 = mtxView._32;
	m_MtxWorld._31 = mtxView._13;
	m_MtxWorld._32 = mtxView._23;
	m_MtxWorld._33 = mtxView._33;

	// 位置を反映
	m_MtxWorld._41 = m_Pos.x;
	m_MtxWorld._42 = m_Pos.y;
	m_MtxWorld._43 = m_Pos.z;
}

//=============================================================================
//    トランスフォーム情報をデバイスに設定する
//=============================================================================
void CEffect3D::SetTransformToDevice(const LPDIRECT3DDEVICE9 pDevice)
{
	pDevice->SetTransform(D3DTS_WORLD, &m_MtxWorld);
}

//=============================================================================
//    頂点バッファ生成処理
//=============================================================================
void CEffect3D::MakeVertex(const LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// 頂点バッファへ値を設定する
	SetVtxBuffValue();
}

//=============================================================================
//    ポリゴンの大きさを設定する処理
//=============================================================================
void CEffect3D::SetScale(float fWidth, float fHeight)
{
	// オフセットの長さと角度を求める
	m_fLength = sqrtf((m_fWidth * m_fWidth) + (m_fHeight * m_fHeight));
	m_fAngle = atan2f(m_fWidth, m_fHeight);
}

//=============================================================================
//    頂点バッファへ値を設定する処理
//=============================================================================
void CEffect3D::SetVtxBuffValue(void)
{
	// 頂点情報の設定
	VERTEX_3D *pVtx;

	// 頂点バッファをロックし,頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 大きさを設定
	SetScale(m_fWidth, m_fHeight);

	// 頂点座標
	pVtx[0].pos = D3DXVECTOR3(-sinf(m_fAngle) * m_fLength, cosf(m_fAngle) * m_fLength, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(sinf(m_fAngle) * m_fLength, cosf(m_fAngle) * m_fLength, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-sinf(m_fAngle) * m_fLength, -cosf(m_fAngle) * m_fLength, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(sinf(m_fAngle) * m_fLength, -cosf(m_fAngle) * m_fLength, 0.0f);

	// 法線計算用ベクトル
	D3DXVECTOR3 vecVer[4];

	// 頂点の法線
	D3DXVECTOR3 nor1, nor2;

	// 外積用にベクトルを作る
	vecVer[0] = pVtx[0].pos - pVtx[2].pos;
	vecVer[1] = pVtx[1].pos - pVtx[2].pos;
	vecVer[2] = pVtx[2].pos - pVtx[1].pos;
	vecVer[3] = pVtx[3].pos - pVtx[1].pos;

	// ベクトルを外積で計算して法線を出す
	D3DXVec3Cross(&nor1, &vecVer[0], &vecVer[1]);
	D3DXVec3Cross(&nor2, &vecVer[3], &vecVer[2]);
	D3DXVec3Normalize(&nor1, &nor1);
	D3DXVec3Normalize(&nor2, &nor2);

	// 法線を代入
	pVtx[0].nor = nor1;
	pVtx[1].nor = (nor1 + nor2) / 2;
	pVtx[2].nor = (nor1 + nor2) / 2;
	pVtx[3].nor = nor2;

	// 頂点カラー
	pVtx[0].col = m_Col;
	pVtx[1].col = m_Col;
	pVtx[2].col = m_Col;
	pVtx[3].col = m_Col;

	// テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(m_fTexU, m_fTexV);
	pVtx[1].tex = D3DXVECTOR2(m_fTexU + m_fTexWidth, m_fTexV);
	pVtx[2].tex = D3DXVECTOR2(m_fTexU, m_fTexV + m_fTexHeight);
	pVtx[3].tex = D3DXVECTOR2(m_fTexU + m_fTexWidth, m_fTexV + m_fTexHeight);

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================================================
//    頂点バッファへ頂点座標を設定する処理
//=============================================================================
void CEffect3D::SetVtxBuffPos(void)
{
	// 頂点情報の設定
	VERTEX_3D *pVtx;

	// 頂点バッファをロックし,頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 大きさを設定
	SetScale(m_fWidth, m_fHeight);

	// 頂点座標
	pVtx[0].pos = D3DXVECTOR3(-sinf(m_fAngle) * m_fLength, 0.0f, cosf(m_fAngle) * m_fLength);
	pVtx[1].pos = D3DXVECTOR3(sinf(m_fAngle) * m_fLength, 0.0f, cosf(m_fAngle) * m_fLength);
	pVtx[2].pos = D3DXVECTOR3(-sinf(m_fAngle) * m_fLength, 0.0f, -cosf(m_fAngle) * m_fLength);
	pVtx[3].pos = D3DXVECTOR3(sinf(m_fAngle) * m_fLength, 0.0f, -cosf(m_fAngle) * m_fLength);

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================================================
//    頂点バッファへ法線ベクトルを設定する処理
//=============================================================================
void CEffect3D::SetVtxBuffNor(void)
{
	// 頂点情報の設定
	VERTEX_3D *pVtx;

	// 頂点バッファをロックし,頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 法線計算用ベクトル
	D3DXVECTOR3 vecVer[4];

	// 頂点の法線
	D3DXVECTOR3 nor1, nor2;

	// 外積用にベクトルを作る
	vecVer[0] = pVtx[0].pos - pVtx[2].pos;
	vecVer[1] = pVtx[1].pos - pVtx[2].pos;
	vecVer[2] = pVtx[2].pos - pVtx[1].pos;
	vecVer[3] = pVtx[3].pos - pVtx[1].pos;

	// ベクトルを外積で計算して法線を出す
	D3DXVec3Cross(&nor1, &vecVer[0], &vecVer[1]);
	D3DXVec3Cross(&nor2, &vecVer[3], &vecVer[2]);
	D3DXVec3Normalize(&nor1, &nor1);
	D3DXVec3Normalize(&nor2, &nor2);

	// 法線を代入
	pVtx[0].nor = nor1;
	pVtx[1].nor = (nor1 + nor2) / 2;
	pVtx[2].nor = (nor1 + nor2) / 2;
	pVtx[3].nor = nor2;

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================================================
//    頂点バッファへ頂点カラーを設定する処理
//=============================================================================
void CEffect3D::SetVtxBuffCol(void)
{
	// 頂点情報の設定
	VERTEX_3D *pVtx;

	// 頂点バッファをロックし,頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点カラー
	pVtx[0].col = m_Col;
	pVtx[1].col = m_Col;
	pVtx[2].col = m_Col;
	pVtx[3].col = m_Col;

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================================================
//    頂点バッファへテクスチャ座標を設定する
//=============================================================================
void CEffect3D::SetVtxBuffTex(void)
{
	// 頂点情報の設定
	VERTEX_3D *pVtx;

	// 頂点バッファをロックし,頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(m_fTexU, m_fTexV);
	pVtx[1].tex = D3DXVECTOR2(m_fTexU + m_fTexWidth, m_fTexV);
	pVtx[2].tex = D3DXVECTOR2(m_fTexU, m_fTexV + m_fTexHeight);
	pVtx[3].tex = D3DXVECTOR2(m_fTexU + m_fTexWidth, m_fTexV + m_fTexHeight);

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================================================
//    変数のクリア
//=============================================================================
void CEffect3D::ClearVariable(void)
{
	// 各種値のクリア
	m_pVtxBuff = NULL;							// 頂点バッファへのポインタ
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ポリゴンの座標
	m_Col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);	// ポリゴンの色
	m_fRot = 0.0f;								// ポリゴンの向き
	m_fWidth = 0.0f;							// ポリゴンの幅
	m_fHeight = 0.0f;							// ポリゴンの高さ
	m_fAngle = 0.0f;							// ポリゴンを出す角度
	m_fLength = 0.0f;							// ポリゴンの長さ
	m_fTexU = 0.0f;								// ポリゴンの左上テクスチャU座標
	m_fTexV = 0.0f;								// ポリゴンの左上テクスチャV座標
	m_fTexWidth = 0.0f;							// ポリゴンのテクスチャ座標の横幅
	m_fTexHeight = 0.0f;						// ポリゴンのテクスチャ座標の縦幅
	D3DXMatrixIdentity(&m_MtxWorld);			// ワールドマトリックス
	m_bLighting = false;						// ライティングするかしないか
	m_nLife = 0;								// 寿命
	m_fCutAlpha = 0.0f;							// 透明度を削る量
}