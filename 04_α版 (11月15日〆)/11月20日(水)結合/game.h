//=============================================================================
//
// ゲームの処理 [game.h]
// Author : 佐藤安純
//
//=============================================================================
#ifndef _GAME_H_
#define _GAME_H_

#include "main.h"
#include "modebase.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CPause;
class CGameCamera;
class CRotationCamera;
class CSceneMeshFiled;
class CLoadTextObject;
class CLoadTextTerritory;
class CLoadTextEffect;
class CPlayer;
class CTime;
class CEnemy;
class CCamera;
class CFieldManager;
class CUI;
class CCharacter;
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_NUMPLAYER (4)	//プレイヤーの最大数
#define ENEMY_MAX (8)		//エネミーの最大数
#define MAX_GAMECAMERA (4)	//ゲームカメラの最大数
#define MAX_CHARACTER (8)	//キャラクターの最大数
//=====================
// 基本クラス
//=====================
class CGame : public CModeBase
{
public:
	typedef enum
	{
		GAMESTATE_NONE = 0,					//何もしていない状態
		GAMESTATE_NORMAL,					//通常状態
		GAMESTATE_CLEAR,					//ゲームクリア
		GAMESTATE_OVER,						//ゲームオーバー
		GAMESTATE_END,						//ゲーム終了
		GAMESTATE_FIRSTCOUNTDOWN,			//ゲーム開始時のカウントダウン
		GAMESTATE_MAX						//最大数
	}GAMESTATE;

	

	CGame();				//コンストラクタ
	~CGame();				//デストラクタ
	void Init(void);		//初期化処理
	void Uninit(void);		//終了処理
	void Update(void);		//更新処理
	void Draw(void);		//描画処理

	void CreateInitPlayer(void);		//プレイヤーの生成
	void CreateInitCamera(void);		//カメラの生成
	//--------------------
	//	Set & Get
	//--------------------
	static void SetGameState(GAMESTATE state) { m_gameState = state; }
	static void SetPause(bool bPause);
	static bool GetPause(void) { return m_bPause; }
	static void SetDebugCamera(bool bDebugCamera) { m_bDebugCamera = bDebugCamera; }
	static bool GetDebugCamera(void) { return m_bDebugCamera; }
	static CLoadTextEffect * GetEffectLoad(void) { return m_pLoadEffect; }
	static GAMESTATE GetGameState(void) { return m_gameState; }
	static int GetScore(int nIdx) { return m_nScore[nIdx]; }
	CGameCamera * GetGameCamera(int nIdx) { return m_pGameCamera[nIdx]; }
	CPlayer * GetPlayer(int nNumPlayer) { return m_pPlayer[nNumPlayer]; }
	CFieldManager * GetFieldManger(void) { return m_pFieldManager; }
	int GetNumPlay(void) { return m_nNumPlay; }
	CUI *GetUI(void) { return m_pUI; }
	//	---<<小林が追加しました>>★	★	★	★	★	★	★	★	★	★
	CCharacter* GetChara(int nCnt) { return m_pCharacter[nCnt]; }
	CEnemy * GetEnemy(int nNum) { return m_pEnemy[nNum]; }
	int GetEnemyNum(void) { return m_nEnemyNum; }
	//	★	★	★	★	★	★	★	★	★	★	★	★	★	★	★	★
	int GetCountry(int nNum) { return m_nCountry[nNum]; }

	static int GetEnemyNumResult(void) { return m_nEnemyNumResult; }
private:
	//--------------------
	//	関数
	//--------------------
	void CreateUI(void);
	void SettingCharaPosition(void);
	void CreateMapCollider(void);
	void ReleaseUI(void);
	void UpdateUI(void);


	//--------------------
	//	メンバ変数
	//--------------------
	static GAMESTATE m_gameState;				//ゲーム状態
	GAMESTATE m_NowGameState;					//現在のゲーム状況
	int m_nCounterGameState;					//状態カウンタ
	static CPause *m_pPause;					//ポーズのポインタ
	static bool m_bDebugCamera;					//デバックカメラのオンオフ
	static bool m_bPause;						//現在ポーズかポーズじゃないか
	static int m_nScore[MAX_CHARACTER];			// スコアを保存
	static CSceneMeshFiled * m_pMeshFiled;		//メッシュフィールド
	static CLoadTextEffect * m_pLoadEffect;		// エフェクト読み込み
	CLoadTextObject * m_pLoadObject;			// オブジェクトの読み込み
	CLoadTextTerritory * m_pLoadTerritory;		// テリトリーの読み込み
	CGameCamera * m_pGameCamera[MAX_GAMECAMERA];				// ゲームカメラ
	CPlayer *m_pPlayer[MAX_NUMPLAYER];
	static CTime *m_pTime[2];
	CEnemy *m_pEnemy[ENEMY_MAX];
	CCamera * m_pDebugCamera;						//
	int m_nNumPlay;									//プレイヤーの総数
	int m_nEnemyNum;								//エネミーの総数
	static int m_nEnemyNumResult;							//リザルト用エネミーの総数

	int m_nPlayerType[4];							//プレイヤーのタイプ
	int m_nCountry[4];								//国
	CFieldManager * m_pFieldManager;				//地面マネージャー
	CUI *m_pUI;										// UIクラスへのポインタ
	CCharacter* m_pCharacter[MAX_CHARACTER];		//キャラクターの総数
	CRotationCamera *m_pRotCamera;					//回転カメラ

	//	エネミーのタイプ選別(プレイヤーの選ばれたタイプとなるべくかぶらないようにするため)
	int m_nSpeedNo, m_nPowerNo, m_nSweeperNo;
	bool m_bTypeMax;
	D3DXVECTOR3 InitPos[MAX_CHARACTER];	//	各キャラクターの初期位置
};
#endif