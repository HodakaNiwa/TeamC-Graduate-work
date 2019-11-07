//*****************************************************************************
//
//     UIの処理[UI.cpp]
//     Author:Hodaka Niwa
//
//*****************************************************************************
#include "UI.h"
#include "score.h"
#include "minimap.h"

//*****************************************************************************
//     マクロ定義
//*****************************************************************************
#define UI_PRIORITY (6)		// UIの描画優先順位

// ゲームスコア用
#define UI_GAMESCORE_POS_0 (D3DXVECTOR3(20.0f,20.0f,0.0f))
#define UI_GAMESCORE_POS_1 (D3DXVECTOR3(1180.0f,20.0f,0.0f))
#define UI_GAMESCORE_POS_2 (D3DXVECTOR3(20.0f,695.0f,0.0f))
#define UI_GAMESCORE_POS_3 (D3DXVECTOR3(1180.0f,695.0f,0.0f))
#define UI_GAMESCORE_SIZE  (D3DXVECTOR3(25.0f,25.0f,0.0f))

//*****************************************************************************
//     静的メンバ変数宣言
//*****************************************************************************


//=============================================================================
//    コンストラクタ
//=============================================================================
CUI::CUI()
{
	m_nNumPlayer = 0;
	m_pMiniMap = NULL;
	for (int nCntScore = 0; nCntScore < MAX_PLAYERNUM; nCntScore++)
	{
		m_pScoreGame[nCntScore] = NULL;
	}
}

//=============================================================================
//    デストラクタ
//=============================================================================
CUI::~CUI()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CUI *CUI::Create(int nNumPlayer)
{
	CUI *pUI = NULL;
	if (pUI != NULL) { return NULL; }

	// メモリを確保
	pUI = new CUI;
	if (pUI == NULL) { return NULL; }

	// 初期化処理
	if (FAILED(pUI->Init(nNumPlayer)))
	{
		pUI->Uninit();
		delete pUI;
		return NULL;
	}

	return pUI;	// インスタンスのアドレスを返す
}


//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CUI::Init(int nNumPlayer)
{
	// プレイヤーの人数を記憶
	m_nNumPlayer = nNumPlayer;

	// ゲームスコアを生成する
	CreateScoreGame();

	// ミニマップクラスを生成する
	CreateMiniMap();

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CUI::Uninit(void)
{
	// ゲームスコアクラスを開放する
	ReleaseScoreGame();

	// ミニマップクラスを開放する
	ReleaseMiniMap();
}

//=============================================================================
//    更新処理
//=============================================================================
void CUI::Update(void)
{
	// ミニマップの更新処理
	if (m_pMiniMap != NULL)
	{
		m_pMiniMap->Update();
	}
}

//=============================================================================
//    ゲームスコアクラスを生成する処理
//=============================================================================
void CUI::CreateScoreGame(void)
{
	// 必要な変数を宣言
	D3DXVECTOR3 pos[MAX_PLAYERNUM] =
	{
		UI_GAMESCORE_POS_0,
		UI_GAMESCORE_POS_1,
		UI_GAMESCORE_POS_2,
		UI_GAMESCORE_POS_3
	};
	D3DXVECTOR3 size = UI_GAMESCORE_SIZE;

	// クラス生成
	for (int nCntPlayer = 0; nCntPlayer < m_nNumPlayer; nCntPlayer++)
	{// ゲームに参加するプレイヤーの数だけ繰り返し
		if (m_pScoreGame[nCntPlayer] != NULL) { continue; }// クリアされていない場合は処理せず次のループへ

		// インスタンス生成
		m_pScoreGame[nCntPlayer] = (CScoreGame*)CScore::Create(CScore::TYPE_GAME, UI_PRIORITY, pos[nCntPlayer], size);
	}
}

//=============================================================================
//    ミニマップクラスを生成する処理
//=============================================================================
void CUI::CreateMiniMap(void)
{
	if (m_pMiniMap != NULL) { return; }
	m_pMiniMap = CMiniMap::Create();
}

//=============================================================================
//    ゲームスコアクラスを開放する処理
//=============================================================================
void CUI::ReleaseScoreGame(void)
{
	for (int nCntScore = 0; nCntScore < MAX_PLAYERNUM; nCntScore++)
	{
		if (m_pScoreGame[nCntScore] != NULL)
		{
			m_pScoreGame[nCntScore]->Uninit();
			m_pScoreGame[nCntScore] = NULL;
		}
	}
}

//=============================================================================
//    ミニマップクラスを開放する処理
//=============================================================================
void CUI::ReleaseMiniMap(void)
{
	if (m_pMiniMap != NULL)
	{
		m_pMiniMap->Uninit();
		delete m_pMiniMap;
		m_pMiniMap = NULL;
	}
}