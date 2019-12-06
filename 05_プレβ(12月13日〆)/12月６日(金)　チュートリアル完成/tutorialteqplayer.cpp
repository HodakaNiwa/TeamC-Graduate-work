//=============================================================================
//
// チュートリアル地雷型の処理 [tutorialteqolayer.cpp]
// Author : 山下敦史 & 佐藤安純
//
//=============================================================================
#include "tutorialplayer.h"
#include "manager.h"
#include "camera.h"
#include "input.h"
#include "loadEffect.h"
#include "collision.h"
#include "model.h"
#include "UI.h"
#include "skilicon.h"
#include "RawMouse.h"
#include "mine.h"

//*****************************************************************************
// マクロ
//*****************************************************************************
#define RECASTTIME (10)				//リキャスト時間
#define MAX_MINE_POINT (100)		//マインを使用できるポイント
#define REDUCED_MINE_POINT (20)		//マインを使用した時に減るポイント
#define RECOVERY_MINE_POINT (10)	//マイン使用ポイントの回復量
#define RECOVERY_TIME (5)			//使用ポイント回復までの時間

//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CTutorialTechniquePlayer::CTutorialTechniquePlayer()
{
	m_nRecastCounter = 0;
	m_nRecastTimer = 0;
	m_nButtonCounter = 0;
	m_bRecast = false;
	m_nInstallationCounter = 0;
	m_nInstallationTimer = 0;
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CTutorialTechniquePlayer::~CTutorialTechniquePlayer(){}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CTutorialTechniquePlayer::Init(D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40], int nNumPlayer, int nType)
{
	//プレイヤーのナンバーを格納
	m_nNumPlayer = nNumPlayer;

	//初期化
	m_bWalk = true;

	//キャラクターの初期化
	CCharacter::Init(nNumPlayer, ModelTxt, MotionTxt, m_CharcterType, nType);
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
	m_pLoadEffect = NULL;

	//コピーラインの初期化
	for (int nCnt = 0; nCnt < MAX_TERRITORY; nCnt++)
	{
		m_apCopyLine[nCnt] = NULL;
	}
	//エフェクトの初期化
	if (m_pLoadEffect == NULL)
	{
		m_pLoadEffect = CLoadEffect::Create(1, D3DXVECTOR3(pos.x, pos.y + 60.0f, pos.z), 5);
	}
	//モーションの情報を取得
	m_pMotion = CCharacter::GetMotion();
	ResetLine();	//ラインの初期化

	//各変数の初期化
	m_nRecastCounter = 0;
	m_nRecastTimer = 0;
	m_nButtonCounter = 0;
	m_bRecast = false;
	m_nInstallationCounter = 0;
	m_nInstallationTimer = 0;
	m_bMineUse = false;
	m_nDamageCounter = 0;
	m_fInertia = GetInertia();			//慣性
	m_bSetMine = false;

	//目的地までのリスト設定
	SetAimPos();

	//移動状態にする
	SetMovePlayer(true, 3);
	return S_OK;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CTutorialTechniquePlayer::Init(void)
{
	//初期化処理
	CTutorialPlayer::Init();
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void  CTutorialTechniquePlayer::Uninit(void)
{
	//プレイヤーの破棄
	CTutorialPlayer::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void  CTutorialTechniquePlayer::Update(void)
{
	//死亡フラグが有効だったら
	if (GetDeth() == true) { return; }

	//アクションの更新
	ActionUpdate();

	//プレイヤーの更新
	CTutorialPlayer::Update();

	if (m_bMoveEnd == true)
	{//地雷を置く
		SetMine();
		m_bMoveEnd = false;
	}
	else
	{
		if ((!m_bMineUse) && (m_pMotion->GetNumMotion() != PLAYERSTATE_ACTION) && (m_bSetMine))
		{//地雷を置いたら次の目的地に向かう
			SetMovePlayer(true, 4);
			m_bSetMine = false;
		}
	}

#if _DEBUG
	DebugInput();	//デバッグ用入力処理
#endif
}

//=============================================================================
// アクションの更新処理
//=============================================================================
void  CTutorialTechniquePlayer::ActionUpdate(void)
{
	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVec3TransformCoord(&pos, &m_pModel[5]->GetPos(), &m_pModel[5]->GetMtxWorld());

	if (m_bMineUse == true)
	{//地雷が置ける状態だったら
		m_nInstallationCounter++;

		if (m_nInstallationCounter % 30 == 0)
		{
			m_nInstallationTimer++;
			if (m_nInstallationTimer == 1)
			{
				//地雷を設置
				CMine::Create(pos, this);
				m_bMineUse = false;
				m_nInstallationCounter = 0;
				m_nInstallationTimer = 0;
			}
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void  CTutorialTechniquePlayer::Draw(void)
{
	//プレイヤーの描画
	CTutorialPlayer::Draw();
}

//=============================================================================
//オブジェクトの生成処理
//=============================================================================
CTutorialTechniquePlayer *CTutorialTechniquePlayer::Create(D3DXVECTOR3 pos, int nNumPlayer, int nType)
{
	CTutorialTechniquePlayer *pTeqPlayer = NULL;
	if (pTeqPlayer == NULL)
	{
		//メモリを動的確保
		pTeqPlayer = new CTutorialTechniquePlayer;
		if (pTeqPlayer != NULL)
		{//NULLチェック
			pTeqPlayer->SetType(nType);												//ゲーム用国タイプ
			pTeqPlayer->m_nCuntry[nNumPlayer] = nType;								//リザルト用の国タイプ
			pTeqPlayer->m_CharcterType = CCharacter::CHARCTERTYPE_TECHNIQUE;		//ゲーム用のキャラタイプ
			pTeqPlayer->m_CharcterTypeResult[nNumPlayer] = CHARCTERTYPE_TECHNIQUE;	//リザルト用のキャラタイプ
			pTeqPlayer->Init(pos, "data\\TEXT\\ModelLoad\\TEQPLAYER_LOAD.txt", "data\\TEXT\\MotionLoad\\TEQPLAYER_MOTION.txt", nNumPlayer, nType);	//プレイヤーの初期化
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
	return pTeqPlayer;
}

//=============================================================================
// 地雷の設定
//=============================================================================
void CTutorialTechniquePlayer::SetMine(void)
{
	//サウンドの取得
	CSound *pSound = CManager::GetSound();
	m_PlayerState = PLAYERSTATE_ACTION;
	m_pMotion->SetNumMotion(PLAYERSTATE_ACTION);
	m_bMineUse = true;
	m_bSetMine = true;
	pSound->PlaySound(CSound::SOUND_LABEL_SE024);
}

//=============================================================================
// デバッグ用入力処理
//=============================================================================
void CTutorialTechniquePlayer::DebugInput(void)
{
	CInputKeyboard * pKeyboard = CManager::GetInputkeyboard();

	if (pKeyboard->GetKeyboardTrigger(DIK_SPACE) == true)
	{//地雷を置く
		SetMine();
	}
}

//=============================================================================
//
//=============================================================================
void  CTutorialTechniquePlayer::Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size){}