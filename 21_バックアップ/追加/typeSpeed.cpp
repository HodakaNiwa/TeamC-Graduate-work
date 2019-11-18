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
CTypeSpeed::~CTypeSpeed()
{

}

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
	//プレイヤーのナンバーを格納
	m_nEnemyNo = nChara;

	//初期化
	m_bWalk = true;
	m_bSprintMotion = false;
	//キャラクターの初期化
	CCharacter::Init(nChara,ModelTxt, MotionTxt);
	CCharacter::SetPos(pos);

	m_nLineNum = 2;	//	最低限のラインを引いたら始点に戻る(拠点を2つ繋いだら始点に戻る、始点に戻ってきたらラインは3つになりポイント加算の条件を満たせる)
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

	//スピードプレイヤー用の変数を初期化
	m_fSpeed = 1.0f;
	m_nSprintCounter = 0;
	m_nSprintTimer = 0;
	m_nRecastCounter = 0;
	m_nRecastTimer = 0;
	m_nButtonCounter = 0;
	m_bRecast = false;
	m_bSprint = false;

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
	//プレイヤーの終了処理
	CEnemy::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void  CTypeSpeed::Update(void)
{
	//スピード型のスキル処理
	SprintUpdate();

	//プレイヤーの更新
	CEnemy::Update();
}

//=============================================================================
// スプリント処理
//=============================================================================
void  CTypeSpeed::SprintUpdate(void)
{
	
			//m_fSpeed = 1.5f;		//スピード型の速さを1.5倍にする
			//m_nButtonCounter = 1;	//ボタンを押せないようにする
			//m_bSprint = true;		//スプリント中にする
			//m_bSprintMotion = true;	//スプリントモーション中かどうか
			//pUi->GetSkilicon(m_nNumPlayer)->RevivalIconMask();

	//スプリント中
	if (m_bSprint == true)
	{
		m_nSprintCounter++;

		if (m_nSprintCounter % 60 == 0)
		{
			m_nSprintTimer++;

			if (m_nSprintTimer <= MAX_SPRINTTIMER)
			{//8秒間速さを上げる
				m_state = STATE_ACTION;	//スプリント状態

				if (m_bSprintMotion == false)
				{
					//m_bSprintMotion = true;
				}
				//if (m_PlayerState == PLAYERSTATE_BLOWAWAY && m_PlayerState == PLAYERSTATE_NONE && m_PlayerState == PLAYERSTATE_WALK)
				//{//スプリント中にぶつかったらスプリント状態に戻す
				//	m_PlayerState = PLAYERSTATE_ACTION;
				//}
			}
			else
			{//8秒超えたら普通の速さに戻す
				m_fSpeed = 1.0f;					//スピードをもとに戻す
				m_nSprintTimer = 0;					//スプリントタイマーを初期化
				m_bRecast = true;					//リキャスト中にする
				m_bWalk = true;						//移動モーションができる状態にする
				m_state = STATE_NONE;	//移動状態
				m_bSprintMotion = false;
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

				m_bRecast = false;		//リキャスト中じゃない
				m_nRecastTimer = 0;		//リキャストタイマーの初期化
				m_nButtonCounter = 0;	//スキルボタンを押せるようにする
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