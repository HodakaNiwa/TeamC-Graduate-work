//=============================================================================
//
// リザルト処理 [result.h]
// Author : 佐藤 安純
//
//=============================================================================
#ifndef _RESULT_H_
#define _RESULT_H_

#include "main.h"
#include "modebase.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_RANKING				(5)		// 満足度の数
#define MAX_RESULTLOGO			(2)		// resultロゴの最大数

//*****************************************************************************
// 前方宣言
//*****************************************************************************

//=====================
// 基本クラス
//=====================
class CResult : public CModeBase
{
public:
	typedef enum
	{
		TYPE_OVER = 0,
		TYPE_CLEAR,
		TYPE_MAX,
	}TYPE;

	CResult();				// コンストラクタ
	~CResult();				// デストラクタ
	void Init(void);		// 初期化処理
	void Uninit(void);		// 終了処理
	void Update(void);		// 更新処理
	void Draw(void);		// 描画処理

	static void SetType(TYPE type) { m_type = type; }

private:
	static TYPE m_type;					//結果状態
};
#endif