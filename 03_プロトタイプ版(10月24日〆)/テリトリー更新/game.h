//=============================================================================
//
// ゲームの処理 [game.h]
// Author : 佐藤安純
//
//=============================================================================
#ifndef _GAME_H_
#define _GAME_H_

#include "main.h"
#include "modebase.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CPause;
class CGameCamera;
class CSceneMeshFiled;
class CLoadTextObject;
class CLoadTextTerritory;
class CPlayer;

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_NUMPLAYER (4)	//プレイヤーの最大数

//=====================
// 基本クラス
//=====================
class CGame : public CModeBase
{
public:
	typedef enum
	{
		GAMESTATE_NONE = 0,					//何もしていない状態
		GAMESTATE_NORMAL,					//通常状態
		GAMESTATE_CLEAR,					//ゲームクリア
		GAMESTATE_OVER,						//ゲームオーバー
		GAMESTATE_END,						//ゲーム終了
		GAMESTATE_MAX						//最大数
	}GAMESTATE;

	CGame();				//コンストラクタ
	~CGame();				//デストラクタ
	void Init(void);		//初期化処理
	void Uninit(void);		//終了処理
	void Update(void);		//更新処理
	void Draw(void);		//描画処理

	//--------------------
	//	Set & Get
	//--------------------
	static void SetGameState(GAMESTATE state) { m_gameState = state; }
	static void SetPause(bool bPause);
	static bool GetPause(void) { return m_bPause; }
	static GAMESTATE GetGameState(void) { return m_gameState; }
	CGameCamera * GetGameCamera(int nIdx) { return m_pGameCamera[nIdx]; }
	CPlayer * GetPlayer(int nNumPlayer) { return m_pPlayer[nNumPlayer]; }

private:
	static GAMESTATE m_gameState;				//ゲーム状態
	GAMESTATE m_NowGameState;					//現在のゲーム状況
	int m_nCounterGameState;					//状態カウンタ
	static CPause *m_pPause;					//ポーズのポインタ
	static bool m_bPause;						//現在ポーズかポーズじゃないか
	
	static CSceneMeshFiled * m_pMeshFiled;		//メッシュフィールド
	CLoadTextObject * m_pLoadObject;			// オブジェクトの読み込み
	CLoadTextTerritory * m_pLoadTerritory;		// テリトリーの読み込み
	CGameCamera * m_pGameCamera[4];				// ゲームカメラ
	CPlayer *m_pPlayer[MAX_NUMPLAYER];
};
#endif