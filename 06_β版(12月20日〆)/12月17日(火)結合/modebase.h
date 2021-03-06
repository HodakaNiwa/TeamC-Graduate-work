//=============================================================================
//
// 画面ベース処理 [modebase.h]
// Author : 佐藤安純
//
//=============================================================================
#ifndef _MODEBASE_H_
#define _MODEBASE_H_

#include "main.h"
#include "sound.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// 前方宣言
//*****************************************************************************

//=====================
// 基本クラス
//=====================
class CModeBase
{
public:
	CModeBase();							// コンストラクタ
	~CModeBase();							// デストラクタ
	virtual void Init(void);				// 初期化処理
	virtual void Uninit(void);				// 終了処理
	virtual void Update(void) = 0;			// 更新処理
	virtual void Draw(void) = 0;			// 描画処理
	LPDIRECT3DTEXTURE9 *GetTexture(void) { return m_ppTexture; }

protected:
	void LoadUI(char * pFileName);
	int	m_nCounter;							// カウンター

private:
	LPDIRECT3DTEXTURE9 *m_ppTexture;		// テクスチャのポインタへのポインタ
	int					m_nMaxTexture;		// 読み込むテクスチャの最大数
};
#endif