//=============================================================================
//
// モデル処理 [model.cpp]
// Author : Kobayashi & Yamashita
//
//=============================================================================
#include "player.h"
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
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_SPRINTTIMER (8)		//スプリント時間
#define RECAST (10)				//スプリントのリキャスト時間
//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CSpeedPlayer::CSpeedPlayer()
{

}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CSpeedPlayer::~CSpeedPlayer()
{

}



//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CSpeedPlayer::Init(D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40],int nNumPlayer)
{
	//プレイヤーのナンバーを格納
	m_nNumPlayer = nNumPlayer;

	//初期化
	m_bWalk = true;
	m_bSprintMotion = false;
	//キャラクターの初期化
	CCharacter::Init(nNumPlayer,ModelTxt, MotionTxt);
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
	m_fSpeed = 1.0f;
	m_nSprintCounter = 0;
	m_nSprintTimer = 0;
	m_nRecastCounter = 0;
	m_nRecastTimer = 0;
	m_nButtonCounter = 0;
	m_bRecast = false;
	m_bSprint = false;

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
	//プレイヤーの終了処理
	CPlayer::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void  CSpeedPlayer::Update(void)
{
	//スピード型のスキル処理
	SprintUpdate();

	//プレイヤーの更新
	CPlayer::Update();
}

//=============================================================================
// スプリント処理
//=============================================================================
void  CSpeedPlayer::SprintUpdate(void)
{
	//Xinputの取得
	CInputXPad * pXPad = CManager::GetXPad();
	CGame *pGame = CManager::GetGame();
	CUI *pUi = pGame->GetUI();
	CRawMouse *pRawMouse = CManager::GetRawMouse();					//RawMouseの取得

	// 接続されているマウスの数を取得
	int nMouseNum = pRawMouse->GetMouseNum();

	//コントローラー（XInput）
	int nControllerIdx = m_nNumPlayer - nMouseNum;
	if (nControllerIdx < 0)
	{
		nControllerIdx = 3;
	}

	if (pXPad->GetTrigger(XINPUT_GAMEPAD_B, nControllerIdx) == true)
	{
		if (m_bRecast == false && m_nButtonCounter == 0)
		{
			m_fSpeed = 1.5f;		//スピード型の速さを1.5倍にする
			m_nButtonCounter = 1;	//ボタンを押せないようにする
			m_bSprint = true;		//スプリント中にする
			m_bSprintMotion = true;	//スプリントモーション中かどうか
			pUi->GetSkilicon(m_nNumPlayer)->RevivalIconMask();
		}
	}

	//スプリント中
	if (m_bSprint == true)
	{
		m_nSprintCounter++;

		if (m_nSprintCounter % 60 == 0)
		{
			m_nSprintTimer++;

			if (m_nSprintTimer <= MAX_SPRINTTIMER)
			{//8秒間速さを上げる
				m_PlayerState = PLAYERSTATE_ACTION;	//スプリント状態

				if (m_bSprintMotion == false)
				{
					//m_bSprintMotion = true;
				}
				//if (m_PlayerState == PLAYERSTATE_BLOWAWAY && m_PlayerState == PLAYERSTATE_NONE && m_PlayerState == PLAYERSTATE_WALK)
				//{//スプリント中にぶつかったらスプリント状態に戻す
				//	m_PlayerState = PLAYERSTATE_ACTION;
				//}
			}
			else
			{//8秒超えたら普通の速さに戻す
				m_fSpeed = 1.0f;					//スピードをもとに戻す
				m_nSprintTimer = 0;					//スプリントタイマーを初期化
				m_bRecast = true;					//リキャスト中にする
				m_bWalk = true;						//移動モーションができる状態にする
				m_PlayerState = PLAYERSTATE_NONE;	//移動状態
				m_bSprintMotion = false;
				m_bSprint = false;
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


}

//=============================================================================
// 描画処理
//=============================================================================
void  CSpeedPlayer::Draw(void)
{
	CPlayer::Draw();
}

//=============================================================================
//オブジェクトの生成処理
//=============================================================================
CSpeedPlayer *CSpeedPlayer::Create(D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40],int nNumPlayer, int nType, CHARCTERTYPE charctertype)
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
			pSpeedPlayer->Init(pos,ModelTxt,MotionTxt,nNumPlayer);			
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
void CSpeedPlayer::UninitOrtbitLine(void)
{
	if (m_pOrbitLine != NULL)
	{
		m_pOrbitLine->Uninit();
		m_pOrbitLine = NULL;
	}
}