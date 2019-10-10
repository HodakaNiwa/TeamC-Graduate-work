//*****************************************************************************
//
//     当たり判定用球モデルの処理[sphereCollider.cpp]
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
#define SPHERECOLLIER_MESH_PRIORITY (5)                              // 可視化用メッシュスフィアの処理優先順位
#define SPHERECOLLIER_MESH_COL      (D3DXCOLOR(0.0f,0.0f,1.0f,0.5f)) // 可視化用メッシュスフィアの色
#define SPHERECOLLIER_MESH_XBLOCK   (10)                             // 可視化用メッシュスフィアの横の分割数
#define SPHERECOLLIER_MESH_YBLOCK   (10)                             // 可視化用メッシュスフィアの縦の分割数

//*****************************************************************************
//     静的メンバ変数
//*****************************************************************************

//=============================================================================
//    コンストラクタ
//=============================================================================
CSphereCollider::CSphereCollider(TYPE type, CScene *pParent) : CCollider(type, pParent)
{
	m_pColDispSphere = NULL;
	m_fRadius = 0.0f;
	m_bReturnFlag = false;
}

//=============================================================================
//    デストラクタ
//=============================================================================
CSphereCollider::~CSphereCollider()
{

}


//=============================================================================
//    生成処理
//=============================================================================
CSphereCollider *CSphereCollider::Create(D3DXVECTOR3 pos, D3DXVECTOR3 scale, float fRadius, bool bReturnFlag)
{
	CSphereCollider *pSphereCollider = NULL;   // 当たり判定用球モデルクラス型のポインタ
	pSphereCollider = new CSphereCollider;
	if (pSphereCollider == NULL) { return NULL; }

	// 各種値の設定
	pSphereCollider->SetPos(pos);                 // 球の座標
	pSphereCollider->SetScale(scale);             // 球の大きさ
	pSphereCollider->SetRadius(fRadius);          // 球の半径
	pSphereCollider->SetReturnFlag(bReturnFlag);  // 当たった際に押し戻すかどうか
	if (FAILED(pSphereCollider->Init()))
	{
		pSphereCollider->Uninit();
		pSphereCollider = NULL;
		return NULL;
	}

	return pSphereCollider;
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CSphereCollider::Init(void)
{
	// トランスフォーム情報を計算
	CalcNotScaleTransform();

#ifdef _DEBUG
	// 当たり判定可視化用メッシュスフィアを作成
	MakeColDispSphere();
#endif

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CSphereCollider::Uninit(void)
{
	// ポインタを空にしておく
	m_pColDispSphere = NULL;

	Release();	// 死亡フラグを立てる
}

//=============================================================================
//    更新処理
//=============================================================================
void CSphereCollider::Update(void)
{
	D3DXMATRIX mtxWorld = GetMtxWorld();
	SetPosOld(D3DXVECTOR3(mtxWorld._41, mtxWorld._42, mtxWorld._43));

	// トランスフォーム情報を計算
	CalcNotScaleTransform();
}

//=============================================================================
//    描画処理
//=============================================================================
void CSphereCollider::Draw(void)
{
#ifdef _DEBUG
	// 当たり判定可視化用メッシュスフィアを動かす
	if (m_pColDispSphere != NULL)
	{
		D3DXMATRIX mtxWorld = GetMtxWorld();
		m_pColDispSphere->SetPos(D3DXVECTOR3(mtxWorld._41, mtxWorld._42, mtxWorld._43));
		m_pColDispSphere->Disp();
	}
#endif
}

//=============================================================================
//    当たり判定処理
//=============================================================================
bool CSphereCollider::Collision(D3DXVECTOR3 *pPos, float fColRadius, CScene *pScene)
{
	// 自分の親と当たり判定はしない
	CScene *pParent = GetParent();
	if (pParent == pScene) { return false; }

	bool bHit = false;
	D3DXMATRIX mtxWorld = GetMtxWorld();
	D3DXVECTOR3 pos = D3DXVECTOR3(mtxWorld._41, mtxWorld._42, mtxWorld._43);

	// 判定する座標との距離を計算
	float fLength = CFunctionLib::Vec3Length(pPos, &pos);

	// 当たっていると判定される範囲を計算
	float fHitRange = m_fRadius + fColRadius;

	// 距離が当たったと判定される範囲より小さい
	if (fLength <= fHitRange)
	{
		if (m_bReturnFlag == true)
		{// 位置を戻すならば
		    // ベクトルで押し返す
			D3DXVECTOR3 VecA = pos - *pPos;
			D3DXVec3Normalize(&VecA, &VecA);
			pPos->x -= VecA.x * (fHitRange - fLength);
			pPos->y -= VecA.y * (fHitRange - fLength);
			pPos->z -= VecA.z * (fHitRange - fLength);
		}
		bHit = true;
	}

	return bHit;
}

//=============================================================================
//    当たり判定可視化用のメッシュスフィアを作成する処理
//=============================================================================
void CSphereCollider::MakeColDispSphere(void)
{
	D3DXVECTOR3 scale = GetScale();
	float fScale = (scale.x + scale.y + scale.z) / 3.0f;
	m_pColDispSphere = CColDispSphere::Create(GetPos(), SPHERECOLLIER_MESH_COL, m_fRadius / fScale,
		SPHERECOLLIER_MESH_XBLOCK, SPHERECOLLIER_MESH_YBLOCK, SPHERECOLLIER_MESH_PRIORITY);
}

//=============================================================================
//    球の半径を設定する処理
//=============================================================================
void CSphereCollider::SetRadius(const float fRadius)
{
	m_fRadius = fRadius;
}

//=============================================================================
//    当たった際に戻すかどうか設定処理
//=============================================================================
void CSphereCollider::SetReturnFlag(const bool bReturnFlag)
{
	m_bReturnFlag = bReturnFlag;
}

//=============================================================================
//    当たり判定可視化用メッシュクラスへのポインタを設定する処理
//=============================================================================
void CSphereCollider::SetColDispSphere(CColDispSphere *pColDispSphere)
{
	m_pColDispSphere = pColDispSphere;
}

//=============================================================================
//    球の半径を取得する処理
//=============================================================================
float CSphereCollider::GetRadius(void)
{
	return m_fRadius;
}

//=============================================================================
//    当たった際に戻すかどうか取得処理
//=============================================================================
bool CSphereCollider::GetReturnFlag(void)
{
	return m_bReturnFlag;
}

//=============================================================================
//    当たり判定可視化用メッシュクラスへのポインタを取得する処理
//=============================================================================
CColDispSphere *CSphereCollider::GetColDispSphere(void)
{
	return m_pColDispSphere;
}