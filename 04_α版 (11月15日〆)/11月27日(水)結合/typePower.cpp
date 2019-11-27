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

#define SKILL_RANGE			(150.0f)

//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CTypePower::CTypePower(int nPriority, OBJTYPE objType) : CEnemy(nPriority, objType) { }

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CTypePower::~CTypePower() { }

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
			pSpeedType->Init(nChara, pos, ModelTxt, MotionTxt,country);
			//佐藤追加しました
			pSpeedType->m_CharcterTypeResult[nChara] = type;
			pSpeedType->m_nCuntry[nChara] = country;
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
HRESULT CTypePower::Init(int nChara, D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40],int country)
{
	m_bWalk = true;

	m_nEnemyNo = nChara;	//	キャラ番号の記憶(生成順)
	m_bSprintMotion = false;

	CCharacter::Init(nChara, ModelTxt, MotionTxt,m_CharcterType,country);	//	初期化
	CCharacter::SetPos(pos);						//	位置反映
	m_nLineNum = 2;	//	最低限のラインを引いたら始点に戻る(拠点を2つ繋いだら始点に戻る、始点に戻ってきたらラインは3つになりポイント加算の条件を満たせ
	InitSort(pos);	//	ゲーム開始時の近場のエリア・テリトリーを見つける


	m_pModel = CCharacter::GetModel();	//	モデル情報の取得
	m_pMotion = CCharacter::GetMotion();//	モーション情報の取得

	//	ライン変数の初期化
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


	ResetLine();	//	ラインの初期化


	m_fSpeed = 1.0f;				//	初期速度
	m_nColliderCnt = 0;
	m_nColliderTimer = 0;
	m_nRecastCounter = 0;
	m_fScale = 0.0f;
	m_bBreakTime = false;
	m_bRecast = false;
	m_nCreateTime = (rand() % 4);	//	始点に戻るまでの時間調整
	m_nLineNum = 2;
	m_bStop = false;
	m_bTrigger = false;
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
	//	ゲームの状態を取得
	int nGameState = CGame::GetGameState();

	if (nGameState != CGame::GAMESTATE_FIRSTCOUNTDOWN && nGameState != CGame::GAMESTATE_END)
	{
		CEnemy::Update();
		ActionUpdate();	//	スキル処理
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
void  CTypePower::Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size) { }

//=============================================================================
// スキル処理
//=============================================================================
void  CTypePower::ActionUpdate(void)
{
	// ブレイクタイムではない && スキルの発動フラグを立てた && 処理を一回しか通さない
	if (m_bBreakTime == false && m_bTrigger == true && m_bStop == false)
	{
		m_bStop = true;				//	更新を一時的に止める
		CreateStartUpCollider();	//　ハンマーの当たり判定
		m_pMotion->SetNumMotion(2);	//　攻撃モーション
		m_fSpeed = 0.0f;			//	アクション中は動きを止める
		m_bTarget = true;			//	ターゲットを変更
	}

	//	スキルの発動フラグが立っている
	if (m_bTrigger == true)
	{
		m_nRecastCounter++;
	}

	switch(m_nRecastCounter)
	{
	case 60:
		CreateColliderSphere();	//	衝撃波の当たり判定を付与
		break;

	case 80:
		m_fSpeed = 1.0f;		//	アクション終了時、動けるように
		m_bBreakTime = true;	//	ブレイクタイムの発生
		m_bTarget = false;		//	ターゲットを拠点に戻す
		break;

	case 60 * RECAST:
		m_bStop = false;		//	また更新できるように
		m_nRecastCounter = 0;	//	カウントの初期化(衝撃波の発生のために)
		m_bBreakTime = false;	//	ブレイクタイムの終了
		m_bTrigger = false;		//	スキルの発動フラグをリセット
		break;
	}
}


//=============================================================================
//    ハンマーの当たり判定を生成する処理
//=============================================================================
void CTypePower::CreateStartUpCollider(void)
{
	//	円筒を生成
	CPlayerAttackSphereCollider *pSphere = CPlayerAttackSphereCollider::Create(D3DXVECTOR3(0.0f, 0.0f, -60.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f),
		30.0f, 100, 1);

	if (pSphere == NULL) { return; }

	//	親を設定
	pSphere->SetParent(this);

	//	武器の場所に判定を付ける
	pSphere->SetParentMtxWorld(&m_pModel[15]->GetMtxWorld());
}

//=============================================================================
//    衝撃波の判定を生成する処理
//=============================================================================
void CTypePower::CreateColliderSphere(void)
{
	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVec3TransformCoord(&pos, &m_pModel[15]->GetPos(), &m_pModel[15]->GetMtxWorld());

	//	スフィアを生成
	CPlayerAttackSphereCollider *pSphere = CPlayerAttackSphereCollider::Create(D3DXVECTOR3(pos.x, pos.y, pos.z + 35.0f),
		D3DXVECTOR3(1.0f, 1.0f, 1.0f), 100.0f, 100, 1);

	if (pSphere == NULL) { return; }

	//	親を設定
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