//=============================================================================
//
// スピード型の処理 [speedplayer.cpp]
// Author : 小林将兵 & 山下敦史
//
//=============================================================================
#include "player.h"
#include "manager.h"
#include "camera.h"
#include "input.h"
#include "loadEffect.h"
#include "line.h"
#include "model.h"
#include "UI.h"
#include "skilicon.h"
#include "RawMouse.h"
#include "debuglog.h"
#include "ripples.h" 

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_SPRINTTIMER (8)		//スプリント時間
#define RECAST (20)				//スプリントのリキャスト時間 20
//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CSpeedPlayer::CSpeedPlayer()
{
	m_bWalk = true;
	m_bSprintMotion = false;
	m_pLoadEffectSpeed = NULL;
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CSpeedPlayer::~CSpeedPlayer(){}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CSpeedPlayer::Init(D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40],int nNumPlayer, int nType)
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
	m_bBlockStartTerritory = false;
	m_pOldStartTerritory = NULL;
	m_bMakeShape = false;
	m_pLoadEffect = NULL;
	m_pLoadEffectSpeed = NULL;

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
	m_fSpeed = 1.0f;
	m_nSprintCounter = 0;
	m_nSprintTimer = 0;
	m_nRecastCounter = 0;
	m_nRecastTimer = 0;
	m_nButtonCounter = 0;
	m_bRecast = false;
	m_bSprint = false;
	m_nControllerIndx = 0;
	m_nControllerType = 0;
	m_bSprintMotion = false;
	m_fInertia = GetInertia();			//慣性

	return S_OK;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CSpeedPlayer::Init(void)
{
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void  CSpeedPlayer::Uninit(void)
{
	if (m_pLoadEffectSpeed != NULL)
	{
		m_pLoadEffectSpeed->Uninit();
		delete m_pLoadEffectSpeed;
		m_pLoadEffectSpeed = NULL;
	}
	//プレイヤーの終了処理
	CPlayer::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void  CSpeedPlayer::Update(void)
{
	CGame *pGame = CManager::GetGame();				// ゲームの取得
	CEventCamera *pEveCam = pGame->GetEveCam();		// イベントカメラの取得

	CSound *pSound = CManager::GetSound();

	//ゲームの状態を取得
	int nGameState = CGame::GetGameState();

	if (nGameState != CGame::GAMESTATE_FIRSTCOUNTDOWN && nGameState != CGame::GAMESTATE_END)
	{
		if (pEveCam == NULL)	// イベントカメラが消えていたら
		{
			//スピード型のスキル処理
			SprintUpdate();
		}
		else
		{//イベントカメラがNULLじゃなかったら
			pSound->StopSound(CSound::SOUND_LABEL_SE030);
		}
	}
	//プレイヤーの更新
	CPlayer::Update();
}

//=============================================================================
// スプリント処理
//=============================================================================
void  CSpeedPlayer::SprintUpdate(void)
{
	CGame *pGame = CManager::GetGame();
	CUI *pUi = pGame->GetUI();
	CInputKeyboard * pInputKeyboard = CManager::GetInputkeyboard();	//キーボードの取得
	CSound *pSound = CManager::GetSound();

	D3DXVECTOR3 pos = CCharacter::GetPos();							//位置の取得

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
	else
	{
		//キーボード操作
		PlayerActionKeyboard();
	}
	
	//スプリント中
	if (m_bSprint == true)
	{
		pUi->GetSkilicon(m_nNumPlayer)->RevivalIconMask();
		m_nSprintCounter++;
		
		if (m_pLoadEffectSpeed != NULL)
		{
			//エフェクトの高さをランダムで
			int nHeight = rand() % 100 + 30;
			m_pLoadEffectSpeed->SetPos(D3DXVECTOR3(pos.x, pos.y + nHeight, pos.z));
			m_pLoadEffectSpeed->OnTimeEffect();
		}

		if (m_nSprintCounter % 60 == 0)
		{
			m_nSprintTimer++;
			if (m_nSprintTimer >= MAX_SPRINTTIMER)
			{//8秒超えたら普通の速さに戻す
				m_fSpeed = 1.0f;					//スピードをもとに戻す
				m_nSprintTimer = 0;					//スプリントタイマーを初期化
				m_bRecast = true;					//リキャスト中にする
				m_bWalk = true;						//移動モーションができる状態にする
				m_bSprintMotion = false;
				m_bSprint = false;

				if (m_PlayerState != PLAYERSTATE_DAMAGE && m_PlayerState != PLAYERSTATE_ROBOTDAMAGE)
				{
					m_PlayerState = PLAYERSTATE_NONE;	//待機状態
				}
			}
		}
		//波紋のエフェクトを表示
		if ((m_nSprintCounter % 15) == 0)
		{
			D3DXVECTOR3 Pos = CCharacter::GetPos();
			CRipples::Create(D3DXVECTOR3(Pos.x, 1.0f, Pos.z + 20.0f));
		}
		else if ((m_nSprintCounter % 20) == 0)
		{
			D3DXVECTOR3 Pos = CCharacter::GetPos();
			CRipples::Create(D3DXVECTOR3(Pos.x, 1.0f, Pos.z - 20.0f));
		}
	}

	//リキャスト中
	if (m_bRecast == true)
	{
		pSound->StopSound(CSound::SOUND_LABEL_SE030);
		m_nRecastCounter++;
		if (m_nRecastCounter % 60 == 0)
		{
			m_nRecastTimer++;
			if (m_nRecastTimer <= RECAST)
			{//10秒以下だったらスプリントが使用できない
				m_bSprint = false;
			}
			else
			{//10秒たったらスプリントを使用できるようにする

				m_bRecast = false;		//リキャスト中じゃない
				m_nRecastTimer = 0;		//リキャストタイマーの初期化
				m_nButtonCounter = 0;	//スキルボタンを押せるようにする
				m_bSprint = false;
			}
		}
	}

	//スプリントの設定
	SetSprint(m_bSprint);
}

//=============================================================================
// PAD処理
//=============================================================================
void  CSpeedPlayer::PlayerActionPad(void)
{
	//情報の取得
	CGame *pGame = CManager::GetGame();
	CUI *pUi = pGame->GetUI();
	CInputXPad * pXPad = CManager::GetXPad();
	CSound *pSound = CManager::GetSound();

	if (pXPad->GetTrigger(XINPUT_GAMEPAD_B, m_nControllerIndx) == true)
	{
		if (m_bRecast == false && m_nButtonCounter == 0
			&& m_PlayerState != PLAYERSTATE_DAMAGE && m_PlayerState != PLAYERSTATE_BLOWAWAY)
		{
			pSound->PlaySound(CSound::SOUND_LABEL_SE030);
			m_fSpeed = 1.5f;		//スピード型の速さを1.5倍にする
			m_nButtonCounter = 1;	//ボタンを押せないようにする
			m_bSprint = true;		//スプリント中にする
			m_bSprintMotion = true;	//スプリントモーション中かどうか
		}
	}
}

//=============================================================================
// MOUSE処理
//=============================================================================
void  CSpeedPlayer::PlayerActionMouse(void)
{
	//情報の取得
	CGame *pGame = CManager::GetGame();
	CUI *pUi = pGame->GetUI();
	CRawMouse *pRawMouse = CManager::GetRawMouse();					//RawMouseの取得
	CSound *pSound = CManager::GetSound();

	if (pRawMouse->GetTrigger(m_nControllerIndx,CRawMouse::RIMS_BUTTON_RIGHT) == true)
	{
		if (m_bRecast == false && m_nButtonCounter == 0
			&& m_PlayerState != PLAYERSTATE_DAMAGE && m_PlayerState != PLAYERSTATE_BLOWAWAY && m_PlayerState != PLAYERSTATE_ROBOTDAMAGE)
		{
			pSound->PlaySound(CSound::SOUND_LABEL_SE030);
			m_fSpeed = 1.5f;		//スピード型の速さを1.5倍にする
			m_nButtonCounter = 1;	//ボタンを押せないようにする
			m_bSprint = true;		//スプリント中にする
			m_bSprintMotion = true;	//スプリントモーション中かどうか
		}
	}
}

//=============================================================================
// MOUSE処理
//=============================================================================
void  CSpeedPlayer::PlayerActionKeyboard(void)
{
	//情報の取得
	CGame *pGame = CManager::GetGame();
	CUI *pUi = pGame->GetUI();
	CInputKeyboard * pInputKeyboard = CManager::GetInputkeyboard();	//キーボードの取得
	CSound *pSound = CManager::GetSound();

	if (pInputKeyboard->GetKeyboardPress(DIK_RETURN) == true)
	{
		if (m_bRecast == false && m_nButtonCounter == 0
			&& m_PlayerState != PLAYERSTATE_DAMAGE && m_PlayerState != PLAYERSTATE_BLOWAWAY && m_PlayerState != PLAYERSTATE_ROBOTDAMAGE)
		{
			pSound->PlaySound(CSound::SOUND_LABEL_SE030);
			m_fSpeed = 1.5f;		//スピード型の速さを1.5倍にする
			m_nButtonCounter = 1;	//ボタンを押せないようにする
			m_bSprint = true;		//スプリント中にする
			m_bSprintMotion = true;	//スプリントモーション中かどうか
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void  CSpeedPlayer::Draw(void)
{
	//描画処理
	CPlayer::Draw();

	if (m_pLoadEffectSpeed != NULL)
	{//エフェクトの描画
		m_pLoadEffectSpeed->Draw();
	}
}

//=============================================================================
//オブジェクトの生成処理
//=============================================================================
CSpeedPlayer *CSpeedPlayer::Create(D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40],int nNumPlayer, int nType,
	CHARCTERTYPE charctertype, int nControllerIndx, int nControllerType)
{
	CSpeedPlayer *pSpeedPlayer = NULL;
	if (pSpeedPlayer == NULL)
	{
		//メモリを動的確保
		pSpeedPlayer = new CSpeedPlayer;
		if (pSpeedPlayer != NULL)
		{//NULLチェック
			pSpeedPlayer->SetType(nType);									//ゲーム用国タイプ
			pSpeedPlayer->m_nCuntry[nNumPlayer] = nType;					//リザルト用の国タイプ
			pSpeedPlayer->m_CharcterType = charctertype;					//ゲーム用のキャラタイプ
			pSpeedPlayer->m_CharcterTypeResult[nNumPlayer] = charctertype;	//リザルト用のキャラタイプ
			pSpeedPlayer->Init(pos,ModelTxt,MotionTxt,nNumPlayer,nType);			
			pSpeedPlayer->m_nControllerIndx = nControllerIndx;				//使用してるコントローラーの番号
			pSpeedPlayer->m_nControllerType = nControllerType;				//使用してるコントローラーのタイプ
		}
		else
		{
			MessageBox(0, "スピードプレイヤーがNULLでした", "警告", MB_OK);
		}
	}
	else
	{
		MessageBox(0, "スピードプレイヤーがNULLじゃないです", "警告", MB_OK);
	}
	return pSpeedPlayer;
}

//=============================================================================
//
//=============================================================================
void  CSpeedPlayer::Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
{

}

//=============================================================================
// ラインの生成処理
//=============================================================================
void CSpeedPlayer::CreateOrbitLine(void)
{
	return;
}

//=============================================================================
// ラインの破棄
//=============================================================================
void CSpeedPlayer::UninitOrtbitLine(void)
{

}