//*****************************************************************************
//
//     当たり判定用筒モデルの処理[boxCollider.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "collision.h"
#include "manager.h"
#include "system.h"
#include "library.h"

//*****************************************************************************
//     マクロ定義
//*****************************************************************************
#define CYLINDERCOLLIDER_LINE_COL (D3DXCOLOR(1.0f,0.0f,1.0f,1.0f))

//*****************************************************************************
//     静的メンバ変数
//*****************************************************************************

//=============================================================================
//    コンストラクタ
//=============================================================================
CCylinderCollider::CCylinderCollider(TYPE type, CScene *pParent) : CCollider(type, pParent)
{
	// 各種値のクリア
	m_pVtxBuff = NULL;      // 頂点バッファへのポインタ
	m_pIdxBuff = NULL;      // インデックスバッファへのポインタ
	m_fRadius = 0.0f;       // 筒の半径
	m_fHeight = 0.0f;       // 筒の高さ
	m_bReturnFlag = false;  // 当たった際に戻すかどうか
}

//=============================================================================
//    デストラクタ
//=============================================================================
CCylinderCollider::~CCylinderCollider()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CCylinderCollider *CCylinderCollider::Create(D3DXVECTOR3 pos, D3DXVECTOR3 scale, float fRadius, float fHeight, bool bReturnFlag, CScene *pParent)
{
	CCylinderCollider *pCylinderCollider = NULL;   // 当たり判定用筒モデルクラス型のポインタ
	pCylinderCollider = new CCylinderCollider;
	if (pCylinderCollider == NULL) { return NULL; }

	// 各種値の設定
	pCylinderCollider->SetPos(pos);                 // 座標
	pCylinderCollider->SetScale(scale);             // 大きさ
	pCylinderCollider->SetRadius(fRadius);          // 筒の横幅
	pCylinderCollider->SetHeight(fHeight);          // 筒の高さ
	pCylinderCollider->SetReturnFlag(bReturnFlag);  // 当たった際に戻すかどうか
	if (FAILED(pCylinderCollider->Init()))
	{
		pCylinderCollider->Uninit();
		pCylinderCollider = NULL;
		return NULL;
	}

	return pCylinderCollider;
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CCylinderCollider::Init(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetDevice();
	if (pDevice == NULL) { return E_FAIL; }

	// 頂点バッファの生成
	MakeVertex(pDevice);

	// インデックスバッファの生成
	MakeIndex(pDevice);

	// トランスフォーム情報を計算
	CalcNotScaleTransform();

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CCylinderCollider::Uninit(void)
{
	Release();	// 死亡フラグを立てる
}

//=============================================================================
//    更新処理
//=============================================================================
void CCylinderCollider::Update(void)
{
	D3DXMATRIX mtxWorld = GetMtxWorld();
	SetPosOld(D3DXVECTOR3(mtxWorld._41, mtxWorld._42, mtxWorld._43));

	// トランスフォーム情報を計算
	CalcNotScaleTransform();
}

//=============================================================================
//    描画処理
//=============================================================================
void CCylinderCollider::Draw(void)
{
#ifdef _DEBUG
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetDevice();
	if (pDevice == NULL) { return; }

	DWORD Culling;
	DWORD Lighting;

	// ライティングしない設定に
	pDevice->GetRenderState(D3DRS_LIGHTING, &Lighting);
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// カリングを外しておく
	pDevice->GetRenderState(D3DRS_CULLMODE, &Culling);
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	// トランスフォーム情報をデバイスに設定
	SetTransformToDevice(pDevice);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(CYLINDER_VERTEX));

	// インデックスバッファをデバイスのデータストリームにバインド
	pDevice->SetIndices(m_pIdxBuff);

	// 頂点フォーマットの設定
	pDevice->SetFVF((D3DFVF_XYZ | D3DFVF_DIFFUSE));

	// テクスチャの設定
	pDevice->SetTexture(0, NULL);

	// 線の描画
	pDevice->DrawIndexedPrimitive(D3DPT_LINELIST, 0, 0, 18, 0, 40);
	//pDevice->DrawIndexedPrimitiveUP(D3DPT_LINELIST, 0, 18, 40, m_wIndex, D3DFMT_INDEX16, m_Vertex, sizeof(CYLINDER_VERTEX));

	// カリングの設定を戻す
	pDevice->SetRenderState(D3DRS_CULLMODE, Culling);

	// ライティングの設定を戻す
	pDevice->SetRenderState(D3DRS_LIGHTING, Lighting);
#endif
}

//=============================================================================
//    頂点情報生成処理
//=============================================================================
void CCylinderCollider::MakeVertex(const LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(CYLINDER_VERTEX) * 18,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// 頂点情報の設定
	CYLINDER_VERTEX *pVtx;

	// 頂点バッファをロックし,頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標を設定(順番は上側 -> 下側)
	D3DXVECTOR3 scale = GetScale();
	float fScale = (scale.x + scale.z) / 2.0f;
	float fRadius = m_fRadius / fScale;
	float fPosY = m_fHeight / scale.y;
	float fCurve = 0.0f;
	for (int nCntUp = 0; nCntUp < 2; nCntUp++)
	{
		for (int nCntDown = 0; nCntDown < 8; nCntDown++)
		{
			pVtx[nCntDown + (nCntUp * 8)].pos = D3DXVECTOR3(sinf(fCurve) * -fRadius, fPosY, cosf(fCurve) * -fRadius);
			fCurve += D3DX_PI / 4;  // 角度を進める
			if (fCurve >= D3DX_PI)
			{// 倍率が円周率を超えた
				fCurve -= D3DX_PI * 2.0f;
			}
		}
		fPosY = 0.0f;
		fCurve = 0.0f;
	}
	pVtx[16].pos = D3DXVECTOR3(0.0f, m_fHeight / scale.y, 0.0f);
	pVtx[17].pos = INITIALIZE_VECTOR3;

	// 頂点カラーを設定(全て赤色に設定)
	for (int nCntVer = 0; nCntVer < 18; nCntVer++)
	{
		pVtx[nCntVer].col = CYLINDERCOLLIDER_LINE_COL;
	}
}

//=============================================================================
//    インデックス情報生成処理
//=============================================================================
void CCylinderCollider::MakeIndex(const LPDIRECT3DDEVICE9 pDevice)
{
	// インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * 80,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIdxBuff,
		NULL);

	WORD *pIdx;       // インデックスデータへのポインタ
	int nCntIdx = 0;  // インデックス番号

	// インデックスバッファをロックし,インデックスデータへのポインタを取得
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	// インデックス情報を設定
	// 上の囲い
	pIdx[0] = 0;
	pIdx[1] = 1;
	pIdx[2] = 1;
	pIdx[3] = 2;
	pIdx[4] = 2;
	pIdx[5] = 3;
	pIdx[6] = 3;
	pIdx[7] = 4;
	pIdx[8] = 4;
	pIdx[9] = 5;
	pIdx[10] = 5;
	pIdx[11] = 6;
	pIdx[12] = 6;
	pIdx[13] = 7;
	pIdx[14] = 7;
	pIdx[15] = 0;

	// 上->下のライン
	pIdx[16] = 0;
	pIdx[17] = 8;
	pIdx[18] = 1;
	pIdx[19] = 9;
	pIdx[20] = 2;
	pIdx[21] = 10;
	pIdx[22] = 3;
	pIdx[23] = 11;
	pIdx[24] = 4;
	pIdx[25] = 12;
	pIdx[26] = 5;
	pIdx[27] = 13;
	pIdx[28] = 6;
	pIdx[29] = 14;
	pIdx[30] = 7;
	pIdx[31] = 15;

	// 下の囲い
	pIdx[32] = 8;
	pIdx[33] = 9;
	pIdx[34] = 9;
	pIdx[35] = 10;
	pIdx[36] = 10;
	pIdx[37] = 11;
	pIdx[38] = 11;
	pIdx[39] = 12;
	pIdx[40] = 12;
	pIdx[41] = 13;
	pIdx[42] = 13;
	pIdx[43] = 14;
	pIdx[44] = 14;
	pIdx[45] = 15;
	pIdx[46] = 15;
	pIdx[47] = 8;

	// 上の蓋
	pIdx[48] = 16;
	pIdx[49] = 0;
	pIdx[50] = 16;
	pIdx[51] = 1;
	pIdx[52] = 16;
	pIdx[53] = 2;
	pIdx[54] = 16;
	pIdx[55] = 3;
	pIdx[56] = 16;
	pIdx[57] = 4;
	pIdx[58] = 16;
	pIdx[59] = 5;
	pIdx[60] = 16;
	pIdx[61] = 6;
	pIdx[62] = 16;
	pIdx[63] = 7;

	// 下の蓋
	pIdx[64] = 17;
	pIdx[65] = 8;
	pIdx[66] = 17;
	pIdx[67] = 9;
	pIdx[68] = 17;
	pIdx[69] = 10;
	pIdx[70] = 17;
	pIdx[71] = 11;
	pIdx[72] = 17;
	pIdx[73] = 12;
	pIdx[74] = 17;
	pIdx[75] = 13;
	pIdx[76] = 17;
	pIdx[77] = 14;
	pIdx[78] = 17;
	pIdx[79] = 15;

	// インデックスバッファをアンロックする
	m_pIdxBuff->Unlock();
}

//=============================================================================
//    当たり判定処理
//=============================================================================
bool CCylinderCollider::Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fColRadius, float fHeight, CScene *pScene, bool *pLand)
{
	// 自分の親と当たり判定はしない
	CScene *pParent = GetParent();
	if (pParent == pScene) { return false; }

	bool bHit = false;

	// 載っているかチェック
	bHit = LandCollision(pPos, pPosOld, pMove, fColRadius, fHeight, pScene, pLand);
	if (bHit == true) { return bHit; }

	// 範囲内チェック
	bHit = RangeCollision(pPos, pPosOld, pMove, fColRadius, fHeight, pScene, pLand);

	return bHit;
}

//=============================================================================
//    載ってるかどうかをチェックする処理
//=============================================================================
bool CCylinderCollider::LandCollision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fColRadius, float fHeight, CScene *pScene, bool *pLand)
{
	bool bHit = false;
	D3DXMATRIX mtxWorld = GetMtxWorld();
	D3DXVECTOR3 pos = D3DXVECTOR3(mtxWorld._41, mtxWorld._42, mtxWorld._43);
	D3DXVECTOR3 posOld = GetPosOld();
	D3DXVECTOR3 Move = pos - posOld;
	float fLength = CFunctionLib::Vec2Length(pos.x, pos.z, pPos->x, pPos->z);
	float fHitRange = m_fRadius + fColRadius;

	if (pPosOld->y >= pos.y + m_fHeight && pPos->y <= pos.y + m_fHeight
		|| pPosOld->y >= posOld.y + m_fHeight && pPos->y <= posOld.y + m_fHeight)
	{// 高さが上から箱の内側に入ってしまった
		if (fLength <= fHitRange)
		{// 箱の内側にいる判定だった
			if (m_bReturnFlag == true)
			{// 位置を戻すならば
				pPos->x += Move.x;
				pPos->z += Move.z;
				pPos->y = pos.y + m_fHeight;
				pMove->y = 0.0f;
				CDebugProc::Print("筒に乗っています\n");
			}
			bHit = true;
			if (pLand != NULL)
			{
				*pLand = true;
			}
		}
	}
	else if (pPosOld->y + fHeight <= pos.y && pPos->y + fHeight > pos.y
		|| pPosOld->y >= posOld.y && pPos->y < posOld.y)
	{// 高さが下から箱の内側に入ってしまった
		if (fLength <= fHitRange)
		{// 箱の内側にいる判定だった
			if (m_bReturnFlag == true)
			{// 位置を戻すならば
				pPos->y = pos.y - fHeight;
				pMove->y = 0.0f;
			}
			bHit = true;
		}
	}

	return bHit;
}

//=============================================================================
//    範囲内チェック処理
//=============================================================================
bool CCylinderCollider::RangeCollision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fColRadius, float fHeight, CScene *pScene, bool *pLand)
{
	// 自分の親と当たり判定はしない
	CScene *pParent = GetParent();
	if (pParent == pScene) { return false; }

	bool bHit = false;
	D3DXMATRIX mtxWorld = GetMtxWorld();
	D3DXVECTOR3 pos = D3DXVECTOR3(mtxWorld._41, mtxWorld._42, mtxWorld._43);

	// 判定する座標との距離を計算
	float fLength = CFunctionLib::Vec2Length(pPos->x, pPos->z, pos.x, pos.z);

	// 当たっていると判定される範囲を計算
	float fHitRange = m_fRadius + fColRadius;

	// 距離が当たったと判定される範囲より小さい
	if (fLength < fHitRange && pPos->y <= pos.y + m_fHeight && pPos->y + fHeight >= pos.y)
	{
		if (m_bReturnFlag == true)
		{// 位置を戻すならば
		    // ベクトルで押し返す
			D3DXVECTOR3 VecA = pos - *pPos;
			D3DXVec3Normalize(&VecA, &VecA);
			pPos->x -= VecA.x * (fHitRange - fLength);
			pPos->z -= VecA.z * (fHitRange - fLength);
		}
		bHit = true;
	}

	return bHit;
}

//=============================================================================
//    筒の半径設定処理
//=============================================================================
void CCylinderCollider::SetRadius(const float fRadius)
{
	m_fRadius = fRadius;
}

//=============================================================================
//    筒の高さ設定処理
//=============================================================================
void CCylinderCollider::SetHeight(const float fHeight)
{
	m_fHeight = fHeight;
}

//=============================================================================
//    当たった際に戻すかどうか設定処理
//=============================================================================
void CCylinderCollider::SetReturnFlag(const bool bReturnFlag)
{
	m_bReturnFlag = bReturnFlag;
}

//=============================================================================
//    筒の半径取得処理
//=============================================================================
float CCylinderCollider::GetRadius(void)
{
	return m_fRadius;
}

//=============================================================================
//    筒の高さ取得処理
//=============================================================================
float CCylinderCollider::GetHeight(void)
{
	return m_fHeight;
}

//=============================================================================
//    当たった際に戻すかどうか取得処理
//=============================================================================
bool CCylinderCollider::GetReturnFlag(void)
{
	return m_bReturnFlag;
}