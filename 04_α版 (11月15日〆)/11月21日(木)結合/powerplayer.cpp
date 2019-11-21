//=============================================================================
//
// パワー型の処理 [power.cpp]
// Author : Yamashita
//
//=============================================================================
#include "player.h"
#include "manager.h"
#include "camera.h"
#include "input.h"
#include "inputmouce.h"
#include "loadEffect.h"
#include "collision.h"
#include "model.h"
#include "rawmouse.h"
#include "UI.h"
#include "skilicon.h"

//*****************************************************************************
// マクロ
//*****************************************************************************
#define WAVE (1)
#define RECASTTIME (10)

//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CPowerPlayer::CPowerPlayer()
{
	m_nColliderCnt = 0;
	m_nColliderTimer = 0;
	m_nRecastCounter = 0;
	m_nRecastTimer = 0;
	m_nButtonCounter = 0;
	m_fScale = 0.0f;
	m_bAction = false;
	m_bRecast = false;
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CPowerPlayer::~CPowerPlayer()
{

}



//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CPowerPlayer::Init(D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40], int nNumPlayer,int nType)
{
	//プレイヤーのナンバーを格納
	m_nNumPlayer = nNumPlayer;

	//初期化
	m_bWalk = true;

	//キャラクターの初期化
	CCharacter::Init(nNumPlayer,ModelTxt, MotionTxt, m_CharcterType,nType);
	CCharacter::SetPos(pos);

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
		m_pLoadEffect = CLoadEffect::Create(1, D3DXVECTOR3(pos.x, pos.y + 20.0f, pos.z), 5);
	}
	//モーションの情報を取得
	m_pMotion = CCharacter::GetMotion();
	m_fSpeed = 1.0f;
	ResetLine();	//ラインの初期化

	m_nColliderCnt = 0;
	m_nColliderTimer = 0;
	m_nRecastCounter = 0;
	m_nRecastTimer = 0;
	m_nButtonCounter = 0;
	m_fScale = 0.0f;
	m_bAction = false;
	m_bRecast = false;
	m_bCollision = false;
	m_nControllerIndx = 0;
	m_nControllerType = 0;
	m_bSuperArmor = false;
	return S_OK;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CPowerPlayer::Init(void)
{
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void  CPowerPlayer::Uninit(void)
{
	//プレイヤーの破棄
	CPlayer::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void  CPowerPlayer::Update(void)
{
	//アクションの更新
	ActionUpdate();
	//プレイヤーの更新
	CPlayer::Update();
}

//=============================================================================
// アクションの更新処理
//=============================================================================
void  CPowerPlayer::ActionUpdate(void)
{
	if (m_nControllerType == 0)
	{
		//コントローラー操作
		PlayerActionPad();
	}
	else if (m_nControllerType == 1)
	{
		//マウス操作
		PlayerActionMouse();
	}
	
	//アクション中
	if (m_bAction == true)
	{
		m_bSuperArmor = true;
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
				if (m_bAction == false)
				{//アクションが終わったらスーパーアーマを解除
					m_bSuperArmor = false;
					//m_PlayerState = PLAYERSTATE_NONE;
				}
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
			if (m_nRecastTimer <= RECASTTIME)
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
// PAD処理
//=============================================================================
void  CPowerPlayer::PlayerActionPad(void)
{
	CGame *pGame = CManager::GetGame();
	CUI *pUi = pGame->GetUI();
	CInputXPad * pXPad = CManager::GetXPad();


	if (pXPad->GetTrigger(XINPUT_GAMEPAD_B, m_nNumPlayer) == true)
	{
		if (m_bRecast == false && m_nButtonCounter == 0)
		{
			CreateStartUpCollider();	//ハンマーに当たり判定を付与する
			m_pMotion->SetNumMotion(2);	//攻撃モーション
			m_bAction = true;			//アクション中にする
			m_nButtonCounter = 1;		//アクションボタンを使用できないようにする
			m_bCollision = true;
			pUi->GetSkilicon(m_nNumPlayer)->RevivalIconMask();
			m_PlayerState = PLAYERSTATE_ACTION;
		}
	}
}

//=============================================================================
// MOUSE処理
//=============================================================================
void  CPowerPlayer::PlayerActionMouse(void)
{
	CGame *pGame = CManager::GetGame();
	CUI *pUi = pGame->GetUI();
	CRawMouse *pRawMouse = CManager::GetRawMouse();					//RawMouseの取得

	m_nNumPlayer;
	if (pRawMouse->GetTrigger(CRawMouse::RIMS_BUTTON_LEFT, m_nNumPlayer) == true)
	{
		if (m_bRecast == false && m_nButtonCounter == 0)
		{
			CreateStartUpCollider();	//ハンマーに当たり判定を付与する
			m_pMotion->SetNumMotion(2);	//攻撃モーション
			m_bAction = true;			//アクション中にする
			m_nButtonCounter = 1;		//アクションボタンを使用できないようにする
			m_bCollision = true;
			pUi->GetSkilicon(m_nNumPlayer)->RevivalIconMask();
			m_PlayerState = PLAYERSTATE_ACTION;

		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void  CPowerPlayer::Draw(void)
{
	//プレイヤーの描画
	CPlayer::Draw();
}

//=============================================================================
//オブジェクトの生成処理
//=============================================================================
CPowerPlayer *CPowerPlayer::Create(D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40], int nNumPlayer, int nType, 
	CHARCTERTYPE charctertype, int nControllerIndx, int nControllerType)
{
	CPowerPlayer *pPowerplayer = NULL;
	if (pPowerplayer == NULL)
	{
		//メモリを動的確保
		pPowerplayer = new CPowerPlayer;
		if (pPowerplayer != NULL)
		{//NULLチェック
			pPowerplayer->SetType(nType);									//ゲーム用国タイプ
			pPowerplayer->m_nCuntry[nNumPlayer] = nType;					//リザルト用の国タイプ
			pPowerplayer->m_CharcterType = charctertype;					//ゲーム用のキャラタイプ
			pPowerplayer->m_CharcterTypeResult[nNumPlayer] = charctertype;	//リザルト用のキャラタイプ
			pPowerplayer->Init(pos, ModelTxt, MotionTxt, nNumPlayer,nType);		//プレイヤーの初期化
			pPowerplayer->m_nControllerIndx = nControllerIndx;				//使用してるコントローラーの番号
			pPowerplayer->m_nControllerType = nControllerType;				//使用してるコントローラーのタイプ
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
	return pPowerplayer;
}

//=============================================================================
//
//=============================================================================
void  CPowerPlayer::Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
{

}

//=============================================================================
//    ハンマーの当たり判定を生成する処理
//=============================================================================
void CPowerPlayer::CreateStartUpCollider(void)
{
	// 円筒を生成
	CPlayerAttackSphereCollider *pSphere = CPlayerAttackSphereCollider::Create(D3DXVECTOR3(0.0f, 0.0f, -60.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f),
		30.0f, 100,1);

	if (pSphere == NULL) { return; }

	// 親を設定
	pSphere->SetParent(this);

	//武器の場所に判定を付ける
	pSphere->SetParentMtxWorld(&m_pModel[15]->GetMtxWorld());
}

//=============================================================================
//    衝撃波の判定を生成する処理
//=============================================================================
void CPowerPlayer::CreateColliderSphere(void)
{
	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
	D3DXVec3TransformCoord(&pos, &m_pModel[15]->GetPos(), &m_pModel[15]->GetMtxWorld());

	// スフィアを生成
	CPlayerAttackSphereCollider *pSphere = CPlayerAttackSphereCollider::Create(D3DXVECTOR3(pos.x,pos.y,pos.z + 35.0f), 
		D3DXVECTOR3(1.0f, 1.0f, 1.0f),100.0f, 100, 1);

	if (pSphere == NULL) { return; }

	// 親を設定
	pSphere->SetParent(this);

}
////=============================================================================
////    通常状態の更新処理
////=============================================================================
//void CMine::None(void)
//{
//	// 寿命を減らす
//	m_nLife--;
//	if (m_nLife <= 0)
//	{// 寿命がなくなった
//		m_nExplosionTimer = 0;
//		m_nType = TYPE_STARTUP;
//	}
//}

////=============================================================================
////    起動状態の更新処理
////=============================================================================
//void CMine::StartUp(void)
//{
//	// 大きくする
//	m_fScaleUp += MINE_EXPLOSION_SCALEUP;
//	D3DXVECTOR3 Scale = MINE_SCALE_INI;
//	Scale.x += m_fScaleUp;
//	Scale.y += m_fScaleUp;
//	Scale.z += m_fScaleUp;
//	SetScale(Scale);
//
//	// カウンターがたまったら爆発
//	m_nExplosionTimer++;
//	if (m_nExplosionTimer >= MINE_EXPLOSION_STARTTIME)
//	{
//		// カウンターを初期化して状態変更
//		m_nExplosionTimer = 0;
//		m_nType = TYPE_EXPLOSION;
//
//		// 爆発のエフェクトと当たり判定を生成する
//		CreateExplosionEffect();
//		CreateExplosionCollider();
//	}
//}

////=============================================================================
////    爆発のエフェクトを生成する処理
////=============================================================================
//void CPowerPlayer::CreateExplosionEffect(void)
//{
//
//}

////=============================================================================
////    爆発用の当たり判定を生成する処理
////=============================================================================
//void CMine::CreateExplosionCollider(void)
//{
//	// 球を生成
//	CPlayerAttackSphereCollider *pShere = CPlayerAttackSphereCollider::Create(GetPos(), D3DXVECTOR3(1.0f, 1.0f, 1.0f),
//		MINE_EXPLOSION_COLRADIUS, MINE_EXPLOSION_TIME + 2, 1);
//	if (pShere == NULL) { return; }
//
//	// 親を設定
//	pShere->SetParent(this);
//
//	// コライダー管轄クラスにポインタを設定
//	CColliderManager *pColManager = GetColloderManger();
//	pColManager->SetCollider(pShere, 1);
//}

////=============================================================================
////    爆発状態の更新処理
////=============================================================================
//void CMine::Explosion(void)
//{
//	// 爆発の寿命が切れたら終了
//	m_nExplosionTimer++;
//	if (m_nExplosionTimer >= MINE_EXPLOSION_TIME)
//	{
//		Uninit();
//	}
//}