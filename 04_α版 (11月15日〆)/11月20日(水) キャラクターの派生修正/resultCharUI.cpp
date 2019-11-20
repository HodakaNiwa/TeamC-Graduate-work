//=============================================================================
//
// リザルトのキャラ表示の処理 [resultCharUI.cpp]
// Author : 佐藤 安純　[ SATO ASUMI ]
//
//=============================================================================
#include "resultCharUI.h"
#include "moveUI.h"
#include "charMultRendering.h"
#include "manager.h"
#include "result.h"
#include "select.h"
#include "game.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define FLAM_TEX	(2)
#define NUM_TEX		(3) 
#define CHARNO_TEX	(5)
#define CPU_TEX_NO	(6)
#define P_TEX_NO	(7)
#define MULT_POSV	(D3DXVECTOR3(0.0f, 80.0f, -70.0f))
#define MULT_POSR	(D3DXVECTOR3(0.0f, 80.0f, 0.0f))
#define RANK_POS1	(D3DXVECTOR3(-120.0f, -80.0f, 0.0f))
#define RANK_POS2	(D3DXVECTOR3(-100.0f, -70.0f, 0.0f))
#define RANK_POS3	(D3DXVECTOR3(-50.0f, -70.0f, 0.0f))

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================

//=============================================================================
// 生成処理
//=============================================================================
CResultCharUI * CResultCharUI::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, int nNumPlayer, int nRank, int nCuntry, int nType)
{
	CResultCharUI *pSelectChar = new CResultCharUI;
	pSelectChar->Init(pos, size, nNumPlayer, nRank, nCuntry, nType);

	return pSelectChar;
}

//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CResultCharUI::CResultCharUI() {}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CResultCharUI::~CResultCharUI() {}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CResultCharUI::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, int nNumPlayer, int nRank, int nCuntry, int nType)
{
	//変数の初期化
	m_pCharMult = NULL;
	m_pBack = NULL;
	m_pFlam = NULL;
	m_pRank = NULL;
	m_pCharNo = NULL;
	m_pCharType = NULL;
	m_pCharMultRender = NULL;
	m_nNumPlayer = nNumPlayer;	//プレイヤー番号
	m_nCountTime = 0;			//時間の加算
	m_nRank = nRank;

	//位置修正
	D3DXVECTOR3 Size = size;
	if (nRank > 2) { Size.x += 5.0f; }

	if (m_pBack == NULL)
	{//背景
		m_pBack = CMoveUI::Create(pos, D3DXVECTOR3(Size.x - 8.0f, 0.0f, 0.0f), CManager::GetResult()->GetCuntryTex(nCuntry));
		m_pBack->SetSizeVertex(size.y - 3, size.y - 15.0f);
	}

	//マルチレンダリング用テクスチャの生成
	LoadMultRendering(nCuntry, nType);

	if ((m_pCharMult == NULL) && (m_pCharMultRender != NULL))
	{
		if (nRank > 2) { Size.x += 30.0f; }
		m_pCharMult = CMoveUI::Create(pos, D3DXVECTOR3(Size.x - 8.0f, 0.0f, 0.0f), m_pCharMultRender->GetTexture());
		m_pCharMult->SetSizeVertex(size.y - 3, size.y - 15.0f);
	}

	//キャラナンバーフレームの生成
	if (m_pCharNo == NULL)
	{
		if (nRank <= 2)
		{
			m_pCharNo = CMoveUI::Create(pos, D3DXVECTOR3(60.0f, 15.0f, 0.0f), CManager::GetResult()->GetTexture(CHARNO_TEX));
		}
		else
		{
			m_pCharNo = CMoveUI::Create(pos, D3DXVECTOR3(30.0f, 10.0f, 0.0f), CManager::GetResult()->GetTexture(CHARNO_TEX));
		}

		//色の設定
		if (m_pCharNo != NULL) { m_pCharNo->SetColoer(CCharacter::m_CountryColor[m_nNumPlayer]); }
	}

	//キャラタイプの生成
	if (m_pCharType == NULL)
	{
		if (m_nNumPlayer < CManager::GetResult()->GetMaxPlayer())
		{//プレイヤー用
			m_pCharType = CMoveUI::Create(pos, D3DXVECTOR3(60.0f, 25.0f, 0.0f), CManager::GetResult()->GetTexture(P_TEX_NO));
			m_pCharType->SetTexUV(0.0f, 1.0f, 0.0f + (m_nNumPlayer * 0.25f), 0.25f + (m_nNumPlayer * 0.25f));
		}
		else
		{//CPU用
			if (nRank > 2)
			{
				m_pCharType = CMoveUI::Create(pos, D3DXVECTOR3(25.0f, 13.0f, 0.0f), CManager::GetResult()->GetTexture(CPU_TEX_NO));
			}
			else
			{
				m_pCharType = CMoveUI::Create(pos, D3DXVECTOR3(40.0f, 18.0f, 0.0f), CManager::GetResult()->GetTexture(CPU_TEX_NO));
			}
		}

		//色の設定
		if (m_pCharType != NULL) 
		{ 
			if (nCuntry == 0)
			{
				m_pCharType->SetColoer(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
			}
			else
			{
				m_pCharType->SetColoer(CCharacter::m_CountryColor[m_nNumPlayer]);
			}
		}
	}


	if (m_pFlam == NULL)
	{//フレーム
		m_pFlam = CMoveUI::Create(pos, size, CManager::GetResult()->GetTexture(FLAM_TEX));
		m_pFlam->SetSizeVertex(size.y, size.y - 10.0f);
	}

	if (m_pRank == NULL)
	{
		D3DXVECTOR3 Pos = pos;
		D3DXVECTOR3 Size = INITIALIZE_VECTOR3;

		if(nRank < 2)
		{
			Pos += RANK_POS1;
			Size = D3DXVECTOR3(75.0f - (nRank * 15.0f), 75.0f - (nRank * 15.0f), 0.0f);
		}
		else if (nRank == 2)
		{
			Pos += RANK_POS2;
			Size = D3DXVECTOR3(50.0f, 50.0f, 0.0f);
		}
		else if (nRank > 2)
		{
			Pos += RANK_POS3;
			Size = D3DXVECTOR3(35.0f, 35.0f, 0.0f);
		}

		m_pRank = CMoveUI::Create(Pos, Size, CManager::GetResult()->GetTexture(NUM_TEX));
		m_pRank->SetTex(0.125f, nRank + 1);
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void  CResultCharUI::Uninit(void)
{
	m_pCharMultRender = NULL;

	m_pCharMult = NULL;
	m_pBack = NULL;
	m_pFlam = NULL;
	m_pCharNo = NULL;
	m_pCharType = NULL;
}

//=============================================================================
// 更新処理
//=============================================================================
void  CResultCharUI::Update(void){}

//=============================================================================
// 描画状態の設定
//=============================================================================
void CResultCharUI::SetDraw(bool bDraw)
{
	if (m_pBack != NULL) { m_pBack->SetDraw(bDraw); }			//背景
	if (m_pCharMult != NULL) { m_pCharMult->SetDraw(bDraw); }	//キャラ表示
	if (m_pFlam != NULL) { m_pFlam->SetDraw(bDraw); }			//フレーム
	if (m_pRank != NULL) { m_pRank->SetDraw(bDraw); }			//ランキング
	if (m_pCharNo != NULL) { m_pCharNo->SetDraw(bDraw); }		//キャラクターナンバーフレーム
	if (m_pCharType != NULL) { m_pCharType->SetDraw(bDraw); }	//キャラのタイプ
}

//=============================================================================
// 目的の位置まで移動
//=============================================================================
void CResultCharUI::UpdateMoveDest(D3DXVECTOR3 AimPos, float fSpeed)
{
	if (m_pBack != NULL) { m_pBack->UpdateMoveDestVertex(AimPos, fSpeed); }			//背景
	if (m_pCharMult != NULL) { m_pCharMult->UpdateMoveDestVertex(AimPos, fSpeed); }	//キャラ表示
	if (m_pFlam != NULL) { m_pFlam->UpdateMoveDestVertex(AimPos, fSpeed); }			//フレーム

	if (m_pCharNo != NULL)
	{//キャラクターナンバーフレーム
		D3DXVECTOR3 Pos = AimPos;

		if (m_nRank <= 2)
		{	
			Pos += D3DXVECTOR3(80.0f + (m_nRank * -15.0f), 73.0f + (m_nRank * -5.0f), 0.0f);
			m_pCharNo->UpdateMoveDest(Pos, fSpeed);
		}
		else
		{
			Pos += D3DXVECTOR3(20.0f, 60.0f, 0.0f);
			m_pCharNo->UpdateMoveDest(Pos, fSpeed);
		}
	}	

	if (m_pCharType != NULL)
	{//キャラタイプ
		D3DXVECTOR3 Pos = AimPos;

		if (m_nRank <= 2)
		{
			Pos += D3DXVECTOR3(83.0f + (m_nRank * -15.0f), 60.0f + (m_nRank * -5.0f), 0.0f);
			m_pCharType->UpdateMoveDest(Pos, fSpeed);
		}
		else
		{
			Pos += D3DXVECTOR3(20.0f, 50.0f, 0.0f);
			m_pCharType->UpdateMoveDest(Pos, fSpeed);
		}
	}



	if (m_pRank != NULL) 
	{//ランキングの移動
		D3DXVECTOR3 Pos = AimPos;

		if (m_nRank < 2)
		{
			Pos += RANK_POS1;
		}
		else if (m_nRank == 2)
		{
			Pos += RANK_POS2;
		}
		else if (m_nRank > 2)
		{
			Pos += RANK_POS3;
		}

		m_pRank->UpdateMoveDest(Pos, fSpeed);
	}
}

//=============================================================================
// マルチレンダリング用テクスチャの読み込み
//=============================================================================
void CResultCharUI::LoadMultRendering(int nCuntry, int nType)
{
	//変数の初期化
	m_pCharMultRender = NULL;
	
	if (nType == 0)
	{
		m_pCharMultRender = CCharMultRender::Create(INITIALIZE_VECTOR3, "data\\TEXT\\ModelLoad\\SPEEDPLAYER_LOAD.txt", "data\\TEXT\\MotionLoad\\SPEEDPLAYER_MOTION.txt",CCharacter::CHARCTERTYPE_SPEED, m_nNumPlayer, nCuntry);
	}
	else if (nType == 1)
	{
		m_pCharMultRender = CCharMultRender::Create(INITIALIZE_VECTOR3, "data\\TEXT\\ModelLoad\\POWERPLAYER_LOAD.txt", "data\\TEXT\\MotionLoad\\POWERPLAYER_MOTION.txt",CCharacter::CHARCTERTYPE_POWER, m_nNumPlayer, nCuntry);
	}
	else
	{
		m_pCharMultRender = CCharMultRender::Create(INITIALIZE_VECTOR3, "data\\TEXT\\ModelLoad\\TEQPLAYER_LOAD.txt", "data\\TEXT\\MotionLoad\\TEQPLAYER_MOTION.txt", CCharacter::CHARCTERTYPE_TECHNIQUE, m_nNumPlayer, nCuntry);
	}

	//視点の設定
	if(m_pCharMultRender != NULL)
	{
		m_pCharMultRender->SetPosV(INITIALIZE_VECTOR3 + MULT_POSV); 
		m_pCharMultRender->SetPosR(INITIALIZE_VECTOR3 + MULT_POSR);
	}
}