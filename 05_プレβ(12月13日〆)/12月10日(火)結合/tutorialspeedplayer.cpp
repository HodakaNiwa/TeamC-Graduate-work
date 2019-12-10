//=============================================================================
//
// チュートリアルスピード型の処理 [tutorialspeedplayer.cpp]
// Author : 小林将兵 & 山下敦史 & 佐藤安純
//
//=============================================================================
#include "tutorialplayer.h"
#include "manager.h"
#include "camera.h"
#include "input.h"
#include "loadEffect.h"
#include "line.h"
#include "sceneOrbit.h"
#include "model.h"
#include "UI.h"
#include "skilicon.h"
#include "RawMouse.h"
#include "debuglog.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_SPRINTTIMER (8)		//スプリント時間
#define RECAST (20)				//スプリントのリキャスト時間 20
//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CTutorialSpeedPlayer::CTutorialSpeedPlayer(){}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CTutorialSpeedPlayer::~CTutorialSpeedPlayer(){}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CTutorialSpeedPlayer::Init(D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40],int nNumPlayer, int nType)
{
	//プレイヤーのナンバーを格納
	m_nNumPlayer = nNumPlayer;

	//初期化
	m_bWalk = true;
	m_bSprintMotion = false;
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
	m_pLoadEffectSpeed = NULL;

	//コピーラインの初期化
	for (int nCnt = 0; nCnt < MAX_TERRITORY; nCnt++)
	{
		m_apCopyLine[nCnt] = NULL;
	}

	//エフェクトの初期化
	if (m_pLoadEffect == NULL)
	{
		m_pLoadEffect = CLoadEffect::Create(1, D3DXVECTOR3(pos.x, pos.y + 60.0f, pos.z), 6);
	}
	if (m_pLoadEffectSpeed == NULL)
	{
		m_pLoadEffectSpeed = CLoadEffect::Create(2, D3DXVECTOR3(pos.x, pos.y + 30.0f, pos.z), 6);
	}

	//モーションの情報を取得
	m_pMotion = CCharacter::GetMotion();
	ResetLine();	//ラインの初期化

	//スピードプレイヤー用の変数を初期化
	m_nSprintCounter = 0;
	m_nSprintTimer = 0;
	m_nRecastCounter = 0;
	m_nRecastTimer = 0;
	m_nButtonCounter = 0;
	m_bRecast = false;
	m_bSprint = false;
	m_bSprintMotion = false;
	m_fInertia = GetInertia();			//慣性
	m_bUseSkill = false;

	//目的地までのリスト設定
	SetAimPos();

	//目的地までのルートを設定
	SetMovePlayer(true, 1);

	return S_OK;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CTutorialSpeedPlayer::Init(void)
{
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void  CTutorialSpeedPlayer::Uninit(void)
{
	//プレイヤーの終了処理
	CTutorialPlayer::Uninit();

	if (m_pLoadEffectSpeed != NULL)
	{
		m_pLoadEffectSpeed->Uninit();
		delete m_pLoadEffectSpeed;
		m_pLoadEffectSpeed = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void  CTutorialSpeedPlayer::Update(void)
{
	//ゲームの状態を取得
	int nGameState = CGame::GetGameState();

	if (nGameState != CGame::GAMESTATE_FIRSTCOUNTDOWN && nGameState != CGame::GAMESTATE_END)
	{
		//スピード型のスキル処理
		SprintUpdate();
	}

	if ((m_nNumPos == 3) && (!m_bUseSkill))
	{//スキルを使用した状態にする
		SkillOn();
		m_bUseSkill = true;
		m_fSpeed = 4.6f;
	}

	//プレイヤーの更新
	CTutorialPlayer::Update();
}

//=============================================================================
// スプリント処理
//=============================================================================
void  CTutorialSpeedPlayer::SprintUpdate(void)
{
	D3DXVECTOR3 pos = CCharacter::GetPos();							//位置の取得

	//スプリント中
	if (m_bSprint == true)
	{
		int nHeight = rand() % 100 + 30;
		m_nSprintCounter++;
		m_pLoadEffectSpeed->SetPos(D3DXVECTOR3(pos.x, pos.y + nHeight, pos.z));
		m_pLoadEffectSpeed->OnTimeEffect();

		if (m_nSprintCounter % 60 == 0)
		{
			m_nSprintTimer++;
			if (m_nSprintTimer >= MAX_SPRINTTIMER)
			{//8秒超えたら普通の速さに戻す
				m_fSpeed = 1.0f;					//スピードをもとに戻す
				m_nSprintTimer = 0;					//スプリントタイマーを初期化
				m_nSprintCounter = 0;
				m_bWalk = true;						//移動モーションができる状態にする
				m_bSprintMotion = false;
				m_bSprint = false;

				if (m_PlayerState != PLAYERSTATE_DAMAGE)
				{
					m_PlayerState = PLAYERSTATE_NONE;	//移動状態
				}
			}
		}
	}

	////リキャスト中
	//if (m_bRecast == true)
	//{
	//	m_nRecastCounter++;
	//	if (m_nRecastCounter % 60 == 0)
	//	{
	//		m_nRecastTimer++;
	//		if (m_nRecastTimer <= RECAST)
	//		{//10秒以下だったらスプリントが使用できない
	//			m_bSprint = false;
	//		}
	//		else
	//		{//10秒たったらスプリントを使用できるようにする

	//			m_bRecast = false;		//リキャスト中じゃない
	//			m_nRecastTimer = 0;		//リキャストタイマーの初期化
	//			m_nButtonCounter = 0;	//スキルボタンを押せるようにする
	//			m_bSprint = false;
	//		}
	//	}
	//}

#if _DEBUG
	DebugInput();	//デバッグ用入力処理
#endif
}

////=============================================================================
//// PAD処理
////=============================================================================
//void  CTutorialSpeedPlayer::PlayerActionPad(void)
//{
//	CGame *pGame = CManager::GetGame();
//	CUI *pUi = pGame->GetUI();
//	CInputXPad * pXPad = CManager::GetXPad();
//
//	if (pXPad->GetTrigger(XINPUT_GAMEPAD_B, m_nControllerIndx) == true)
//	{
//		if (m_bRecast == false && m_nButtonCounter == 0
//			&& m_PlayerState != PLAYERSTATE_DAMAGE && m_PlayerState != PLAYERSTATE_BLOWAWAY)
//		{
//			m_fSpeed = 1.5f;		//スピード型の速さを1.5倍にする
//			m_nButtonCounter = 1;	//ボタンを押せないようにする
//			m_bSprint = true;		//スプリント中にする
//			m_bSprintMotion = true;	//スプリントモーション中かどうか
//			pUi->GetSkilicon(m_nNumPlayer)->RevivalIconMask();
//			//m_PlayerState = PLAYERSTATE_ACTION;	//スプリント状態
//
//		}
//	}
//}

//=============================================================================
// 描画処理
//=============================================================================
void  CTutorialSpeedPlayer::Draw(void)
{
	CTutorialPlayer::Draw();

	if (m_pLoadEffectSpeed != NULL)
	{
		m_pLoadEffectSpeed->Draw();
	}
}

//=============================================================================
//オブジェクトの生成処理
//=============================================================================
CTutorialSpeedPlayer *CTutorialSpeedPlayer::Create(D3DXVECTOR3 pos, int nNumPlayer, int nType)
{
	CTutorialSpeedPlayer *pSpeedPlayer = NULL;
	if (pSpeedPlayer == NULL)
	{
		//メモリを動的確保
		pSpeedPlayer = new CTutorialSpeedPlayer;
		if (pSpeedPlayer != NULL)
		{//NULLチェック
			pSpeedPlayer->SetType(nType);														//ゲーム用国タイプ
			pSpeedPlayer->m_nCuntry[nNumPlayer] = nType;										//リザルト用の国タイプ
			pSpeedPlayer->m_CharcterType = CCharacter::CHARCTERTYPE_SPEED;						//ゲーム用のキャラタイプ
			pSpeedPlayer->m_CharcterTypeResult[nNumPlayer] = CCharacter::CHARCTERTYPE_SPEED;	//リザルト用のキャラタイプ
			pSpeedPlayer->Init(pos, "data\\TEXT\\ModelLoad\\SPEEDPLAYER_LOAD.txt", "data\\TEXT\\MotionLoad\\SPEEDPLAYER_MOTION.txt", nNumPlayer,nType);
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
	return pSpeedPlayer;
}

//=============================================================================
// スキル発動処理
//=============================================================================
void CTutorialSpeedPlayer::SkillOn(void)
{
	m_fSpeed = 1.5f;		//スピード型の速さを1.5倍にする
	m_nButtonCounter = 1;	//ボタンを押せないようにする
	m_bSprint = true;		//スプリント中にする
	m_bSprintMotion = true;	//スプリントモーション中かどうか
}

//=============================================================================
// デバッグ用入力処理
//=============================================================================
void CTutorialSpeedPlayer::DebugInput(void)
{
	CInputKeyboard * pKeyboard = CManager::GetInputkeyboard();

	if (pKeyboard->GetKeyboardTrigger(DIK_SPACE) == true)
	{//スキル発動処理
		SkillOn();
	}
}

//=============================================================================
//
//=============================================================================
void  CTutorialSpeedPlayer::Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size){}

//=============================================================================
// ラインの生成処理
//=============================================================================
void CTutorialSpeedPlayer::CreateOrbitLine(void){}

//=============================================================================
// ラインの破棄
//=============================================================================
void CTutorialSpeedPlayer::UninitOrtbitLine(void){}