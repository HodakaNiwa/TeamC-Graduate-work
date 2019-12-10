//====================================================================================
//
// 第9戦力処理	[robot.cpp]
// Author;荒谷由朗
//
//====================================================================================
#include "robot.h"
#include "camera.h"
#include "manager.h"
#include "renderer.h"
#include "collision.h"
#include "model.h"
#include "loadEffect.h"
#include "debuglog.h"
#include "effectManager.h"
#include "robotUI.h"
#include "emitter.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CRobot::CRobot(int nPriority, OBJTYPE objType) : CCharacter(nPriority, objType)
{
	m_pos			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nNumTaget		= 0;
	m_nCntAttack	= 0;
	m_fDropTime		= 0.0f;
	m_fJumpTime		= 0.0f;
	m_nCntEnd		= 0;
	m_RobotState	= ROBOTSTATE_APPEARANCE;		// 出現状態に
	m_pRobotUI		= NULL;
	m_pEmitter		= NULL;
	m_nCntEmitter	= 0;
}

//=============================================================================
// デストラクタ
//=============================================================================
CRobot::~CRobot()
{
}

//=============================================================================
// 生成処理
//=============================================================================
CRobot *CRobot::Create(D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40], int nNumPlayer, TYPE type, CHARCTERTYPE charctertype)
{
	CRobot *pRobot = NULL;

	if (pRobot == NULL)
	{
		pRobot = new CRobot;

		if (pRobot != NULL)
		{
			pRobot->m_CharcterType = charctertype;
			pRobot->Init(pos, ModelTxt, MotionTxt, nNumPlayer);
		}
		else
		{
			MessageBox(0, "pRobotがNULLでした", "警告", MB_OK);
		}
	}
	else
	{
		MessageBox(0, "pRobotがNULLじゃないです", "警告", MB_OK);
	}
	return pRobot;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CRobot::Init(D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40], int nNumPlayer)
{
	//プレイヤーのナンバーを取得
	m_nNumPlayer = nNumPlayer;

	//変数の初期化
	m_bPlayerMotionState = false;
	m_bDeth = false;
	m_bEndGravity = false;

	//キャラクターの初期化
	CCharacter::Init(nNumPlayer, ModelTxt, MotionTxt, m_CharcterType, 0);
	CCharacter::SetPos(pos);

	m_pos = pos;

	//モデルの情報を取得
	m_pModel = CCharacter::GetModel();

	//モーションの情報を取得
	m_pMotion = CCharacter::GetMotion();
	m_pMotion->SetNumMotion(m_RobotState);

	return S_OK;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CRobot::Init(void)
{
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void  CRobot::Uninit(void)
{
	CGame *pGame = CManager::GetGame();		// ゲームの取得

	//キャラクターの破棄
	CCharacter::Uninit();

	pGame->SetRobot(NULL);
	if (m_pEmitter != NULL) { m_pEmitter = NULL; }

	ReleaseRobotUI();

	//if (m_pLoadEffect != NULL)
	//{//エフェクトの破棄
	//	m_pLoadEffect->Uninit();
	//	delete m_pLoadEffect;
	//	m_pLoadEffect = NULL;
	//}
}

//=============================================================================
// 更新処理
//=============================================================================
void CRobot::Update(void)
{
	switch (m_RobotState)
	{
	case ROBOTSTATE_NONE:			// ニュートラル
		Search();	// 捜索処理
		break;
	case ROBOTSTATE_WALK:			// 移動
		Wake();		// 移動処理
		Attack();	// 攻撃処理
		break;
	case ROBOTSTATE_APPEARANCE:		// 出現
		Gravity();	// 落下処理
		break;
	case ROBOTSTATE_JUMP:			// ジャンプ
		Jump();		// ジャンプ処理
		break;
	case ROBOTSTATE_ATTACK:			// 近接攻撃
		if (m_pMotion->GetNumMotion() == 0)
		{
			m_RobotState = ROBOTSTATE_NONE;		// ニュートラル状態に
		}
		break;
	case ROBOTSTATE_SHOOT:			// 射撃攻撃
		break;
	}

	if (m_nCntEnd >= ROBOT_ENDTIME && m_RobotState != ROBOTSTATE_JUMP)
	{// 退場時間になったら
		m_RobotState = ROBOTSTATE_JUMP;		// ジャンプ状態に
		m_pMotion->SetNumMotion(m_RobotState);
	}
	m_nCntEnd++;	// 退場カウンターの加算

	//キャラクターの更新
	CCharacter::Update();
	UpdateRobotUI();	// UIの更新

	D3DXVECTOR3 ModelPos = m_pModel[0]->GetPos();

	CDebugProc::Print("ロボット:%f,%f,%f", m_pos.x, m_pos.y, m_pos.z);

	if (m_bDeth) { Uninit(); }
}

//=============================================================================
// 描画処理
//=============================================================================
void  CRobot::Draw(void)
{
	//キャラクターの描画
	CCharacter::Draw();
	DrawRobotUI();	// UIの描画処理
}

//=============================================================================
//
//=============================================================================
void  CRobot::Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
{
}

//=============================================================================
// 捜索の判定
//=============================================================================
void CRobot::Search(void)
{
	CGame *pGame = CManager::GetGame();		// ゲームの取得

	float VecOld = 10000.0f;	// 最短ベクトル

	for (int nCntChara = 0; nCntChara < MAX_CHARACTER; nCntChara++)
	{
		CCharacter *pCharacter = pGame->GetChara(nCntChara);	// キャラクターの取得
		D3DXVECTOR3 CharaPos = pCharacter->GetPos();			// キャラクター座標の取得

		if (pCharacter->GetCharaState() != 4)
		{// プレイヤーが吹っ飛んでないとき
			// ベクトルの計算
			D3DXVECTOR3 VecP = CharaPos - m_pos;					// ベクトルP = 目標 - 自分
			float VecP_Length = D3DXVec3Length(&VecP);				// ベクトルPの長さを算出

			if (VecP_Length <= VecOld)
			{// 計算したベクトルが最短ベクトルより小さかった時
				VecOld = VecP_Length;		// 最短ベクトルの更新
				m_nNumTaget = nCntChara;	// 標的番号の更新
			}
		}
	}

	if (VecOld < 10000.0f)
	{
		m_RobotState = ROBOTSTATE_WALK;	// 移動状態に
		m_pMotion->SetNumMotion(m_RobotState);
	}
}

//=============================================================================
// 移動処理
//=============================================================================
void CRobot::Wake(void)
{
	// データの取得
	CCharacter *pCharacter = CManager::GetGame()->GetChara(m_nNumTaget);	// キャラクターの取得
	D3DXVECTOR3 CharaPos = pCharacter->GetPos();							// キャラクター座標の取得

	// 差分の生成
	float fAngle = atan2f(CharaPos.x - m_pos.x, CharaPos.z - m_pos.z) + D3DX_PI;

	if (fAngle > D3DX_PI)
	{
		fAngle -= D3DX_PI * 2;
	}
	if (fAngle < -D3DX_PI)
	{
		fAngle += D3DX_PI * 2;
	}

	m_rot.y = fAngle;	// 向きに代入

	m_pos.x -= sinf(m_rot.y) * ROBOT_VIGILANCESPEED;
	m_pos.z -= cosf(m_rot.y) * ROBOT_VIGILANCESPEED;

	//	位置・回転情報の反映
	CCharacter::SetPos(m_pos);
	CCharacter::SetRot(m_rot);
}

//=============================================================================
// 攻撃の判定
//=============================================================================
void CRobot::Attack(void)
{
	CGame *pGame = CManager::GetGame();		// ゲームの取得

	for (int nCntChara = 0; nCntChara < MAX_CHARACTER; nCntChara++)
	{
		CCharacter *pCharacter = pGame->GetChara(nCntChara);	// キャラクターの取得
		D3DXVECTOR3 CharaPos = pCharacter->GetPos();			// キャラクター座標の取得

		if (pCharacter->GetCharaState() != 4)
		{// プレイヤーが吹っ飛んでないとき
		// ベクトルの計算
			D3DXVECTOR3 VecP = CharaPos - m_pos;					// ベクトルP = 目標 - 自分
			float VecP_Length = D3DXVec3Length(&VecP);				// ベクトルPの長さを算出

			if (VecP_Length <= ROBOT_ATTACK_RADIUS)
			{// ベクトルPが指定した範囲よりも小さいとき
				m_RobotState = ROBOTSTATE_ATTACK;
				m_pMotion->SetNumMotion(m_RobotState);
				CreateColliderSphere();
			}
		}
	}
}
//=============================================================================
// 落下処理
//=============================================================================
void CRobot::Gravity(void)
{
	CGame *pGame = CManager::GetGame();		// ゲームの取得
	m_fDropTime++;								// 滞空している時間の加算
	float fDrop = m_fDropTime * (ROBOT_GRAVITY/50);		// 落下量 = 滞空している時間 * 重力加速度
	if (m_pos.y + fDrop > 0)
	{// 落下量によって、プレイヤーが地面にめり込まなければ
		m_pos.y += fDrop;
	}
	else
	{
		m_pos.y = 0;			// 地面にめり込むので、地面にめり込まない位置に移動
		m_fDropTime = 0.0f;		// 滞空している時間のリセット

		//エフェクト
		CEffectManager *pEffectManager = pGame->GetEffectManager();

		if (m_bEndGravity == false)
		{
			m_pEmitter = pEffectManager->SetEffect(D3DXVECTOR3(m_pos.x, m_pos.y - 30.0f, m_pos.z), INITIALIZE_VECTOR3, 4);
		}
		m_bEndGravity = true;

		CEventCamera *pEveCam = pGame->GetEveCam();

		if (pEveCam == NULL)
		{
			m_RobotState = ROBOTSTATE_NONE;		// ニュートラル状態に
			m_pMotion->SetNumMotion(m_RobotState);
		}
		else
		{
			if (m_nCntEmitter >= ROBOT_ENDEMITTER)
			{
				if (m_pEmitter != NULL)
				{
					m_pEmitter->Uninit();
					m_pEmitter = NULL;
				}
			}
			m_nCntEmitter++;
		}
	}
	//	位置情報の反映
	CCharacter::SetPos(m_pos);
}

//=============================================================================
// ジャンプ処理
//=============================================================================
void CRobot::Jump(void)
{
	m_fJumpTime++;								// 滞空している時間の加算
	float fDrop = m_fJumpTime * (ROBOT_GRAVITY / 10) * -1.0f;		// 上昇量 = 滞空している時間 * 重力加速度 * 「-」で上昇に
	if (m_pos.y + fDrop < ROBOT_UNINITPOS)
	{// 落下量によって、プレイヤーが地面にめり込まなければ
		m_pos.y += fDrop;
		//	位置情報の反映
		CCharacter::SetPos(m_pos);
	}
	else
	{
		m_bDeth = true;
	}
}

//=============================================================================
//    ロボットUIを生成する処理
//=============================================================================
void CRobot::CreateRobotUI(int nNum)
{
	if (m_pRobotUI != NULL) { return; }
	m_pRobotUI = CRobotUI::Create(nNum);
}

//=============================================================================
// 2つのベクトルABのなす角度θを求める
//=============================================================================
double CRobot::VectorAngle(D3DXVECTOR3 A, D3DXVECTOR3 B)
{
	//ベクトルPとEの長さを計算する
	double Length_A = GetVecLength(A);
	double Length_B = GetVecLength(B);

	//内積とベクトル長さを使ってcosθを求める
	double cos_sita = D3DXVec3Dot(&A, &B) / (Length_A * Length_B);

	//cosθからθを求める
	double sita = acos(cos_sita);

	//ラジアンを0～180の角度にする
	sita = sita * 180.0 / D3DX_PI;

	return sita;
}

void CRobot::CreateOrbitLine(void)
{
}
void CRobot::UninitOrtbitLine(void)
{
}

//==============================================
//			円筒を作成する処理
//==============================================
void CRobot::CreateCylinderCollider(void)
{
	// コライダー管轄クラスを生成
	CColliderManager *pColManager = CColliderManager::Create();
	if (pColManager == NULL) { return; }

	// 円筒を作成
	CCylinderCollider *pCylinder = CCylinderCollider::Create(D3DXVECTOR3(0.0f, 50.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f),
		ROBOT_COLLISION_RADIUS, ROBOT_COLLISION_HEIGHT, false);
	if (pCylinder == NULL) { return; }

	// 親を設定
	pCylinder->SetParent(this);
	pCylinder->SetParentMtxWorld(&GetMtx());

	// コライダー管轄クラスのポインタを設定
	pColManager->SetCollider(pCylinder, 0);
	SetCOlliderManager(pColManager);
}

//=============================================================================
//    衝撃波の判定を生成する処理
//=============================================================================
void CRobot::CreateColliderSphere(void)
{
	D3DXVECTOR3 pos = m_pModel[0]->GetPos();
	//D3DXVec3TransformCoord(&pos, &m_pModel[0]->GetPos(), &m_pModel[0]->GetMtxWorld());
	pos += m_pos;
	// スフィアを生成
	CPlayerAttackSphereCollider *pSphere = CPlayerAttackSphereCollider::Create(D3DXVECTOR3(pos.x, pos.y - 70.0f, pos.z),
		D3DXVECTOR3(1.0f, 1.0f, 1.0f), 100.0f, 100, 1);

	if (pSphere == NULL) { return; }

	// 親を設定
	pSphere->SetParent(this);

}

//=============================================================================
//    ロボットUIを開放する処理
//=============================================================================
void CRobot::ReleaseRobotUI(void)
{
	if (m_pRobotUI != NULL)
	{
		m_pRobotUI->Uninit();
		delete m_pRobotUI;
		m_pRobotUI = NULL;
	}
}

//=============================================================================
//    ロボットUIの更新処理
//=============================================================================
void CRobot::UpdateRobotUI(void)
{
	if (m_pRobotUI != NULL)
	{
		m_pRobotUI->Update();
	}
}

//=============================================================================
//    ロボットUIの描画処理
//=============================================================================
void CRobot::DrawRobotUI(void)
{
	/*if (m_pRobotUI != NULL)
	{
		m_pRobotUI->Draw();
	}*/
}
