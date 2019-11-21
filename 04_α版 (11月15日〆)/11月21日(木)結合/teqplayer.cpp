//=============================================================================
//
// 地雷型の処理 [teq.cpp]
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
#define RECOVERY_MINE_POINT (5)		//マイン使用ポイントの回復量
#define RECOVERY_TIME (2)			//使用ポイント回復までの時間

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
HRESULT CTechniquePlayer::Init(D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40], int nNumPlayer,int nType)
{
	//プレイヤーのナンバーを格納
	m_nNumPlayer = nNumPlayer;

	//初期化
	m_bWalk = true;

	//キャラクターの初期化
	CCharacter::Init(nNumPlayer, ModelTxt, MotionTxt, m_CharcterType,nType);
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
	//アクションの更新
	ActionUpdate();
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
	{
		m_PlayerState = PLAYERSTATE_ACTION;

		m_bSuperArmor = true;
		m_nInstallationCounter++;
		if (m_nInstallationCounter % 30 == 0)
		{
			m_nInstallationTimer++;
			if (m_nInstallationTimer == 1)
			{
				//地雷を設置
				CMine::Create(pos, pCharacter);
				m_nInstallationTimer = 0;
				m_bMineUse = false;
				m_bWalk = true;
				if (m_bMineUse == false)
				{//地雷設置が終わったらスーパーアーマを解除
					m_bSuperArmor = false;
					m_PlayerState = PLAYERSTATE_NONE;
				}
			}
		}
	}

	//回復カウンター
	m_nReCounter++;
	if (m_nReCounter % 120 == 0)
	{
		//回復タイマー
		m_nReTimer++;
		if (m_nReTimer == RECOVERY_TIME)
		{
			//3秒ごとに2回復する
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
// PAD処理
//=============================================================================
void  CTechniquePlayer::PlayerActionPad(void)
{
	CGame *pGame = CManager::GetGame();
	CUI *pUi = pGame->GetUI();
	CInputXPad * pXPad = CManager::GetXPad();
	CCharacter *pCharacter = pGame->GetChara(m_nNumPlayer);

	if (pXPad->GetTrigger(XINPUT_GAMEPAD_B, m_nNumPlayer) == true)
	{
		//マインポイントが20以上なら設置可能
		if (m_nMinePoint >= REDUCED_MINE_POINT)
		{
			m_pMotion->SetNumMotion(2);
			m_nButtonCounter = 1;	//
			pUi->GetSkilicon(m_nNumPlayer)->RevivalIconMask();	//スキルアイコン
			m_bMineUse = true;
		}
	}
}

//=============================================================================
// MOUSE処理
//=============================================================================
void  CTechniquePlayer::PlayerActionMouse(void)
{
	CGame *pGame = CManager::GetGame();
	CUI *pUi = pGame->GetUI();
	CRawMouse *pRawMouse = CManager::GetRawMouse();					//RawMouseの取得
	

	if (pRawMouse->GetTrigger(CRawMouse::RIMS_BUTTON_LEFT, m_nNumPlayer) == true)
	{
		//マインポイントが20以上なら設置可能
		if (m_nMinePoint >= REDUCED_MINE_POINT)
		{
			m_pMotion->SetNumMotion(2);
			m_nButtonCounter = 1;	//
			pUi->GetSkilicon(m_nNumPlayer)->RevivalIconMask();	//スキルアイコン
			m_bMineUse = true;
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
			pTeqPlayer->Init(pos, ModelTxt, MotionTxt, nNumPlayer,nType);		//プレイヤーの初期化
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


