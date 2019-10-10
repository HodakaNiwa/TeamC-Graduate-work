//*****************************************************************************
//
//     プレイヤーの攻撃球モデルの処理[playerAttackSphereCollider.cpp]
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
#define PLAYERATTACKSPHERECOLLIER_MESH_PRIORITY (5)                              // 可視化用メッシュスフィアの処理優先順位
#define PLAYERATTACKSPHERECOLLIER_MESH_COL      (D3DXCOLOR(1.0f,1.0f,0.0f,0.5f)) // 可視化用メッシュスフィアの色
#define PLAYERATTACKSPHERECOLLIER_MESH_XBLOCK   (10)                             // 可視化用メッシュスフィアの横の分割数
#define PLAYERATTACKSPHERECOLLIER_MESH_YBLOCK   (10)                             // 可視化用メッシュスフィアの縦の分割数

//*****************************************************************************
//     静的メンバ変数
//*****************************************************************************


//=============================================================================
//    コンストラクタ
//=============================================================================
CPlayerAttackSphereCollider::CPlayerAttackSphereCollider(TYPE type, CScene *pParent) : CSphereCollider(type, pParent)
{
	// 各種値のクリア
	m_nPower = 0;
	m_nLife = 0;
}

//=============================================================================
//    デストラクタ
//=============================================================================
CPlayerAttackSphereCollider::~CPlayerAttackSphereCollider()
{

}


//=============================================================================
//    生成処理
//=============================================================================
CPlayerAttackSphereCollider *CPlayerAttackSphereCollider::Create(D3DXVECTOR3 pos, D3DXVECTOR3 scale, float fRadius, int nLife, int nPower)
{
	CPlayerAttackSphereCollider *pSphereCollider = NULL;   // プレイヤーの攻撃球モデルクラス型のポインタ
	pSphereCollider = new CPlayerAttackSphereCollider;
	if (pSphereCollider == NULL) { return NULL; }

	// 各種値の設定
	pSphereCollider->SetPos(pos);           // 球の座標
	pSphereCollider->SetScale(scale);       // 球の大きさ
	pSphereCollider->SetRadius(fRadius);    // 球の半径
	pSphereCollider->SetReturnFlag(false);  // 当たった際に押し戻すかどうか
	pSphereCollider->SetPower(nPower);      // 攻撃力
	pSphereCollider->SetLife(nLife);        // 寿命
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
HRESULT CPlayerAttackSphereCollider::Init(void)
{
	if (FAILED(CSphereCollider::Init()))
	{
		return E_FAIL;
	}

	// トランスフォーム情報を計算
	CalcNotScaleTransform();

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CPlayerAttackSphereCollider::Uninit(void)
{
	CSphereCollider::Uninit();
}

//=============================================================================
//    更新処理
//=============================================================================
void CPlayerAttackSphereCollider::Update(void)
{
	CSphereCollider::Update();

	// 寿命を減らす
	m_nLife--;
	if (m_nLife <= 0)
	{// 寿命がなくなった
		Uninit();
	}
}

//=============================================================================
//    描画処理
//=============================================================================
void CPlayerAttackSphereCollider::Draw(void)
{
	CSphereCollider::Draw();
}

//=============================================================================
//    当たり判定可視化用のメッシュスフィアを作成する処理
//=============================================================================
void CPlayerAttackSphereCollider::MakeColDispSphere(void)
{
	D3DXVECTOR3 scale = GetScale();
	float fScale = (scale.x + scale.y + scale.z) / 3.0f;
	float fRadius = GetRadius();
	CColDispSphere *pColDispSphere = CColDispSphere::Create(GetPos(), PLAYERATTACKSPHERECOLLIER_MESH_COL, fRadius / fScale,
		PLAYERATTACKSPHERECOLLIER_MESH_XBLOCK, PLAYERATTACKSPHERECOLLIER_MESH_YBLOCK, PLAYERATTACKSPHERECOLLIER_MESH_PRIORITY);
	SetColDispSphere(pColDispSphere);
}

//=============================================================================
//    攻撃力を設定する処理
//=============================================================================
void CPlayerAttackSphereCollider::SetPower(const int nPower)
{
	m_nPower = nPower;
}

//=============================================================================
//    寿命を設定する処理
//=============================================================================
void CPlayerAttackSphereCollider::SetLife(const int nLife)
{
	m_nLife = nLife;
}

//=============================================================================
//    攻撃力を取得する処理
//=============================================================================
int CPlayerAttackSphereCollider::GetPower(void)
{
	return m_nPower;
}

//=============================================================================
//    寿命を取得する処理
//=============================================================================
int CPlayerAttackSphereCollider::GetLife(void)
{
	return m_nLife;
}