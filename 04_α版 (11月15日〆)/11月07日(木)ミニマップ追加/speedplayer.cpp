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

	//キャラクターの初期化
	CCharacter::Init(ModelTxt, MotionTxt);
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
	CPlayer::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void  CSpeedPlayer::Update(void)
{

	CPlayer::Update();
	//プレイヤーの移動
	//SPlayerMove();
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
CSpeedPlayer *CSpeedPlayer::Create(D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40],int nNumPlayer, TYPE type, PLAYERTYPE playertype)
{
	CSpeedPlayer *pSpeedPlayer = NULL;
	if (pSpeedPlayer == NULL)
	{
		//メモリを動的確保
		pSpeedPlayer = new CSpeedPlayer;
		if (pSpeedPlayer != NULL)
		{//NULLチェック
			pSpeedPlayer->SetType(type);
			pSpeedPlayer->m_PlayerType = playertype;
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
//
////=============================================================================
////
////=============================================================================
//void  CPlayer::Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
//{
//
//}