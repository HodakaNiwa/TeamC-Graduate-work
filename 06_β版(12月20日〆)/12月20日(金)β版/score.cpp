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
#include "game.h"
#include "select.h"
#include "moveUI.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define CLEAR_TIME	(15)
#define RED_TIME	(20)
#define GAMESCORE_INTERVAL (1.8f)
#define SCORE_DESTPOS (1000.0f)
#define FLAG_POS_X	  (310.0f)
#define NAME_POS_X	  (215.0f)
#define CHARNAME_POS_X	(560.0f)
#define POS_Y	  (180.0f)

//国旗
#define JAPAN_FLAG			("data/TEXTURE/Select/NationalFlag/Japan.png")
#define ROSSIA_FLAG			("data/TEXTURE/Select/NationalFlag/Russia.png")
#define AMERICA_FLAG		("data/TEXTURE/Select/NationalFlag/America.png")
#define BLAZIL_FLAG			("data/TEXTURE/Select/NationalFlag/Brazil.png")
#define ENGLAND_FLAG		("data/TEXTURE/Select/NationalFlag/Engrand.png")
#define ITARY_FLAG			("data/TEXTURE/Select/NationalFlag/Italy.png")
#define NEWZYLAND_FLAG		("data/TEXTURE/Select/NationalFlag/Newzealand.png")
#define SOUTHAFRICA_FLAG	("data/TEXTURE/Select/NationalFlag/SouthAfrica.png")

//国名
#define JAPAN_NAME			("data/TEXTURE/Select/CountryName/JPN.png")
#define ROSSIA_NAME			("data/TEXTURE/Select/CountryName/RUS.png")
#define AMERICA_NAME		("data/TEXTURE/Select/CountryName/USA.png")
#define BLAZIL_NAME			("data/TEXTURE/Select/CountryName/BRA.png")
#define ENGLAND_NAME		("data/TEXTURE/Select/CountryName/GBR.png")
#define ITARY_NAME			("data/TEXTURE/Select/CountryName/ITA.png")
#define NEWZYLAND_NAME		("data/TEXTURE/Select/CountryName/NGL.png")
#define SOUTHAFRICA_NAME	("data/TEXTURE/Select/CountryName/ZAF.png")

//キャラ名
#define JAPAN_NAME_CHAR			("data/TEXTURE/Ranking/CharName/JapanName.png")
#define ROSSIA_NAME_CHAR		("data/TEXTURE/Ranking/CharName/RossiaName.png")
#define AMERICA_NAME_CHAR		("data/TEXTURE/Ranking/CharName/AmericaName.png")
#define BLAZIL_NAME_CHAR		("data/TEXTURE/Ranking/CharName/BrazilName.png")
#define ENGLAND_NAME_CHAR		("data/TEXTURE/Ranking/CharName/EnglandName.png")
#define ITARY_NAME_CHAR			("data/TEXTURE/Ranking/CharName/ItalyName.png")
#define NEWZYLAND_NAME_CHAR		("data/TEXTURE/Ranking/CharName/NewzyLandName.png")
#define SOUTHAFRICA_NAME_CHAR	("data/TEXTURE/Ranking/CharName/SouthAfricaName.png")

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
//スコアクラス

//スコアゲームクラス

//スコアランキングクラス
int CScoreRanking::m_nNumber[RANK][MAX_NUMBER] = {};
int CScoreRanking::m_nHighScore[RANK] = {};
CScoreRanking::FLASH CScoreRanking::m_Flash[RANK] = {};
LPDIRECT3DTEXTURE9 CScoreRanking::m_pCuntryTex[CUNTRY] = {};
LPDIRECT3DTEXTURE9 CScoreRanking::m_pCuntryNameTex[CUNTRY] = {};
LPDIRECT3DTEXTURE9 CScoreRanking::m_pCharNameTex[CUNTRY] = {};

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
CScore::CScore(int nPriority):CScene(nPriority , OBJTYPE_SCORE)
{
	m_nScore = 0;
	for (int nCnt = 0; nCnt < MAX_NUMBER; nCnt++)
	{
		m_nScoreData[nCnt] = 0;
	}
	m_size = INITIALIZE_VECTOR3;
	m_pos = INITIALIZE_VECTOR3;
}

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
CScoreGame::CScoreGame(int nPriority):CScore(nPriority)
{
	for (int nCnt = 0; nCnt < MAX_NUMBER; nCnt++)
	{
		m_pNumber[nCnt] = NULL;
		m_nNumber[nCnt] = 0;
	}
}

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

	CGame *pGame = CManager::GetGame();				// ゲームの取得←追加(よしろう)
	CEventCamera *pEveCam = pGame->GetEveCam();		// イベントカメラの取得←追加(よしろう)
	if (pEveCam == NULL)							// イベントカメラが消えていたら←追加(よしろう)
	{
		for (nCntNumber = 0; nCntNumber < MAX_NUMBER; nCntNumber++)
		{
			if (m_pNumber[nCntNumber] != NULL)
			{
				m_pNumber[nCntNumber]->Draw();
			}
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
			m_pNumber[nCntNumber] = CNumber2D::Create(D3DXVECTOR3(pos.x + (nCntNumber * (size.x * GAMESCORE_INTERVAL)), pos.y, pos.z), size, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

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
	m_bFlash = false;
	m_nCounter = 0;
	m_nMovingIdx = 0;

	for (int nCntRank = 0; nCntRank < RANK; nCntRank++)
	{
		for (nCntNumber = 0; nCntNumber < MAX_NUMBER; nCntNumber++)
		{
			m_pNumber[nCntRank][nCntNumber] = NULL;
		}
	}

	for (int nCnt = 0; nCnt < RANK; nCnt++)
	{
		m_nCuntry[nCnt] = 0;
		m_nCharType[nCnt] = 0;
		m_nTime[nCnt] = 0;
		m_bMoving[nCnt] = false;
		m_ScorePos[nCnt] = INITIALIZE_VECTOR3;
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
	
	for (int nCntRank = 0; nCntRank < RANK; nCntRank++)
	{
		for (nCntNum = 0; nCntNum < MAX_NUMBER; nCntNum++)
		{
			m_nNumber[nCntRank][nCntNum] = 0;
		}

		//変数の初期化
		m_bFlash = false;
		m_nTime[nCntRank] = 0;
		m_nCuntry[nCntRank] = 0;
		m_nCharType[nCntRank] = 0;
		m_pCuntryFlag[nCntRank] = NULL;
		m_pCuntryName[nCntRank] = NULL;
		m_pCharName[nCntRank] = NULL;

		//スコアの設定
		m_nHighScore[nCntRank] = CManager::GetRankScore(nCntRank);
		m_nCuntry[nCntRank] = CManager::GetRankCuntry(nCntRank);
		m_nCharType[nCntRank] = CManager::GetRankCharType(nCntRank);
		m_Flash[nCntRank] = FLASH_OFF;
	}
	//ランキングのソート
	SortRanking();

	Set(m_pos, m_size);

	for (int nCnt = 0; nCnt < RANK; nCnt++)
	{
		m_ScorePos[nCnt] = m_pNumber[nCnt][0]->GetPos();
		m_bMoving[nCnt] = false;

		//国旗の生成
		if (m_pCuntryFlag[nCnt] == NULL)
		{
			m_pCuntryFlag[nCnt] = CMoveUI::Create(D3DXVECTOR3(1460.0f, POS_Y + (nCnt * 60.0f), 0.0f), D3DXVECTOR3(37.0f, 27.0f, 0.0f), m_pCuntryTex[m_nCuntry[nCnt]]);
		}

		//国名の生成
		if (m_pCuntryName[nCnt] == NULL)
		{
			m_pCuntryName[nCnt] = CMoveUI::Create(D3DXVECTOR3(1400.0f, POS_Y + (nCnt * 60.0f), 0.0f), D3DXVECTOR3(50.0f, 27.0f, 0.0f), m_pCuntryNameTex[m_nCuntry[nCnt]]);
		}

		//キャラ名の生成
		if (m_pCharName[nCnt] == NULL)
		{
			m_pCharName[nCnt] = CMoveUI::Create(D3DXVECTOR3(1660.0f, POS_Y + (nCnt * 60.0f), 0.0f), D3DXVECTOR3(200.0f, 27.0f, 0.0f), m_pCharNameTex[m_nCuntry[nCnt]]);

			//UV設定
			int nType = m_nCharType[nCnt];
			m_pCharName[nCnt]->SetTexUV(0.0f, 1.0f, 0.0f + (nType * 0.25f), 0.25f + (nType * 0.25f));
		}
	}

	m_bMoving[RANK - 1] = true;
	m_nMovingIdx = RANK -1;

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
		CManager::SetRankScore(nCntRank, m_nHighScore[nCntRank]);	//スコアの保存
		CManager::SetRankCuntry(nCntRank, m_nCuntry[nCntRank]);		//国の保存
		CManager::SetRankCharType(nCntRank, m_nCharType[nCntRank]);	//キャラタイプの保存

		m_pCuntryFlag[nCntRank] = NULL;
		m_pCuntryName[nCntRank] = NULL;
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
			m_nTime[nCntRank]++;	//時間の加算

			//色の設定
			for (int nCntNumber = 0; nCntNumber < MAX_NUMBER; nCntNumber++)
			{
				m_pNumber[nCntRank][nCntNumber]->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.0f));
			}

			//状態の変更
			if ((m_nTime[nCntRank] % CLEAR_TIME) == 0)
			{
				m_Flash[nCntRank] = FLASH_RED;
				m_nTime[nCntRank] = 0;
			}
			break;

		case FLASH_RED:
			m_nTime[nCntRank]++;	//時間の加算

						//色の設定
			for (int nCntNumber = 0; nCntNumber < MAX_NUMBER; nCntNumber++)
			{
				m_pNumber[nCntRank][nCntNumber]->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			}

			//状態の変更
			if ((m_nTime[nCntRank] % RED_TIME) == 0)
			{
				m_Flash[nCntRank] = FLASH_CLEAR;
				m_nTime[nCntRank] = 0;
			}
			break;
		}
	}

	// カウンターを増加
	m_nCounter++;

	if (m_nCounter == 10 && 0 <= m_nMovingIdx)
	{
		m_nCounter = 0;
		m_nMovingIdx--;
		m_bMoving[m_nMovingIdx] = true;
	}


	for (int nCnt = 0; nCnt < RANK; nCnt++)
	{
		if (m_bMoving[nCnt] == true)
		{
			m_ScorePos[nCnt].x -= 60.0f;

			//移動処理
			if (m_pCuntryFlag[nCnt] != NULL) { m_pCuntryFlag[nCnt]->UpdateMoveDest(D3DXVECTOR3(FLAG_POS_X, POS_Y + (nCnt * 60.0f), 0.0f), 60.0f); }
			if (m_pCuntryName[nCnt] != NULL) { m_pCuntryName[nCnt]->UpdateMoveDest(D3DXVECTOR3(NAME_POS_X, POS_Y + (nCnt * 60.0f), 0.0f), 60.0f); }
			if (m_pCharName[nCnt] != NULL) { m_pCharName[nCnt]->UpdateMoveDest(D3DXVECTOR3(CHARNAME_POS_X, POS_Y + (nCnt * 60.0f), 0.0f), 60.0f); }
		}

		// 目的の位置より上に動かしていたら戻す
		if (m_ScorePos[nCnt].x < SCORE_DESTPOS)
		{
			m_ScorePos[nCnt].x = SCORE_DESTPOS;
		}
		m_pNumber[nCnt][2]->SetPos(D3DXVECTOR3(m_ScorePos[nCnt].x, 180.0f + (60 * nCnt), 0.0f));
		m_pNumber[nCnt][1]->SetPos(D3DXVECTOR3(m_ScorePos[nCnt].x - 40, 180.0f + (60 * nCnt), 0.0f));
		m_pNumber[nCnt][0]->SetPos(D3DXVECTOR3(m_ScorePos[nCnt].x - 80, 180.0f + (60 * nCnt), 0.0f));
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
				m_pNumber[nCntRank][nCntNumber] = CNumber2D::Create(D3DXVECTOR3(pos.x + (nCntNumber * (size.x * 2)), pos.y + (nCntRank * 60.0f), pos.z), size, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

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
	int nScore = 0;
	int nSubCuntry = 0;
	int nCuntry = 0;
	int nSubCharType = 0;
	int nCharType = 0;
	int nMaxPlayer = CSelect::GetEntryPlayer();

	for (int nCntChar = 0; nCntChar < nMaxPlayer; nCntChar++)
	{
		nScore = CManager::GetScore(nCntChar);			//マネージャーから値を取得
		nCuntry = CManager::GetCuntry(nCntChar);		//マネージャーから国のタイプを取得
		nCharType = CManager::GetCharType(nCntChar);	//マネージャーからキャラタイプ取得

		for (int nCntRank = 0; nCntRank < RANK; nCntRank++)
		{
			if (m_nHighScore[nCntRank] < nScore)
			{
				//スコアの入替え
				nSubScore = m_nHighScore[nCntRank];
				m_nHighScore[nCntRank] = nScore;
				nScore = nSubScore;

				//国の入替え
				nSubCuntry = m_nCuntry[nCntRank];
				m_nCuntry[nCntRank] = nCuntry;
				nCuntry = nSubCuntry;

				//キャラタイプの入替え
				nSubCharType = m_nCharType[nCntRank];
				m_nCharType[nCntRank] = nCharType;
				nCharType = nSubCharType;

				if (m_bFlash == false)
				{
					for (int nCnt = RANK - 1; nCnt > nCntRank; nCnt--)
					{
						if (m_Flash[nCnt] == FLASH_CLEAR)
						{
							m_Flash[nCnt] = FLASH_OFF;
							
							if (nCnt < RANK - 1) { m_Flash[nCnt + 1] = FLASH_CLEAR; }
						}
					}

					//状態の設定
					SetState(nCntRank, STATE_NEW);
					m_bFlash = true;
				}
			}
		}

		m_bFlash = false;
	}

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

//=============================================================================
// テクスチャの読み込み
//=============================================================================
void CScoreRanking::LoadTex(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//国旗テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, JAPAN_FLAG, &m_pCuntryTex[0]);
	D3DXCreateTextureFromFile(pDevice, ROSSIA_FLAG, &m_pCuntryTex[1]);
	D3DXCreateTextureFromFile(pDevice, AMERICA_FLAG, &m_pCuntryTex[2]);
	D3DXCreateTextureFromFile(pDevice, BLAZIL_FLAG, &m_pCuntryTex[3]);
	D3DXCreateTextureFromFile(pDevice, ENGLAND_FLAG, &m_pCuntryTex[4]);
	D3DXCreateTextureFromFile(pDevice, ITARY_FLAG, &m_pCuntryTex[5]);
	D3DXCreateTextureFromFile(pDevice, NEWZYLAND_FLAG, &m_pCuntryTex[6]);
	D3DXCreateTextureFromFile(pDevice, SOUTHAFRICA_FLAG, &m_pCuntryTex[7]);

	//国名テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, JAPAN_NAME, &m_pCuntryNameTex[0]);
	D3DXCreateTextureFromFile(pDevice, ROSSIA_NAME, &m_pCuntryNameTex[1]);
	D3DXCreateTextureFromFile(pDevice, AMERICA_NAME, &m_pCuntryNameTex[2]);
	D3DXCreateTextureFromFile(pDevice, BLAZIL_NAME, &m_pCuntryNameTex[3]);
	D3DXCreateTextureFromFile(pDevice, ENGLAND_NAME, &m_pCuntryNameTex[4]);
	D3DXCreateTextureFromFile(pDevice, ITARY_NAME, &m_pCuntryNameTex[5]);
	D3DXCreateTextureFromFile(pDevice, NEWZYLAND_NAME, &m_pCuntryNameTex[6]);
	D3DXCreateTextureFromFile(pDevice, SOUTHAFRICA_NAME, &m_pCuntryNameTex[7]);

	//キャラ名テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, JAPAN_NAME_CHAR, &m_pCharNameTex[0]);
	D3DXCreateTextureFromFile(pDevice, ROSSIA_NAME_CHAR, &m_pCharNameTex[1]);
	D3DXCreateTextureFromFile(pDevice, AMERICA_NAME_CHAR, &m_pCharNameTex[2]);
	D3DXCreateTextureFromFile(pDevice, BLAZIL_NAME_CHAR, &m_pCharNameTex[3]);
	D3DXCreateTextureFromFile(pDevice, ENGLAND_NAME_CHAR, &m_pCharNameTex[4]);
	D3DXCreateTextureFromFile(pDevice, ITARY_NAME_CHAR, &m_pCharNameTex[5]);
	D3DXCreateTextureFromFile(pDevice, NEWZYLAND_NAME_CHAR, &m_pCharNameTex[6]);
	D3DXCreateTextureFromFile(pDevice, SOUTHAFRICA_NAME_CHAR, &m_pCharNameTex[7]);
}

//=============================================================================
// テクスチャの破棄
//=============================================================================
void CScoreRanking::UnloadTex(void)
{
	for (int nCnt = 0; nCnt < CUNTRY; nCnt++)
	{
		if (m_pCuntryTex[nCnt] != NULL)
		{//国旗テクスチャの破棄
			m_pCuntryTex[nCnt]->Release();
			m_pCuntryTex[nCnt] = NULL;
		}

		if (m_pCuntryNameTex[nCnt] != NULL)
		{//国名テクスチャの破棄
			m_pCuntryNameTex[nCnt]->Release();
			m_pCuntryNameTex[nCnt] = NULL;
		}

		if (m_pCharNameTex[nCnt] != NULL)
		{//キャラ名テクスチャの破棄
			m_pCharNameTex[nCnt]->Release();
			m_pCharNameTex[nCnt] = NULL;
		}
	}
}