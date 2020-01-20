//*****************************************************************************
//
//     分断イベント用の壁の処理[divisionWall.cpp]
//     Author:Hodaka Niwa
//
//*****************************************************************************
#include "divisionWall.h"
#include "manager.h"
#include "renderer.h"

//*****************************************************************************
//     マクロ定義
//*****************************************************************************
#define DIVISIONWALL_MAXHEIGHT (500.0f)

//*****************************************************************************
//    静的メンバ変数宣言
//*****************************************************************************

//=============================================================================
//    コンストラクタ
//=============================================================================
CDivisionWall::CDivisionWall(int nPriority, OBJTYPE objType) : CScene(nPriority, objType)
{
	// 各種値のクリア
	m_pTexture = NULL;							// テクスチャへのポインタ
	m_pVtxBuff = NULL;							// 頂点バッファへのポインタ
	m_Pos = INITIALIZE_VECTOR3;					// ポリゴンの位置
	m_Rot = INITIALIZE_VECTOR3;					// ポリゴンの向き
	D3DXMatrixIdentity(&m_MtxWorld);			// ワールドマトリックス
	m_Col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);	// ポリゴンの色
	m_fWidth = 0.0f;							// ポリゴンの幅
	m_fHeight = 0.0f;							// ポリゴンの高さ
	m_fAngle = 0.0f;							// ポリゴンの角度
	m_fLength = 0.0f;							// ポリゴンの長さ
	m_fTexU = 0.0f;								// 左上テクスチャU座標
	m_fTexV = 0.0f;								// 左上テクスチャV座標
	m_fTexWidth = 0.0f;							// テクスチャ座標の横幅
	m_fTexHeight = 0.0f;						// テクスチャ座標の縦幅
	m_fMoveHeight = 0.0f;						// 高さの移動量
}

//=============================================================================
//    デストラクタ
//=============================================================================
CDivisionWall::~CDivisionWall()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CDivisionWall *CDivisionWall::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fWidth,
	float fHeight, float fMoveHeight, int nPriority, float fTexU, float fTexV, float fTexWidth, float fTexHeight)
{
	CDivisionWall *pScene3D = NULL;             // 3Dポリゴンクラス型のポインタ
	if (pScene3D == NULL)
	{// メモリが空になっている
		pScene3D = new CDivisionWall(nPriority);
		if (pScene3D != NULL)
		{// インスタンスを生成できた
		 // 各種値の代入
			pScene3D->SetPos(pos);					// ポリゴンの座標
			pScene3D->SetRot(rot);					// ポリゴンの向き
			pScene3D->SetCol(col);					// ポリゴンの色
			pScene3D->SetWidth(fWidth);				// ポリゴンの幅
			pScene3D->SetHeight(fHeight);			// ポリゴンの高さ
			pScene3D->SetTexU(fTexU);				// ポリゴンの左上テクスチャU座標
			pScene3D->SetTexV(fTexV);				// ポリゴンの左上テクスチャV座標
			pScene3D->SetTexWidth(fTexWidth);		// ポリゴンのテクスチャ座標の横幅
			pScene3D->SetTexHeight(fTexHeight);		// ポリゴンのテクスチャ座標の縦幅
			pScene3D->SetMoveHeight(fMoveHeight);	// 高さの移動量

			if (FAILED(pScene3D->Init()))
			{// 初期化に失敗した
				return NULL;
			}
		}
		else
		{// インスタンスを生成できなかった
			return NULL;
		}
	}

	return pScene3D;  	// インスタンスのアドレスを返す
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CDivisionWall::Init(void)
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
void CDivisionWall::Uninit(void)
{
	// 頂点バッファの破棄
	DIRECT_RELEASE(m_pVtxBuff);

	// リリース処理
	CScene::Release();
}

//=============================================================================
//    更新処理
//=============================================================================
void CDivisionWall::Update(void)
{
	// 座標をずらす
	m_Pos.y += m_fMoveHeight;
	if (m_Pos.y <= 0.0f - (m_fHeight * 0.5f))
	{
		m_Pos.y = -(m_fHeight * 0.5f);
	}
	else if (m_Pos.y >= DIVISIONWALL_MAXHEIGHT)
	{
		m_Pos.y = DIVISIONWALL_MAXHEIGHT;
	}
}

//=============================================================================
//    描画処理
//=============================================================================
void CDivisionWall::Draw(void)
{
	// 描画デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	if (pDevice == NULL) { return; }

	// トランスフォーム情報を計算しデバイスに設定
	CalcNotScaleTransform();
	SetTransformToDevice(pDevice);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, m_pTexture);

	//αブレンディングを加算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// カリングしない設定に
	DWORD CullMode;
	pDevice->GetRenderState(D3DRS_CULLMODE, &CullMode);
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	// カリングの設定を戻す
	pDevice->SetRenderState(D3DRS_CULLMODE, CullMode);

	//αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//=============================================================================
//    大きさを含めないトランスフォーム情報を計算する処理
//=============================================================================
void CDivisionWall::CalcNotScaleTransform(void)
{
	D3DXMATRIX mtxRot; // 計算用マトリックス

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_MtxWorld);

	// 回転行列を作成(D3DXMatrixRotationYawPitchRoll参照)
	float fSinPitch = sinf(m_Rot.x);
	float fCosPitch = cosf(m_Rot.x);
	float fSinYaw = sinf(m_Rot.y);
	float fCosYaw = cosf(m_Rot.y);
	float fSinRoll = sinf(m_Rot.z);
	float fCosRoll = cosf(m_Rot.z);
	mtxRot._11 = fSinRoll * fSinPitch * fSinYaw + fCosRoll * fCosYaw;
	mtxRot._12 = fSinRoll * fCosPitch;
	mtxRot._13 = fSinRoll * fSinPitch * fCosYaw - fCosRoll * fSinYaw;
	mtxRot._21 = fCosRoll * fSinPitch * fSinYaw - fSinRoll * fCosYaw;
	mtxRot._22 = fCosRoll * fCosPitch;
	mtxRot._23 = fCosRoll * fSinPitch * fCosYaw + fSinRoll * fSinYaw;
	mtxRot._31 = fCosPitch * fSinYaw;
	mtxRot._32 = -fSinPitch;
	mtxRot._33 = fCosPitch * fCosYaw;

	// 回転を反映する
	m_MtxWorld._11 = mtxRot._11;
	m_MtxWorld._12 = mtxRot._12;
	m_MtxWorld._13 = mtxRot._13;
	m_MtxWorld._21 = mtxRot._21;
	m_MtxWorld._22 = mtxRot._22;
	m_MtxWorld._23 = mtxRot._23;
	m_MtxWorld._31 = mtxRot._31;
	m_MtxWorld._32 = mtxRot._32;
	m_MtxWorld._33 = mtxRot._33;

	// オフセット位置を反映
	m_MtxWorld._41 = m_Pos.x;
	m_MtxWorld._42 = m_Pos.y;
	m_MtxWorld._43 = m_Pos.z;
}

//=============================================================================
//    トランスフォーム情報をデバイスに設定する処理
//=============================================================================
void CDivisionWall::SetTransformToDevice(const LPDIRECT3DDEVICE9 pDevice)
{
	pDevice->SetTransform(D3DTS_WORLD, &m_MtxWorld);
}

//=============================================================================
//    頂点バッファ生成処理
//=============================================================================
void CDivisionWall::MakeVertex(const LPDIRECT3DDEVICE9 pDevice)
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
void CDivisionWall::SetScale(float fWidth, float fHeight)
{
	// 幅高さを保存
	m_fWidth = fWidth;
	m_fHeight = fHeight;

	// オフセットの長さと角度を求める
	m_fLength = sqrtf((m_fWidth * m_fWidth) + (m_fHeight * m_fHeight));
	m_fLength *= 0.5f;
	m_fAngle = atan2f(m_fWidth, m_fHeight);
}

//=============================================================================
//    頂点バッファへ値を設定する処理
//=============================================================================
void CDivisionWall::SetVtxBuffValue(void)
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
void CDivisionWall::SetVtxBuffPos(void)
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

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================================================
//    頂点バッファへ頂点カラーを設定する処理
//=============================================================================
void CDivisionWall::SetVtxBuffCol(void)
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
void CDivisionWall::SetVtxBuffTex(void)
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
