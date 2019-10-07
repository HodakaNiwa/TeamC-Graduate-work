//=============================================================================
//
// チュートリアル処理 [tutorial.h]
// Author : 佐藤 安純
//
//=============================================================================
#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_SCORE_UI_TUTORIAL	(6)

//*****************************************************************************
// 前方宣言
//*****************************************************************************

//=================================
// チュートリアルクラス
//=================================
class CTutorial
{
public:
	
	CTutorial();							// コンストラクタ
	~CTutorial();							// デストラクタ
	HRESULT Init();							// 初期化処理
	void	Uninit(void);					// 終了処理
	void	Update(void);					// 更新処理
	void	Draw(void);						// 描画処理

private:
	void LoadUI(void);

	LPDIRECT3DTEXTURE9 * m_ppTexture;		// テクスチャのポインタへのポインタ
	int					m_nMaxTexture;		// 読み込むテクスチャの最大数
};
#endif