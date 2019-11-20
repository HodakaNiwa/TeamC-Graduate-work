//=============================================================================
//
// リザルト用プレイヤーの処理 [charResult.cpp]
// Author : 佐藤 安純　[ SATO ASUMI ]
//
//=============================================================================
#include "charResult.h"
#include "input.h"
#include "camera.h"
#include "manager.h"
#include "renderer.h"
#include "scene2D.h"
#include "scene3D.h"
#include "game.h"
#include "title.h"
#include "player.h"
#include "tutorial.h"
#include "collision.h"
#include "model.h"
#include "Territory.h"
#include "library.h"
#include "line.h"
#include "loadEffect.h"
#include "inputmouce.h"
#include "RawMouse.h"
#include "select.h"
#include "camera.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define SPEED			(2.0f)
#define MODEL_RENG		(50)
#define MAX_BUNDCNT		(120)
#define MAX_BULLET		(50)

//ビューポートの位置設定
#define POS_V	(D3DXVECTOR3(0.0f, 70.0f, -100.0f))
#define POS_R	(D3DXVECTOR3(0.0f, 70.0f, 0.0f))
#define POS_U	(D3DXVECTOR3(0.0f, 1.0f, 0.0f))

//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CCharResult::CCharResult(int nPriority, OBJTYPE objType) : CCharacter(nPriority,objType){}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CCharResult::~CCharResult(){}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CCharResult::Init(D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40],int nNumPlayer)
{
	//プレイヤーのナンバーを取得
	m_PlayerState = PLAYERSTATE_NONE;

	//キャラクターの初期化
	CCharacter::Init(nNumPlayer,ModelTxt, MotionTxt);
	CCharacter::SetPos(pos);

	//モデルの情報を取得
	m_pModel = CCharacter::GetModel();

	//モーションの情報を取得
	m_pMotion = CCharacter::GetMotion();
	return S_OK;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CCharResult::Init(void)
{
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void  CCharResult::Uninit(void)
{
	CCharacter::Uninit();
	
	//モーションの破棄
	if (m_pMotion != NULL)
	{
		m_pMotion = NULL;
	}

	//モデルの破棄
	if (m_pModel != NULL)
	{
		m_pModel = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void  CCharResult::Update(void)
{
	CCharacter::Update();
	//CGame *pGame = CManager::GetGame();

	int nGameState = CGame::GetGameState();

	if (nGameState != CGame::GAMESTATE_END && nGameState != CGame::GAMESTATE_FIRSTCOUNTDOWN)
	{
		switch (m_PlayerState)
		{
		case PLAYERSTATE_NONE: break;
		case PLAYERSTATE_WALK: break;
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void  CCharResult::Draw(void)
{
	CCharacter::Draw();
}

//=============================================================================
//オブジェクトの生成処理
//=============================================================================
CCharResult *CCharResult::Create(D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40])
{
	CCharResult *pPlayer = NULL;
	if (pPlayer == NULL)
	{
		//メモリを動的確保
		pPlayer = new CCharResult;
		if (pPlayer != NULL)
		{//NULLチェック
			pPlayer->Init(pos, ModelTxt,MotionTxt, 0);
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
	return pPlayer;
}

//=============================================================================
//
//=============================================================================
void  CCharResult::Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size){}

//=============================================================================
// ラインの生成処理
//=============================================================================
void CCharResult::CreateOrbitLine(void){}

//=============================================================================
// ラインの破棄
//=============================================================================
void CCharResult::UninitOrtbitLine(void){}