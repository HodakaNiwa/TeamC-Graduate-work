//*****************************************************************************
//
//     UIの処理[UI.cpp]
//     Author:Hodaka Niwa
//
//*****************************************************************************
#include "UI.h"
#include "score.h"
#include "minimap.h"
#include "skilicon.h"

//*****************************************************************************
//     マクロ定義
//*****************************************************************************
#define UI_PRIORITY (6)		// UIの描画優先順位

// ゲームスコア用
#define UI_GAMESCORE_POS_0 (D3DXVECTOR3(20.0f,20.0f,0.0f))
#define UI_GAMESCORE_POS_1 (D3DXVECTOR3(1180.0f,20.0f,0.0f))
#define UI_GAMESCORE_POS_2 (D3DXVECTOR3(20.0f,695.0f,0.0f))
#define UI_GAMESCORE_POS_3 (D3DXVECTOR3(1180.0f,695.0f,0.0f))
#define UI_GAMESCORE_POS_4 (D3DXVECTOR3(10000.0f,695.0f,0.0f))
#define UI_GAMESCORE_SIZE  (D3DXVECTOR3(18.0f,30.0f,0.0f))

//*****************************************************************************
//     静的メンバ変数宣言
//*****************************************************************************


//=============================================================================
//    コンストラクタ
//=============================================================================
CUI::CUI()
{
	m_nNumPlayer = 0;
	m_nNumAllCharacter = 0;
	m_pMiniMap = NULL;
	for (int nCntScore = 0; nCntScore < MAX_PLAYERNUM * 2; nCntScore++)
	{
		m_pScoreGame[nCntScore] = NULL;
	}
	for (int nCntSkil = 0; nCntSkil < MAX_PLAYERNUM; nCntSkil++)
	{
		m_pSkilicon[nCntSkil] = NULL;
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
CUI *CUI::Create(int nNumPlayer, int nAllCharacter)
{
	CUI *pUI = NULL;
	if (pUI != NULL) { return NULL; }

	// メモリを確保
	pUI = new CUI;
	if (pUI == NULL) { return NULL; }

	// 初期化処理
	if (FAILED(pUI->Init(nNumPlayer, nAllCharacter)))
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
HRESULT CUI::Init(int nNumPlayer, int nAllCharacter)
{
	// プレイヤーの人数を記憶
	m_nNumPlayer = nNumPlayer;

	// 全ての参加人数を記憶
	m_nNumAllCharacter = nAllCharacter;

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

	// スキルアイコンクラスを開放する
	ReleaseSkilicon();
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

	// スキルアイコンの更新処理
	UpdateSkilicon();
}

//=============================================================================
//    描画処理
//=============================================================================
void CUI::Draw(void)
{
	// スキルアイコンの描画処理
	DrawSkilicon();
}

//=============================================================================
//    ゲームスコアクラスを生成する処理
//=============================================================================
void CUI::CreateScoreGame(void)
{
	// 必要な変数を宣言
	D3DXVECTOR3 pos[MAX_PLAYERNUM * 2] =
	{
		UI_GAMESCORE_POS_0,
		UI_GAMESCORE_POS_1,
		UI_GAMESCORE_POS_2,
		UI_GAMESCORE_POS_3,
		UI_GAMESCORE_POS_4,
		UI_GAMESCORE_POS_4,
		UI_GAMESCORE_POS_4,
		UI_GAMESCORE_POS_4,
	};
	D3DXVECTOR3 size = UI_GAMESCORE_SIZE;

	// クラス生成
	for (int nCntPlayer = 0; nCntPlayer < m_nNumAllCharacter; nCntPlayer++)
	{// ゲームに参加するプレイヤーの数だけ繰り返し
		if (m_pScoreGame[nCntPlayer] != NULL) { continue; }// クリアされていない場合は処理せず次のループへ
		if (nCntPlayer >= m_nNumPlayer) { pos[nCntPlayer] = UI_GAMESCORE_POS_4; }

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
//    スキルアイコンクラスを生成する処理
//=============================================================================
void CUI::CreateSkilicon(CCharacter *pChara, int nIdx, int nRecastTime)
{
	if (m_pSkilicon[nIdx] != NULL) { return; }
	m_pSkilicon[nIdx] = CSkilicon::Create(pChara, nIdx, nRecastTime);
}

//=============================================================================
//    ゲームスコアクラスを開放する処理
//=============================================================================
void CUI::ReleaseScoreGame(void)
{
	for (int nCntScore = 0; nCntScore < MAX_PLAYERNUM * 2; nCntScore++)
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

//=============================================================================
//    スキルアイコンクラスを開放する処理
//=============================================================================
void CUI::ReleaseSkilicon(void)
{
	for (int nCntSkil = 0; nCntSkil < MAX_PLAYERNUM; nCntSkil++)
	{
		if (m_pSkilicon[nCntSkil] != NULL)
		{
			m_pSkilicon[nCntSkil]->Uninit();
			delete m_pSkilicon[nCntSkil];
			m_pSkilicon[nCntSkil] = NULL;
		}
	}
}

//=============================================================================
//    スキルアイコンクラスの更新処理
//=============================================================================
void CUI::UpdateSkilicon(void)
{
	for (int nCntSkil = 0; nCntSkil < m_nNumPlayer; nCntSkil++)
	{
		if (m_pSkilicon[nCntSkil] != NULL)
		{
			m_pSkilicon[nCntSkil]->Update();
		}
	}
}

//=============================================================================
//    スキルアイコンを描画する処理
//=============================================================================
void CUI::DrawSkilicon(void)
{
	for (int nCntSkil = 0; nCntSkil < m_nNumPlayer; nCntSkil++)
	{
		if (m_pSkilicon[nCntSkil] != NULL)
		{
			m_pSkilicon[nCntSkil]->Draw();
		}
	}
}