//=============================================================================
//
// 地雷型の処理 [teqolayer.cpp]
// Author : 山下敦史
//
//=============================================================================
#include "player.h"
#include "manager.h"
#include "camera.h"
#include "input.h"
#include "loadEffect.h"
#include "collision.h"
#include "model.h"
#include "UI.h"
#include "skilicon.h"
#include "RawMouse.h"
#include "mine.h"

//*****************************************************************************
// マクロ
//*****************************************************************************
#define RECASTTIME (10)				//リキャスト時間
#define MAX_MINE_POINT (100)		//マインを使用できるポイント
#define REDUCED_MINE_POINT (20)		//マインを使用した時に減るポイント
#define RECOVERY_MINE_POINT (2)		//マイン使用ポイントの回復量
#define RECOVERY_TIME (1)			//使用ポイント回復までの時間

//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CTechniquePlayer::CTechniquePlayer()
{
	m_nRecastCounter = 0;
	m_nRecastTimer = 0;
	m_nButtonCounter = 0;
	m_bRecast = false;
	m_nMinePoint = 0;
	m_nReCounter = 0;			//回復カウンター
	m_nReTimer = 0;				//回復タイマー
	m_nInstallationCounter = 0;
	m_nInstallationTimer = 0;
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CTechniquePlayer::~CTechniquePlayer()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CTechniquePlayer::Init(D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40], int nNumPlayer, int nType)
{
	//プレイヤーのナンバーを格納
	m_nNumPlayer = nNumPlayer;

	//初期化
	m_bWalk = true;

	//キャラクターの初期化
	CCharacter::Init(nNumPlayer, ModelTxt, MotionTxt, m_CharcterType, nType);
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
		m_pLoadEffect = CLoadEffect::Create(1, D3DXVECTOR3(pos.x, pos.y + 60.0f, pos.z), 5);
	}
	//モーションの情報を取得
	m_pMotion = CCharacter::GetMotion();
	ResetLine();	//ラインの初期化

					//各変数の初期化
	m_nRecastCounter = 0;
	m_nRecastTimer = 0;
	m_nButtonCounter = 0;
	m_bRecast = false;
	m_bCollision = false;
	m_nControllerIndx = 0;
	m_nControllerType = 0;
	m_nReCounter = 0;
	m_nReTimer = 0;
	m_nMinePoint = MAX_MINE_POINT;
	m_nInstallationCounter = 0;
	m_nInstallationTimer = 0;
	m_bMineUse = false;
	m_nDamageCounter = 0;
	m_fSpeed = 1.0f;
	m_bSuperArmor = false;
	m_fInertia = GetInertia();			//慣性
	m_bInit = false;
	m_nInitCnt = 0;
	return S_OK;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CTechniquePlayer::Init(void)
{
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void  CTechniquePlayer::Uninit(void)
{
	//プレイヤーの破棄
	CPlayer::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void  CTechniquePlayer::Update(void)
{
	//ゲームの状態を取得
	int nGameState = CGame::GetGameState();

	if (nGameState != CGame::GAMESTATE_FIRSTCOUNTDOWN && nGameState != CGame::GAMESTATE_END)
	{
		//アクションの更新
		ActionUpdate();
	}

	//プレイヤーの更新
	CPlayer::Update();
}

//=============================================================================
// アクションの更新処理
//=============================================================================
void  CTechniquePlayer::ActionUpdate(void)
{
	//Xinputの取得
	CInputXPad * pXPad = CManager::GetXPad();
	CRawMouse *pRawMouse = CManager::GetRawMouse();					//RawMouseの取得
	CGame *pGame = CManager::GetGame();
	CCharacter *pCharacter = pGame->GetChara(m_nNumPlayer);
	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVec3TransformCoord(&pos, &m_pModel[5]->GetPos(), &m_pModel[5]->GetMtxWorld());

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

	//0以下になるまでマインを使用できる
	if (m_nMinePoint <= MAX_MINE_POINT)
	{
		if (m_nButtonCounter == 1)
		{
			//地雷の使用ポイントを減らす
			m_nMinePoint -= REDUCED_MINE_POINT;
			m_nButtonCounter = 0;
			if (m_nMinePoint <= 0)
			{
				m_nMinePoint = 0;	//0にする
			}
		}
	}

	if (m_bMineUse == true)
	{//マインを設置中
		m_bSuperArmor = true;			//吹っ飛ばなくなる
		m_nInstallationCounter++;

		if (m_nInstallationCounter % 30 == 0)
		{
			m_nInstallationTimer++;
			if (m_nInstallationTimer == 1)
			{
				if (m_PlayerState != PLAYERSTATE_DAMAGE && m_PlayerState != PLAYERSTATE_BLOWAWAY)
				{
					//地雷を設置
					CMine::Create(pos, pCharacter);
				}
				m_bMineUse = false;	//マイン設置終了
				m_bInit = true;		//値の初期化
			}
		}
	}

	//値の初期化
	if (m_bInit == true)
	{
		m_nInitCnt++;
		if (m_nInitCnt >= 18)
		{
			m_nInstallationTimer = 0;
			m_nInitCnt = 0;
			m_bSuperArmor = false;
			m_bWalk = true;
			m_bInit = false;
			if (m_PlayerState != PLAYERSTATE_DAMAGE)
			{
				m_PlayerState = PLAYERSTATE_NONE;
			}
		}
	}

	//回復カウンター
	m_nReCounter++;
	if (m_nReCounter % 60 == 0)
	{
		//回復タイマー
		m_nReTimer++;
		if (m_nReTimer == RECOVERY_TIME)
		{
			//1秒ごとに2回復する
			m_nMinePoint += RECOVERY_MINE_POINT;
			m_nReTimer = 0;

			if (m_nMinePoint >= MAX_MINE_POINT)
			{
				//100を超えたら
				m_nMinePoint = MAX_MINE_POINT;
			}
		}
	}
}

//=============================================================================
// コントローラーの処理
//=============================================================================
void  CTechniquePlayer::PlayerActionPad(void)
{
	CGame *pGame = CManager::GetGame();
	CUI *pUi = pGame->GetUI();
	CInputXPad * pXPad = CManager::GetXPad();
	CCharacter *pCharacter = pGame->GetChara(m_nNumPlayer);
	//サウンドの取得
	CSound *pSound = CManager::GetSound();
	if (pXPad->GetTrigger(XINPUT_GAMEPAD_B, m_nControllerIndx) == true)
	{
		m_nMinePoint = 100;
	}
	if (pXPad->GetTrigger(XINPUT_GAMEPAD_B, m_nControllerIndx) == true)
	{
		if (m_PlayerState != PLAYERSTATE_DAMAGE && m_PlayerState != PLAYERSTATE_ACTION
			&& m_PlayerState != PLAYERSTATE_BLOWAWAY)
		{
			//マインポイントが20以上なら設置可能
			if (m_nMinePoint >= REDUCED_MINE_POINT)
			{
				m_PlayerState = PLAYERSTATE_ACTION;
				m_pMotion->SetNumMotion(2);
				m_nButtonCounter = 1;	//
				pUi->GetSkilicon(m_nNumPlayer)->RevivalIconMask();	//スキルアイコン
				m_bMineUse = true;
				pSound->PlaySound(CSound::SOUND_LABEL_SE024);
				//pSound->SetVolume(CSound::SOUND_LABEL_SE024, 30.0f);
			}
		}
	}
}

//=============================================================================
// マウス操作の処理
//=============================================================================
void  CTechniquePlayer::PlayerActionMouse(void)
{
	CGame *pGame = CManager::GetGame();
	CUI *pUi = pGame->GetUI();
	CRawMouse *pRawMouse = CManager::GetRawMouse();					//RawMouseの取得
	CInputKeyboard * pInputKeyboard = CManager::GetInputkeyboard();	//キーボードの取得
																	//サウンドの取得
	CSound *pSound = CManager::GetSound();

	if (pRawMouse->GetTrigger(m_nControllerIndx, CRawMouse::RIMS_BUTTON_RIGHT) == true)
	{
		if (m_PlayerState != PLAYERSTATE_DAMAGE && m_PlayerState != PLAYERSTATE_ACTION
			&& m_PlayerState != PLAYERSTATE_BLOWAWAY)
		{
			//マインポイントが20以上なら設置可能
			if (m_nMinePoint >= REDUCED_MINE_POINT)
			{
				m_PlayerState = PLAYERSTATE_ACTION;

				m_pMotion->SetNumMotion(2);
				m_nButtonCounter = 1;	//
				pUi->GetSkilicon(m_nNumPlayer)->RevivalIconMask();	//スキルアイコン
				m_bMineUse = true;
				pSound->PlaySound(CSound::SOUND_LABEL_SE024);
				//pSound->SetVolume(CSound::SOUND_LABEL_SE024, 30.0f);
			}
		}
	}
}

//=============================================================================
// キーボード操作の処理
//=============================================================================
void  CTechniquePlayer::PlayerActionKeyboard(void)
{
	CGame *pGame = CManager::GetGame();
	CUI *pUi = pGame->GetUI();
	CInputKeyboard * pInputKeyboard = CManager::GetInputkeyboard();	//キーボードの取得
	CSound *pSound = CManager::GetSound();							//サウンドの取得
	if (pInputKeyboard->GetKeyboardPress(DIK_RETURN) == true)
	{
		if (m_PlayerState != PLAYERSTATE_DAMAGE && m_PlayerState != PLAYERSTATE_ACTION
			&& m_PlayerState != PLAYERSTATE_BLOWAWAY)
		{
			//マインポイントが20以上なら設置可能
			if (m_nMinePoint >= REDUCED_MINE_POINT)
			{
				m_PlayerState = PLAYERSTATE_ACTION;

				m_pMotion->SetNumMotion(2);
				m_nButtonCounter = 1;	//
				pUi->GetSkilicon(m_nControllerIndx)->RevivalIconMask();	//スキルアイコン
				m_bMineUse = true;
				pSound->PlaySound(CSound::SOUND_LABEL_SE024);
				pSound->SetVolume(CSound::SOUND_LABEL_SE024, 20.0f);
			}
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void  CTechniquePlayer::Draw(void)
{
	//プレイヤーの描画
	CPlayer::Draw();
}

//=============================================================================
//オブジェクトの生成処理
//=============================================================================
CTechniquePlayer *CTechniquePlayer::Create(D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40], int nNumPlayer, int nType,
	CHARCTERTYPE charctertype, int nControllerIndx, int nControllerType)
{
	CTechniquePlayer *pTeqPlayer = NULL;
	if (pTeqPlayer == NULL)
	{
		//メモリを動的確保
		pTeqPlayer = new CTechniquePlayer;
		if (pTeqPlayer != NULL)
		{//NULLチェック
			pTeqPlayer->SetType(nType);									//ゲーム用国タイプ
			pTeqPlayer->m_nCuntry[nNumPlayer] = nType;					//リザルト用の国タイプ
			pTeqPlayer->m_CharcterType = charctertype;					//ゲーム用のキャラタイプ
			pTeqPlayer->m_CharcterTypeResult[nNumPlayer] = charctertype;	//リザルト用のキャラタイプ
			pTeqPlayer->Init(pos, ModelTxt, MotionTxt, nNumPlayer, nType);		//プレイヤーの初期化
			pTeqPlayer->m_nControllerIndx = nControllerIndx;			//使用してるコントローラーの番号
			pTeqPlayer->m_nControllerType = nControllerType;			//使用してるコントローラーのタイプ

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
	return pTeqPlayer;
}

//=============================================================================
//
//=============================================================================
void  CTechniquePlayer::Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
{

}


