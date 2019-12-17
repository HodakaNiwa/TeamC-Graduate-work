//*****************************************************************************
//
//     コリジョンの処理[collider.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "collision.h"

//*****************************************************************************
//    マクロ定義
//*****************************************************************************

//*****************************************************************************
//    静的メンバ変数
//*****************************************************************************
bool      CCollider::m_bDrawAll = true;                    // 全てのコライダーを描画するかどうか
CCollider *CCollider::m_apTop[CCollider::TYPE_MAX] = {};   // 先頭のコライダークラスへのポインタ
CCollider *CCollider::m_apCur[CCollider::TYPE_MAX] = {};   // 現在の最後尾ポインタ保存用

//=============================================================================
//    コンストラクタ
//=============================================================================
CCollider::CCollider(TYPE type, CScene *pParent)
{
	// 各種値のクリア
	m_pParent = pParent;                       // コライダーを所持しているオブジェクトへのポインタ
	m_pPrev = NULL;                            // 自身の前のアドレス
	m_pNext = NULL;                            // 自身の次のアドレス
	m_bDeath = false;                          // 死亡フラグ
	m_Type = type;                             // コライダーの種類
	m_bDeath = false;                          // 死亡フラグ
	m_Pos = INITIALIZE_VECTOR3;                // 現在の位置
	m_PosOld = INITIALIZE_VECTOR3;             // 前回の位置
	m_Rot = INITIALIZE_VECTOR3;                // 向き
	m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);   // 大きさ
	D3DXMatrixIdentity(&m_MtxWorld);           // ワールドマトリックス
	m_pParentMtxWorld = NULL;                  // 親のワールドマトリックス
	m_bDisp = true;                            // 描画するかどうか

	if (m_apTop[type] == NULL)
	{// 先頭アドレスが空である
		m_apTop[type] = this; // 自身のポインタを代入
	}
	if (m_apCur[type] != NULL)
	{// 最後尾アドレスが空ではない
		m_apCur[type]->m_pNext = this;
		this->m_pPrev = m_apCur[type];
	}
	m_apCur[type] = this;     // 最後尾アドレス更新
}

//=============================================================================
//    デストラクタ
//=============================================================================
CCollider::~CCollider()
{

}

//=============================================================================
//    コライダーの更新処理
//=============================================================================
void CCollider::UpdateAll(void)
{
	CCollider *pCollider;  // 更新処理の開始ポインタを宣言
	for (int nCntType = 0; nCntType < TYPE_MAX; nCntType++)
	{// コリジョンの種類の数だけ繰り返し
		pCollider = m_apTop[nCntType];  // 更新処理の開始ポインタを代入
		while (pCollider != NULL)
		{// メモリが空になるまでループ
			CCollider *pColliderNext = pCollider->m_pNext; // 自身の次のアドレスを保存(更新処理で自身のポインタが空になる可能性があるので)
			pCollider->Update();                           // 更新処理
			pCollider = pColliderNext;                     // 次の更新処理の自身の次のアドレスの描画処理を行う
		}
	}
}

//=============================================================================
//    当たり判定を可視化する
//=============================================================================
void CCollider::DrawAll(void)
{
	// 可視化しないなら処理終了
	if (m_bDrawAll == false) { return; }

	CCollider *pCollider;  // 描画処理の開始ポインタを宣言
	for (int nCntType = 0; nCntType < TYPE_MAX; nCntType++)
	{// コリジョンの種類の数だけ繰り返し
		pCollider = m_apTop[nCntType];  // 描画処理の開始ポインタを代入
		while (pCollider != NULL)
		{// メモリが空になるまでループ
			CCollider *pColliderNext = pCollider->m_pNext; // 自身の次のアドレスを保存(描画処理で自身のポインタが空になる可能性があるので)
			pCollider->Draw();                             // 描画処理
			pCollider = pColliderNext;                     // 次の描画処理の自身の次のアドレスの描画処理を行う
		}
	}
}

//=============================================================================
//    コライダーを全て開放する処理
//=============================================================================
void CCollider::ReleaseAll(void)
{
	CCollider *pCollider;  // 終了処理の開始ポインタを宣言
	for (int nCntType = 0; nCntType < TYPE_MAX; nCntType++)
	{// コリジョンの種類の数だけ繰り返し
		pCollider = m_apTop[nCntType];  // 終了処理の開始ポインタを代入
		while (pCollider != NULL)
		{// メモリが空になるまでループ
			CCollider *pColliderNext = pCollider->m_pNext; // 自身の次のアドレスを保存(描画処理で自身のポインタが空になる可能性があるので)
			pCollider->Uninit();                           // 終了処理
			pCollider = pColliderNext;                     // 次の終了処理の自身の次のアドレスの終了処理の行う
		}
	}

	// 死亡フラグチェック
	DeathCheck();

	// メモリを初期化しておく
	for (int nCntType = 0; nCntType < TYPE_MAX; nCntType++)
	{// コリジョンの種類の数だけ繰り返し
		m_apTop[nCntType] = NULL;   // 先頭アドレス
		m_apCur[nCntType] = NULL;   // 現在の最後尾アドレス
	}
}

//=============================================================================
//    死亡フラグを立てる処理
//=============================================================================
void CCollider::Release(void)
{
	if (m_bDeath == false)
	{// 死亡フラグがたっていない
		m_bDeath = true;  // 死亡フラグを立てる
	}
	else
	{// 死亡フラグがすでに立っている
		if (m_pPrev != NULL)
		{// 自身の前のアドレスが空ではない
			m_pPrev->m_pNext = m_pNext;
		}
		if (m_pNext != NULL)
		{// 自身の次のアドレスが空ではない
			m_pNext->m_pPrev = m_pPrev;
		}
		if (this == m_apTop[m_Type])
		{// 自身のアドレスが先頭アドレスだった
			m_apTop[m_Type] = m_pNext;
		}
		if (this == m_apCur[m_Type])
		{// 自身のアドレスが現在の最後尾アドレスだった
			m_apCur[m_Type] = m_pPrev;
		}
		// メモリの開放
		delete this;
	}
}

//=============================================================================
//    死亡フラグを確認する処理
//=============================================================================
void CCollider::DeathCheck(void)
{
	CCollider *pCollider;  // チェック開始ポインタ
	for (int nCntType = 0; nCntType < TYPE_MAX; nCntType++)
	{// 描画優先順位の数だけ繰り返し
		pCollider = m_apTop[nCntType]; // チェック開始ポインタを先頭にする
		while (pCollider != NULL)
		{// メモリが空になるまでループ
			CCollider *pColliderNext = pCollider->m_pNext; // 自身の次のアドレスを保存
			if (pCollider->m_bDeath == true)
			{// 死亡フラグがたっている
				pCollider->Release();                      // リストから削除する
			}
			pCollider = pColliderNext;                     // 次の死亡フラグチェックは自身の次のアドレスのチェックを行う
		}
	}
}

//=============================================================================
//    当たり判定可視化ON/OFFを切り替える
//=============================================================================
void CCollider::SwicthDraw(void)
{
	m_bDrawAll = m_bDrawAll ? false : true;
}

//=============================================================================
//    当たり判定を可視化するかどうかを取得する
//=============================================================================
bool CCollider::GetDrawAll(void)
{
	return m_bDrawAll;
}

//=============================================================================
//    先頭のコライダークラスへのポインタ取得
//=============================================================================
CCollider *CCollider::GetTop(int nType)
{
	return m_apTop[nType];
}

//=============================================================================
//    ワールドマトリックスを計算する(スケールは反映しない)処理
//=============================================================================
void CCollider::CalcNotScaleTransform(void)
{
	D3DXMATRIX mtxRot, mtxParent; // 計算用マトリックス

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

	// 親がいれば情報を反映
	if (m_pParentMtxWorld != NULL)
	{
		mtxParent._11 = m_pParentMtxWorld->_11;
		mtxParent._12 = m_pParentMtxWorld->_12;
		mtxParent._13 = m_pParentMtxWorld->_13;
		mtxParent._14 = m_pParentMtxWorld->_14;
		mtxParent._21 = m_pParentMtxWorld->_21;
		mtxParent._22 = m_pParentMtxWorld->_22;
		mtxParent._23 = m_pParentMtxWorld->_23;
		mtxParent._24 = m_pParentMtxWorld->_24;
		mtxParent._31 = m_pParentMtxWorld->_31;
		mtxParent._32 = m_pParentMtxWorld->_32;
		mtxParent._33 = m_pParentMtxWorld->_33;
		mtxParent._34 = m_pParentMtxWorld->_34;
		mtxParent._41 = m_pParentMtxWorld->_41;
		mtxParent._42 = m_pParentMtxWorld->_42;
		mtxParent._43 = m_pParentMtxWorld->_43;
		mtxParent._44 = m_pParentMtxWorld->_44;
		D3DXMatrixMultiply(&m_MtxWorld, &m_MtxWorld, &mtxParent);
	}
}

//=============================================================================
//    トランスフォーム情報をデバイスに設定する
//=============================================================================
void CCollider::SetTransformToDevice(const LPDIRECT3DDEVICE9 pDevice)
{
	pDevice->SetTransform(D3DTS_WORLD, &m_MtxWorld);
}


//=============================================================================
//    コライダーを所持しているオブジェクトへのポインタを設定する
//=============================================================================
void CCollider::SetParent(CScene *pParent)
{
	m_pParent = pParent;
}

//=============================================================================
//    現在の位置を設定
//=============================================================================
void CCollider::SetPos(const D3DXVECTOR3 pos)
{
	m_Pos = pos;
}

//=============================================================================
//    前回の位置を設定
//=============================================================================
void CCollider::SetPosOld(const D3DXVECTOR3 posOld)
{
	m_PosOld = posOld;
}

//=============================================================================
//    向きを設定
//=============================================================================
void CCollider::SetRot(const D3DXVECTOR3 rot)
{
	m_Rot = rot;
}

//=============================================================================
//    大きさを設定
//=============================================================================
void CCollider::SetScale(const D3DXVECTOR3 scale)
{
	m_Scale = scale;
}

//=============================================================================
//    ワールドマトリックスを設定
//=============================================================================
void CCollider::SetMtxWorld(const D3DXMATRIX mtxWorld)
{
	m_MtxWorld = mtxWorld;
}

//=============================================================================
//    親のワールドマトリックスへのポインタを設定
//=============================================================================
void CCollider::SetParentMtxWorld(D3DXMATRIX *pParentMtxWorld)
{
	m_pParentMtxWorld = pParentMtxWorld;
}

//=============================================================================
//    当たり判定を可視化するかどうか設定
//=============================================================================
void CCollider::SetDisp(const bool bDisp)
{
	m_bDisp = bDisp;
}

//=============================================================================
//    コライダーを所持しているオブジェクトへのポインタを取得
//=============================================================================
CScene *CCollider::GetParent(void)
{
	return m_pParent;
}

//=============================================================================
//    次のアドレスを取得
//=============================================================================
CCollider *CCollider::GetNext(void)
{
	return m_pNext;
}

//=============================================================================
//    前のアドレスを取得
//=============================================================================
CCollider *CCollider::GetPrev(void)
{
	return m_pPrev;
}

//=============================================================================
//    種類を取得
//=============================================================================
CCollider::TYPE CCollider::GetType(void)
{
	return m_Type;
}

//=============================================================================
//    現在の位置を取得
//=============================================================================
D3DXVECTOR3 CCollider::GetPos(void)
{
	return m_Pos;
}

//=============================================================================
//    前回の位置を取得
//=============================================================================
D3DXVECTOR3 CCollider::GetPosOld(void)
{
	return m_PosOld;
}

//=============================================================================
//    向きを取得
//=============================================================================
D3DXVECTOR3 CCollider::GetRot(void)
{
	return m_Rot;
}

//=============================================================================
//    大きさを取得
//=============================================================================
D3DXVECTOR3 CCollider::GetScale(void)
{
	return m_Scale;
}

//=============================================================================
//    ワールドマトリックスを取得
//=============================================================================
D3DXMATRIX CCollider::GetMtxWorld(void)
{
	return m_MtxWorld;
}

//=============================================================================
//    親のワールドマトリックスへのポインタを取得
//=============================================================================
D3DXMATRIX *CCollider::GetParentMtxWorld(void)
{
	return m_pParentMtxWorld;
}

//=============================================================================
//    当たり判定を可視化するかどうかを取得
//=============================================================================
bool CCollider::GetDisp(void)
{
	return m_bDisp;
}