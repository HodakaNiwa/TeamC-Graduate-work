//*****************************************************************************
//
//     地雷の処理[mine.cpp]
//     Author:Hodaka Niwa
//
//*****************************************************************************
#include "mine.h"
#include "manager.h"
#include "renderer.h"
#include "collision.h"
#include "loadEffect.h"

//*****************************************************************************
//     マクロ定義
//*****************************************************************************
#define MINE_MODEL_FILENAME      "data/MODEL/mine000.x"			// 地雷モデルのファイルパス名
#define MINE_SCALE_INI           (D3DXVECTOR3(5.0f,5.0f,5.0f))	// 地雷の大きさ(初期値)
#define MINE_LINE_INI            (15 * 60)						// 地雷の寿命(初期値)
#define MINE_COLRADIUS           (30.0f)						// 地雷が起動する判定の範囲
#define MINE_EXPLOSION_SCALEUP   (0.3f)							// 地雷が爆発する前にどれくらい大きくなるか
#define MINE_EXPLOSION_TIME      (45)							// 地雷の爆発時間
#define MINE_EXPLOSION_COLRADIUS (50.0f)						// 地雷の爆発有効範囲
#define MINE_EXPLOSION_STARTTIME (5)							// 地雷が爆発するまでの時間

//*****************************************************************************
//     静的メンバ変数宣言
//*****************************************************************************
LPD3DXMESH CMine::m_pMesh = NULL;		// メッシュへのポインタ
LPD3DXBUFFER CMine::m_pBuffMat = NULL;	// マテリアル情報へのポインタ
DWORD CMine::m_nNumMat = 0;				// マテリアル情報の数

//=============================================================================
//    コンストラクタ
//=============================================================================
CMine::CMine(int nPriority, OBJTYPE objtype) : CSceneX(nPriority, objtype)
{
	m_nType = TYPE_NONE;
	m_nLife = 0;
	m_fScaleUp = 0.0f;
	m_nExplosionTimer = 0;
}

//=============================================================================
//    デストラクタ
//=============================================================================
CMine::~CMine()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CMine *CMine::Create(D3DXVECTOR3 pos, CScene *pParentChara)
{
	// メモリを確保
	CMine *pMine = NULL;
	if (pMine != NULL) { return NULL; }
	pMine = new CMine;
	if (pMine == NULL) { return NULL; }

	// 各種値の設定
	pMine->SetPos(pos);
	pMine->SetParentChara(pParentChara);

	// 初期化処理
	if (FAILED(pMine->Init()))
	{
		pMine->Uninit();
		delete pMine;
		return NULL;
	}

	return pMine;
}

//=============================================================================
//    モデルの読み込み処理
//=============================================================================
HRESULT CMine::Load(void)
{
	// ポインタがクリアされていないなら失敗
	if (m_pMesh != NULL || m_pBuffMat != NULL) { return E_FAIL; }

	// モデルの読み込み
	D3DXLoadMeshFromX(MINE_MODEL_FILENAME,
		D3DXMESH_SYSTEMMEM,
		CManager::GetRenderer()->GetDevice(),
		NULL,
		&m_pBuffMat,
		NULL,
		&m_nNumMat,
		&m_pMesh);

	// モデルが読み込めてないなら失敗
	if (m_pMesh == NULL || m_pBuffMat == NULL) { return E_FAIL; }

	return S_OK;
}

//=============================================================================
//    モデルの開放処理
//=============================================================================
void CMine::UnLoad(void)
{
	// メッシュへのポインタ
	if (m_pMesh != NULL)
	{
		m_pMesh->Release();
		m_pMesh = NULL;
	}

	// マテリアル情報へのポインタ
	if (m_pBuffMat != NULL)
	{
		m_pBuffMat->Release();
		m_pBuffMat = NULL;
	}

	// マテリアル情報の数を初期化
	m_nNumMat = 0;
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CMine::Init(void)
{
	// モデル割り当て
	BindModel(m_pMesh, m_pBuffMat, m_nNumMat, NULL);
	SetMat(1.0f);
	SetScale(D3DXVECTOR3(5.0f, 5.0f, 5.0f));

	// 寿命の初期値を設定
	m_nLife = MINE_LINE_INI;

	// 地雷起動用の当たり判定を付与する
	CreateStartUpCollider();

	//変数の初期化
	m_pLoadEffect = NULL;

	//エフェクトの初期化
	if (m_pLoadEffect == NULL)
	{
		m_pLoadEffect = CLoadEffect::Create(3, GetPos(), 5);
	}
	return S_OK;
}

//=============================================================================
//    起動用の当たり判定を生成する処理
//=============================================================================
void CMine::CreateStartUpCollider(void)
{
	// コライダー管轄クラスを生成
	CColliderManager *pColManager = CColliderManager::Create(2);
	if (pColManager == NULL) { return; }

	// 円筒を生成
	CCylinderCollider *pCylinder = CCylinderCollider::Create(GetPos(), D3DXVECTOR3(1.0f, 1.0f, 1.0f),
		MINE_COLRADIUS, MINE_COLRADIUS, false);
	if (pCylinder == NULL) { return; }

	// 親を設定
	pCylinder->SetParent(this);

	// コライダー管轄クラスのポインタを設定
	pColManager->SetCollider(pCylinder, 0);
	SetCOlliderManager(pColManager);
}

//=============================================================================
//    終了処理
//=============================================================================
void CMine::Uninit(void)
{
	if (m_pLoadEffect != NULL)
	{//エフェクトの破棄
		m_pLoadEffect->Uninit();
		delete m_pLoadEffect;
		m_pLoadEffect = NULL;
	}

	// 共通の終了処理
	CSceneX::Uninit();
}

//=============================================================================
//    更新処理
//=============================================================================
void CMine::Update(void)
{
	switch (m_nType)
	{// 状態によって処理わけ
	case TYPE_NONE:
		None();
		break;
	case TYPE_STARTUP:
		StartUp();
		break;
	case TYPE_EXPLOSION:
		Explosion();
		break;
	}
}

//=============================================================================
//    通常状態の更新処理
//=============================================================================
void CMine::None(void)
{
	// 寿命を減らす
	m_nLife--;
	if (m_nLife <= 0)
	{// 寿命がなくなった
		m_nExplosionTimer = 0;
		m_nType = TYPE_STARTUP;
	}
}

//=============================================================================
//    起動状態の更新処理
//=============================================================================
void CMine::StartUp(void)
{
	// 大きくする
	m_fScaleUp += MINE_EXPLOSION_SCALEUP;
	D3DXVECTOR3 Scale = MINE_SCALE_INI;
	Scale.x += m_fScaleUp;
	Scale.y += m_fScaleUp;
	Scale.z += m_fScaleUp;
	SetScale(Scale);

	// カウンターがたまったら爆発
	m_nExplosionTimer++;
	if (m_nExplosionTimer >= MINE_EXPLOSION_STARTTIME)
	{
		// カウンターを初期化して状態変更
		m_nExplosionTimer = 0;
		m_nType = TYPE_EXPLOSION;

		// 爆発のエフェクトと当たり判定を生成する
		CreateExplosionEffect();
		CreateExplosionCollider();
	}
}

//=============================================================================
//    爆発のエフェクトを生成する処理
//=============================================================================
void CMine::CreateExplosionEffect(void)
{
	//スピードUPの時はこっち
	m_pLoadEffect->OnTimeEffect();
}

//=============================================================================
//    爆発用の当たり判定を生成する処理
//=============================================================================
void CMine::CreateExplosionCollider(void)
{
	// 球を生成
	CPlayerAttackSphereCollider *pShere = CPlayerAttackSphereCollider::Create(GetPos(), D3DXVECTOR3(1.0f, 1.0f, 1.0f),
		MINE_EXPLOSION_COLRADIUS, MINE_EXPLOSION_TIME + 2, 1);
	if (pShere == NULL) { return; }

	// 親を設定
	pShere->SetParent(this);

	// コライダー管轄クラスにポインタを設定
	CColliderManager *pColManager = GetColloderManger();
	pColManager->SetCollider(pShere, 1);
}

//=============================================================================
//    爆発状態の更新処理
//=============================================================================
void CMine::Explosion(void)
{
	// 爆発の寿命が切れたら終了
	m_nExplosionTimer++;
	if (m_nExplosionTimer >= MINE_EXPLOSION_TIME)
	{
		Uninit();
	}
}

//=============================================================================
//    描画処理
//=============================================================================
void CMine::Draw(void)
{
	// 爆発状態なら描画しない
	if (m_nType == TYPE_EXPLOSION) { return; }

	//レンダリングクラスを取得
	CRenderer * pRenderer = NULL;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// 法線を正規化する(大きさによって法線が変わるので)
	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);

	// 共通の描画処理
	CSceneX::Draw();

	// 法線を正規化しない
	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, false);

	if (m_pLoadEffect != NULL)
	{//エフェクトの描画
		m_pLoadEffect->Draw();
	}
}

//=============================================================================
//    当たり判定処理
//=============================================================================
void CMine::Collision(void)
{
	if (m_nType != TYPE_NONE) { return; }

	// 各種値の取得
	D3DXVECTOR3 pos = m_pos;
	float fLength = MINE_COLRADIUS;

	// 判定開始
	CCollider *pCol = NULL;
	CCollider *pColNext = NULL;
	for (int nCntType = 0; nCntType < CCollider::TYPE_MAX; nCntType++)
	{
		pCol = CCollider::GetTop(nCntType);

		while (pCol != NULL)
		{
			pColNext = pCol->GetNext();
			CollisionCollider(pCol, pos, fLength);
			pCol = pColNext;
		}
	}
}

//=============================================================================
//    コライダーとの当たり判定処理
//=============================================================================
void CMine::CollisionCollider(CCollider *pCollider, D3DXVECTOR3 pos, float fLength)
{
	if (pCollider->GetType() == CCollider::TYPE_SPHERE_PLAYERATTACK)
	{
		if (CollisionPlayerAttackSphereCollider((CPlayerAttackSphereCollider*)pCollider, pos, fLength) == true)
		{
			CScene *pParent = pCollider->GetParent();
			if (pParent->GetObjType() == OBJTYPE_PLAYER || pParent->GetObjType() == OBJTYPE_ENEMY)
			{// プレイヤーか敵だったら地雷を起動させる
				SetType(TYPE_STARTUP);
			}
		}
	}
}

//=============================================================================
//    プレイヤーの攻撃球との当たり判定処理
//=============================================================================
bool CMine::CollisionPlayerAttackSphereCollider(CPlayerAttackSphereCollider *pShere, D3DXVECTOR3 pos, float fLength)
{
	if (pShere->Collision(&pos, fLength, this) == true)
	{
		return true;
	}

	return false;
}