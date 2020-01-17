//*****************************************************************************
//
//     ミニマップの処理[minimap.h]
//     Author:Hodaka Niwa
//
//*****************************************************************************
#ifndef _MINIMAP_H_
#define _MINIMAP_H_

//*****************************************************************************
//    インクルードファイル
//*****************************************************************************
#include "main.h"
#include "scene2D.h"

//*****************************************************************************
//    マクロ定義
//*****************************************************************************
#define MINIMAP_MAX_PLAYERNUM (8)

//*****************************************************************************
//    前方宣言
//*****************************************************************************
class CCharacter;

//*****************************************************************************
//    ミニマップクラスの定義
//*****************************************************************************
class CMiniMap
{
public:    // 誰でもアクセス可能
	CMiniMap();
	~CMiniMap();

	static CMiniMap *Create(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SettingPlayerIconPos(void);
	void SettingBGPos(void);

	//--------------------
	//	Set & Get
	//--------------------
	void SetCharacter(CCharacter *pChara, const int nIdx) { m_pCharacter[nIdx] = pChara; }

	CScene2D *GetBG(void) { return m_pBG; }
	CScene2D *GetPlayerIcon(const int nIdx) { return m_pPlayerIcon[nIdx]; }
	CCharacter *GetCharacter(const int nIdx) { return m_pCharacter[nIdx]; }

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	//--------------------
	//	関数
	//--------------------
	void CreateBG(void);
	void CreatePlayerIcon(void);
	void ReleaseBG(void);
	void ReleasePlayerIcon(void);


	//--------------------
	//	メンバ変数
	//--------------------
	CScene2D *m_pBG;									// 背景に使用するポリゴン
	CScene2D *m_pPlayerIcon[MINIMAP_MAX_PLAYERNUM];		// 各プレイヤーの位置を表示するポリゴン
	CCharacter *m_pCharacter[MINIMAP_MAX_PLAYERNUM];	// プレイヤーの情報へのポインタ
	D3DXVECTOR3 m_BGPos;								// 背景の位置
};

#endif