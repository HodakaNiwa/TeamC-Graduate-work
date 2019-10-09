//=============================================================================
//
// ボタンの処理 [Bottun.h]
// Author : 佐藤 安純　[ SATO ASUMI ]
//
//=============================================================================
#ifndef _BOTTUN_H_
#define _BOTTUN_H_

#include "main.h"
#include "scene.h"
#include "scene2D.h"

//=============================================================================
// 2Dポリゴンクラス
//=============================================================================
class CBottun: public CScene2D
{
public:
	typedef enum
	{
		STATE_NORMAL = 0,
		STATE_ENTER,
		STATE_MAX,
	}STATE;

	CBottun(int nPriority, CScene::OBJTYPE type);
	~CBottun();

	//メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	STATE m_State;		//状態設定
	float m_fCol_a;		//色のα値
	float m_fChangeCol;	//色の変化量
};

#endif