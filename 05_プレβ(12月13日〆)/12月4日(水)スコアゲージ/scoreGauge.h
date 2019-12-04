//*****************************************************************************
//
//     スコアゲージの処理[scoreGauge.h]
//     Author:Hodaka Niwa
//
//*****************************************************************************
#ifndef _SCOREGAUGE_H_
#define _SCOREGAUGE_H_

//*****************************************************************************
//    インクルードファイル
//*****************************************************************************
#include "main.h"
#include "game.h"

//*****************************************************************************
//    マクロ定義
//*****************************************************************************
#define MAX_CROWNNUM (3)

//*****************************************************************************
//    前方宣言
//*****************************************************************************
class CIcon2D;

//*****************************************************************************
//    スコアゲージクラスの定義
//*****************************************************************************
class CScoreGauge
{
public:     // 誰でもアクセス可能
	CScoreGauge();
	~CScoreGauge();

	static CScoreGauge *Create(const int nNumPlayer);

	HRESULT Init(const int nNumPlayer);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void AddGauge(const int nIdx, const int nPoint);
	void CutGauge(const int nIdx, const int nPoint);
	void CheckDispCrown(const int nIdx, const int nRank);

	//--------------------
	//	Set & Get
	//--------------------
	void SetNumPlayer(const int nNumPlayer) { m_nNumPlayer = nNumPlayer; }
	void SetGaugeBg(CIcon2D *pGaugeBg) { m_pGaugeBg = pGaugeBg; }
	void SetGauge2D(CIcon2D *pGauge2D, const int nIdx) { m_pGauge2D[nIdx] = pGauge2D; }
	void SetPlayerIdx(CIcon2D *pPlayerIdx, const int nIdx) { m_pPlayerIdx[nIdx] = pPlayerIdx; }
	void SetCrown(CIcon2D *pCrown, const int nIdx) { m_pCrown[nIdx] = pCrown; }
	void SetDispCrown(const bool bDisp, const int nIdx) { m_bDispCrown[nIdx] = bDisp; }
	void SetGaugeOnce(const float fGaugeOnce) { m_fGaugeOnce = fGaugeOnce; }

	int GetNumPlayer(void) { return m_nNumPlayer; }
	CIcon2D *GetGaugeBg(const int nIdx) { return m_pGaugeBg; }
	CIcon2D *GetGauge2D(const int nIdx) { return m_pGauge2D[nIdx]; }
	CIcon2D *GetPlayerIdx(const int nIdx) { return m_pPlayerIdx[nIdx]; }
	CIcon2D *GetCrown(const int nIdx) { return m_pCrown[nIdx]; }
	float GetGaugeOnce(void) { return m_fGaugeOnce; }

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	void CreateGaugeBg(void);
	void CreateGauge2D(void);
	void CreatePlayerIdx(void);
	void CreateCrown(void);
	void CalcGaugeOnceFromRing(void);

	void ReleaseGaugeBg(void);
	void ReleaseGauge2D(void);
	void ReleasePlayerIdx(void);
	void ReleaseCrown(void);

	void DrawGaugeBg(void);
	void DrawGauge2D(void);
	void DrawPlayerIdx(void);
	void DrawCrown(void);

	float m_fGaugePosY;							// ゲージの位置(Y座標)
	float m_fGaugeHeight;						// ゲージの高さ
	int m_nNumPlayer;							// ゲームに参加しているプレイヤーの人数
	CIcon2D *m_pGaugeBg;						// ゲージの背景
	CIcon2D *m_pGauge2D[MAX_NUMPLAYER * 2];		// 各プレイヤーのゲージ量
	CIcon2D *m_pPlayerIdx[MAX_NUMPLAYER * 2];	// 各プレイヤーのゲージを表す番号
	CIcon2D *m_pCrown[MAX_CROWNNUM];			// 王冠
	bool m_bDispCrown[MAX_CROWNNUM];			// 王冠を描画するかどうか
	float m_fGaugeOnce;							// 1ポイント毎のゲージ量
};

#endif