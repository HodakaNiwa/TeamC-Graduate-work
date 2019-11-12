//=============================================================================
//
// ライン処理 [line.h]
// Author : 佐藤安純 Sato_Asumi
//
//=============================================================================
#ifndef _LINE_H_
#define _LINE_H_

#include "main.h"
#include "scene3D.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define MAX_POS	(4)

//=============================================================================
// ビルボードクラス
//=============================================================================
class CLine : public CScene3D
{
public:
	CLine();
	~CLine();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CLine * Create(D3DXVECTOR3 StartPos, D3DXVECTOR3 EndPos, int nNumPlayer, int nId);
	void SetNumPlayer(int nNumPlayer) { m_nNumPlayer = nNumPlayer; }
	void SetTerritoryPos(D3DXVECTOR3 StartPos, D3DXVECTOR3 EndPos) { m_StartPos = StartPos; m_EndPos = EndPos; };
	void SetID(int nId) { m_nID = nId; }
	void SetCompleteShape(bool bCompShape) { m_bCompleteShape = bCompShape; }
	bool ColChange(void);

private:
	void RequestVecA(void);
	bool CollsionLine(D3DXVECTOR3 PlayerPos);

	//変数
	int m_nNumPlayer;
	int m_nID;
	D3DXVECTOR3 m_StartPos;
	D3DXVECTOR3 m_EndPos;
	D3DXVECTOR3 m_VecA[MAX_POS];
	D3DXVECTOR3 m_WorldPos[MAX_POS];
	D3DXCOLOR m_col;
	bool m_bCompleteShape;				//図形が完成したかどうか
};
#endif