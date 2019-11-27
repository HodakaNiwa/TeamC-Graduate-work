//=============================================================================
//
// チュートリアル処理 [tutorial.h]
// Author : 佐藤 安純
//
//=============================================================================
#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_

#include "main.h"
#include "modebase.h"
#include <dshow.h>

//=================================
// 前方宣言
//=================================
class CMovie;

//=================================
// チュートリアルクラス
//=================================
class CTutorial : public CModeBase
{
public:
	CTutorial();				// コンストラクタ
	~CTutorial();				// デストラクタ
	void	Init(void);			// 初期化処理
	void	Uninit(void);		// 終了処理
	void	Update(void);		// 更新処理
	void	Draw(void);			// 描画処理

private:
	CMovie * m_pMovie;
};
#endif