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
#define CUNTRY					(8)		// 国の数
#define RESULT_TEX				(1)		// リザルトのテクスチャ

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CResultCamera;
class CFieldManager;
class CLoadTextObject;
class CLoadTextTerritory;
class CCharResult;
class CMoveUI;
class CScene2D;

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

	typedef enum
	{
		STATE_WHITE = 0,
		STATE_ZOOM,
		STATE_RANKING,
	}STATE;

	CResult();				// コンストラクタ
	~CResult();				// デストラクタ
	void Init(void);		// 初期化処理
	void Uninit(void);		// 終了処理
	void Update(void);		// 更新処理
	void Draw(void);		// 描画処理

	void LoadTex(void);
	void UnloadTex(void);
	static void SetType(TYPE type) { m_type = type; }

private:
	void UpdateWhite(void);
	void UpdateZoom(void);
	void UpdateRanking(void);
	static TYPE m_type;					//結果状態

	STATE m_state;
	LPDIRECT3DTEXTURE9 m_pCuntryTex[CUNTRY];
	LPDIRECT3DTEXTURE9 m_pTexture[RESULT_TEX];
	CResultCamera * m_pCamera;				//カメラ
	CFieldManager * m_pFieldManager;		//フィールドマネージャー
	CLoadTextObject * m_pLoadTextObj;		//オブジェクト読み込み
	CLoadTextTerritory * m_pLoadTerritory;	// テリトリーの読み込み
	CCharResult * m_pWinChar;				//勝利キャラ
	CScene2D * m_pWhiteBack;				//白背景
	CMoveUI * m_pNatinalFlag;				//国旗
	CMoveUI * m_pWinerName;					//勝者名

	int m_nCounter;
	int m_nTime;
	bool m_bWhiteIn;						//フェードイン出来るかどうか
};
#endif