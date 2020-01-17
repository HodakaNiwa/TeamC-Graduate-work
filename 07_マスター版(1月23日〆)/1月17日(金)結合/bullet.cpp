//====================================================================================
//
// ロボットの弾処理	[bullet.cpp]
// Author;荒谷由朗
//
//====================================================================================
#include "bullet.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"
#include "collision.h"
#include "loadEffect.h"
#include "debuglog.h"
#include "collision.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define BULLETMODEL_TEXT			("data/MODEL/ROBOT/99_Robot_bullet.x")

#define BULLET_COLRADIUS			(30.0f)		// ミサイルが起動する判定の範囲
#define BULLET_LIFE					(40)		// ミサイルの寿命
#define BULLET_EXPLOSION_TIME		(15)		// ミサイルの爆発時間
#define BULLET_EXPLOSION_COLRADIUS	(100.0f)	// 地雷の爆発有効範囲
#define BULLET_MOVESPEED			(0.5f)		// 移動速度

//=============================================================================
// 前方宣言
//=============================================================================
LPDIRECT3DTEXTURE9	CBullet::m_ppTexture	= NULL;
LPD3DXMESH			CBullet::m_pMesh		= NULL;
LPD3DXBUFFER		CBullet::m_pBuffMat		= NULL;
DWORD				CBullet::m_pNumMat		= NULL;
D3DXVECTOR3			CBullet::m_pVtxMax		= {};
D3DXVECTOR3			CBullet::m_pVtxMin		= {};

//=============================================================================
// コンストラクタ
//=============================================================================
CBullet::CBullet(int nPriority, OBJTYPE objtype) : CSceneX(nPriority, objtype)
{
	m_pos				= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posOld			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot				= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move				= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Angle				= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nType				= TYPE_MOVE;
	m_nLife				= BULLET_LIFE;
	m_bDeth				= false;
	m_nExplosionTimer	= 0;
	m_bExplosion		= false;

	for (int nCnt = 0; nCnt < BULLET_MAX_EFFECT; nCnt++)
	{
		m_pLoadEffect[nCnt] = NULL;
	}
}

//=============================================================================
// デストラクタ
//=============================================================================
CBullet::~CBullet()
{
}

//=============================================================================
// 生成処理
//=============================================================================
CBullet * CBullet::Create(const D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale)
{
	// メモリを確保
	CBullet *pBullet = NULL;
	if (pBullet != NULL) { return NULL; }
	pBullet = new CBullet;
	if (pBullet == NULL) { return NULL; }

	// 初期化処理
	if (FAILED(pBullet->Init()))
	{
		pBullet->Uninit();
		delete pBullet;
		return NULL;
	}
	pBullet->SetPos		(pos);
	pBullet->m_pos = pos;
	pBullet->SetRot		(rot);
	pBullet->m_rot = rot;
	pBullet->SetScale	(scale);
	pBullet->m_Scale = scale;

	// モデル・テクスチャの割り当て
	pBullet->BindModel	(m_pMesh, m_pBuffMat, m_pNumMat, m_ppTexture);

	// 起動用の当たり判定を付与する
	pBullet->CreateCollider();

	return pBullet;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CBullet::Init(void)
{
	CSceneX::Init();

	//変数の初期化
	for (int nCnt = 0; nCnt < BULLET_MAX_EFFECT; nCnt++)
	{
		m_pLoadEffect[nCnt] = NULL;
	}

	//エフェクトの初期化
	if (m_pLoadEffect[0] == NULL)
	{
		m_pLoadEffect[0] = CLoadEffect::Create(3, GetPos(), 5);
	}
	if (m_pLoadEffect[1] == NULL)
	{
		m_pLoadEffect[1] = CLoadEffect::Create(1, GetPos(), 6);
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CBullet::Uninit(void)
{
	for (int nCnt = 0; nCnt < BULLET_MAX_EFFECT; nCnt++)
	{
		if (m_pLoadEffect[nCnt] != NULL)
		{//エフェクトの破棄
			m_pLoadEffect[nCnt]->Uninit();
			delete m_pLoadEffect[nCnt];
			m_pLoadEffect[nCnt] = NULL;
		}
	}

	CSceneX::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CBullet::Update(void)
{
	//前回の位置を格納
	m_posOld = m_pos;

	switch (m_nType)
	{
	case TYPE_NONE:			// ニュートラル
		break;
	case TYPE_MOVE:			// 移動
		Move();			// 移動処理
		break;
	case TYPE_EXPLOSION:	// 爆発
		Explosion();	// 爆発処理
		break;
	}
	CSceneX::Update();

	//CDebugProc::Print("ミサイル座標:%f,%f,%f", m_pos.x, m_pos.y, m_pos.z);
	//CDebugProc::Print("ミサイル向き:%f,%f,%f", m_rot.x, m_rot.y, m_rot.z);

	if (m_bDeth == true) { Uninit(); }		// 死亡フラグチェック
}

//=============================================================================
// 描画処理
//=============================================================================
void CBullet::Draw(void)
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

	for (int nCnt = 0; nCnt < BULLET_MAX_EFFECT; nCnt++)
	{
		if (m_pLoadEffect[nCnt] != NULL)
		{//エフェクトの描画
			m_pLoadEffect[nCnt]->Draw();
		}
	}
}

//=============================================================================
// 読み込み処理
//=============================================================================
void CBullet::LoadModel(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// Xファイルの読み込み
	D3DXLoadMeshFromX(BULLETMODEL_TEXT,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_pBuffMat,
		NULL,
		&m_pNumMat,
		&m_pMesh);

	D3DXMATERIAL *pmat;			// マテリアルデータへのポインタ
	pmat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	for (DWORD tex = 0; tex < m_pNumMat; tex++)
	{// カウント
		if (pmat[tex].pTextureFilename != NULL &&
			lstrlen(pmat[tex].pTextureFilename) > 0)
		{// テクスチャを使用している
			if (FAILED(D3DXCreateTextureFromFile(pDevice,
				pmat[tex].pTextureFilename,
				&m_ppTexture)))
			{
				MessageBox(NULL, "テクスチャの読み込みに失敗しました", NULL, MB_OK);
			}
		}
	}

	// 変数宣言
	int		nNumVtx		= 0;	// 頂点数
	DWORD	sizeFVF		= 0;	// 頂点フォーマットのサイズ
	BYTE *	pVtxBuff	= 0;	// 頂点バッファへのポインタ

	// 頂点数を取得
	nNumVtx = m_pMesh->GetNumVertices();

	// 頂点フォーマットのサイズを取得
	sizeFVF = D3DXGetFVFVertexSize(m_pMesh->GetFVF());

	m_pVtxMax = D3DXVECTOR3(-100000.0f, 0.0f, -100000.0f);	// 頂点の最大値
	m_pVtxMin = D3DXVECTOR3(100000.0f, 0.0f, 100000.0f);	// 頂点の最小値

	// 頂点バッファをロック
	m_pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;	// 頂点座標の代入

		// 最大値を比較
		if (m_pVtxMax.x < vtx.x)
		{
			m_pVtxMax.x = vtx.x;
		}
		if (m_pVtxMax.y < vtx.y)
		{
			m_pVtxMax.y = vtx.y;
		}
		if (m_pVtxMax.z < vtx.z)
		{
			m_pVtxMax.z = vtx.z;
		}

		// 最小値を比較
		if (m_pVtxMin.x > vtx.x)
		{
			m_pVtxMin.x = vtx.x;
		}
		if (m_pVtxMin.y > vtx.y)
		{
			m_pVtxMin.y = vtx.y;
		}
		if (m_pVtxMin.z > vtx.z)
		{
			m_pVtxMin.z = vtx.z;
		}
		// サイズ分ポインタを進める
		pVtxBuff += sizeFVF;
	}

	// 頂点バッファをアンロック
	m_pMesh->UnlockVertexBuffer();
}

//=============================================================================
// 読み込みの破棄
//=============================================================================
void CBullet::UnloadModel(void)
{
	// メッシュの破棄
	if (m_pMesh != NULL)
	{
		m_pMesh->Release();
		m_pMesh = NULL;
	}

	// バッファの破棄
	if (m_pBuffMat != NULL)
	{
		m_pBuffMat->Release();
		m_pBuffMat = NULL;
	}

	// テクスチャの破棄
	if (m_ppTexture != NULL)
	{
		m_ppTexture->Release();
		m_ppTexture = NULL;
	}
}

//=============================================================================
// 移動処理
//=============================================================================
void CBullet::Move(void)
{
	m_move.x -= sinf(m_rot.y) * BULLET_MOVESPEED;
	m_move.z -= cosf(m_rot.y) * BULLET_MOVESPEED;
	m_pos += m_move;
	SetPos(m_pos);
	if (m_pLoadEffect[1] != NULL)
	{
		//移動エフェクト
		m_pLoadEffect[1]->SetPos(D3DXVECTOR3(GetPos().x, GetPos().y + 20.0f, GetPos().z));
		//通常の移動はこっち
		m_pLoadEffect[1]->Update();
	}
}

//=============================================================================
// 爆発処理
//=============================================================================
void CBullet::Explosion(void)
{
	if (m_bExplosion == false)
	{// 爆発していなかったとき
		CreateExplosionEffect();	// 爆発エフェクトの生成
		CreateExplosionCollider();	// 爆発当たり判定の生成
		m_bExplosion = true;
	}
	else
	{
		// 爆発の寿命が切れたら終了
		m_nExplosionTimer++;
		if (m_nExplosionTimer >= BULLET_EXPLOSION_TIME)
		{
			m_bDeth = true;
		}
	}
}

//=============================================================================
//    起動用の当たり判定を生成する処理
//=============================================================================
void CBullet::CreateCollider(void)
{
	// コライダー管轄クラスを生成
	CColliderManager *pColManager = CColliderManager::Create(2);
	if (pColManager == NULL) { return; }

	// 円筒を生成
	CCylinderCollider *pCylinder = CCylinderCollider::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f),
		BULLET_COLRADIUS, BULLET_COLRADIUS + 100.0f, false);
	if (pCylinder == NULL) { return; }

	// 親を設定
	pCylinder->SetParent(this);
	pCylinder->SetParentMtxWorld(&this->m_mtxWorld);

	// コライダー管轄クラスのポインタを設定
	pColManager->SetCollider(pCylinder, 0);
	SetCOlliderManager(pColManager);
}

//=============================================================================
//    爆発のエフェクトを生成する処理
//=============================================================================
void CBullet::CreateExplosionEffect(void)
{
	// 爆発
	if (m_pLoadEffect[0] != NULL)
	{
		m_pLoadEffect[0]->SetPos(m_pos);
		m_pLoadEffect[0]->OnTimeEffect();
	}
}

//=============================================================================
//    爆発用の当たり判定を生成する処理
//=============================================================================
void CBullet::CreateExplosionCollider(void)
{
	// 球を生成
	CPlayerAttackSphereCollider *pShere = CPlayerAttackSphereCollider::Create(GetPos(), D3DXVECTOR3(1.0f, 1.0f, 1.0f),
		BULLET_EXPLOSION_COLRADIUS, BULLET_EXPLOSION_TIME + 2, 1);
	if (pShere == NULL) { return; }

	// 親を設定
	pShere->SetParent(this);

	// コライダー管轄クラスにポインタを設定
	CColliderManager *pColManager = GetColloderManger();
	pColManager->SetCollider(pShere, 1);
}

//=============================================================================
//　プレイヤーとテリトリーの当たり判定
//=============================================================================
void CBullet::Collision(void)
{
	//各種値の取得
	D3DXVECTOR3 pos = m_pos;
	D3DXVECTOR3 posOld = m_posOld;
	D3DXVECTOR3 Move = INITIALIZE_VECTOR3;
	D3DXVECTOR3 ColRange;
	//判定開始
	CCollider *pCol = NULL;
	CCollider *pColNext = NULL;

	for (int nCntType = 0; nCntType < CCollider::TYPE_MAX; nCntType++)
	{
		pCol = CCollider::GetTop(nCntType);

		while (pCol != NULL)
		{
			pColNext = pCol->GetNext();
			CollisionCollider(pCol, pos, posOld, m_move, D3DXVECTOR3(15.0f, 15.0f, 15.0f));
			pCol = pColNext;
		}
	}
}

//=============================================================================
//　コライダーとの当たり判定処理
//=============================================================================
bool CBullet::CollisionCollider(CCollider *pCollider, D3DXVECTOR3 &pos, D3DXVECTOR3 &posOld, D3DXVECTOR3 &Move, D3DXVECTOR3 &ColRange)
{
	if (pCollider->GetType() == CCollider::TYPE_BOX)
	{
		if (CollisionBoxCollider((CBoxCollider*)pCollider, pos, posOld, Move, ColRange) == true)
		{

		}
	}
	return false;
}

//=============================================================================
//　ボックスコライダーとの当たり判定処理
//=============================================================================
bool CBullet::CollisionBoxCollider(CBoxCollider *pBoxCollider, D3DXVECTOR3 &pos, D3DXVECTOR3 &posOld, D3DXVECTOR3 &Move, D3DXVECTOR3 &ColRange)
{
	bool bLand = false;

	if (pBoxCollider->Collision(&pos, &posOld, &Move, ColRange, this, &bLand) == true)
	{
		if (bLand == true)
		{
		}
		m_bDeth = true;
		return true;
	}
	return false;
}