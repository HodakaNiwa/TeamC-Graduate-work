//◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆
//					地雷型 [typeSweeper.h]
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
#include "mine.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define RECASTTIME (10)				//リキャスト時間
#define MAX_MINE_POINT (100)		//マインを使用できるポイント
#define REDUCED_MINE_POINT (20)		//マインを使用した時に減るポイント
#define RECOVERY_MINE_POINT (2)		//マイン使用ポイントの回復量
#define RECOVERY_TIME (3)			//使用ポイント回復までの時間

//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CTypeSweeper::CTypeSweeper()
{

}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CTypeSweeper::~CTypeSweeper()
{

}

//=============================================================================
//オブジェクトの生成処理
//=============================================================================
CTypeSweeper *CTypeSweeper::Create(int nChara, int country, CHARCTERTYPE type, D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40])
{
	CTypeSweeper *pSpeedType = NULL;
	if (pSpeedType == NULL)
	{
		pSpeedType = new CTypeSweeper;

		if (pSpeedType != NULL)
		{
			pSpeedType->SetType(country);
			pSpeedType->m_CharcterType = type;
			pSpeedType->Init(nChara, pos, ModelTxt, MotionTxt, country);
			//佐藤追加しました
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
HRESULT CTypeSweeper::Init(int nChara, D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40],int country)
{
	//プレイヤーのナンバーを格納
	m_nEnemyNo = nChara;

	//初期化
	m_bWalk = true;
	m_bSprintMotion = false;
	//キャラクターの初期化
	CCharacter::Init(nChara,ModelTxt, MotionTxt,m_CharcterType,country);
	CCharacter::SetPos(pos);

	InitSort(pos);	//	ゲーム開始時の近場のエリア・テリトリーを見つける


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
		m_pLoadEffect = CLoadEffect::Create(1, D3DXVECTOR3(pos.x, pos.y + 60.0f, pos.z), 8);
	}

	//モーションの情報を取得
	m_pMotion = CCharacter::GetMotion();
	ResetLine();	//ラインの初期化

	
					//	各変数の初期化
	m_fSpeed = 1.0f;
	m_nRecastCounter = 0;
	m_nRecastTimer = 0;
	m_nButtonCounter = 0;
	m_bRecast = false;
	m_nReCounter = 0;
	m_nReTimer = 0;
	m_nMinePoint = MAX_MINE_POINT;
	m_nInstallationCounter = 0;
	m_nInstallationTimer = 0;
	m_nLineNum = 2;
	m_nTiming = 0;
	m_bStop = false;

	return S_OK;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CTypeSweeper::Init(void)
{
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void  CTypeSweeper::Uninit(void)
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
void  CTypeSweeper::Update(void)
{
	//ゲームの状態を取得
	int nGameState = CGame::GetGameState();

	if (nGameState != CGame::GAMESTATE_FIRSTCOUNTDOWN && nGameState != CGame::GAMESTATE_END)
	{
		ActionUpdate();	//	スキル処理
		CEnemy::Update();
	}
}



//=============================================================================
// 描画処理
//=============================================================================
void  CTypeSweeper::Draw(void)
{
	CEnemy::Draw();
}

//=============================================================================
//
//=============================================================================
void  CTypeSweeper::Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
{

}
//=============================================================================
// スプリント処理
//=============================================================================
void  CTypeSweeper::ActionUpdate(void)
{
	if (m_nMinePoint == MAX_MINE_POINT)	//	満タン時とりあえず地雷を置く
	{
		Process();

	}

	if (m_apTerritory[0] != NULL)
	{
		m_nTiming++;
		if (m_nTiming == 50) { Process(); }
		if (m_nTiming == 100) { Process(); }
	}
	else { m_nTiming = 0; }

	//	ポイントの回復
	m_nReCounter++;
	if (m_nReCounter % 180 == 0 && m_nMinePoint < MAX_MINE_POINT)
	{
		m_nReCounter = 0;

		m_nMinePoint += RECOVERY_MINE_POINT;	//	回復値

		if (m_nMinePoint >= MAX_MINE_POINT)		//	上限値を越させない
		{
			m_nMinePoint = MAX_MINE_POINT;
		}
	}
}
//=============================================================================
// 地雷使用時の処理
//=============================================================================
void CTypeSweeper::Process()
{
	CGame *pGame = CManager::GetGame();
	CCharacter *pCharacter = pGame->GetChara(m_nEnemyNo);

	//	(残りポイントがあれば)
	if (m_state != STATE_DAMAGE)
	{
		if (m_nMinePoint >= REDUCED_MINE_POINT)
		{
			//	設置
			CMine::Create(CCharacter::GetPos(), pCharacter);

			//	使用可能ポイントを減らす
			m_nMinePoint -= REDUCED_MINE_POINT;
			if (m_nMinePoint <= 0)	//	0以下にさせない
			{
				m_nMinePoint = 0;
			}
		}
	}
}

