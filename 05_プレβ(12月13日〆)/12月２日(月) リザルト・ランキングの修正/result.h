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
#include "select.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_RANKING				(5)		// 満足度の数
#define MAX_RESULTLOGO			(2)		// resultロゴの最大数
#define CUNTRY					(8)		// 国の数
#define RESULT_TEX				(21)	// リザルトのテクスチャ
#define MAX_PLAYER				(7)		// プレイヤーの最大数
#define CHARIMAGE_TEX			(8)
#define MAX_COLOR				(5)		//色の最大数

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
class CScore;
class CSelect;

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
		int nCuntry;					//国の種類
		int nType;						//タイプ
		int nScore;						//スコア
		int nCharNo;					//キャラ番号
		int nControlerNo;				//コントローラーの番号
		CSelect::INPUTTYPE m_InputType;	//コントローラーのタイプ
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
	void InitWinerChar(int nCuntry, int nType, int nNumPlayer);
	void InitScoreResult(void);
	void SortScore(void);
	void CreateThankyou(int nCnt);
	void UninitCharRanking(void);
	void CreateBarun(void);
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
	CMoveUI * m_pTotalLogo;								//合計ロゴ
	CMoveUI * m_pGetNumLogo;							//獲得数
	CMoveUI * m_pMakeNum;								//生成数
	CMoveUI * m_pRobbedNum;								//奪われた数
	CMoveUI * m_pCharImage[MAX_CHARACTER];				//キャラ画像
	CMoveUI * m_pRankNum[MAX_CHARACTER];				//キャラのランキングナンバー
	CMoveUI * m_pFlag[MAX_CHARACTER];					//国旗
	CMoveUI * m_pTable[MAX_CHARACTER];					//表
	CMoveUI * m_pNameFlam[MAX_CHARACTER];				//名前のフレーム
	CMoveUI * m_pName[MAX_CHARACTER];					//名前のフレーム
	CMoveUI * m_pNameJapanese[MAX_CHARACTER];			//日本語の名前表記
	CMoveUI * m_pNumber[MAX_CHARACTER];					//数字
	CMoveUI * m_pBackCharImage[MAX_CHARACTER];			//キャラ背景
	CMoveUI * m_pThanksBack[MAX_CHARACTER];				//Thankyouの背景
	CMoveUI * m_pThanksBG[MAX_CHARACTER];				//Thankyouの背景
	CMoveUI * m_pThanks[MAX_CHARACTER];					//Thankyou
	CScore * m_pTotalScore[MAX_CHARACTER];				//合計スコア
	CScore * m_pGetScore[MAX_CHARACTER];				//獲得数
	CScore * m_pMakeScore[MAX_CHARACTER];				//作成数
	CScore * m_pRobotedScore[MAX_CHARACTER];			//奪われた数
	CResultCharUI * m_pResultCharUI[MAX_PLAYER];		//リザルトのUI
	CHARACTER_INFO m_CharInfo[MAX_CHARACTER];			//キャラの最大数
	CHARACTER_INFO m_CharResultScore[MAX_CHARACTER];	//キャラリザルトスコア用の変数

	int m_nCounter;
	int m_nTime;
	int m_nFadeOutTime;
	int m_nCountID;							//IDのカウンター
	int m_nMaxPlayer;						//プレイヤーの最大数
	int m_nCountMakeShape[MAX_CHARACTER];			//図形を作った数
	int m_nCountGetTerritory[MAX_CHARACTER];		//テリトリーの取得数
	int m_nCountRobbotedTerritory[MAX_CHARACTER];	//テリトリーの奪われた数
	bool m_bWhiteIn;						//フェードイン出来るかどうか
	bool m_bCharFlagDown[MAX_CHARACTER];	//キャラクター画像を下すフラグ
	bool m_bThanksFlagDown[MAX_CHARACTER];	//画像を下すフラグ
	D3DXCOLOR m_Color[MAX_COLOR];			//色

};
#endif