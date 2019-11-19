//◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆
//					脳筋型 [typePower.h]
//			Author : Kobayashi_Sho-hei / 小林 将兵
//◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆
#include "enemy.h"
#include "manager.h"
#include "camera.h"
#include "input.h"
#include "inputmouce.h"
#include "loadEffect.h"
#include "line.h"
#include "sceneOrbit.h"
#include "model.h"
#include "UI.h"
#include "skilicon.h"
#include "RawMouse.h"
#include "collision.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_SPRINTTIMER (8)		//	スプリント時間
#define WAVE (1)				//	
#define RECAST (10)				//	スプリントのリキャスト時間
//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CTypePower::CTypePower(int nPriority, OBJTYPE objType) : CEnemy(nPriority, objType)
{

}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CTypePower::~CTypePower()
{

}

//=============================================================================
//オブジェクトの生成処理
//=============================================================================
CTypePower *CTypePower::Create(int nChara, int country, CHARCTERTYPE type, D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40])
{
	CTypePower *pSpeedType = NULL;
	if (pSpeedType == NULL)
	{
		pSpeedType = new CTypePower;

		if (pSpeedType != NULL)
		{
			pSpeedType->SetType(country);
			pSpeedType->m_CharcterType = type;
			pSpeedType->Init(nChara, pos, ModelTxt, MotionTxt);
		}
		else
		{
			MessageBox(0, "プレイヤーがNULLでした", "警告", MB_OK);
		}
	}
	else
	{
		MessageBox(0, "プレイヤーがNULLじゃないです", "警告", MB_OK);
	}
	return pSpeedType;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CTypePower::Init(int nChara, D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40])
{
	//プレイヤーのナンバーを格納
	m_nEnemyNo = nChara;

	//初期化
	m_bWalk = true;
	m_bSprintMotion = false;
	//キャラクターの初期化
	CCharacter::Init(nChara,ModelTxt, MotionTxt, m_CharcterType);
	CCharacter::SetPos(pos);

	m_nLineNum = 2;	//	最低限のラインを引いたら始点に戻る(拠点を2つ繋いだら始点に戻る、始点に戻ってきたらラインは3つになりポイント加算の条件を満たせる)
	InitSort(pos);	//	ゲーム開始時の近場のエリア・テリトリーを見つける


	//モデルの取得
	m_pModel = CCharacter::GetModel();
	
	//ライン変数の初期化
	m_nMaxLineTime = FIRST_LINETIME;
	m_nLineTime = m_nMaxLineTime;
	m_bBlockStartTerritory = false;
	m_pOldStartTerritory = NULL;
	m_nCountTime = 0;
	m_pOrbitLine = NULL;
	m_bMakeShape = false;
	m_nCntTimeCopyLine = 0;
	m_pLoadEffect = NULL;


	//コピーラインの初期化
	for (int nCnt = 0; nCnt < MAX_TERRITORY; nCnt++)
	{
		m_apCopyLine[nCnt] = NULL;
	}

	//エフェクトの初期化
	if (m_pLoadEffect == NULL)
	{
		m_pLoadEffect = CLoadEffect::Create(1, D3DXVECTOR3(pos.x, pos.y + 60.0f, pos.z), 8);
	}

	//モーションの情報を取得
	m_pMotion = CCharacter::GetMotion();
	ResetLine();	//ラインの初期化

	//スピードプレイヤー用の変数を初期化
	m_nColliderCnt = 0;
	m_nColliderTimer = 0;
	m_nRecastCounter = 0;
	m_nRecastTimer = 0;
	m_nButtonCounter = 0;
	m_fScale = 0.0f;
	m_bAction = false;
	m_bRecast = false;
	m_bCollision = false;

	return S_OK;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CTypePower::Init(void)
{
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void  CTypePower::Uninit(void)
{
	if (m_pLoadEffect != NULL)
	{//エフェクトの破棄
		m_pLoadEffect->Uninit();
		delete m_pLoadEffect;
		m_pLoadEffect = NULL;
	}
	//プレイヤーの終了処理
	CEnemy::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void  CTypePower::Update(void)
{
	//脳筋型のスキル処理
	//ActionUpdate();

	//プレイヤーの更新
	CEnemy::Update();
}

//=============================================================================
// スプリント処理
//=============================================================================
void  CTypePower::ActionUpdate(void)
{
	if (m_bRecast == false && m_nButtonCounter == 0)
	{
		//CreateStartUpCollider();	//ハンマーに当たり判定を付与する
		m_pMotion->SetNumMotion(2);	//攻撃モーション
		m_bAction = true;			//アクション中にする
		m_nButtonCounter = 1;		//アクションボタンを使用できないようにする
		m_bCollision = true;

	}

	//アクション中
	if (m_bAction == true)
	{
		m_nColliderCnt++;

		if (m_nColliderCnt % 60 == 0)
		{
			m_nColliderTimer++;
			if (m_nColliderTimer >= WAVE)
			{//1秒経過したら衝撃波を出す

				CreateColliderSphere();	//衝撃波の当たり判定を付与
				m_nColliderTimer = 0;	//タイマーを初期化
				m_nColliderCnt = 0;
				m_bRecast = true;		//リキャスト中にする	
				m_bAction = false;		//アクションを終了
			}
		}
	}

	//リキャスト中
	if (m_bRecast == true)
	{
		m_nRecastCounter++;
		if (m_nRecastCounter % 60 == 0)
		{
			m_nRecastTimer++;
			if (m_nRecastTimer <= RECAST)
			{//リキャスト中はアクションを使用できない
				m_bRecast = true;
			}
			else
			{//10秒経過したらアクションを使用できる
				m_nRecastTimer = 0;					//タイマーを初期化
				m_bRecast = false;					//リキャスト終了
				m_bAction = false;					//アクションを使用できる
				m_nButtonCounter = 0;				//アクションボタンが使用できる
			}
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void  CTypePower::Draw(void)
{
	CEnemy::Draw();
}
//=============================================================================
//
//=============================================================================
void  CTypePower::Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
{

}

//=============================================================================
//    衝撃波の判定を生成する処理
//=============================================================================
void CTypePower::CreateColliderSphere(void)
{
	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVec3TransformCoord(&pos, &m_pModel[15]->GetPos(), &m_pModel[15]->GetMtxWorld());

	// スフィアを生成
	CPlayerAttackSphereCollider *pSphere = CPlayerAttackSphereCollider::Create(D3DXVECTOR3(pos.x, pos.y, pos.z + 35.0f),
		D3DXVECTOR3(1.0f, 1.0f, 1.0f), 100.0f, 100, 1);

	if (pSphere == NULL) { return; }

	// 親を設定
	pSphere->SetParent(this);

}


//=============================================================================
// ラインの生成処理
//=============================================================================
void CTypePower::CreateOrbitLine(void)
{
	if (m_pOrbitLine == NULL)
	{
		m_pOrbitLine = CSceneOrbit::Create(CSceneOrbit::TYPE_PLAYER, CCharacter::GetPos());
		m_pOrbitLine->SetMtxParent(&m_pModel[10]->GetMtxWorld());
		m_pOrbitLine->SetMtxParentEnd(&m_pModel[1]->GetMtxWorld());
	}
}

//=============================================================================
// ラインの破棄
//=============================================================================
void CTypePower::UninitOrtbitLine(void)
{
	if (m_pOrbitLine != NULL)
	{
		m_pOrbitLine->Uninit();
		m_pOrbitLine = NULL;
	}
}