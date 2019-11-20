//◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆
//					スピード型 [typeSpeed.h]
//			Author : Kobayashi_Sho-hei / 小林 将兵
//◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆
#include "enemy.h"
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
CTypeSpeed::CTypeSpeed() 
{ 
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CTypeSpeed::~CTypeSpeed() { }

//=============================================================================
//オブジェクトの生成処理
//=============================================================================
CTypeSpeed *CTypeSpeed::Create(int nChara, int country, CHARCTERTYPE type, D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40])
{
	CTypeSpeed *pSpeedType = NULL;
	if (pSpeedType == NULL)
	{
		pSpeedType = new CTypeSpeed;

		if (pSpeedType != NULL)
		{
			pSpeedType->SetType(country);
			pSpeedType->m_CharcterType = type;
			pSpeedType->Init(nChara, pos, ModelTxt, MotionTxt);
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
	return pSpeedType;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CTypeSpeed::Init(int nChara, D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40])
{
	m_bWalk = true;
	
	m_nEnemyNo = nChara;	//	キャラ番号の記憶(生成順)

	CCharacter::Init(nChara,ModelTxt, MotionTxt);	//	初期化
	CCharacter::SetPos(pos);						//	位置反映

	InitSort(pos);	//	ゲーム開始時の近場のエリア・テリトリーを見つける
	
	m_pModel = CCharacter::GetModel();	//	モデル情報の取得
	m_pMotion = CCharacter::GetMotion();//	モーション情報の取得

	//	---ライン変数の初期化---
	m_nMaxLineTime = FIRST_LINETIME;
	m_nLineTime = m_nMaxLineTime;
	m_bBlockStartTerritory = false;
	m_pOldStartTerritory = NULL;
	m_nCountTime = 0;
	m_pOrbitLine = NULL;
	m_bMakeShape = false;
	m_nCntTimeCopyLine = 0;


	//	コピーラインの初期化
	for (int nCnt = 0; nCnt < MAX_TERRITORY; nCnt++)
	{
		m_apCopyLine[nCnt] = NULL;
	}

	//	エフェクトの初期化
	if (m_pLoadEffect == NULL)
	{
		m_pLoadEffect = CLoadEffect::Create(1, D3DXVECTOR3(pos.x, pos.y + 60.0f, pos.z), 8);
	}

	
	ResetLine();	//	ラインの初期化

	
	m_fSpeed = 1.0f;				//	初期速度
	m_nSprintCounter = 0;
	m_nSprintTimer = 0;
	m_nRecastCounter = 0;
	m_nRecastTimer = 0;
	m_bRecast = false;
	m_bSprint = false;
	m_nCreateTime = (rand() % 4);	//	始点に戻るまでの時間調整
	return S_OK;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CTypeSpeed::Init(void)
{
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void  CTypeSpeed::Uninit(void)
{
	if (m_pLoadEffect != NULL)
	{//エフェクトの破棄
		m_pLoadEffect->Uninit();
		delete m_pLoadEffect;
		m_pLoadEffect = NULL;
	}

	//プレイヤーの終了処理
	CEnemy::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void  CTypeSpeed::Update(void)
{
	SprintUpdate();		//	スキル処理
	CEnemy::Update();
}

//=============================================================================
// スプリント処理
//=============================================================================
void  CTypeSpeed::SprintUpdate(void)
{
	
	if (m_bRecast == false && m_bCheckS == false)
	{
		m_bCheckS = true;
		m_state = STATE_ACTION;	//スプリント状態
		m_fSpeed = 1.5f;		//スピード型の速さを1.5倍にする
		m_bSprint = true;		//スプリント中にする
	}

	//スプリント中
	if (m_bSprint == true)
	{
		m_nSprintCounter++;

		if (m_nSprintCounter % 60 == 0)
		{
			m_nSprintTimer++;
			
			if (m_nSprintTimer >= MAX_SPRINTTIMER)	//	一定時間で元の速度に
			{
				m_fSpeed = 1.0f;					//スピードをもとに戻す
				m_nSprintTimer = 0;					//スプリントタイマーを初期化
				m_bRecast = true;					//リキャスト中にする
				m_bWalk = true;						//移動モーションができる状態にする
				m_state = STATE_NONE;				//移動状態
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
				m_bCheckS = false;
				m_bRecast = false;		//リキャスト中じゃない
				m_nRecastTimer = 0;		//リキャストタイマーの初期化
				m_bSprint = false;

			}
		}
	}


}

//=============================================================================
// 描画処理
//=============================================================================
void  CTypeSpeed::Draw(void)
{
	CEnemy::Draw();
}

//=============================================================================
//
//=============================================================================
void  CTypeSpeed::Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
{

}

//=============================================================================
// ラインの生成処理
//=============================================================================
void CTypeSpeed::CreateOrbitLine(void)
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
void CTypeSpeed::UninitOrtbitLine(void)
{
	if (m_pOrbitLine != NULL)
	{
		m_pOrbitLine->Uninit();
		m_pOrbitLine = NULL;
	}
}