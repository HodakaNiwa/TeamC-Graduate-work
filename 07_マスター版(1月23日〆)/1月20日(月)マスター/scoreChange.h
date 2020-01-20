//*****************************************************************************
//
//     スコア変動の処理[scoreChange.h]
//     Author:Hodaka Niwa
//
//*****************************************************************************
#ifndef _SCORECHANGE_H_
#define _SCORECHANGE_H_

//*****************************************************************************
//    インクルードファイル
//*****************************************************************************
#include "scene2D.h"

//*****************************************************************************
//    マクロ定義
//*****************************************************************************


//*****************************************************************************
//    前方宣言
//*****************************************************************************
class CNumber2D;

//*****************************************************************************
//    スコア変動クラスの定義
//*****************************************************************************
class CScoreChange : public CScene2D
{
public:     // 誰でもアクセス可能
	CScoreChange(int nPriority = 3, OBJTYPE objType = OBJTYPE_2DPOLYGON);
	~CScoreChange();

	static CScoreChange *Create(D3DXVECTOR3 pos, D3DXCOLOR col, D3DXVECTOR3 size, int nScoreChange);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	D3DXVECTOR3 SizeAdjust(D3DXVECTOR3 size);

	//--------------------
	//	Set & Get
	//--------------------
	void SetNumber2D(CNumber2D *pNumber2D, const int nIdx) { m_pNumber2D[nIdx] = pNumber2D; }
	void SetNumberDigit(const int nNumberDigit) { m_nNumberDigit = nNumberDigit; }
	void SetScoreChange(const int nScoreChange) { m_nScoreChange = nScoreChange; }
	void SetChangeTimer(const int nChangeTimer) { m_nChangeTimer = nChangeTimer; }

	CNumber2D *GetNumber2D(const int nIdx) { return m_pNumber2D[nIdx]; }
	int GetNumberDigit(void) { return m_nNumberDigit; }
	int GetScoreChange(void) { return m_nScoreChange; }
	int GetChangeTimer(void) { return m_nChangeTimer; }

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	//--------------------
	//	関数
	//--------------------
	void CalcScoreChangeDigit(void);
	void CreateNumber(void);
	void ReleaseNumber(void);
	void PosDown(void);
	void PosDown_Number(void);
	void AlphaAdd(void);
	void AlphaAdd_Number(const D3DXCOLOR col);
	void DrawNumber(void);


	//--------------------
	//	メンバ変数
	//--------------------
	CNumber2D **m_pNumber2D;	// 数字ポリゴンクラスへのポインタ
	int m_nNumberDigit;			// 数字の桁数
	int m_nScoreChange;			// 変動したスコア量
	int m_nChangeTimer;			// α値の変更を管理するカウンター
};

#endif