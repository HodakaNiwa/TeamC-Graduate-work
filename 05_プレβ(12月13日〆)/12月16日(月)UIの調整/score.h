//=============================================================================
//
// スコアの処理 [score.h]
// Author : 佐藤 安純　[ SATO ASUMI ]
//
//=============================================================================
#ifndef _SCORE_H_
#define _SCORE_H_

#include "main.h"
#include "scene.h"
#include "scene2D.h"

//=============================================================================
// 前方宣言
//=============================================================================
class CNumber2D;
class CIcon2D;
class CMoveUI;

//=============================================================================
// マクロ定義
//=============================================================================
#define MAX_NUMBER	(3)
#define RANK		(8)
#define CUNTRY		(8)

//=============================================================================
// スコアクラス
//=============================================================================
class CScore : public CScene
{
public:
	typedef enum
	{
		TYPE_GAME,
		TYPE_RANKING,
		TYPE_NONE
	}TYPE;

	//メンバ関数
	CScore(int nPriority);
	virtual ~CScore();
	virtual HRESULT Init(void) = 0;
	virtual void Uninit(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;
	static CScore * Create(TYPE type, int Priority, D3DXVECTOR3 pos, D3DXVECTOR3 size);
	void AddScore(int nScore);
	int GetScore(void);
	void SetSizePos(const D3DXVECTOR3 pos, const D3DXVECTOR3 size) { m_size = size; m_pos = pos; };

protected:
	int m_nScore;
	int m_nScoreData[MAX_NUMBER];
	D3DXVECTOR3 m_size;
	D3DXVECTOR3 m_pos;
};

//=============================================================================
// スコアゲーム
//=============================================================================
class CScoreGame : public CScore
{
public:
	CScoreGame(int Priority);
	~CScoreGame();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	//関数宣言
	void Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);
	CNumber2D * m_pNumber[MAX_NUMBER];	// ナンバーのポインタ
	int m_nNumber[MAX_NUMBER];			// 各数字番号
};

//=============================================================================
// スコアランキング
//=============================================================================
class CScoreRanking : public CScore
{
public:
	typedef enum
	{
		STATE_NEW = 0,
		STATE_NOMAL,
	}STATE;

	typedef enum
	{
		FLASH_CLEAR = 0,
		FLASH_RED,
		FLASH_OFF,
	}FLASH;

	CScoreRanking(int Priority);
	~CScoreRanking();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static void LoadTex(void);
	static void UnloadTex(void);

private:
	void SortRanking(void);
	static void SetState(int nCnt, STATE state);

	//関数宣言
	static LPDIRECT3DTEXTURE9 m_pCuntryTex[CUNTRY];
	static LPDIRECT3DTEXTURE9 m_pCuntryNameTex[CUNTRY];
	static LPDIRECT3DTEXTURE9 m_pCharNameTex[CUNTRY];
	void Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);
	CNumber2D * m_pNumber[RANK][MAX_NUMBER];					//ナンバーのポインタ
	CMoveUI * m_pCuntryFlag[RANK];								//国旗
	CMoveUI * m_pCuntryName[RANK];								//国名
	CMoveUI * m_pCharName[RANK];								//キャラ名
	static int			m_nNumber[RANK][MAX_NUMBER];			//各数字番号
	static int			m_nHighScore[RANK];						//スコアの数
	int					m_nCuntry[RANK];						//国の種類
	int					m_nCharType[RANK];						//キャラタイプ
	static FLASH		m_Flash[RANK];							//フラッシュ状態
	bool				m_bFlash;								//新しいスコアを取得したかどうか
	int					m_nTime[RANK];							//時間の加算
	int					m_nCounter = 0;
	bool                m_bMoving[RANK];
	int                 m_nMovingIdx;
	D3DXVECTOR3			m_ScorePos[RANK];
};

#endif
