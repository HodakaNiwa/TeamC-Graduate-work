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
//class CCharacter;
class CPlayer;
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
	CGameCamera * GetGameCamera(void) { return m_pGameCamera; }
	//CCharacter* GetCharacter() { return m_pCharacter; }
	CPlayer* GetPlayer() { return m_pPlayer; }

private:
	static GAMESTATE m_gameState;				//ゲーム状態
	GAMESTATE m_NowGameState;					//現在のゲーム状況
	int m_nCounterGameState;					//状態カウンタ
	static CPause *m_pPause;					//ポーズのポインタ
	static bool m_bPause;						//現在ポーズかポーズじゃないか
	
	CGameCamera * m_pGameCamera;				// カメラ
	static CSceneMeshFiled * m_pMeshFiled;		//メッシュフィールド


	//	---<<小林が追加しました>>★	★	★	★	★	★	★	★	★	★
	//static CCharacter* m_pCharacter;	//	モデル
	static CPlayer* m_pPlayer;	//	モデル
	//	★	★	★	★	★	★	★	★	★	★	★	★	★	★	★	★

};
#endif