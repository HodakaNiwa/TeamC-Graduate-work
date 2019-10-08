//=============================================================================
//
// ポーズ処理 [pause.h]
// Author : 佐藤 安純　[ SATO ASUMI ]
//
//=============================================================================
#ifndef _PAUSE_H_
#define _PAUSE_H_

#include "main.h"
#include "scene.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define MAX_POLYGON (5)

//=============================================================================
// 前方宣言
//=============================================================================
class CScene2D;

//=============================================================================
// ポーズクラス
//=============================================================================
class CPause
{
public:
	typedef enum
	{
		STATE_CONTINUE = 0,
		STATE_RETRY,
		STATE_QUIT,
		STATE_MAX,
	}STATE;

	typedef enum
	{
		STATEBOTTUN_NORMAL = 0,
		STATEBOTTUN_SELECT,
	}STATE_BOTTUN;

	CPause();
	~CPause();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CPause * Create(void);

private:
	void BottunUpdate(void);
	CScene2D * m_pScene2D[MAX_POLYGON];	//2Dポリゴンのダブルポインタ
	STATE m_state;						//状態
	STATE_BOTTUN m_StateBottun;			//ボタンの状態設定
	bool m_bDeth;						//死亡状態
};
#endif
