//*****************************************************************************
//
//     キャラアイコンの処理[charaicon.h]
//     Author:Hodaka Niwa
//
//*****************************************************************************
#ifndef _CHARAICON_H_
#define _CHARAICON_H_

//*****************************************************************************
//    インクルードファイル
//*****************************************************************************
#include "skilicon.h"

//*****************************************************************************
//    マクロ定義
//*****************************************************************************


//*****************************************************************************
//    前方宣言
//*****************************************************************************
class CCharacter;

//*****************************************************************************
//    キャラアイコンクラスの定義
//*****************************************************************************
class CCharaicon : public CIcon2D
{
public:    // 誰でもアクセス可能
	CCharaicon();
	~CCharaicon();

	static CCharaicon *Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, int nIdx, int nNumPlayer, float fTexU = 0.0f, float fTexV = 0.0f, float fTexWidth = 1.0f, float fTexHeight = 1.0f, float fRot = 0.0f);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//--------------------
	//	Set & Get
	//--------------------
	void SetParentChara(CCharacter *pParentChara) { m_pParentChara = pParentChara; }
	void SetDestChara(CCharacter *pDestChara) { m_pDestChara = pDestChara; }
	void SetDisp(const bool bDisp) { m_bDisp = bDisp; }
	void SetIdx(const int nIdx) { m_nIdx = nIdx; }
	void SetNumPlayer(const int nNumPlayer) { m_nNumPlayer = nNumPlayer; }

	CCharacter *GetParentChara(void) { return m_pParentChara; }
	CCharacter *GetDestChara(void) { return m_pDestChara; }
	bool GetDisp(void) { return m_bDisp; }
	int GetIdx(void) { return m_nIdx; }
	int GetNumPlayer(void) { return m_nNumPlayer; }

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	//--------------------
	//	関数
	//--------------------
	void CheckDraw(void);
	void CalcCrossPoint_ScreenLine(void);
	D3DXVECTOR3 ShiftScreenPos(D3DXVECTOR3 pos);

	//--------------------
	//	メンバ変数
	//--------------------
	CCharacter *m_pParentChara;		// 親となるプレイヤー
	CCharacter *m_pDestChara;		// 位置を指す目的のプレイヤー
	bool m_bDisp;					// 描画するかしないか
	int m_nIdx;						// どの番号の画面に紐づいているか
	int m_nNumPlayer;				// ゲームに参加しているプレイヤーの人数
};

#endif