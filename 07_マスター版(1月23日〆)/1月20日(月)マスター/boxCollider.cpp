//*****************************************************************************
//
//     当たり判定用箱モデルの処理[boxCollider.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "collision.h"
#include "manager.h"
#include "renderer.h"
#include "library.h"

//*****************************************************************************
//     マクロ定義
//*****************************************************************************
#define BOXCOLLIDER_LINE_COL (D3DXCOLOR(1.0f,0.0f,1.0f,1.0f))

//*****************************************************************************
//     静的メンバ変数
//*****************************************************************************

//=============================================================================
//    コンストラクタ
//=============================================================================
CBoxCollider::CBoxCollider(TYPE type, CScene *pParent) : CCollider(type, pParent)
{
	// 各種値のクリア
	m_pVtxBuff = NULL;      // 頂点バッファへのポインタ
	m_pIdxBuff = NULL;      // インデックスバッファへのポインタ
	m_fWidth = 0.0f;        // 箱の横幅
	m_fHeight = 0.0f;       // 箱の高さ
	m_fDepth = 0.0f;        // 箱の奥行
	m_bReturnFlag = false;  // 当たった際に戻すかどうか
}

//=============================================================================
//    デストラクタ
//=============================================================================
CBoxCollider::~CBoxCollider()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CBoxCollider *CBoxCollider::Create(D3DXVECTOR3 pos, D3DXVECTOR3 scale, float fWidth, float fHeight, float fDepth, bool bReturnFlag)
{
	CBoxCollider *pBoxCollider = NULL;   // 当たり判定用箱モデルクラス型のポインタ
	pBoxCollider = new CBoxCollider;
	if (pBoxCollider == NULL) { return NULL; }

	// 各種値の設定
	pBoxCollider->SetPos(pos);                 // 座標
	pBoxCollider->SetScale(scale);             // 大きさ
	pBoxCollider->SetWidth(fWidth);            // 箱の横幅
	pBoxCollider->SetHeight(fHeight);          // 箱の高さ
	pBoxCollider->SetDepth(fDepth);            // 箱の奥行
	pBoxCollider->SetReturnFlag(bReturnFlag);  // 当たった際に戻すかどうか
	if (FAILED(pBoxCollider->Init()))
	{
		pBoxCollider->Uninit();
		pBoxCollider = NULL;
		return NULL;
	}

	return pBoxCollider;
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CBoxCollider::Init(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	if (pDevice == NULL) { return E_FAIL; }

#ifdef _DEBUG
	// 頂点バッファの生成
	MakeVertex(pDevice);

	// インデックスバッファの生成
	MakeIndex(pDevice);
#endif

	// トランスフォーム情報を計算
	CalcNotScaleTransform();

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CBoxCollider::Uninit(void)
{
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	if (m_pIdxBuff != NULL)
	{
		m_pIdxBuff->Release();
		m_pIdxBuff = NULL;
	}

	Release();	// 死亡フラグを立てる
}

//=============================================================================
//    更新処理
//=============================================================================
void CBoxCollider::Update(void)
{
	D3DXMATRIX mtxWorld = GetMtxWorld();
	SetPosOld(D3DXVECTOR3(mtxWorld._41, mtxWorld._42, mtxWorld._43));

	// トランスフォーム情報を計算
	CalcNotScaleTransform();
}

//=============================================================================
//    描画処理
//=============================================================================
void CBoxCollider::Draw(void)
{
#ifdef _DEBUG
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
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
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(BOX_VERTEX));

	// インデックスバッファをデバイスのデータストリームにバインド
	pDevice->SetIndices(m_pIdxBuff);

	// 頂点フォーマットの設定
	pDevice->SetFVF((D3DFVF_XYZ | D3DFVF_DIFFUSE));

	// テクスチャの設定
	pDevice->SetTexture(0, NULL);

	// 線の描画
	pDevice->DrawIndexedPrimitive(D3DPT_LINELIST, 0, 0, 8, 0, 12);

	// カリングの設定を戻す
	pDevice->SetRenderState(D3DRS_CULLMODE, Culling);

	// ライティングの設定を戻す
	pDevice->SetRenderState(D3DRS_LIGHTING, Lighting);
#endif
}

//=============================================================================
//    頂点情報生成処理
//=============================================================================
void CBoxCollider::MakeVertex(const LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(BOX_VERTEX) * 8,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// 頂点情報の設定
	BOX_VERTEX *pVtx;

	// 頂点バッファをロックし,頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標を設定(順番は 上側の 左奥 -> 右奥 -> -> 左前 -> 右奥
	//                       下側の 左奥 -> 右奥 -> -> 左前 -> 右奥)
	D3DXVECTOR3 scale = GetScale();
	float fCutWidth = -m_fWidth / 2.0f / scale.x;
	float fAddWidth = m_fWidth / 2.0f / scale.x;
	float fCutDepth = -m_fDepth / 2.0f / scale.z;
	float fAddDepth = m_fDepth / 2.0f / scale.z;
	float fHeight = m_fHeight / scale.y;
	pVtx[0].pos = D3DXVECTOR3(fCutWidth, fHeight, fAddDepth);
	pVtx[1].pos = D3DXVECTOR3(fAddWidth, fHeight, fAddDepth);
	pVtx[2].pos = D3DXVECTOR3(fCutWidth, fHeight, fCutDepth);
	pVtx[3].pos = D3DXVECTOR3(fAddWidth, fHeight, fCutDepth);
	pVtx[4].pos = D3DXVECTOR3(fCutWidth, 0.0f, fAddDepth);
	pVtx[5].pos = D3DXVECTOR3(fAddWidth, 0.0f, fAddDepth);
	pVtx[6].pos = D3DXVECTOR3(fCutWidth, 0.0f, fCutDepth);
	pVtx[7].pos = D3DXVECTOR3(fAddWidth, 0.0f, fCutDepth);

	// 頂点カラーを設定(全て赤色に設定)
	for (int nCntVer = 0; nCntVer < 8; nCntVer++)
	{
		pVtx[nCntVer].col = BOXCOLLIDER_LINE_COL;
	}

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}


//=============================================================================
//    インデックス情報生成処理
//=============================================================================
void CBoxCollider::MakeIndex(const LPDIRECT3DDEVICE9 pDevice)
{
	// インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * 24,
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
	pIdx[0] = 0;
	pIdx[1] = 1;
	pIdx[2] = 1;
	pIdx[3] = 3;
	pIdx[4] = 3;
	pIdx[5] = 2;
	pIdx[6] = 2;
	pIdx[7] = 0;
	pIdx[8] = 0;
	pIdx[9] = 4;
	pIdx[10] = 1;
	pIdx[11] = 5;
	pIdx[12] = 2;
	pIdx[13] = 6;
	pIdx[14] = 3;
	pIdx[15] = 7;
	pIdx[16] = 4;
	pIdx[17] = 5;
	pIdx[18] = 5;
	pIdx[19] = 7;
	pIdx[20] = 7;
	pIdx[21] = 6;
	pIdx[22] = 6;
	pIdx[23] = 4;

	// インデックスバッファをアンロックする
	m_pIdxBuff->Unlock();
}

//=============================================================================
//    当たり判定処理
//=============================================================================
bool CBoxCollider::Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 ColRange, CScene *pScene, bool *pLand)
{
	// 自分の親と当たり判定はしない
	CScene *pParent = GetParent();
	if (pParent == pScene) { return false; }

	bool bHit = false;  // 当たったかどうか
	D3DXMATRIX mtxWorld = GetMtxWorld();
	D3DXVECTOR3 pos = D3DXVECTOR3(mtxWorld._41, mtxWorld._42, mtxWorld._43);
	D3DXVECTOR3 posOld = GetPosOld();
	D3DXVECTOR3 Move = pos - posOld;
	float fCutWidth = -m_fWidth / 2.0f;
	float fAddWidth = m_fWidth / 2.0f;
	float fCutDepth = -m_fDepth / 2.0f;
	float fAddDepth = m_fDepth / 2.0f;

	// Y軸上の判定
	if (pPos->y < pos.y + m_fHeight && pPos->y + ColRange.y > pos.y)
	{// 現在の高さが箱の中にいる
	 // X軸上の判定
		if (pPos->z + ColRange.z > pos.z + fCutDepth && pPos->z - ColRange.z < pos.z + fAddDepth)
		{// Z軸上にいるかどうか判定
			if (pPosOld->x + ColRange.x <= pos.x + fCutWidth  && pPos->x + ColRange.x > pos.x + fCutWidth
				|| pPosOld->x + ColRange.x <= posOld.x + fCutWidth && pPos->x + ColRange.x >= posOld.x + fCutWidth
				|| Move.x < 0.0f && posOld.x + fCutWidth >= pPos->x + ColRange.x && pos.x + fCutWidth < pPos->x + ColRange.x)
			{// 箱の左側から入った
				if (m_bReturnFlag == true)
				{// 位置を戻すならば
					pPos->x = pos.x - ColRange.x + fCutWidth - 0.1f;
					if (pMove->y > 0.0f)
					{
						D3DXVECTOR3 Vec = *pPos - *pPosOld;
						D3DXVECTOR3 Reflect;
						CFunctionLib::CalcReflectVector3(&Reflect, &Vec, &D3DXVECTOR3(-1.0f, 0.0f, 0.0f));
						D3DXVec3Normalize(&Reflect, &Reflect);
						pMove->y *= Reflect.y;
					}
				}
				bHit = true;
			}
			else if (pPosOld->x - ColRange.x >= pos.x + fAddWidth && pPos->x - ColRange.x < pos.x + fAddWidth
				|| pPosOld->x - ColRange.x >= posOld.x + fAddWidth && pPos->x - ColRange.x <= posOld.x + fAddWidth
				|| Move.x > 0.0f && posOld.x + fAddWidth <= pPos->x - ColRange.x && pos.x + fAddWidth > pPos->x - ColRange.x)
			{// 箱の右側から入った
				if (m_bReturnFlag == true)
				{// 位置を戻すならば
					pPos->x = pos.x + fAddWidth + ColRange.x + 0.1f;
					if (pMove->y > 0.0f)
					{
						D3DXVECTOR3 Vec = *pPos - *pPosOld;
						D3DXVECTOR3 Reflect;
						CFunctionLib::CalcReflectVector3(&Reflect, &Vec, &D3DXVECTOR3(1.0f, 0.0f, 0.0f));
						D3DXVec3Normalize(&Reflect, &Reflect);
						pMove->y *= Reflect.y;
					}
				}
				bHit = true;
			}
		}
		// Z軸上の判定
		if (pPos->x + ColRange.x > pos.x + fCutWidth && pPos->x - ColRange.x < pos.x + fAddWidth)
		{// X軸上にいるかどうか判定
			if (pPosOld->z - ColRange.z >= pos.z + fAddDepth && pPos->z - ColRange.z < pos.z + fAddDepth
				|| pPosOld->z - ColRange.z >= posOld.z + fAddDepth && pPos->z - ColRange.z < posOld.z + fAddDepth
				|| Move.z > 0.0f && posOld.z + fAddDepth <= pPos->z - ColRange.z && pos.z + fAddDepth > pPos->z - ColRange.z)
			{// 箱の奥側から入った
				if (m_bReturnFlag == true)
				{// 位置を戻すならば
					pPos->z = pos.z + fAddDepth + ColRange.z + 0.1f;
					if (pMove->y > 0.0f)
					{
						D3DXVECTOR3 Vec = *pPos - *pPosOld;
						D3DXVECTOR3 Reflect;
						CFunctionLib::CalcReflectVector3(&Reflect, &Vec, &D3DXVECTOR3(0.0f, 0.0f, 1.0f));
						D3DXVec3Normalize(&Reflect, &Reflect);
						pMove->y *= Reflect.y;
					}
				}
				bHit = true;
			}
			else if (pPosOld->z + ColRange.z <= pos.z + fCutDepth && pPos->z + ColRange.z > pos.z + fCutDepth
				|| pPosOld->z + ColRange.z <= posOld.z + fCutDepth && pPos->z + ColRange.z > posOld.z + fCutDepth
				|| Move.z < 0.0f && posOld.z + fCutDepth >= pPos->z + ColRange.z && pos.z + fCutDepth < pPos->z + ColRange.z)
			{// 箱の手前側から入った
				if (m_bReturnFlag == true)
				{// 位置を戻すならば
					pPos->z = pos.z - ColRange.z + fCutDepth - 0.1f;
					if (pMove->y > 0.0f)
					{
						D3DXVECTOR3 Vec = *pPos - *pPosOld;
						D3DXVECTOR3 Reflect;
						CFunctionLib::CalcReflectVector3(&Reflect, &Vec, &D3DXVECTOR3(0.0f, 0.0f, -1.0f));
						D3DXVec3Normalize(&Reflect, &Reflect);
						pMove->y *= Reflect.y;
					}
				}
				bHit = true;
			}
		}
	}

	if (pPosOld->y >= pos.y + m_fHeight && pPos->y <= pos.y + m_fHeight
		 || pPosOld->y >= posOld.y + m_fHeight && pPos->y <= posOld.y + m_fHeight)
	{// 高さが上から箱の内側に入ってしまった
		if (pPos->x + ColRange.x > pos.x + fCutWidth && pPos->x - ColRange.x < pos.x + fAddWidth
			&& pPos->z + ColRange.z > pos.z + fCutDepth && pPos->z - ColRange.z < pos.z + fAddDepth)
		{// 箱の内側にいる判定だった
			if (m_bReturnFlag == true)
			{// 位置を戻すならば
				pPos->x += Move.x;
				pPos->z += Move.z;
				pPos->y = pos.y + m_fHeight;
				pMove->y = 0.0f;
			}
			bHit = true;
			if (pLand != NULL)
			{
				*pLand = true;
			}
		}
	}
	else if (pPosOld->y + ColRange.y <= pos.y && pPos->y + ColRange.y > pos.y
		|| pPosOld->y >= posOld.y + m_fHeight && pPos->y < posOld.y + m_fHeight)
	{// 高さが下から箱の内側に入ってしまった
		if (pPos->x + ColRange.x > pos.x + fCutWidth && pPos->x - ColRange.x < pos.x + fAddWidth
			&& pPos->z + ColRange.z > pos.z + fCutDepth && pPos->z - ColRange.z < pos.z + fAddDepth)
		{// 箱の内側にいる判定だった
			if (m_bReturnFlag == true)
			{// 位置を戻すならば
				pPos->y = pos.y - ColRange.y;
				pMove->y = 0.0f;
			}
			bHit = true;
		}
	}

	return bHit;
}

//=============================================================================
//    箱の横幅設定処理
//=============================================================================
void CBoxCollider::SetWidth(const float fWidth)
{
	m_fWidth = fWidth;
}

//=============================================================================
//    箱の高さ設定処理
//=============================================================================
void CBoxCollider::SetHeight(const float fHeight)
{
	m_fHeight = fHeight;
}

//=============================================================================
//    箱の奥行設定処理
//=============================================================================
void CBoxCollider::SetDepth(const float fDepth)
{
	m_fDepth = fDepth;
}

//=============================================================================
//    当たった際に戻すかどうか設定処理
//=============================================================================
void CBoxCollider::SetReturnFlag(const bool bReturnFlag)
{
	m_bReturnFlag = bReturnFlag;
}

//=============================================================================
//    箱の横幅取得処理
//=============================================================================
float CBoxCollider::GetWidth(void)
{
	return m_fWidth;
}

//=============================================================================
//    箱の高さ取得処理
//=============================================================================
float CBoxCollider::GetHeight(void)
{
	return m_fHeight;
}

//=============================================================================
//    箱の奥行取得処理
//=============================================================================
float CBoxCollider::GetDepth(void)
{
	return m_fDepth;
}

//=============================================================================
//    当たった際に戻すかどうか取得処理
//=============================================================================
bool CBoxCollider::GetReturnFlag(void)
{
	return m_bReturnFlag;
}