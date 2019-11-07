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
class CTitle;
class CSelect;
class CTutorial;
class CGame;
class CResult;
class CRanking;
class CFade;
class CModeBase;
class CInputXPad;
class CInputMouse;
class CRawMouse;

//=============================================================================
// マクロ定義
//=============================================================================
#define RANK (5)			//ランキングの数
#define MAX_PLAYERNUM	(4)	//プレイヤーの最大数

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
		MODE_SELECT,
		MODE_TUTORIAL,
		MODE_GAME,
		MODE_RESULT,
		MODE_RANKING,
		MODE_MAX,
	}MODE;

	//ゲームモードの状態
	typedef enum
	{
		GAMESTATE_NONE = 0,
		GAMESTATE_FIRSTCOUNTDOWN,
		GAMESTATE_GAME,
		GAMESTATE_FIN,
		GAMESTATE_MAX
	}GAMESTATE;

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
	static void SetScore(int nScore) { m_nScore = nScore; };											//スコアの設定
	static int GetScore(void) { return m_nScore; };														//スコアの取得
	static int GetNumDethButterfly(void) { return m_nDethButterfly; };									//倒した蝶の数を取得
	static int GetMaxCombo(void) { return m_nMaxConbo; };												//コンボ数の追加
	static int GetRankScore(int nCntRank) { return m_nRankScore[nCntRank]; };							//ランキングスコアの取得
	static void SetRankScore(int nCntRank, int nRankScore) { m_nRankScore[nCntRank] = nRankScore; };	//ランキングスコアの設定
	static CInputXPad * GetXPad(void) { return m_pXPad; }												//Xインプットの取得
	static CInputMouse * GetInputMouse(void) { return m_pInputMouse; }									//マウスの取得
	static GAMESTATE GetGameState(void) { return m_GameState; }											//ゲームの状態を取得
	static void SetGameState(GAMESTATE GameState) { m_GameState = GameState; }
	static CRawMouse * GetRawMouse(void) { return m_pRawMouse; }										//ラウマウスの取得
	static void CreateRawMouse(HWND hWnd);
	//画面の取得
	static CTitle * GetTitle(void) { return (CTitle *)m_pModeBase; }			//タイトル
	static CSelect * GetSelect(void) { return(CSelect *)m_pModeBase; }			//セレクト
	static CTutorial * GetTutorial(void) { return(CTutorial *)m_pModeBase; }	//チュートリアル
	static CGame * GetGame(void) { return (CGame *)m_pModeBase; };				//ゲーム
	static CResult * GetResult(void) { return (CResult *)m_pModeBase; }			//リザルト
	static CRanking * GetRanking(void) { return (CRanking *)m_pModeBase; }		//ランキング
	static CModeBase * GetModeBase(void) { return m_pModeBase; }				//モードベースの取得
private:
	static CRenderer * m_pRenderer;
	static CSound * m_pSound;
	static CInputKeyboard * m_pInputKeyboard;
	static CGamePad * m_pInputGamePad;
	static CLight * m_pLight;
	static CModeBase * m_pModeBase;
	static CFade * m_pFade;
	static MODE m_mode;
	static int m_nScore;
	static int m_nRankScore[RANK];
	static int m_nMaxConbo;
	static int m_nDethButterfly;
	static CInputXPad * m_pXPad;
	static GAMESTATE m_GameState;
	static CInputMouse * m_pInputMouse;
	static CRawMouse * m_pRawMouse;
};
#endif