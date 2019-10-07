//=============================================================================
//
// マネージャーの処理 [manager.h]
// Author : 佐藤 安純　[ SATO ASUMI ]
//
//=============================================================================
#ifndef _MANAGER_H_
#define _MANAGER_H_

#include "main.h"

//=============================================================================
// 前方宣言
//=============================================================================
class CRenderer;
class CSound;
class CInputKeyboard;
class CGamePad;
class CLight;
class CGame;
class CRanking;
class CTitle;
class CTutorial;
class CFade;
class CResult;

//=============================================================================
// マクロ定義
//=============================================================================
#define RANK (5)			//ランキングの数
#define MAX_PLAYER	(10)

//=============================================================================
// マネージャークラス
//=============================================================================
class CManager
{
public:
	CManager();
	~CManager();

	//モードの種類
	typedef enum
	{
		MODE_TITLE = 0,
		MODE_TUTORIAL,
		MODE_GAME,
		MODE_RESULT,
		MODE_RANKING,
		MODE_MAX,
	}MODE;

	//メンバ関数
	HRESULT Init(HINSTANCE hInstance, HWND hWnd, bool bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CRenderer * GetRenderer(void);
	static CInputKeyboard * GetInputkeyboard(void);
	static CGamePad * GetInputGamePad(void) { return m_pInputGamePad; };
	static CSound * GetSound(void);
	static CLight * GetLight(void) { return m_pLight; };												//ライトの取得
	static void SetMode(MODE mode);																		//モードの設定
	static CFade * GetFade(void) { return m_pFade; };													//フェードポインタの取得
	static MODE GetMode(void) { return m_mode; };														//モードの取得
	static CGame * GetGame(void) { return m_pGame; };													//ゲームのポインタの取得
	static CTutorial * GetTutorial(void) { return m_pTutorial; };										//チュートリアルのポインタを取得
	static void SetScore(int nScore) { m_nScore = nScore; };											//スコアの設定
	static int GetScore(void) { return m_nScore; };														//スコアの取得
	static int GetNumDethButterfly(void) { return m_nDethButterfly; };									//倒した蝶の数を取得
	static int GetMaxCombo(void) { return m_nMaxConbo; };												//コンボ数の追加
	static int GetRankScore(int nCntRank) { return m_nRankScore[nCntRank]; };							//ランキングスコアの取得
	static void SetRankScore(int nCntRank, int nRankScore) { m_nRankScore[nCntRank] = nRankScore; };	//ランキングスコアの設定

protected:
	
private:
	static CRenderer * m_pRenderer;
	static CSound * m_pSound;
	static CInputKeyboard * m_pInputKeyboard;
	static CGamePad * m_pInputGamePad;
	static CLight * m_pLight;
	static CGame * m_pGame;
	static CResult * m_pResult;
	static CRanking * m_pRanking;
	static CTitle * m_pTitle;
	static CTutorial * m_pTutorial;
	static CFade * m_pFade;
	static MODE m_mode;
	static int m_nScore;
	static int m_nRankScore[RANK];
	static int m_nMaxConbo;
	static int m_nDethButterfly;
};
#endif