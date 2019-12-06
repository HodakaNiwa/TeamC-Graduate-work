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
class CEffectManager;
class CEmitter;
class CRobot;	// ←追加(よしろう)
class CRobotUI;	// ←追加(よしろう)
class CEventCamera; // ←追加(よしろう)
class CIcon2D;
class CBoxCollider;
class CDivisionWall;

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

	void CreateInitPlayer(void);			//プレイヤーの生成
	void CreateInitCamera(void);			//カメラの生成
	void CreateInitEventCamera_Robot(void);	// イベントカメラの生成←追加(よしろう)
	void CreateRobot(void);					// ロボットの生成←追加(よしろう)

	// エリアボーナスイベント用(外部で呼ぶ)
	void ReleaseAreaBounsEventUI(void);
	void ReleaseDivisionEventUI(void);
	void ReleaseDivisionWall(void);
	void HideEventField(void);
	void UpdateAreaBonusField_Start(void);
	void UpdateAreaBonusField_Finish(void);
	void UpdateDivisionWall_Start(void);
	void UpdateDivisionWall_Finish(void);

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
	CEffectManager *GetEffectManager(void) { return m_pEffectManager; }
	CGameCamera * GetGameCamera(int nIdx) { return m_pGameCamera[nIdx]; }
	CPlayer * GetPlayer(int nNumPlayer) { return m_pPlayer[nNumPlayer]; }
	CFieldManager * GetFieldManger(void) { return m_pFieldManager; }
	int GetNumPlay(void) { return m_nNumPlay; }
	CUI *GetUI(void) { return m_pUI; }
	CRobotUI *GetRobotUI(void) { return m_pRobotUI; }					// ←追加(よしろう)

	//	---<<小林が追加しました>>★	★	★	★	★	★	★	★	★	★
	CCharacter* GetChara(int nCnt) { return m_pCharacter[nCnt]; }
	CEnemy * GetEnemy(int nNum) { return m_pEnemy[nNum]; }
	int GetEnemyNum(void) { return m_nEnemyNum; }
	//	★	★	★	★	★	★	★	★	★	★	★	★	★	★	★	★
	int GetCountry(int nNum) { return m_nCountry[nNum]; }

	static int GetEnemyNumResult(void) { return m_nEnemyNumResult; }
	static int GetCountMakeShape(int nIndx) { return m_nCountMakeScore[nIndx]; }
	static int GetCountGetTerritory(int nIndx) { return m_nCountGetTerritry[nIndx]; }
	static int GetRobbotedTerritory(int nIndx) { return m_nCountRobottedTerritory[nIndx]; }

	//ロボットGet&Set関数
	void SetRobot(CRobot *pRobot) { m_pRoboCharacter = pRobot; }
	CRobot *GetRobot(void) { return m_pRoboCharacter; }
	void SetEveCam(CEventCamera *pEventCamera) { m_pEventCamera = pEventCamera; }
	CEventCamera *GetEveCam(void) { return m_pEventCamera; }
	void SetEveCamFlag(bool EndFlag) { m_bEveCam = EndFlag; }
	bool GetEVeCamFlag(void) { return m_bEveCam; }

	// エリアボーナスイベント用Set&Get関数
	void SetAreaBonusEventFlag(const bool bAreaBonusEventFlag) { m_bAreaBonusEventFlag = bAreaBonusEventFlag; }
	bool GetAreaBonusEventFlag(void) { return m_bAreaBonusEventFlag; }
	int GetAreaBonusIdx(void) { return m_nAreaBonusIdx; }

	// 分断イベント用Set&Get関数
	void SetDivisionEventFlag(const bool bDivisionEventFlag) { m_bDivisionEventFlag = bDivisionEventFlag; }
	bool GetDivisionEventFlag(void) { return m_bDivisionEventFlag; }

private:
	//--------------------
	//	関数
	//--------------------
	void CreateUI(void);
	void SettingCharaPosition(void);
	void CreateMapCollider(void);
	void ReleaseUI(void);
	void UpdateUI(void);
	void DrawUI_Icon(const int nIdx);
	void GetCharInfo(void);

	// エリアボーナスイベント用関数
	void StartAreaBonusEvent(void);
	void CreateEventCamera_AreaBonusStart(void);
	void CreateAreaBonusBg_Start(void);
	void CreateAreaBonusLogo_Start(void);
	void SetEventFieldPosition(void);
	void CreateEventCamera_AreaBonusFinish(void);
	void CreateAreaBonusBg_Finish(void);
	void CreateAreaBonusLogo_Finish(void);
	void DecideAreaBonusIdxFromRandom(void);
	void FinishAreaBonusEvent(void);
	void UpdateAreaBonusEvent(void);
	void UpdateAreaBonusUI(void);
	void DrawAreaBounsEventUI(void);

	// 分断イベント用関数
	void StartDivisionEvent(void);
	void CreateEventCamera_DivisionStart(void);
	void CreateDivisionBg_Start(void);
	void CreateDivisionLogo_Start(void);
	void CreateDivisionCollider(void);
	void CreateDivisionWall(void);
	void CreateEventCamera_DivisionFinish(void);
	void CreateDivisionBg_Finish(void);
	void CreateDivisionLogo_Finish(void);
	void ReleaseDivisionCollider(void);
	void FinishDivisionEvent(void);
	void UpdateDivisionEvent(void);
	void UpdateDivisionUI(void);
	void DrawDivisionEventUI(void);

	//ロボット関数
	void ReleaseRobotUI(void);
	void UpdateRobotUI(void);
	void DrawRobotUI(void);
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
	CEffectManager *m_pEffectManager;			//	エフェクト管轄クラスへのポインタ
	CLoadTextObject * m_pLoadObject;			// オブジェクトの読み込み
	CLoadTextTerritory * m_pLoadTerritory;		// テリトリーの読み込み
	CGameCamera * m_pGameCamera[MAX_GAMECAMERA];				// ゲームカメラ
	CPlayer *m_pPlayer[MAX_NUMPLAYER];
	static CTime *m_pTime[2];
	CEnemy *m_pEnemy[ENEMY_MAX];
	CCamera * m_pDebugCamera;						//
	int m_nNumPlay;									//プレイヤーの総数
	int m_nEnemyNum;								//エネミーの総数
	static int m_nEnemyNumResult;					//リザルト用エネミーの総数

	int m_nPlayerType[4];							//プレイヤーのタイプ
	int m_nCountry[4];								//国
	CFieldManager * m_pFieldManager;				//地面マネージャー
	CUI *m_pUI;										// UIクラスへのポインタ
	CCharacter* m_pCharacter[MAX_CHARACTER];		//キャラクターの総数
	CRotationCamera *m_pRotCamera;					//回転カメラ
	int m_nControllerNum[4];						//コントローラーの番号
	int m_nControllerType[4];						//コントローラーのタイプ

	//ロボット変数
	int m_nCntRobot;
	CRobotUI *m_pRobotUI;
	CRobot* m_pRoboCharacter;
	CEventCamera *m_pEventCamera;
	bool m_bEveCam;			// イベントカメラの死亡フラグ
	CEmitter *m_pEmitter;	// 最後に生成したエミッタクラスへのポインタ

	//	エネミーのタイプ選別(プレイヤーの選ばれたタイプとなるべくかぶらないようにするため)
	int m_nSpeedNo, m_nPowerNo, m_nSweeperNo;
	bool m_bTypeMax;
	D3DXVECTOR3 InitPos[MAX_CHARACTER];	//	各キャラクターの初期位置

	//リザルト用変数
	static int m_nCountMakeScore[MAX_CHARACTER];			//図形を作った数
	static int m_nCountGetTerritry[MAX_CHARACTER];			//テリトリーの取得数
	static int m_nCountRobottedTerritory[MAX_CHARACTER];	//テリトリーの奪われた数


	// エリアボーナスイベント用変数
	bool m_bAreaBonusEventFlag;								// エリアボーナスイベントが始まったかどうか
	int m_nAreaBonusIdx;									// ボーナスポイントを付与するエリアの番号
	int m_nAreaBonusEventCounter;							// エリアボーナスイベントを管理するカウンター
	int m_nAreaBonusUICounter;								// エリアボーナスUIを管理するカウンター
	CIcon2D *m_pAreaBonusBgUp;								// エリアボーナス用背景(上側)
	CIcon2D *m_pAreaBonusBgDown;							// エリアボーナス用背景(下側)
	CIcon2D *m_pAreaBonusLogo[2];							// エリアボーナス用ロゴ

	// 分断イベント用変数
	bool m_bDivisionEventFlag;								// 分断イベントが始まったかどうか
	int m_nDivisionDirection;								// 分断の方向を示す番号
	int m_nDivisionEventCounter;							// 分断イベントを管理するカウンター
	int m_nDivisionUICounter;								// 分断UIを管理するカウンター
	CIcon2D *m_pDivisionBgUp;								// 分断用背景(上側)
	CIcon2D *m_pDivisionBgDown;								// 分断用背景(下側)
	CIcon2D *m_pDivisionLogo[2];							// 分断用ロゴ
	CBoxCollider *m_pDivisionCollider;						// 分断用の当たり判定
	CDivisionWall *m_pDivisionWall;							// 分断イベント用の壁
};
#endif