//◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆
//					脳筋型 [typePower.h]
//			Author : Kobayashi_Sho-hei / 小林 将兵
//◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆
#include "enemy.h"
#include "manager.h"
#include "model.h"
#include "loadEffect.h"
#include "effectManager.h"
#include "line.h"
#include "collision.h"
#include "debuglog.h"
#include "crack.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_SPRINTTIMER (8)		//	スプリント時間
#define RECAST (10)				//	スプリントのリキャスト時間
#define SKILL_RANGE			(150.0f)	// スキルの識別範囲

//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CTypePower::CTypePower(int nPriority, OBJTYPE objType) : CEnemy(nPriority, objType)
{ 
	m_bTrigger = false;
	m_bBreakTime = false;
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CTypePower::~CTypePower() {}

//=============================================================================
//オブジェクトの生成処理
//=============================================================================
CTypePower *CTypePower::Create(int nChara, int country, CHARCTERTYPE type, D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40])
{
	CTypePower *pPowerType = NULL;
	if (pPowerType == NULL)
	{
		pPowerType = new CTypePower;

		if (pPowerType != NULL)
		{
			pPowerType->SetType(country);
			pPowerType->m_CharcterType = type;
			pPowerType->Init(nChara, pos, ModelTxt, MotionTxt,country);
			//佐藤追加しました
			pPowerType->m_CharcterTypeResult[nChara] = type;
			pPowerType->m_nCuntry[nChara] = country;
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
	return pPowerType;
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
	m_bBlockStartTerritory = false;
	m_pOldStartTerritory = NULL;
	m_bMakeShape = false;
	m_pLoadEffect = NULL;

	//エフェクトの初期化
	if (m_pLoadEffect == NULL)
	{
		m_pLoadEffect = CLoadEffect::Create(1, D3DXVECTOR3(pos.x, pos.y + 60.0f, pos.z), 8);
	}


	ResetLine();	//	ラインの初期化


	m_fSpeed = 1.0f;				//	初期速度
	m_nRecastCounter = 0;
	m_bBreakTime = false;
	m_bRecast = false;
	m_nLineNum = 2;
	m_bStop = false;
	m_bTrigger = false;
	m_state = STATE_NONE;

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
	CGame *pGame = CManager::GetGame();				// ゲームの取得
	CEventCamera *pEveCam = pGame->GetEveCam();		// イベントカメラの取得

	//ゲームの状態を取得
	int nGameState = CGame::GetGameState();

	if (nGameState != CGame::GAMESTATE_FIRSTCOUNTDOWN && nGameState != CGame::GAMESTATE_END)
	{

		CEnemy::Update();
		if (pEveCam == NULL)	// イベントカメラが消えていたら
		{
			ActionUpdate();	//	スキル処理
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
void  CTypePower::Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size) { }

//=============================================================================
// スキル処理
//=============================================================================
void  CTypePower::ActionUpdate(void)
{
	// ブレイクタイムではない && スキルの発動フラグを立てた && 処理を一回しか通さない
	if (m_bBreakTime == false && m_bTrigger == true && m_bStop == false && m_state != STATE_DAMAGE)
	{
		m_bStop = true;							//	更新を一時的に止める
		m_pMotion->SetNumMotion(STATE_ACTION);	//　攻撃モーション
		m_state = STATE_ACTION;
		m_fSpeed = 0.0f;						//	アクション中は動きを止める
		m_bTarget = true;						//	ターゲットを変更
	}

	//	スキルの発動フラグが立っている
	if (m_bTrigger == true)
	{
		m_nRecastCounter++;
	}

	switch (m_nRecastCounter)
	{
	case 60:
		if (m_state != STATE_DAMAGE && m_state != STATE_BLOWAWAY && m_state != STATE_ROBOTDAMAGE)
		{//ダメージ中じゃなかったら衝撃波を出す
			CreateColliderSphere();	//衝撃波の当たり判定を付与
		}
		break;

	case 120:
		m_fSpeed = 1.0f;			//	アクション終了時、動けるように
		m_bBreakTime = true;		//	ブレイクタイムの発生
		m_bTarget = false;			//	ターゲットを拠点に戻す
		m_bSuperArmor = false;
		m_bWalk = true;
		if (m_state != STATE_DAMAGE && m_state != STATE_ROBOTDAMAGE)
		{//ダメージ中じゃなかったら
			m_state = STATE_NONE;
		}

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
//    衝撃波の判定を生成する処理
//=============================================================================
void CTypePower::CreateColliderSphere(void)
{
	//ゲームの取得
	CGame *pGame = CManager::GetGame();
	//エフェクト
	CEffectManager *pEffectManager = pGame->GetEffectManager();

	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVec3TransformCoord(&pos, &m_pModel[15]->GetPos(), &m_pModel[15]->GetMtxWorld());

	// スフィアを生成
	CPlayerAttackSphereCollider *pSphere = CPlayerAttackSphereCollider::Create(D3DXVECTOR3(pos.x + sinf(m_rot.y - D3DX_PI) * 100, pos.y, pos.z + cosf(m_rot.y - D3DX_PI) * 100),
		D3DXVECTOR3(1.0f, 1.0f, 1.0f), 150.0f, 40, 1);

	D3DXVECTOR3 EffectPos = D3DXVECTOR3(pos.x + sinf(m_rot.y - D3DX_PI) * 100, pos.y - 30.0f, pos.z + cosf(m_rot.y - D3DX_PI) * 100);

	//衝撃波のエフェクト表示
	pEffectManager->SetEffect(EffectPos, INITIALIZE_VECTOR3, 1);
	pEffectManager->SetEffect(EffectPos, INITIALIZE_VECTOR3, 2);
	pEffectManager->SetEffect(EffectPos, INITIALIZE_VECTOR3, 3);

	//ひび割れを表示
	CCrack::Create(D3DXVECTOR3(EffectPos.x, 1.0f, EffectPos.z));

	if (pSphere == NULL) { return; }

	//	親を設定
	pSphere->SetParent(this);

}


//=============================================================================
//	スキルを使用する範囲
//=============================================================================
bool CTypePower::CollisionSkillTiming(CCylinderCollider *pCylinderCollider, D3DXVECTOR3 &pos, D3DXVECTOR3 &posOld, D3DXVECTOR3 &Move, D3DXVECTOR3 &ColRange)
{
	bool bLand = false;

	// [[★スキル発動範囲]]
	if (pCylinderCollider->Collision(&pos, &posOld, &Move, SKILL_RANGE, 0.0f, this, &bLand) == true)
	{
		if (m_CharcterType == CCharacter::CHARCTERTYPE_POWER)
		{
			D3DXVECTOR3 thisPos = CCharacter::GetPos(); // 自身の位置情報
			CScene *pParent = pCylinderCollider->GetParent(); // 他キャラの情報を取得

			if (m_bBreakTime == false && m_state != STATE_DAMAGE && m_state != STATE_BLOWAWAY && m_state != STATE_ROBOTDAMAGE) // ブレイクタイムでなければ...
			{
				m_bTrigger = true;	//	使用フラグを立てる
				
				m_bSuperArmor = true;
				CCharacter *pCharacter = (CCharacter*)pParent;
				if (pCharacter == NULL) { return true; }
				D3DXVECTOR3 targetPos = pCharacter->GetPos(); // 対象の位置情報を取得

				// [[★角度調整の処理]]^^^^^^^^^^^^^^^^^^^^^^^^^^^^
				float fDestAngle = atan2f(thisPos.x - targetPos.x, thisPos.z - targetPos.z);
				float fAngle = fDestAngle - m_rot.y;
				if (fAngle > D3DX_PI) { fAngle -= D3DX_PI * 2; }
				if (fAngle < -D3DX_PI) { fAngle += D3DX_PI * 2; }
				m_rot.y += fAngle * 0.1f;
				if (m_rot.y > D3DX_PI) { m_rot.y -= D3DX_PI * 2; }
				if (m_rot.y < -D3DX_PI) { m_rot.y += D3DX_PI * 2; }
				CCharacter::SetPos(targetPos);
				CCharacter::SetRot(m_rot);
				// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
			}
			return true;
		}
	}

	return false;
}