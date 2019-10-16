//=============================================================================
//
// スコアの処理 [score.cpp]
// Author : 佐藤安純　SATOU ASUMI
//
//=============================================================================
#include "score.h"
#include "manager.h"
#include "scene2D.h"
#include "number2D.h"
#include "scene.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define CLEAR_TIME	(15)
#define RED_TIME	(20)

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
//スコアクラス
int CScore::m_nScoreData[MAX_NUMBER] = {};
int CScore::m_nScore = 0;

//スコアゲームクラス
int CScoreGame::m_nNumber[MAX_NUMBER] = {};

//スコアランキングクラス
int CScoreRanking::m_nNumber[RANK][MAX_NUMBER] = {};
int CScoreRanking::m_nHighScore[RANK] = {};
int CScoreRanking::m_nTime = 0;
CScoreRanking::FLASH CScoreRanking::m_Flash[RANK] = {};
bool CScoreRanking::m_bFlash = false;

//=============================================================================
// 生成処理
//=============================================================================
CScore * CScore::Create(TYPE type, int Priority, D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	CScore * pScore = NULL;

	if (type == TYPE_GAME)
	{
		pScore = new CScoreGame(Priority);
		pScore->SetSizePos(pos, size);
		pScore->Init();
	}
	else if (type == TYPE_RANKING)
	{
		pScore = new CScoreRanking(Priority);
		pScore->SetSizePos(pos, size);
		pScore->Init();
	}

	return pScore;
}

//=============================================================================
// コンストラクタ
//=============================================================================
CScore::CScore(int nPriority):CScene(nPriority){}

//=============================================================================
// デストラクタ
//=============================================================================
CScore::~CScore(){}

//=============================================================================
// スコアの加算
//=============================================================================
void CScore::AddScore(int nScore)
{
	//変数宣言
	int nCntNum;			//スコアの桁数のカウンター
	int nNum = 1;
	int nNum2 = 1;

	//スコアの加算
    m_nScore += nScore;

	//桁数の計算
	for (nCntNum = 0; nCntNum < MAX_NUMBER; nCntNum++)
	{
		nNum *= 10;
	}
	for (nCntNum = 0; nCntNum < MAX_NUMBER - 1; nCntNum++)
	{
		nNum2 *= 10;
	}

	for (nCntNum = 0; nCntNum < MAX_NUMBER; nCntNum++)
	{
		m_nScoreData[nCntNum] = (m_nScore % nNum) / nNum2;	//格納する数字を計算

		//桁数を１桁減らす
		nNum = nNum / 10;
		nNum2 = nNum2 / 10;
	}
}

//=============================================================================
// スコアの取得
//=============================================================================
int CScore::GetScore(void)
{
	return m_nScore;
}

//=============================================================================
//
// スコアゲーム
//
//=============================================================================

//=============================================================================
// コンストラクタ
//=============================================================================
CScoreGame::CScoreGame(int nPriority):CScore(nPriority){}

//=============================================================================
// デストラクタ
//=============================================================================
CScoreGame::~CScoreGame(){}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CScoreGame::Init()
{
	//変数宣言
	int nCntNum;
	int nCntNumber;

	//数字のポインタの初期化
	for (nCntNumber = 0; nCntNumber < MAX_NUMBER; nCntNumber++)
	{
		m_pNumber[nCntNumber] = NULL;
	}

	//数字の初期化
	for (nCntNum = 0; nCntNum < MAX_NUMBER; nCntNum++)
	{
		m_nNumber[nCntNum] = 0;
	}

	m_nScore = 0;

	Set(m_pos, m_size);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CScoreGame::Uninit(void)
{
	//変数宣言
	int nCntNumber;

	CManager::SetScore(m_nScore);	//スコアのセット

	for (nCntNumber = 0; nCntNumber < MAX_NUMBER; nCntNumber++)
	{
		if (m_pNumber[nCntNumber] != NULL)
		{
			m_pNumber[nCntNumber]->Uninit();

			delete m_pNumber[nCntNumber];
			m_pNumber[nCntNumber] = NULL;
		}
	}

	m_nScore = 0;

	for (int nCntNum = 0; nCntNum < MAX_NUMBER; nCntNum++)
	{
		m_nScoreData[nCntNum] = 0;
	}

	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CScoreGame::Update(void)
{
	//変数宣言
	int nCntNumber;

	for (nCntNumber = 0; nCntNumber < MAX_NUMBER; nCntNumber++)
	{
		if (m_pNumber[nCntNumber] != NULL)
		{
			m_pNumber[nCntNumber]->Update();
			m_pNumber[nCntNumber]->SetNumber(m_nScoreData[nCntNumber]);
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CScoreGame::Draw(void)
{
	//変数宣言
	int nCntNumber;

	for (nCntNumber = 0; nCntNumber < MAX_NUMBER; nCntNumber++)
	{
		if (m_pNumber[nCntNumber] != NULL)
		{
			m_pNumber[nCntNumber]->Draw();
		}
	}
}

//=============================================================================
// 設定処理
//=============================================================================
void CScoreGame::Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
{
	//変数宣言
	int nCntNumber;

	for (nCntNumber = 0; nCntNumber < MAX_NUMBER; nCntNumber++)
	{
		if (m_pNumber[nCntNumber] == NULL)
		{
			m_pNumber[nCntNumber] = CNumber2D::Create(D3DXVECTOR3(pos.x + (nCntNumber * (size.x * 2)), pos.y, pos.z), size, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

			if (m_pNumber[nCntNumber] != NULL)
			{
				m_pNumber[nCntNumber]->SetbUse(true);
			}
		}
	}
}

//=============================================================================
//
// スコアランキング
//
//=============================================================================

//=============================================================================
// コンストラクタ
//=============================================================================
CScoreRanking::CScoreRanking(int nPriority) :CScore(nPriority)
{
	//変数宣言
	int nCntNumber;

	for (int nCntRank = 0; nCntRank < RANK; nCntRank++)
	{
		for (nCntNumber = 0; nCntNumber < MAX_NUMBER; nCntNumber++)
		{
			m_pNumber[nCntRank][nCntNumber] = NULL;
		}
	}
}

//=============================================================================
// デストラクタ
//=============================================================================
CScoreRanking::~CScoreRanking()
{
	//変数宣言
	int nCntNum;

	for (int nCntRank = 0; nCntRank < RANK; nCntRank++)
	{
		for (nCntNum = 0; nCntNum < MAX_NUMBER; nCntNum++)
		{
			m_nNumber[nCntRank][nCntNum] = 0;
		}
	}	
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CScoreRanking::Init()
{
	//変数宣言
	int nCntNum;
	m_bFlash = false;	//新しいスコアの更新した状態じゃない

	for (int nCntRank = 0; nCntRank < RANK; nCntRank++)
	{
		for (nCntNum = 0; nCntNum < MAX_NUMBER; nCntNum++)
		{
			m_nNumber[nCntRank][nCntNum] = 0;
		}

		//スコアの設定
		m_nHighScore[nCntRank] = CManager::GetRankScore(nCntRank);
		m_Flash[nCntRank] = FLASH_OFF;
	}

	//ランキングのソート
	SortRanking();

	Set(m_pos, m_size);
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CScoreRanking::Uninit(void)
{
	//変数宣言
	int nCntNumber;

	//ランキングのスコアを保存する
	for (int nCntRank = 0; nCntRank < RANK; nCntRank++)
	{
		CManager::SetRankScore(nCntRank, m_nHighScore[nCntRank]);
	}

	for (int nCntRank = 0; nCntRank < RANK; nCntRank++)
	{
		for (nCntNumber = 0; nCntNumber < MAX_NUMBER; nCntNumber++)
		{
			if (m_pNumber[nCntRank][nCntNumber] != NULL)
			{
				m_pNumber[nCntRank][nCntNumber]->Uninit();

				delete m_pNumber[nCntRank][nCntNumber];
				m_pNumber[nCntRank][nCntNumber] = NULL;
			}
		}
	}

	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CScoreRanking::Update(void)
{
	for (int nCntRank = 0; nCntRank < RANK; nCntRank++)
	{
		switch (m_Flash[nCntRank])
		{
		case FLASH_CLEAR:
			m_nTime++;	//時間の加算

			//色の設定
			for (int nCntNumber = 0; nCntNumber < MAX_NUMBER; nCntNumber++)
			{
				m_pNumber[nCntRank][nCntNumber]->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.0f));
			}

			//状態の変更
			if ((m_nTime % CLEAR_TIME) == 0)
			{
				m_Flash[nCntRank] = FLASH_RED;
				m_nTime = 0;
			}
			break;

		case FLASH_RED:
			m_nTime++;	//時間の加算

			//色の設定
			for (int nCntNumber = 0; nCntNumber < MAX_NUMBER; nCntNumber++)
			{
				m_pNumber[nCntRank][nCntNumber]->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			}

			//状態の変更
			if ((m_nTime % RED_TIME) == 0)
			{
				m_Flash[nCntRank] = FLASH_CLEAR;
				m_nTime = 0;
			}
			break;
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CScoreRanking::Draw(void)
{
	//変数宣言
	int nCntNumber;
	for (int nCntRank = 0; nCntRank < RANK; nCntRank++)
	{
		for (nCntNumber = 0; nCntNumber < MAX_NUMBER; nCntNumber++)
		{
			if (m_pNumber[nCntRank][nCntNumber] != NULL)
			{
				m_pNumber[nCntRank][nCntNumber]->Draw();
			}
		}
	}
}

//=============================================================================
// 設定処理
//=============================================================================
void CScoreRanking::Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
{
	//変数宣言
	int nCntNumber;

	for (int nCntRank = 0; nCntRank < RANK; nCntRank++)
	{
		for (nCntNumber = 0; nCntNumber < MAX_NUMBER; nCntNumber++)
		{
			if (m_pNumber[nCntRank][nCntNumber] == NULL)
			{
				m_pNumber[nCntRank][nCntNumber] = CNumber2D::Create(D3DXVECTOR3(pos.x + (nCntNumber * (size.x * 2)), pos.y + (nCntRank * 100.0f), pos.z), size, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

				if (m_pNumber[nCntRank][nCntNumber] != NULL)
				{
					m_pNumber[nCntRank][nCntNumber]->SetbUse(true);
					m_pNumber[nCntRank][nCntNumber]->SetNumber(m_nNumber[nCntRank][nCntNumber]);
				}
			}
		}
	}
}

//=============================================================================
// ランキングのソート
//=============================================================================
void CScoreRanking::SortRanking(void)
{
	//変数宣言
	int nSubScore = 0;
	int nSubScore2 = 0;
	bool bGetNewScore = false;
	int nNum = 1;
	int nNum2 = 1;

	int nScore = CManager::GetScore();	//マネージャーから値を取得

	//ランキングのソート
	for (int nCntRank = 0; nCntRank < RANK; nCntRank++)
	{
		for (int nCntRank2 = 0; nCntRank2 < RANK - 1; nCntRank2++)
		{
			if (m_nHighScore[nCntRank] > m_nHighScore[nCntRank2])
			{
				nSubScore = m_nHighScore[nCntRank2];
				m_nHighScore[nCntRank2] = m_nHighScore[nCntRank];
				m_nHighScore[nCntRank] = nSubScore;
			}
		}
	}
	
	for (int nCntRank = 0; nCntRank < RANK; nCntRank++)
	{
		if (m_nHighScore[nCntRank] < nScore)
		{
			nSubScore = m_nHighScore[nCntRank];
			m_nHighScore[nCntRank] = nScore;
			nScore = nSubScore;

			if (m_bFlash == false)
			{
				//状態の設定
				SetState(nCntRank, STATE_NEW);
				m_bFlash = true;
			}
		}
	}

	//スコアを初期化する
	nScore = 0;
	CManager::SetScore(nScore);

	for (int nCntRank = 0; nCntRank < RANK; nCntRank++)
	{
		//変数の初期化
		nNum = 1;
		nNum2 = 1;

		//桁数の計算
		for (int nCntNum = 0; nCntNum < MAX_NUMBER; nCntNum++)
		{
			nNum *= 10;
		}
		for (int nCntNum = 0; nCntNum < MAX_NUMBER - 1; nCntNum++)
		{
			nNum2 *= 10;
		}

		//数字の設定
		for (int nCntNum = 0; nCntNum < MAX_NUMBER; nCntNum++)
		{
			m_nNumber[nCntRank][nCntNum] = (m_nHighScore[nCntRank] % nNum) / nNum2;	//格納する数字を計算

			//桁数を１桁減らす
			nNum = nNum / 10;
			nNum2 = nNum2 / 10;
		}
	}
}

//=============================================================================
// 状態の設定
//=============================================================================
void CScoreRanking::SetState(int nCnt, STATE state)
{
	if (state == STATE_NEW)
	{
		m_Flash[nCnt] = FLASH_CLEAR;
	}
}
