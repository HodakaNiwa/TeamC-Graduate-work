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
#include "game.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_RANKING				(5)		// 満足度の数
#define MAX_RESULTLOGO			(2)		// resultロゴの最大数
#define CUNTRY					(8)		// 国の数
#define RESULT_TEX				(14)		// リザルトのテクスチャ
#define MAX_PLAYER				(7)		// プレイヤーの最大数
#define CHARIMAGE_TEX			(1)

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
class CResultCharUI;

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
		STATE_SCORERESULT,
	}STATE;

	typedef struct
	{
		int nCuntry;		//国の種類
		int nType;			//タイプ
		int nScore;			//スコア
		int nCharNo;		//キャラ番号
	}CHARACTER_INFO;

	CResult();				// コンストラクタ
	~CResult();				// デストラクタ
	void Init(void);		// 初期化処理
	void Uninit(void);		// 終了処理
	void Update(void);		// 更新処理
	void Draw(void);		// 描画処理

	void LoadTex(void);
	void UnloadTex(void);
	static void SetType(TYPE type) { m_type = type; }
	LPDIRECT3DTEXTURE9 GetCuntryTex(int nCuntry) { return m_pCuntryTex[nCuntry]; }
	LPDIRECT3DTEXTURE9 GetTexture(int nTex) { return m_pTexture[nTex]; }
	int GetMaxPlayer(void) { return m_nMaxPlayer; }

private:
	void UpdateWhite(void);
	void UpdateZoom(void);
	void UpdateRanking(void);
	void UpdateScoreResult(void);
	void InitCharUI(void);
	void InitCharImage(void);
	void InitWinerChar(int nCuntry, int nType);
	void InitScoreResult(void);
	void SortScore(void);
	static TYPE m_type;					//結果状態

	STATE m_state;
	LPDIRECT3DTEXTURE9 m_pCuntryTex[CUNTRY];
	LPDIRECT3DTEXTURE9 m_pTexture[RESULT_TEX];
	LPDIRECT3DTEXTURE9 m_pCharaTex[MAX_CHARACTER];
	CResultCamera * m_pCamera;							//カメラ
	CFieldManager * m_pFieldManager;					//フィールドマネージャー
	CLoadTextObject * m_pLoadTextObj;					//オブジェクト読み込み
	CLoadTextTerritory * m_pLoadTerritory;				// テリトリーの読み込み
	CCharResult * m_pWinChar;							//勝利キャラ
	CScene2D * m_pWhiteBack;							//白背景
	CMoveUI * m_pNatinalFlag;							//国旗
	CMoveUI * m_pWinerName;								//勝者名
	CMoveUI * m_pBackLine;								//黒のライン
	CMoveUI * m_pRankNo1;								//ランクナンバー１
	CMoveUI * m_pCharImage[MAX_CHARACTER];				//キャラ画像
	CMoveUI * m_pRankNum[MAX_CHARACTER];				//キャラのランキングナンバー
	CMoveUI * m_pFlag[MAX_CHARACTER];					//国旗
	CMoveUI * m_pTable[MAX_CHARACTER];					//表
	CMoveUI * m_pNameFlam[MAX_CHARACTER];				//名前のフレーム
	CMoveUI * m_pName[MAX_CHARACTER];					//名前のフレーム
	CMoveUI * m_pNameJapanese[MAX_CHARACTER];			//日本語の名前表記
	CMoveUI * m_pNumber[MAX_CHARACTER];					//数字
	CResultCharUI * m_pResultCharUI[MAX_PLAYER];		//リザルトのUI
	CHARACTER_INFO m_CharInfo[MAX_CHARACTER];			//キャラの最大数
	CHARACTER_INFO m_CharResultScore[MAX_CHARACTER];	//キャラリザルトスコア用の変数

	int m_nCounter;
	int m_nTime;
	int m_nCountID;							//IDのカウンター
	int m_nMaxPlayer;						//プレイヤーの最大数
	bool m_bWhiteIn;						//フェードイン出来るかどうか
	bool m_bCharFlagDown[MAX_PLAYER];		//キャラクター画像を下すフラグ
};
#endif