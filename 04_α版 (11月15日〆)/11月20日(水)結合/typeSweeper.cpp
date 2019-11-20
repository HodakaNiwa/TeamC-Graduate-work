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
#define MAX_SPRINTTIMER (8)		//スプリント時間
#define RECAST (10)				//スプリントのリキャスト時間
#define MAX_MINE_POINT (100)		//マインの置けるポイント
#define REDUCED_MINE_POINT (20)		//マインを置いたときに減るポイント

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
HRESULT CTypeSweeper::Init(int nChara, D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40])
{
	//プレイヤーのナンバーを格納
	m_nEnemyNo = nChara;

	//初期化
	m_bWalk = true;
	m_bSprintMotion = false;
	//キャラクターの初期化
	CCharacter::Init(nChara,ModelTxt, MotionTxt,m_CharcterType);
	CCharacter::SetPos(pos);

	m_nLineNum = 2;	//	最低限のラインを引いたら始点に戻る(拠点を2つ繋いだら始点に戻る、始点に戻ってきたらラインは3つになりポイント加算の条件を満たせ
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

	
	m_fSpeed = 1.0f;				//	初期速度
	m_nRecastCounter = 0;
	m_nRecastTimer = 0;
	m_nButtonCounter = 0;
	m_bRecast = false;
	m_bCollision = false;
	m_nMinePoint = MAX_MINE_POINT;
	m_nCreateTime = (rand() % 4);
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
//	if (m_pLoadEffect != NULL)
//	{//エフェクトの破棄
//		m_pLoadEffect->Uninit();
//		delete m_pLoadEffect;
//		m_pLoadEffect = NULL;
//	}

	//プレイヤーの終了処理
	CEnemy::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void  CTypeSweeper::Update(void)
{
	//スピード型のスキル処理
	//ActionUpdate();

	//プレイヤーの更新
	CEnemy::Update();
}

//=============================================================================
// スプリント処理
//=============================================================================
void  CTypeSweeper::ActionUpdate(void)
{
	CGame *pGame = CManager::GetGame();
	CUI *pUi = pGame->GetUI();
	CCharacter *pCharacter = pGame->GetChara(m_nEnemyNo);

	if (m_bRecast == false && m_nButtonCounter == 0)
	{
		//地雷を設置
		CMine::Create(CCharacter::GetPos(), pCharacter);

		m_nButtonCounter = 0;	//ボタンを押せないようにする

		pUi->GetSkilicon(m_nEnemyNo)->RevivalIconMask();	//スキルアイコン

		m_bRecast = false;
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
// ラインの生成処理
//=============================================================================
void CTypeSweeper::CreateOrbitLine(void)
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
void CTypeSweeper::UninitOrtbitLine(void)
{
	if (m_pOrbitLine != NULL)
	{
		m_pOrbitLine->Uninit();
		m_pOrbitLine = NULL;
	}
}