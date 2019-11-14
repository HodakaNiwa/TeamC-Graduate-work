//=============================================================================
//
// プレイヤーのアイコンの処理 [Logo.h]
// Author : 佐藤 安純　[ SATO ASUMI ]
//
//=============================================================================
#ifndef _PLAYERICON_H_
#define _PLAYERICON_H_

#include "main.h"
#include "scene.h"
#include "scene2D.h"

//=============================================================================
// 前方宣言
//=============================================================================
class CMoveUI;

//=============================================================================
// 2Dポリゴンクラス
//=============================================================================
class CPlayerIcon: public CScene2D
{
public:
	typedef enum
	{
		STATE_NORMAL = 0,
		STATE_MOVE,
		STATE_MAX,
	}STATE;

	CPlayerIcon();
	CPlayerIcon(int nPriority, CScene::OBJTYPE type);
	~CPlayerIcon();

	//メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CPlayerIcon * Create(D3DXVECTOR3 Pos, int nNumPlayer);
	int GetNumCuntry(void) { return m_nNumCuntry; }
	void SetNumCuntry(int nNumCuntry) { m_nNumCuntry = nNumCuntry; }
	void UpdateMoveLeft(float fMove);
	void UpdateMoveRight(float fMove);

private:
	void SetNumPlayer(int nNumPlayer) { m_nNumPlayer = nNumPlayer;}
	void UpdateInputXpad(void);
	void UpdateInputKeyboard(void);
	void UpdatePos(void);
	void UpdateMove(void);

	static STATE m_state;		//状態
	int m_nNumCuntry;			//国番号
	int m_nOldNumCuntry;		//前回の国番号
	int m_nNumPlayer;			//プレイヤーの番号
	int m_nCountInput;			//インプットの数
	CMoveUI * m_pNationalFlag;	//国旗	
};

#endif