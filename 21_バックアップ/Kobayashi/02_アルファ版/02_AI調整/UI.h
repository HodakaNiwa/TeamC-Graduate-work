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
class CMiniMap;
class CCharacter;
class CSkilicon;

//*****************************************************************************
//    UIクラスの定義
//*****************************************************************************
class CUI
{
public:    // 誰でもアクセス可能
	CUI();
	~CUI();

	static CUI *Create(int nNumPlayer, int nAllCharacter);

	HRESULT Init(int nNumPlayer, int nAllCharacter);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void CreateSkilicon(CCharacter *pChara, int nIdx, int nRecastTime);

	//--------------------
	//	Set & Get
	//--------------------
	void SetNumPlayer(const int nNumPlayer) { m_nNumPlayer = nNumPlayer; }
	void SetNumAllCharacter(const int nNumAllCharacter) { m_nNumAllCharacter = nNumAllCharacter; }
	void SetScoreGame(CScoreGame *pScoreGame, const int nIdx) { m_pScoreGame[nIdx] = pScoreGame; }
	void SetMiniMap(CMiniMap *pMiniMap) { m_pMiniMap = pMiniMap; }
	void SetSkilicon(CSkilicon *pSkilicon, const int nIdx) { m_pSkilicon[nIdx] = pSkilicon; }
	int GetNumPlayer(void) { return m_nNumPlayer; }
	int GetNumAllCharacter(void) { return m_nNumAllCharacter; }
	CScoreGame *GetScoreGame(const int nIdx) { return m_pScoreGame[nIdx]; }
	CMiniMap *GetMiniMap(void) { return m_pMiniMap; }
	CSkilicon *GetSkilicon(const int nIdx) { return m_pSkilicon[nIdx]; }

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	//--------------------
	//	関数
	//--------------------
	void CreateScoreGame(void);
	void CreateMiniMap(void);

	void ReleaseScoreGame(void);
	void ReleaseMiniMap(void);
	void ReleaseSkilicon(void);

	void UpdateSkilicon(void);

	void DrawSkilicon(void);

	//--------------------
	//	メンバ変数
	//--------------------
	int m_nNumPlayer;								// ゲームに参加しているプレイヤーの人数(保存用)
	int m_nNumAllCharacter;							// ゲームに参加している人数(保存用)
	CScoreGame *m_pScoreGame[MAX_PLAYERNUM * 2];	// ゲームスコアクラスへのポインタ
	CMiniMap *m_pMiniMap;							// ミニマップクラスへのポインタ
	CSkilicon *m_pSkilicon[MAX_PLAYERNUM];			// スキルアイコンクラスへのポインタ
};

#endif