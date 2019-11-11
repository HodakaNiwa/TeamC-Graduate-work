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
//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CPowerPlayer::CPowerPlayer()
{

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
HRESULT CPowerPlayer::Init(D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40], int nNumPlayer)
{
	//プレイヤーのナンバーを格納
	m_nNumPlayer = nNumPlayer;

	//初期化
	m_bWalk = true;

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
	//モーションの情報を取得
	m_pMotion = CCharacter::GetMotion();
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
	CPlayer::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void  CPowerPlayer::Update(void)
{
	CPlayer::Update();
	//プレイヤーの移動
	//SPlayerMove();
}


//=============================================================================
// 描画処理
//=============================================================================
void  CPowerPlayer::Draw(void)
{
	CPlayer::Draw();
}

//=============================================================================
//オブジェクトの生成処理
//=============================================================================
CPowerPlayer *CPowerPlayer::Create(D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40], int nNumPlayer, TYPE type, PLAYERTYPE playertype)
{
	CPowerPlayer *pPowerplayer = NULL;
	if (pPowerplayer == NULL)
	{
		//メモリを動的確保
		pPowerplayer = new CPowerPlayer;
		if (pPowerplayer != NULL)
		{//NULLチェック
			pPowerplayer->SetType(type);
			pPowerplayer->m_PlayerType = playertype;
			pPowerplayer->Init(pos, ModelTxt, MotionTxt, nNumPlayer);
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
//
////=============================================================================
////
////=============================================================================
//void  CPlayer::Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
//{
//
//}