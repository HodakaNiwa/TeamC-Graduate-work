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
#include "RawMouse.h"
#include "select.h"
#include "camera.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MODEL_RENG		(50)
#define MAX_BUNDCNT		(120)
#define MAX_BULLET		(50)
#define SPEED			(5.5f);

//ビューポートの位置設定
#define POS_V	(D3DXVECTOR3(0.0f, 70.0f, -100.0f))
#define POS_R	(D3DXVECTOR3(0.0f, 70.0f, 0.0f))
#define POS_U	(D3DXVECTOR3(0.0f, 1.0f, 0.0f))

//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CCharResult::CCharResult(int nPriority, OBJTYPE objType) : CCharacter(nPriority,objType)
{
	m_pModel = NULL;
	m_pMotion = NULL;
	m_move = INITIALIZE_VECTOR3;
	m_rot = INITIALIZE_VECTOR3;
	m_PlayerType = PLAYERTYPE_SPEED;
	m_MotionType = MOTION_TYPE_NONE;
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CCharResult::~CCharResult(){}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CCharResult::Init(D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40],int nNumPlayer, int nCuntry)
{
	//初期化処理
	m_MotionType = MOTION_TYPE_NONE;
	m_bStopMotion = false;
	m_move = INITIALIZE_VECTOR3;

	//キャラクターの初期化
	CCharacter::Init(nNumPlayer,ModelTxt, MotionTxt,m_CharcterType, nCuntry);
	CCharacter::SetPos(pos);

	//モデルの情報を取得
	m_pModel = CCharacter::GetModel();

	//モーションの情報を取得
	m_pMotion = CCharacter::GetMotion();

	//移動処理の初期化
	InitMove();
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
	if (m_bStopMotion) { return; }
	if (m_pMotion == NULL) { return; }

	UpdateMove();
	CCharacter::Update();

 	int nKey = m_pMotion->GetKey();
	int nFram = m_pMotion->GetnCountMotion();

	if (m_CharcterType == CCharacter::CHARCTERTYPE_POWER)
	{
		if ((nKey == 14) && (nFram == 14)) 
		{ 
			m_bStopMotion = true; 
		}
	}
	else if (m_CharcterType == CCharacter::CHARCTERTYPE_TECHNIQUE)
	{
		if ((nKey == 6) && (nFram == 9))
		{ 
			m_bStopMotion = true;
		}
	}

	if (m_pMotion->GetNumMotion() == (int)m_MotionType) { return; }
	
	switch (m_MotionType)
	{
	case MOTION_TYPE_WIN:
		m_pMotion->SetNumMotion(MOTION_TYPE_WIN);
		break;

	case MOTION_TYPE_NONE:
		m_pMotion->SetNumMotion(MOTION_TYPE_NONE);
		break;
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
CCharResult *CCharResult::Create(D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40], CHARCTERTYPE charactertype, int nCharaNum, int nCuntry)
{
	CCharResult *pPlayer = NULL;
	if (pPlayer == NULL)
	{
		//メモリを動的確保
		pPlayer = new CCharResult;
		if (pPlayer != NULL)
		{//NULLチェック
			pPlayer->m_CharcterType = charactertype;
			pPlayer->Init(pos, ModelTxt,MotionTxt, nCharaNum, nCuntry);
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
// 移動処理の初期化
//=============================================================================
void CCharResult::InitMove(void)
{
	if (m_CharcterType != CCharacter::CHARCTERTYPE_SPEED) { return; }

	D3DXVECTOR3 Pos = CCharacter::GetPos();		//位置を取得
	float fAngle = CFunctionLib::CalcAngleToDest(40.0f, 0.0f, Pos.x, Pos.z);

	//移動量を求める
	m_move.x = sinf(fAngle) * -SPEED;
	m_move.z = cosf(fAngle) * -SPEED;

	//キャラの向きを変える
	CCharacter::SetRot(D3DXVECTOR3(0.0f, fAngle, 0.0F));
}

//=============================================================================
// 移動処理
//=============================================================================
void CCharResult::UpdateMove(void)
{
	if (m_CharcterType != CCharacter::CHARCTERTYPE_SPEED) { return; }

	D3DXVECTOR3 Pos = CCharacter::GetPos();		//位置を取得
	Pos += m_move;

	//位置修正
	int nCount = 0;
	if (Pos.x < 40.0f)
	{
		Pos.x = 40.0f;
		nCount++;
	} 
	if (Pos.z < 0.0f)
	{
		Pos.z = 0.0f;
		nCount++;
	}

	if (nCount == 2) { m_MotionType = MOTION_TYPE_NONE; }

	CCharacter::SetPos(Pos);
}

//=============================================================================
//
//=============================================================================
void  CCharResult::Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size){}