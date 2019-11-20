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
#define MAX_MINE_POINT (100)		//マインの置けるポイント
#define REDUCED_MINE_POINT (20)		//マインを置いたときに減るポイント

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
HRESULT CTechniquePlayer::Init(D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40], int nNumPlayer)
{
	//プレイヤーのナンバーを格納
	m_nNumPlayer = nNumPlayer;

	//初期化
	m_bWalk = true;

	//キャラクターの初期化
	CCharacter::Init(nNumPlayer, ModelTxt, MotionTxt);
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
		m_pLoadEffect = CLoadEffect::Create(4, D3DXVECTOR3(pos.x, pos.y + 60.0f, pos.z), 5);
	}
	//モーションの情報を取得
	m_pMotion = CCharacter::GetMotion();
	m_fSpeed = 1.0f;
	ResetLine();	//ラインの初期化

	m_nRecastCounter = 0;
	m_nRecastTimer = 0;
	m_nButtonCounter = 0;

	m_bRecast = false;
	m_bCollision = false;
	m_nControllerIndx = 0;
	m_nControllerType = 0;
	m_nMinePoint = MAX_MINE_POINT;
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
		if (m_bRecast == false && m_nButtonCounter == 0)
		{
			//地雷を設置
			CMine::Create(CCharacter::GetPos(), pCharacter);

			m_nButtonCounter = 0;	//ボタンを押せないようにする

			pUi->GetSkilicon(m_nNumPlayer)->RevivalIconMask();	//スキルアイコン

			m_bRecast = false;
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
	CCharacter *pCharacter = pGame->GetChara(m_nNumPlayer);


	if (pRawMouse->GetTrigger(CRawMouse::RIMS_BUTTON_LEFT, m_nNumPlayer) == true)
	{
		if (m_bRecast == false && m_nButtonCounter == 0)
		{
			//地雷を設置
			CMine::Create(CCharacter::GetPos(), pCharacter);
			m_nButtonCounter = 1;								//ボタンを押せないようにする
			pUi->GetSkilicon(m_nNumPlayer)->RevivalIconMask();	//スキルアイコン

			m_bRecast = false;
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
			pTeqPlayer->Init(pos, ModelTxt, MotionTxt, nNumPlayer);		//プレイヤーの初期化
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


