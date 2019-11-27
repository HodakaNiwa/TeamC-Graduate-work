//=============================================================================
//
// ロゴの処理 [Logo.h]
// Author : 佐藤 安純　[ SATO ASUMI ]
//
//=============================================================================
#ifndef _LOGO_H_
#define _LOGO_H_

#include "main.h"
#include "scene.h"
#include "scene2D.h"

//=============================================================================
// 2Dポリゴンクラス
//=============================================================================
class CLogo: public CScene2D
{
public:
	CLogo(int nPriority, CScene::OBJTYPE type);
	~CLogo();

	//メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
private:
};

#endif