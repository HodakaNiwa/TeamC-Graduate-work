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
#include "charaicon.h"
#include "game.h"
#include "character.h"


#include "debuglog.h"

//*****************************************************************************
//     マクロ定義
//*****************************************************************************
#define UI_PRIORITY                (6)									// UIの描画優先順位

// ゲームスコア用
#define UI_GAMESCORE_POS_0         (D3DXVECTOR3(280.0f,23.0f,0.0f))		// 1Pのスコア
#define UI_GAMESCORE_POS_1         (D3DXVECTOR3(920.0f,23.0f,0.0f))		// 2Pのスコア
#define UI_GAMESCORE_POS_2         (D3DXVECTOR3(280.0f,380.0f,0.0f))	// 3Pのスコア
#define UI_GAMESCORE_POS_3         (D3DXVECTOR3(920.0f,380.0f,0.0f))	// 4Pのスコア
#define UI_GAMESCORE_POS_4         (D3DXVECTOR3(10000.0f,380.0f,0.0f))	// 敵のスコア(画面外へ配置)
#define UI_GAMESCORE_SIZE          (D3DXVECTOR3(20.0f,40.0f,0.0f))		// スコアの数字のサイズ

// キャラアイコン用
#define UI_CHARAICON_WIDTH_LARGE   (50.0f)								// 1位のキャラアイコンの幅
#define UI_CHARAICON_WIDTH_MIDDLE  (15.0f)								// 2位のキャラアイコンの幅
#define UI_CHARAICON_WIDTH_SMALL   (10.0f)								// 3位のキャラアイコンの幅
#define UI_CHARAICON_HEIGHT_LARGE  (50.0f)								// 1位のキャラアイコンの高さ
#define UI_CHARAICON_HEIGHT_MIDDLE (15.0f)								// 2位のキャラアイコンの高さ
#define UI_CHARAICON_HEIGHT_SMALL  (10.0f)								// 3位のキャラアイコンの高さ
#define UI_CHARAICON_TEXIDX        (1)									// キャラアイコンの使用するテクスチャの番号

// プレイヤー番号アイコン用
#define UI_PLAYERIDXICON_POS       (D3DXVECTOR3(0.0f, 120.0f, 0.0f))	// プレイヤー番号アイコンの大きさ
#define UI_PLAYERIDXICON_COL       (D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f))	// プレイヤー番号アイコンの色
#define UI_PLAYERIDXICON_WIDTH     (25.0f)								// プレイヤー番号アイコンの幅
#define UI_PLAYERIDXICON_HEIGHT    (15.0f)								// プレイヤー番号アイコンの高さ
#define UI_PLAYERIDXICON_TEXIDX    (0)									// プレイヤー番号アイコンの使用するテクスチャの番号

// 王冠アイコン用
#define UI_CROWNICON_POS           (D3DXVECTOR3(0.0f, 120.0f, 0.0f))	// 王冠アイコンの大きさ
#define UI_CROWNICON_COL           (D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f))	// 王冠アイコンの色
#define UI_CROWNICON_WIDTH         (20.0f)								// 王冠アイコンの幅
#define UI_CROWNICON_HEIGHT        (17.0f)								// 王冠アイコンの高さ
#define UI_CROWNICON_TEXIDX        (1)									// 王冠アイコンの使用するテクスチャの番号

//*****************************************************************************
//     静的メンバ変数宣言
//*****************************************************************************


//*****************************************************************************
//     CUIの処理
//*****************************************************************************
//=============================================================================
//    コンストラクタ
//=============================================================================
CUI::CUI()
{
	m_nNumPlayer = 0;
	m_nNumAllCharacter = 0;
	m_pMiniMap = NULL;

	// スコア
	for (int nCntScore = 0; nCntScore < MAX_PLAYERNUM * 2; nCntScore++)
	{
		m_pScoreGame[nCntScore] = NULL;
	}

	// スキルアイコン
	for (int nCntSkil = 0; nCntSkil < MAX_PLAYERNUM; nCntSkil++)
	{
		m_pSkilicon[nCntSkil] = NULL;
	}

	// キャラアイコン
	for (int nCntChara = 0; nCntChara < MAX_PLAYERNUM; nCntChara++)
	{
		for (int nCntIcon = 0; nCntIcon < UI_RANKICON_NUM; nCntIcon++)
		{
			m_pChraicon[nCntChara][nCntIcon] = NULL;
		}
	}

	// 順位(8人分)
	for (int nCntIcon = 0; nCntIcon < MAX_PLAYERNUM * 2; nCntIcon++)
	{
		m_nRank[nCntIcon] = nCntIcon;
	}

	// プレイヤー番号アイコン
	for (int nCntIdx = 0; nCntIdx < MAX_PLAYERNUM * 2; nCntIdx++)
	{
		m_pPlayerIdxicon[nCntIdx] = NULL;
	}

	// 王冠アイコン
	for (int nCntCrown = 0; nCntCrown < UI_RANKICON_NUM; nCntCrown++)
	{
		m_pCrownicon[nCntCrown] = NULL;
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

	// キャラアイコンを生成する
	CreateCharaicon();

	// プレイヤー番号アイコンを生成する
	CreatePlayerIdxicon();

	// 王冠アイコンを生成する
	CreateCrownRank();

	// テクスチャを割り当てる処理
	BindTextureToIcon();

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

	// キャラアイコンを開放する
	ReleaseCharaicon();

	// プレイヤー番号アイコンを開放する
	ReleasePlayerIdxicon();

	// 王冠アイコンを開放する
	ReleaseCrownIcon();
}

//=============================================================================
//    更新処理
//=============================================================================
void CUI::Update(void)
{
	// ゲームクラスの取得
	CGame *pGame = (CGame*)CManager::GetModeBase();
	if (pGame == NULL) { return; }

	// スキルアイコンの更新処理
	UpdateSkilicon();

	// キャラアイコンの更新処理
	UpdateCharaicon();

	// プレイヤー番号アイコンの更新処理
	UpdatePlayerIdxicon(pGame);

	// 王冠アイコンの更新処理
	UpdateCrownicon(pGame);


	for (int nCntRank = 0; nCntRank < m_nNumAllCharacter; nCntRank++)
	{
		CDebugProc::Print("スコア : %d\n", m_pScoreGame[nCntRank]->GetScore());
	}
	CDebugProc::Print("\n");
	for (int nCntRank = 0; nCntRank < m_nNumAllCharacter; nCntRank++)
	{
		CDebugProc::Print("順位 : %d\n", m_nRank[nCntRank]);
	}
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

	if (m_nNumPlayer == 1)
	{// プレイ人数が一人だったら
		float fValue = UI_GAMESCORE_SIZE.x * 1.5f;
		pos[0].x = SCREEN_WIDTH / 2 - fValue - (fValue / 2.0f);
		pos[0].y += UI_GAMESCORE_SIZE.y / 3.0f;
		size.x *= 1.5f;
		size.y *= 1.5f;
	}

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
//    キャラアイコンを生成する処理
//=============================================================================
void CUI::CreateCharaicon(void)
{
	for (int nCntChara = 0; nCntChara < MAX_PLAYERNUM; nCntChara++)
	{
		for (int nCntIcon = 0; nCntIcon < UI_RANKICON_NUM; nCntIcon++)
		{
			if (m_pChraicon[nCntChara][nCntIcon] != NULL) { continue; }
			m_pChraicon[nCntChara][nCntIcon] = CCharaicon::Create(INITIALIZE_VECTOR3, D3DXCOLOR(1.0f,1.0f,1.0f,1.0f),
				UI_CHARAICON_WIDTH_LARGE, UI_CHARAICON_HEIGHT_LARGE);
		}
	}
}

//=============================================================================
//    プレイヤー番号アイコンを生成する処理
//=============================================================================
void CUI::CreatePlayerIdxicon(void)
{
	D3DXVECTOR3 pos[MAX_PLAYERNUM] =
	{
		INITIALIZE_VECTOR3,
		INITIALIZE_VECTOR3,
		INITIALIZE_VECTOR3,
		INITIALIZE_VECTOR3,
	};
	D3DXCOLOR col;
	for (int nCntIdx = 0; nCntIdx < m_nNumAllCharacter; nCntIdx++)
	{
		col = CCharacter::m_CountryColor[nCntIdx];
		if (nCntIdx >= m_nNumPlayer) { col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f); }
		if (m_pPlayerIdxicon[nCntIdx] != NULL) { continue; }
		m_pPlayerIdxicon[nCntIdx] = CIconBillboard::Create(pos[nCntIdx], col,
			UI_PLAYERIDXICON_WIDTH, UI_PLAYERIDXICON_HEIGHT);
	}
}

//=============================================================================
//    王冠アイコンを生成する処理
//=============================================================================
void CUI::CreateCrownRank(void)
{
	D3DXVECTOR3 pos[UI_RANKICON_NUM] =
	{
		INITIALIZE_VECTOR3,
		INITIALIZE_VECTOR3,
		INITIALIZE_VECTOR3,
	};
	for (int nCntCrown = 0; nCntCrown < UI_RANKICON_NUM; nCntCrown++)
	{
		if (m_pCrownicon[nCntCrown] != NULL) { continue; }
		m_pCrownicon[nCntCrown] = CIconBillboard::Create(pos[nCntCrown], UI_CROWNICON_COL,
			UI_CROWNICON_WIDTH, UI_CROWNICON_HEIGHT);
	}
}

//=============================================================================
//    アイコンにテクスチャを割り当てる処理
//=============================================================================
void CUI::BindTextureToIcon(void)
{
	// ゲームクラスの取得
	CGame *pGame = (CGame*)CManager::GetModeBase();
	if (pGame == NULL) { return; }

	// テクスチャへのポインタを保存
	m_pTexture = pGame->GetTexture();
	if (m_pTexture == NULL) { return; }

	// キャラアイコンにテクスチャを割り当てる
	BindTextureToIcon_Chara(pGame);

	// プレイヤー番号アイコンにテクスチャを割り当てる
	BindTextureToIcon_PlayerIdx(pGame);

	// 王冠アイコンにテクスチャを割り当てる
	BindTextureToIcon_Crown(pGame);
}

//=============================================================================
//    キャラアイコンにテクスチャを割り当てる処理
//=============================================================================
void CUI::BindTextureToIcon_Chara(CGame *pGame)
{
	for (int nCntChara = 0; nCntChara < m_nNumPlayer; nCntChara++)
	{
		for (int nCntIcon = 0; nCntIcon < UI_RANKICON_NUM; nCntIcon++)
		{
			if (m_pChraicon[nCntChara][nCntIcon] == NULL) { continue; }

			// キャラクターへのポインタを設定
			CCharacter *pParentChara = pGame->GetChara(nCntChara);

			// 親となるキャラクターへのポインタを設定
			m_pChraicon[nCntChara][nCntIcon]->SetParentChara(pParentChara);

			// テクスチャの設定
			m_pChraicon[nCntChara][nCntIcon]->BindTexture(m_pTexture[UI_CHARAICON_TEXIDX + nCntIcon]);
		}
	}
}

//=============================================================================
//    プレイヤー番号アイコンにテクスチャを割り当てる処理
//=============================================================================
void CUI::BindTextureToIcon_PlayerIdx(CGame *pGame)
{
	float fHeight = 0.0f;
	for (int nCntIdx = 0; nCntIdx < m_nNumAllCharacter; nCntIdx++)
	{
		// キャラクタークラスを取得
		CCharacter *pChara = pGame->GetChara(nCntIdx);
		if (pChara == NULL || m_pPlayerIdxicon[nCntIdx] == NULL) { continue; }

		// テクスチャを割り当て
		m_pPlayerIdxicon[nCntIdx]->BindTexture(m_pTexture[UI_PLAYERIDXICON_TEXIDX]);

		// 順番によってテクスチャの座標を設定
		fHeight = 0.20f * nCntIdx;
		if (nCntIdx >= m_nNumPlayer)
		{
			fHeight = 0.80f;
		}
		m_pPlayerIdxicon[nCntIdx]->SetTexV(fHeight);
		m_pPlayerIdxicon[nCntIdx]->SetTexHeight(0.20f);
		m_pPlayerIdxicon[nCntIdx]->SetVtxBuffTex();
	}
}

//=============================================================================
//    王冠アイコンにテクスチャを割り当てる処理
//=============================================================================
void CUI::BindTextureToIcon_Crown(CGame *pGame)
{
	for (int nCntCrown = 0; nCntCrown < UI_RANKICON_NUM; nCntCrown++)
	{
		// テクスチャを割り当て
		m_pCrownicon[nCntCrown]->BindTexture(m_pTexture[UI_CROWNICON_TEXIDX + nCntCrown]);
	}
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
//    キャラアイコンを開放する処理
//=============================================================================
void CUI::ReleaseCharaicon(void)
{
	for (int nCntChara = 0; nCntChara < MAX_PLAYERNUM; nCntChara++)
	{
		for (int nCntIcon = 0; nCntIcon < UI_RANKICON_NUM; nCntIcon++)
		{
			if (m_pChraicon[nCntChara][nCntIcon] != NULL)
			{
				m_pChraicon[nCntChara][nCntIcon]->Uninit();
				delete m_pChraicon[nCntChara][nCntIcon];
				m_pChraicon[nCntChara][nCntIcon] = NULL;
			}
		}
	}
}

//=============================================================================
//    プレイヤー番号アイコンを開放する処理
//=============================================================================
void CUI::ReleasePlayerIdxicon(void)
{
	for (int nCntIdx = 0; nCntIdx < MAX_PLAYERNUM * 2; nCntIdx++)
	{
		if (m_pPlayerIdxicon[nCntIdx] != NULL)
		{
			m_pPlayerIdxicon[nCntIdx]->Uninit();
			delete m_pPlayerIdxicon[nCntIdx];
			m_pPlayerIdxicon[nCntIdx] = NULL;
		}
	}
}

//=============================================================================
//    王冠ポリゴンを開放する処理
//=============================================================================
void CUI::ReleaseCrownIcon(void)
{
	for (int nCntCrown = 0; nCntCrown < UI_RANKICON_NUM; nCntCrown++)
	{
		if (m_pCrownicon[nCntCrown] != NULL)
		{
			m_pCrownicon[nCntCrown]->Uninit();
			delete m_pCrownicon[nCntCrown];
			m_pCrownicon[nCntCrown] = NULL;
		}
	}
}

//=============================================================================
//    スコアのソート
//=============================================================================
void CUI::SortRankFromScore(void)
{
	int nScore = 0;
	int nRankScore[MAX_PLAYERNUM * 2];

	// スコアを取得
	for (int nCntRank = 0; nCntRank < MAX_PLAYERNUM * 2; nCntRank++)
	{// 1～8位まで繰り返し
		if (m_pScoreGame[nCntRank] == NULL) { continue; }
		nRankScore[nCntRank] = m_pScoreGame[nCntRank]->GetScore();
		m_nRank[nCntRank] = 0;
	}

	// 順位判定(同じ順位も込み)
	int nJudgScore = 0;
	for (int nCntRank = 0; nCntRank < MAX_PLAYERNUM * 2; nCntRank++)
	{
		nJudgScore = nRankScore[nCntRank];
		for (int nCntScore = 0; nCntScore < MAX_PLAYERNUM * 2; nCntScore++)
		{
			if (nJudgScore > nRankScore[nCntScore])
			{
				m_nRank[nCntScore]++;
			}
		}
	}

	// 同順位判定(配列の若い順に直す)
	int nSet = 0;
	int nSetIdx[MAX_PLAYERNUM * 2] = {};
	for (int nCntRank = 0; nCntRank < MAX_PLAYERNUM * 2; nCntRank++)
	{
		for (int nCntScore = (nCntRank + 1); nCntScore < MAX_PLAYERNUM * 2; nCntScore++)
		{
			if (m_nRank[nCntRank] == m_nRank[nCntScore])
			{// 同じ順位だった場合
				nSetIdx[nSet] = nCntScore;
				nSet++;
			}
		}

		for (int nCntSet = 0; nCntSet < nSet; nCntSet++)
		{// 同じ順位が見つかった数だけずらしておく
			m_nRank[nSetIdx[nCntSet]]++;
		}
		nSet = 0;
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
//    キャラアイコンクラスの更新処理
//=============================================================================
void CUI::UpdateCharaicon(void)
{
	CGame *pGame = CManager::GetGame();
	if (pGame == NULL) { return; }

	for (int nCntChara = 0; nCntChara < m_nNumPlayer; nCntChara++)
	{
		for (int nCntIcon = 0; nCntIcon < UI_RANKICON_NUM; nCntIcon++)
		{
			// キャラアイコンに目的のプレイヤーを設定
			CCharacter *pDestChara = pGame->GetChara(m_nRank[nCntIcon]);

			// キャラアイコンの更新処理
			if (m_pChraicon[nCntChara][nCntIcon] != NULL)
			{
				m_pChraicon[nCntChara][nCntIcon]->SetDestChara(pDestChara);
				m_pChraicon[nCntChara][nCntIcon]->Update();
			}
		}
	}
}

//=============================================================================
//    プレイヤー番号アイコンの更新処理
//=============================================================================
void CUI::UpdatePlayerIdxicon(CGame *pGame)
{
	for (int nCntIdx = 0; nCntIdx < m_nNumAllCharacter; nCntIdx++)
	{
		// キャラクタークラスを取得
		CCharacter *pChara = pGame->GetChara(nCntIdx);
		if (pChara == NULL || m_pPlayerIdxicon[nCntIdx] == NULL) { continue; }

		// キャラクターの位置を取得
		D3DXVECTOR3 pos = pChara->GetPos();

		// アイコンの位置を設定
		m_pPlayerIdxicon[nCntIdx]->SetPos(pos + UI_PLAYERIDXICON_POS);
	}
}

//=============================================================================
//    王冠アイコンの更新処理
//=============================================================================
void CUI::UpdateCrownicon(CGame *pGame)
{
	int nRank = 0;
	int nSetIcon[UI_RANKICON_NUM] = {-1,-1,-1};
	for (int nCntCrown = 0; nCntCrown < UI_RANKICON_NUM; nCntCrown++)
	{
		// ランクを設定
		nRank = (m_nRank[nCntCrown]);
		for (int nCntRank = 0; nCntRank < UI_RANKICON_NUM; nCntRank++)
		{
			if (nSetIcon[nCntRank] == nRank)
			{
				nRank = (m_nRank[nCntCrown + 1]);
			}
		}

		// キャラクタークラスを取得
		CCharacter *pChara = pGame->GetChara(nRank);
		if (pChara == NULL || m_pCrownicon[nCntCrown] == NULL) { continue; }

		// キャラクターの位置を取得
		D3DXVECTOR3 pos = pChara->GetPos();

		// アイコンの位置を設定
		m_pCrownicon[nCntCrown]->SetPos(pos + UI_CROWNICON_POS);
		nSetIcon[nCntCrown] = nCntCrown;
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

//=============================================================================
//    アイコンを描画する処理
//=============================================================================
void CUI::DrawIcon(int nPlayerIdx)
{
	for (int nCntIcon = 0; nCntIcon < UI_RANKICON_NUM; nCntIcon++)
	{
		if (m_pChraicon[nPlayerIdx][nCntIcon] != NULL)
		{
			m_pChraicon[nPlayerIdx][nCntIcon]->Draw();
		}
	}

	// 王冠アイコンの描画
	DrawCrownIcon();

	// プレイヤー番号アイコンの描画
	DrawPlayerIdxicon();
}

//=============================================================================
//    プレイヤー番号アイコンを描画する処理
//=============================================================================
void CUI::DrawPlayerIdxicon(void)
{
	for (int nCntIdx = 0; nCntIdx < m_nNumAllCharacter; nCntIdx++)
	{
		if (m_pPlayerIdxicon[nCntIdx] != NULL)
		{
			m_pPlayerIdxicon[nCntIdx]->Draw();
		}
	}
}

//=============================================================================
//    王冠アイコンを描画する処理
//=============================================================================
void CUI::DrawCrownIcon(void)
{
	for (int nCntCrown = 0; nCntCrown < UI_RANKICON_NUM; nCntCrown++)
	{
		if (m_pCrownicon[nCntCrown] != NULL)
		{
			m_pCrownicon[nCntCrown]->Draw();
		}
	}
}


//*****************************************************************************
//     CIconBillboardの処理
//*****************************************************************************
//=============================================================================
//    コンストラクタ
//=============================================================================
CIconBillboard::CIconBillboard()
{
	m_pTexture = NULL;
}

//=============================================================================
//    デストラクタ
//=============================================================================
CIconBillboard::~CIconBillboard()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CIconBillboard *CIconBillboard::Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, bool bLighting, float fRot, float fTexU, float fTexV, float fTexWidth, float fTexHeight)
{
	// メモリを確保
	CIconBillboard *pIconBillboard = NULL;
	pIconBillboard = new CIconBillboard;
	if (pIconBillboard == NULL) { return NULL; }

	// 変数のクリア
	pIconBillboard->ClearVariable();

	// 各種値の代入
	pIconBillboard->SetPos(pos);              // ポリゴンの座標
	pIconBillboard->SetCol(col);              // ポリゴンの色
	pIconBillboard->SetRot(fRot);             // ポリゴンの向き
	pIconBillboard->SetWidth(fWidth);         // ポリゴンの幅
	pIconBillboard->SetHeight(fHeight);       // ポリゴンの高さ
	pIconBillboard->SetTexU(fTexU);           // ポリゴンの左上テクスチャU座標
	pIconBillboard->SetTexV(fTexV);           // ポリゴンの左上テクスチャV座標
	pIconBillboard->SetTexWidth(fTexWidth);   // ポリゴンのテクスチャ座標の横幅
	pIconBillboard->SetTexHeight(fTexHeight); // ポリゴンのテクスチャ座標の縦幅
	pIconBillboard->SetLighting(bLighting);   // ライティングするかしないか

	// 初期化処理
	if (FAILED(pIconBillboard->Init()))
	{
		pIconBillboard->Uninit();
		delete pIconBillboard;
		return NULL;
	}

	return pIconBillboard;
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CIconBillboard::Init(void)
{
	// 描画デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	if (pDevice == NULL) { return E_FAIL; }

	// 頂点バッファの作成
	MakeVertex(pDevice);

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CIconBillboard::Uninit(void)
{
	// 頂点バッファの破棄
	DIRECT_RELEASE(m_pVtxBuff);
}

//=============================================================================
//    更新処理
//=============================================================================
void CIconBillboard::Update(void)
{

}

//=============================================================================
//    描画処理
//=============================================================================
void CIconBillboard::Draw(void)
{
	// 描画デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	if (pDevice == NULL) { return; }

	// ライティングの設定
	DWORD Lighting;
	pDevice->GetRenderState(D3DRS_LIGHTING, &Lighting);
	if (m_bLighting == false)
	{// ライティングを外す
		pDevice->SetRenderState(D3DRS_LIGHTING, false);
	}

	// トランスフォーム情報を計算しデバイスに設定
	CalcNotScaleTransform(pDevice);
	SetTransformToDevice(pDevice);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, m_pTexture);

	// Zテストをしない設定に
	DWORD ZTest;
	pDevice->GetRenderState(D3DRS_ZENABLE, &ZTest);
	pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	// Zテストをする設定に
	pDevice->SetRenderState(D3DRS_ZENABLE, ZTest);

	// ライティングを戻す
	pDevice->SetRenderState(D3DRS_LIGHTING, Lighting);
}

//=============================================================================
//    トランスフォーム情報(大きさを含めない)を計算する
//=============================================================================
void CIconBillboard::CalcNotScaleTransform(const LPDIRECT3DDEVICE9 pDevice)
{
	D3DXMATRIX mtxView, mtxTrans; // 計算用マトリックス

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_MtxWorld);

	// カメラの向きを取得
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	// ワールドマトリックスに向きを設定(カメラの逆行列を入れることでカメラの正面に向きを補正)
	m_MtxWorld._11 = mtxView._11;
	m_MtxWorld._12 = mtxView._21;
	m_MtxWorld._13 = mtxView._31;
	m_MtxWorld._21 = mtxView._12;
	m_MtxWorld._22 = mtxView._22;
	m_MtxWorld._23 = mtxView._32;
	m_MtxWorld._31 = mtxView._13;
	m_MtxWorld._32 = mtxView._23;
	m_MtxWorld._33 = mtxView._33;

	// 位置を反映
	m_MtxWorld._41 = m_Pos.x;
	m_MtxWorld._42 = m_Pos.y;
	m_MtxWorld._43 = m_Pos.z;
}

//=============================================================================
//    トランスフォーム情報をデバイスに設定する
//=============================================================================
void CIconBillboard::SetTransformToDevice(const LPDIRECT3DDEVICE9 pDevice)
{
	pDevice->SetTransform(D3DTS_WORLD, &m_MtxWorld);
}

//=============================================================================
//    頂点バッファ生成処理
//=============================================================================
void CIconBillboard::MakeVertex(const LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// 頂点バッファへ値を設定する
	SetVtxBuffValue();
}

//=============================================================================
//    ポリゴンの大きさを設定する処理
//=============================================================================
void CIconBillboard::SetScale(float fWidth, float fHeight)
{
	// オフセットの長さと角度を求める
	m_fLength = sqrtf((m_fWidth * m_fWidth) + (m_fHeight * m_fHeight));
	m_fAngle = atan2f(m_fWidth, m_fHeight);
}

//=============================================================================
//    頂点バッファへ値を設定する処理
//=============================================================================
void CIconBillboard::SetVtxBuffValue(void)
{
	// 頂点情報の設定
	VERTEX_3D *pVtx;

	// 頂点バッファをロックし,頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 大きさを設定
	SetScale(m_fWidth, m_fHeight);

	// 頂点座標
	pVtx[0].pos = D3DXVECTOR3(-sinf(m_fAngle) * m_fLength, cosf(m_fAngle) * m_fLength, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(sinf(m_fAngle) * m_fLength, cosf(m_fAngle) * m_fLength, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-sinf(m_fAngle) * m_fLength, -cosf(m_fAngle) * m_fLength, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(sinf(m_fAngle) * m_fLength, -cosf(m_fAngle) * m_fLength, 0.0f);

	// 法線計算用ベクトル
	D3DXVECTOR3 vecVer[4];

	// 頂点の法線
	D3DXVECTOR3 nor1, nor2;

	// 外積用にベクトルを作る
	vecVer[0] = pVtx[0].pos - pVtx[2].pos;
	vecVer[1] = pVtx[1].pos - pVtx[2].pos;
	vecVer[2] = pVtx[2].pos - pVtx[1].pos;
	vecVer[3] = pVtx[3].pos - pVtx[1].pos;

	// ベクトルを外積で計算して法線を出す
	D3DXVec3Cross(&nor1, &vecVer[0], &vecVer[1]);
	D3DXVec3Cross(&nor2, &vecVer[3], &vecVer[2]);
	D3DXVec3Normalize(&nor1, &nor1);
	D3DXVec3Normalize(&nor2, &nor2);

	// 法線を代入
	pVtx[0].nor = nor1;
	pVtx[1].nor = (nor1 + nor2) / 2;
	pVtx[2].nor = (nor1 + nor2) / 2;
	pVtx[3].nor = nor2;

	// 頂点カラー
	pVtx[0].col = m_Col;
	pVtx[1].col = m_Col;
	pVtx[2].col = m_Col;
	pVtx[3].col = m_Col;

	// テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(m_fTexU, m_fTexV);
	pVtx[1].tex = D3DXVECTOR2(m_fTexU + m_fTexWidth, m_fTexV);
	pVtx[2].tex = D3DXVECTOR2(m_fTexU, m_fTexV + m_fTexHeight);
	pVtx[3].tex = D3DXVECTOR2(m_fTexU + m_fTexWidth, m_fTexV + m_fTexHeight);

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================================================
//    頂点バッファへ頂点座標を設定する処理
//=============================================================================
void CIconBillboard::SetVtxBuffPos(void)
{
	// 頂点情報の設定
	VERTEX_3D *pVtx;

	// 頂点バッファをロックし,頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 大きさを設定
	SetScale(m_fWidth, m_fHeight);

	// 頂点座標
	pVtx[0].pos = D3DXVECTOR3(-sinf(m_fAngle) * m_fLength, cosf(m_fAngle) * m_fLength, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(sinf(m_fAngle) * m_fLength, cosf(m_fAngle) * m_fLength, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-sinf(m_fAngle) * m_fLength, -cosf(m_fAngle) * m_fLength, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(sinf(m_fAngle) * m_fLength, -cosf(m_fAngle) * m_fLength, 0.0f);

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================================================
//    頂点バッファへ法線ベクトルを設定する処理
//=============================================================================
void CIconBillboard::SetVtxBuffNor(void)
{
	// 頂点情報の設定
	VERTEX_3D *pVtx;

	// 頂点バッファをロックし,頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 法線計算用ベクトル
	D3DXVECTOR3 vecVer[4];

	// 頂点の法線
	D3DXVECTOR3 nor1, nor2;

	// 外積用にベクトルを作る
	vecVer[0] = pVtx[0].pos - pVtx[2].pos;
	vecVer[1] = pVtx[1].pos - pVtx[2].pos;
	vecVer[2] = pVtx[2].pos - pVtx[1].pos;
	vecVer[3] = pVtx[3].pos - pVtx[1].pos;

	// ベクトルを外積で計算して法線を出す
	D3DXVec3Cross(&nor1, &vecVer[0], &vecVer[1]);
	D3DXVec3Cross(&nor2, &vecVer[3], &vecVer[2]);
	D3DXVec3Normalize(&nor1, &nor1);
	D3DXVec3Normalize(&nor2, &nor2);

	// 法線を代入
	pVtx[0].nor = nor1;
	pVtx[1].nor = (nor1 + nor2) / 2;
	pVtx[2].nor = (nor1 + nor2) / 2;
	pVtx[3].nor = nor2;

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================================================
//    頂点バッファへ頂点カラーを設定する処理
//=============================================================================
void CIconBillboard::SetVtxBuffCol(void)
{
	// 頂点情報の設定
	VERTEX_3D *pVtx;

	// 頂点バッファをロックし,頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点カラー
	pVtx[0].col = m_Col;
	pVtx[1].col = m_Col;
	pVtx[2].col = m_Col;
	pVtx[3].col = m_Col;

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================================================
//    頂点バッファへテクスチャ座標を設定する
//=============================================================================
void CIconBillboard::SetVtxBuffTex(void)
{
	// 頂点情報の設定
	VERTEX_3D *pVtx;

	// 頂点バッファをロックし,頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(m_fTexU, m_fTexV);
	pVtx[1].tex = D3DXVECTOR2(m_fTexU + m_fTexWidth, m_fTexV);
	pVtx[2].tex = D3DXVECTOR2(m_fTexU, m_fTexV + m_fTexHeight);
	pVtx[3].tex = D3DXVECTOR2(m_fTexU + m_fTexWidth, m_fTexV + m_fTexHeight);

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================================================
//    変数のクリア
//=============================================================================
void CIconBillboard::ClearVariable(void)
{
	// 各種値のクリア
	m_pTexture = NULL;                         // テクスチャへのポインタ
	m_pVtxBuff = NULL;                         // 頂点バッファへのポインタ
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);     // ポリゴンの座標
	m_Col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f); // ポリゴンの色
	m_fRot = 0.0f;                             // ポリゴンの向き
	m_fWidth = 0.0f;                           // ポリゴンの幅
	m_fHeight = 0.0f;                          // ポリゴンの高さ
	m_fAngle = 0.0f;                           // ポリゴンを出す角度
	m_fLength = 0.0f;                          // ポリゴンの長さ
	m_fTexU = 0.0f;                            // ポリゴンの左上テクスチャU座標
	m_fTexV = 0.0f;                            // ポリゴンの左上テクスチャV座標
	m_fTexWidth = 0.0f;                        // ポリゴンのテクスチャ座標の横幅
	m_fTexHeight = 0.0f;                       // ポリゴンのテクスチャ座標の縦幅
	D3DXMatrixIdentity(&m_MtxWorld);           // ワールドマトリックス
	m_bLighting = false;                       // ライティングするかしないか
}