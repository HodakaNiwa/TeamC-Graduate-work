//*****************************************************************************
//
//     UIの処理[UI.h]
//     Author:Hodaka Niwa
//
//*****************************************************************************
#ifndef _UI_H_
#define _UI_H_

//*****************************************************************************
//    インクルードファイル
//*****************************************************************************
#include "main.h"
#include "manager.h"

//*****************************************************************************
//    マクロ定義
//*****************************************************************************

//*****************************************************************************
//    前方宣言
//*****************************************************************************
class CScoreGame;

//*****************************************************************************
//    UIクラスの定義
//*****************************************************************************
class CUI
{
public:    // 誰でもアクセス可能
	CUI();
	~CUI();

	static CUI *Create(int nNumPlayer);

	HRESULT Init(int nNumPlayer);
	void Uninit(void);
	void Update(void);

	void SetNumPlayer(const int nNumPlayer) { m_nNumPlayer = nNumPlayer; }
	void SetScoreGame(CScoreGame *pScoreGame, const int nIdx) { m_pScoreGame[nIdx] = pScoreGame; }
	int GetNumPlayer(void) { return m_nNumPlayer; }
	CScoreGame *GetScoreGame(const int nIdx) { return m_pScoreGame[nIdx]; }

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	//--------------------
	//	関数
	//--------------------
	void CreateScoreGame(void);
	void ReleaseScoreGame(void);

	//--------------------
	//	メンバ変数
	//--------------------
	int m_nNumPlayer;							// ゲームに参加している人数(保存用)
	CScoreGame *m_pScoreGame[MAX_PLAYERNUM];	// ゲームスコアクラスへのポインタ
};

#endif