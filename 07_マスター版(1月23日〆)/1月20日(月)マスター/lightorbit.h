//*****************************************************************************
//
//     光の軌跡の処理[lightorbit.h]
//     Author:Hodaka Niwa
//
//*****************************************************************************
#ifndef _LIGHTORBIT_H_
#define _LIGHTORBIT_H_

//*****************************************************************************
//    インクルードファイル
//*****************************************************************************
#include "scene.h"

//*****************************************************************************
//    光の軌跡クラスの定義
//*****************************************************************************
class CLightOrbit : public CScene
{
public:		// 誰でもアクセス可能
	CLightOrbit(int nPriority = 3, OBJTYPE objType = OBJTYPE_LIGHTORBIT);
	~CLightOrbit();

	static CLightOrbit *Create(D3DXVECTOR3 pos, D3DXVECTOR3 *pDestPos, int nDestPlayerIdx, int nPriority = 3);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size) {}

	//------------------------
	//  Set & Get
	//------------------------
	void SetPos(const D3DXVECTOR3 pos) { m_Pos = pos; }
	void SetMove(const D3DXVECTOR3 move) { m_Move = move; }
	void SetDestPos(D3DXVECTOR3 *pDestPos) { m_pDestPos = pDestPos; }
	void SetCounter(const int nCounter) { m_nCounter = nCounter; }
	void SetDestPlayerIdx(const int nDestPlayerIdx) { m_nDestPlayerIdx = nDestPlayerIdx; }

	D3DXVECTOR3 GetPos(void) { return m_Pos; }
	D3DXVECTOR3 GetMove(void) { return m_Move; }
	D3DXVECTOR3 *GetDestPos(void) { return m_pDestPos; }
	int GetCounter(void) { return m_nCounter; }
	int GetDestPlayerIdx(void) { return m_nDestPlayerIdx; }


protected:	// このクラスと派生クラスだけがアクセス可能
	void ClearVariable(void);

private:	// このクラスだけがアクセス可能
	//------------------------
	//  関数
	//------------------------
	bool CalcMove(void);
	void Movement(void);
	void CreateEffect3D(void);

	//------------------------
	//  メンバ変数
	//------------------------
	D3DXVECTOR3 m_Pos;			// 自身の位置
	D3DXVECTOR3 m_Move;			// 移動量
	D3DXVECTOR3 *m_pDestPos;	// 目的の座標
	int m_nCounter;				// エフェクトの出現を管理するカウンター
	int m_nDestPlayerIdx;		// 目的のプレイヤーの番号
	float m_fAccel;				// 加速量
};

#endif