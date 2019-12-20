//=============================================================================
//
// チュートリアルパワー型の処理 [tutorialpawerplayer.cpp]
// Author : 山下敦史　&　佐藤安純
//
//=============================================================================
#include "tutorialplayer.h"
#include "manager.h"
#include "camera.h"
#include "input.h"
#include "loadEffect.h"
#include "collision.h"
#include "model.h"
#include "rawmouse.h"
#include "UI.h"
#include "skilicon.h"
#include "effectManager.h"
#include "debuglog.h"
#include "tutorial.h"

//*****************************************************************************
// マクロ
//*****************************************************************************
#define WAVE (1)
#define RECASTTIME (5)

//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CTutorialPowerPlayer::CTutorialPowerPlayer()
{
	m_nColliderCnt = 0;
	m_nColliderTimer = 0;
	m_nRecastCounter = 0;
	m_nRecastTimer = 0;
	m_nButtonCounter = 0;
	m_bAction = false;
	m_bRecast = false;
	m_bInit = false;
	m_bUseSkill = false;
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CTutorialPowerPlayer::~CTutorialPowerPlayer(){}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CTutorialPowerPlayer::Init(D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40], int nNumPlayer,int nType)
{
	//プレイヤーのナンバーを格納
	m_nNumPlayer = nNumPlayer;

	//キャラクターの初期化
	CCharacter::Init(nNumPlayer,ModelTxt, MotionTxt, m_CharcterType,nType);
	CCharacter::SetPos(pos);

	//モデルの取得
	m_pModel = CCharacter::GetModel();  

	//ライン変数の初期化
	m_bBlockStartTerritory = false;
	m_pOldStartTerritory = NULL;
	m_bMakeShape = false;

	//モーションの情報を取得
	m_pMotion = CCharacter::GetMotion();
	ResetLine();	//ラインの初期化

	//初期化
	m_bWalk = true;
	m_nColliderCnt = 0;
	m_nColliderTimer = 0;
	m_nRecastCounter = 0;
	m_nRecastTimer = 0;
	m_nButtonCounter = 0;
	m_bAction = false;
	m_bRecast = false;
	m_bUseSkill = false;

	//目的地までのリスト設定
	SetAimPos();

	//移動ルートの設定
	SetMovePlayer(true, 6);
	return S_OK;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CTutorialPowerPlayer::Init(void)
{
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void  CTutorialPowerPlayer::Uninit(void)
{
	//プレイヤーの破棄
	CTutorialPlayer::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void  CTutorialPowerPlayer::Update(void)
{
	//死亡フラグが有効だったら
	if (GetDeth() == true) { return; }

	//アクションの更新
	ActionUpdate();

	//プレイヤーの更新
	CTutorialPlayer::Update();

#if _DEBUG
	DebugInput();
#endif
}

//=============================================================================
// アクションの更新処理
//=============================================================================
void  CTutorialPowerPlayer::ActionUpdate(void)
{
	//サウンドの取得
	CSound *pSound = CManager::GetSound();
	
	//アクション中
	if (m_bAction == true)
	{
		m_nColliderCnt++;
		if (m_nColliderCnt % 60 == 0)
		{
			m_nColliderTimer++;
			if (m_nColliderTimer >= WAVE)
			{
				if (m_PlayerState != PLAYERSTATE_DAMAGE && m_nColliderTimer == 1)
				{
					pSound->PlaySound(CSound::SOUND_LABEL_SE022);
					CreateColliderSphere();	//衝撃波の当たり判定を付与
					m_nColliderTimer = 0;
					m_nColliderCnt = 0;
					m_bAction = false;
				}
			}
		}
	}

	if ((m_bMoveEnd) && (!m_bUseSkill))
	{
		SkillOn();
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void  CTutorialPowerPlayer::Draw(void)
{
	//プレイヤーの描画
	CTutorialPlayer::Draw();
}

//=============================================================================
//オブジェクトの生成処理
//=============================================================================
CTutorialPowerPlayer *CTutorialPowerPlayer::Create(D3DXVECTOR3 pos, int nNumPlayer, int nType)
{
	CTutorialPowerPlayer *pPowerplayer = NULL;
	if (pPowerplayer == NULL)
	{
		//メモリを動的確保
		pPowerplayer = new CTutorialPowerPlayer;
		if (pPowerplayer != NULL)
		{//NULLチェック
			pPowerplayer->SetType(nType);											//ゲーム用国タイプ
			pPowerplayer->m_nCuntry[nNumPlayer] = nType;							//リザルト用の国タイプ
			pPowerplayer->m_CharcterType = CCharacter::CHARCTERTYPE_POWER;			//ゲーム用のキャラタイプ
			pPowerplayer->m_CharcterTypeResult[nNumPlayer] = CHARCTERTYPE_POWER;	//リザルト用のキャラタイプ
			pPowerplayer->Init(pos, "data\\TEXT\\ModelLoad\\POWERPLAYER_LOAD.txt",
							   "data\\TEXT\\MotionLoad\\POWERPLAYER_MOTION.txt", nNumPlayer,nType);		//プレイヤーの初期化
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
void  CTutorialPowerPlayer::Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size){}

//=============================================================================
//    衝撃波の判定を生成する処理
//=============================================================================
void CTutorialPowerPlayer::CreateColliderSphere(void)
{
	if (GetDeth() == true) { return; }

	//エフェクト
	CEffectManager *pEffectManager = CManager::GetTutorial()->GetEffectManager();

	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
	D3DXVec3TransformCoord(&pos, &m_pModel[15]->GetPos(), &m_pModel[15]->GetMtxWorld());

	// スフィアを生成
	CPlayerAttackSphereCollider *pSphere = CPlayerAttackSphereCollider::Create(D3DXVECTOR3(pos.x + sinf(m_rot.y - D3DX_PI) * 100,pos.y,pos.z + cosf(m_rot.y - D3DX_PI) * 100),
		D3DXVECTOR3(1.0f, 1.0f, 1.0f),150.0f, 40, 1);

	D3DXVECTOR3 EffectPos = D3DXVECTOR3(pos.x + sinf(m_rot.y - D3DX_PI) * 100, pos.y - 30.0f, pos.z + cosf(m_rot.y - D3DX_PI) * 100);

	if (pEffectManager != NULL)
	{
		pEffectManager->SetEffect(EffectPos, INITIALIZE_VECTOR3, 1);
		pEffectManager->SetEffect(EffectPos, INITIALIZE_VECTOR3, 2);
		pEffectManager->SetEffect(EffectPos, INITIALIZE_VECTOR3, 3);
	}

	if (pSphere == NULL) { return; }

	// 親を設定
	pSphere->SetParent(this);
}

//=============================================================================
// デバッグ用入力処理
//=============================================================================
void CTutorialPowerPlayer::DebugInput(void)
{
	//入力デバイスの取得
	CInputKeyboard * pKeyboard = CManager::GetInputkeyboard();

	if (pKeyboard->GetKeyboardTrigger(DIK_SPACE) == true)
	{
		SkillOn();
	}
}

//=============================================================================
// スキル発動処理
//=============================================================================
void CTutorialPowerPlayer::SkillOn(void)
{
	m_bAction = true;								//アクション中にする
	m_bUseSkill = true;		//スキルを使用した状態にする
	m_bMoveEnd = false;		//初期化する
	m_PlayerState = PLAYERSTATE_ACTION;
	m_pMotion->SetNumMotion(PLAYERSTATE_ACTION);	//攻撃モーション
}
