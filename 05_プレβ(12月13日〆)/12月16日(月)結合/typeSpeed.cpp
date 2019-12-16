//◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆
//					スピード型 [typeSpeed.h]
//			Author : Kobayashi_Sho-hei / 小林 将兵
//◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆
#include "enemy.h"
#include "manager.h"
#include "camera.h"
#include "input.h"
#include "loadEffect.h"
#include "line.h"
#include "model.h"
#include "debuglog.h"
#include "ripples.h" 

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_SPRINTTIMER (8)		//スプリント時間
#define RECAST (20)				//スプリントのリキャスト時間

//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CTypeSpeed::CTypeSpeed(int nPriority, OBJTYPE objType) : CEnemy(nPriority, objType)
{

}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CTypeSpeed::~CTypeSpeed()
{

}

//=============================================================================
//オブジェクトの生成処理
//=============================================================================
CTypeSpeed *CTypeSpeed::Create(int nChara, int nLevel,int country, CHARCTERTYPE type, D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40])
{
	CTypeSpeed *pSpeedType = NULL;
	if (pSpeedType == NULL)
	{
		pSpeedType = new CTypeSpeed;

		if (pSpeedType != NULL)
		{
			pSpeedType->m_nLevel = nLevel;
			pSpeedType->SetType(country);
			pSpeedType->m_CharcterType = type;
			pSpeedType->Init(nChara, pos, ModelTxt, MotionTxt,country);
			pSpeedType->m_CharcterTypeResult[nChara] = type;
			pSpeedType->m_nCuntry[nChara] = country;
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
HRESULT CTypeSpeed::Init(int nChara, D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40],int country)
{
	m_nEnemyNo = nChara;	//	キャラ番号の記憶(生成順)

	CCharacter::Init(nChara, ModelTxt, MotionTxt,m_CharcterType, country);	//	初期化
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
	m_pLoadEffect = NULL;
	m_pLoadEffectSpeed =  NULL;

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
	if (m_pLoadEffectSpeed == NULL)
	{
		m_pLoadEffectSpeed = CLoadEffect::Create(2, D3DXVECTOR3(pos.x, pos.y + 30.0f, pos.z), 6);
	}

	ResetLine();	//	ラインの初期化

	m_fSpeed = 1.0f;				// 初期速度
	m_nCnt = 0;						// タイマー処理
	m_bSkillFlag = false;			// スキルを発動しているか
	m_bStop = false;				// 複数更新の阻止
	m_nLineNum = 2;					// 最低ライン
	m_state = STATE_NONE;
	m_bSprintMotion = true;
	m_nTimingCnt = 0;
	m_bWalk = true;

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
	if (m_pLoadEffectSpeed != NULL)
	{
		m_pLoadEffectSpeed->Uninit();
		delete m_pLoadEffectSpeed;
		m_pLoadEffectSpeed = NULL;
	}

	//プレイヤーの終了処理
	CEnemy::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void  CTypeSpeed::Update(void)
{
	CGame *pGame = CManager::GetGame();				// ゲームの取得
	CEventCamera *pEveCam = pGame->GetEveCam();		// イベントカメラの取得

	//ゲームの状態を取得
	int nGameState = CGame::GetGameState();

	if (nGameState != CGame::GAMESTATE_FIRSTCOUNTDOWN && nGameState != CGame::GAMESTATE_END)
	{
		if (pEveCam == NULL)
		{
			SprintUpdate();		//	スキル処理
		}

		CEnemy::Update();
	}
	CDebugProc::Print("\nエネミーステート%d", m_state);
}

//=============================================================================
// スキルの発動
//=============================================================================
void  CTypeSpeed::SprintUpdate(void)
{
	D3DXVECTOR3 pos = CCharacter::GetPos();

	// [[AIレベルごとにタイミングが変わる...]]
	if (m_nLevel == 3)//レベル3だけ即スキル使用可能
	{
		Trigger();// スキルの発動
	}
	else // レベル1と2
	{
		m_nTimingCnt += (m_nLevel * 2);
		if (m_nTimingCnt >= 600)
		{
			Trigger();// スキルの発動
		}
	}

	// [[スキル発動後...]]
	if (m_bSkillFlag == true)
	{
		m_nCnt++;
		int nHeight = rand() % 100 + 30;
		m_pLoadEffectSpeed->SetPos(D3DXVECTOR3(pos.x, pos.y + nHeight, pos.z));
		m_pLoadEffectSpeed->OnTimeEffect();
		if (m_nCnt == (MAX_SPRINTTIMER * 60))	//	一定時間で元の速度に
		{
			m_fSpeed = 1.0f;		//	スピードをもとに戻す
			m_nCnt = 0;				//	スプリントタイマーを初期化
			m_bWalk = true;			//	移動モーションができる状態にする
			m_state = STATE_NONE;	
			m_bSkillFlag = false;
		}

		//波紋のエフェクトを表示
		if ((m_nCnt % 15) == 0)
		{
			D3DXVECTOR3 Pos = CCharacter::GetPos();
			CRipples::Create(D3DXVECTOR3(Pos.x,1.0f,Pos.z + 20.0f));
		}
		else if ((m_nCnt % 20) == 0)
		{
			D3DXVECTOR3 Pos = CCharacter::GetPos();
			CRipples::Create(D3DXVECTOR3(Pos.x, 1.0f, Pos.z - 20.0f));
		}
	}
	// [[再使用まで...]]
	else
	{
		m_nCnt++;
		if (m_nCnt == (RECAST * 60))
		{
			m_nCnt = 0;
			m_bStop = false;
			m_nTimingCnt = 0;
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void  CTypeSpeed::Draw(void)
{
	CEnemy::Draw();

	if (m_pLoadEffectSpeed != NULL)
	{
		m_pLoadEffectSpeed->Draw();
	}
}

//=============================================================================
// スキルの発動
//=============================================================================
void CTypeSpeed::Trigger(void)
{
	if (m_bSkillFlag == false && m_bStop == false)
	{
		m_bStop = true;
		m_state = STATE_ACTION;
		m_fSpeed = 1.5f;		// スピード型の速さを1.5倍にする
		m_bSkillFlag = true;		// スプリント状態にする
		m_bSprintMotion = true;
		m_nCnt = 0;
	}
}
//=============================================================================
//
//=============================================================================
void  CTypeSpeed::Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
{ }

