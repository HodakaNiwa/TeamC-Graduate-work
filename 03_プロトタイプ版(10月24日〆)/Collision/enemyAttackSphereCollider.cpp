//*****************************************************************************
//
//     敵の攻撃球モデルの処理[enemyAttackSphereCollider.cpp]
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
#define ENEMYATTACKSPHERECOLLIER_MESH_PRIORITY (5)                              // 可視化用メッシュスフィアの処理優先順位
#define ENEMYATTACKSPHERECOLLIER_MESH_COL      (D3DXCOLOR(1.0f,0.0f,1.0f,0.5f)) // 可視化用メッシュスフィアの色
#define ENEMYATTACKSPHERECOLLIER_MESH_XBLOCK   (10)                             // 可視化用メッシュスフィアの横の分割数
#define ENEMYATTACKSPHERECOLLIER_MESH_YBLOCK   (10)                             // 可視化用メッシュスフィアの縦の分割数

//*****************************************************************************
//     静的メンバ変数
//*****************************************************************************


//=============================================================================
//    コンストラクタ
//=============================================================================
CEnemyAttackSphereCollider::CEnemyAttackSphereCollider(TYPE type, CScene *pParent) : CSphereCollider(type, pParent)
{
	// 各種値のクリア
	m_nPower = 0;
	m_nLife = 0;
}

//=============================================================================
//    デストラクタ
//=============================================================================
CEnemyAttackSphereCollider::~CEnemyAttackSphereCollider()
{

}


//=============================================================================
//    生成処理
//=============================================================================
CEnemyAttackSphereCollider *CEnemyAttackSphereCollider::Create(D3DXVECTOR3 pos, D3DXVECTOR3 scale, float fRadius, int nLife, int nPower)
{
	CEnemyAttackSphereCollider *pSphereCollider = NULL;   // 敵の攻撃球モデルクラス型のポインタ
	pSphereCollider = new CEnemyAttackSphereCollider;
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
HRESULT CEnemyAttackSphereCollider::Init(void)
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
void CEnemyAttackSphereCollider::Uninit(void)
{
	CSphereCollider::Uninit();
}

//=============================================================================
//    更新処理
//=============================================================================
void CEnemyAttackSphereCollider::Update(void)
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
void CEnemyAttackSphereCollider::Draw(void)
{
	CSphereCollider::Draw();
}

//=============================================================================
//    当たり判定可視化用のメッシュスフィアを作成する処理
//=============================================================================
void CEnemyAttackSphereCollider::MakeColDispSphere(void)
{
	D3DXVECTOR3 scale = GetScale();
	float fScale = (scale.x + scale.y + scale.z) / 3.0f;
	float fRadius = GetRadius();
	CColDispSphere *pColDispSphere = CColDispSphere::Create(GetPos(), ENEMYATTACKSPHERECOLLIER_MESH_COL, fRadius / fScale,
		ENEMYATTACKSPHERECOLLIER_MESH_XBLOCK, ENEMYATTACKSPHERECOLLIER_MESH_YBLOCK, ENEMYATTACKSPHERECOLLIER_MESH_PRIORITY);
	SetColDispSphere(pColDispSphere);
}

//=============================================================================
//    攻撃力を設定する処理
//=============================================================================
void CEnemyAttackSphereCollider::SetPower(const int nPower)
{
	m_nPower = nPower;
}

//=============================================================================
//    寿命を設定する処理
//=============================================================================
void CEnemyAttackSphereCollider::SetLife(const int nLife)
{
	m_nLife = nLife;
}

//=============================================================================
//    攻撃力を取得する処理
//=============================================================================
int CEnemyAttackSphereCollider::GetPower(void)
{
	return m_nPower;
}

//=============================================================================
//    寿命を取得する処理
//=============================================================================
int CEnemyAttackSphereCollider::GetLife(void)
{
	return m_nLife;
}