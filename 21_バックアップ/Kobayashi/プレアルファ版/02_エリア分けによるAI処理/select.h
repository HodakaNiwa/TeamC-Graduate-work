//=============================================================================
//
// セレクト処理 [select.h]
// Author : 佐藤 安純
//
//=============================================================================
#ifndef _SELECT_H_
#define _SELECT_H_

#include "main.h"
#include "modebase.h"

//=================================
// セレクトクラス
//=================================
class CSelect : public CModeBase
{
public:
	CSelect();				// コンストラクタ
	~CSelect();				// デストラクタ
	void Init(void);		// 初期化処理
	void Uninit(void);		// 終了処理
	void Update(void);		// 更新処理
	void Draw(void);		// 描画処理
};
#endif