//======================================================================
//
//			チュートリアルプレイヤー処理 [tutorialplayer.h]
//			Author : Kobayashi_Sho-hei / 小林 将兵
//
//======================================================================
#ifndef _TUTORIALPLAYER_H_
#define _TUTORIALPLAYER_H_

//=================================================
//		include / 他のデータ・コードの呼び出し・利用
//=================================================
#include "main.h"
#include "sceneX.h"
#include "game.h"
#include "scene.h"
#include "character.h"

//==============================================
//					マクロ定義
//==============================================
#define MODEL_UV	("DATA/TEXTURE/MODEL/player.png")
#define FIRST_LINETIME	(60 * 20)	//ラインの時間の初期数
#define MIN_TERRITORY	(3)			//テリトリーの最低限数
#define TERRITORY_TOP	(0)			//テリトリーの先頭情報
#define NOT_GETTIME		(60)		//テリトリーの取得できない時間
#define LINE_TIME		(90)		//ラインを消さない時間

//プレイヤーステータス
#define TUTORIALPLAYER_SPEED	(3.2f)
#define PLAYER_GRAVITY	(0.5f)
#define PLAYER_MAX_PATTERN		(10)
#define MAX_PLAY (2)
#define MAX_TERRITORY	(10)

//目的地のリスト
#define MAX_LIST		(3)

//==============================================
//					前方宣言
//==============================================
class CSound;
class CModel;
class CScene3D;
class CObject;
class CEnemy;
class CSceneX;
class CCollider;
class CMeshCollider;
class CBoxCollider;
class CSphereCollider;
class CCylinderCollider;
class CPlayerAttackSphereCollider;
class CModel;
class CTerritory;
class CSceneOrbit;
class CLine;
class CLoadEffect;
class CMotion;

//==============================================
//			キャラクターの派生 → プレイヤー
//==============================================
class CTutorialPlayer : public CCharacter
{
public:
	typedef enum
	{//プレイヤーの状態
		PLAYERSTATE_NONE = 0,
		PLAYERSTATE_WALK,		//移動状態
		PLAYERSTATE_ACTION,		//
		PLAYERSTATE_BLOWAWAY,
		PLAYERSTATE_DAMAGE,
		PLAYERSTATE_MAX
	}PLAYERSTATE;

	typedef struct
	{
		D3DXVECTOR3 * List;	//目的地のリスト
		int nMaxList;		//リストの最大数
	}AIMPOS_LIST;

	CTutorialPlayer(int nPriority = 3, OBJTYPE objType = OBJTYPE_PLAYER);
	~CTutorialPlayer();

	HRESULT Init(D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40],int nNumPlayer, int nType);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CTutorialPlayer *Create(D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40],int nNumPlayer,TYPE type,CHARCTERTYPE playertype);
	void  Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);

	//プレイヤーの情報取得関数
	PLAYERSTATE GetPlayerState(void) { return m_PlayerState; }
	bool GetBool(void) { return m_bSprintMotion; }
	void SetMovePlayer(bool bMovePlayer, int nNumList) { m_bMovePlayer = bMovePlayer; m_nNumList = nNumList; }
	bool GetMovePlayer(void) { return m_bMovePlayer; }
	void SetMoveEnd(bool bMoveEnd) { m_bMoveEnd = bMoveEnd; }
	bool GetMoveEnd(void) { return m_bMoveEnd; }

protected:							//親と子クラスが使用できる

	void BlowAway(D3DXVECTOR3 AnotherPos);

	int m_nDamageCount;		// ダメージを受けてからの時間を計測する
	float m_fBlowAngle;		// 吹っ飛ぶ方向
	float m_fBlowLength;	// 吹っ飛ぶ距離
	int m_nDownCount;

	//各情報変数
	CModel** m_pModel;						//モデルの情報
	PLAYERSTATE m_PlayerState;				//プレイヤーの状態
	CMotion *m_pMotion;						//モーションの情報
	CLoadEffect * m_pLoadEffect;			//エフェクトの情報

	//	位置・回転情報
	D3DXVECTOR3 m_posOld;					//位置
	D3DXVECTOR3 m_move;						//移動量
	D3DXVECTOR3 m_rot;						//回転量
	D3DXVECTOR3 m_Angle;					//角度

	//プレイヤー変数
	int m_nNumPlayer;						//プレイヤーの数
	bool m_bWalk;							//移動をしてるか
	bool m_bSprint;							//スプリントを使用してるか
	bool m_bSprintMotion;
	float m_fSpeed;							//プレイヤーの速さ
	float m_fInertia;						//慣性

	int m_nDamageCounter;					//ダメージカウンター
	bool m_bSuperArmor;						//スーパーアーマ状態

private:
	void DebugMove(void);
	void UpdateMoveTutorial(void);
	void SetAimPos(void);
	void SetMotion(PLAYERSTATE PlayerState);
	void CreateOrbitLine(void);
	void UninitOrtbitLine(void);

	bool m_bPlayerMotionState;				//プレイヤーがモーションしてるか
	bool m_bMovePlayer;
	bool m_bMoveEnd;
	int m_nNumPos;							//何番目位置を参照しているのか
	int m_nNumList;							//リストの何番目か
	AIMPOS_LIST m_AimPosList[MAX_LIST];
};

////==============================================
////		スピード型プレイヤー
////==============================================
//class CSpeedPlayer : public CTutorialPlayer
//{
//public:
//	CSpeedPlayer();
//	~CSpeedPlayer();
//
//	HRESULT Init(D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40], int nNumPlayer, int nType);
//	HRESULT Init(void);
//	void Uninit(void);
//	void Update(void);
//	void Draw(void);
//	void PlayerActionPad();				//プレイヤーの移動処理
//	void PlayerActionMouse();				//プレイヤーの移動処理
//	void SprintUpdate(void);	//スピード型のスキル処理
//	static CSpeedPlayer *Create(D3DXVECTOR3 pos,char ModelTxt[40], char MotionTxt[40],int nNumPlayer,int nType, 
//		CHARCTERTYPE charctertype,int nControllerIndx, int nControllerType);
//	void  Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);
//	void CreateOrbitLine(void);
//	void UninitOrtbitLine(void);
//private:
//	int m_nSprintCounter;	//スプリント用カウンター
//	int m_nSprintTimer;		//スプリント用タイマー
//	CLoadEffect * m_pLoadEffectSpeed;			//エフェクトの情報
//
//};
////==============================================
////		パワー型プレイヤー
////==============================================
//class CPowerPlayer : public CTutorialPlayer
//{
//public:
//	CPowerPlayer();
//	~CPowerPlayer();
//
//	HRESULT Init(D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40], int nNumPlayer, int nType);
//	HRESULT Init(void);
//	void Uninit(void);
//	void Update(void);
//	void Draw(void);
//	void ActionUpdate(void);
//	void PlayerActionPad();				//プレイヤーの移動処理
//	void PlayerActionMouse();				//プレイヤーの移動処理
//	void InitNum(void);
//	static CPowerPlayer *Create(D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40], int nNumPlayer, int nType, 
//		CHARCTERTYPE charctertype,int nControllerIndx, int nControllerType);
//	void  Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);
//
//protected:
//	
//
//private:
//	void CreateStartUpCollider(void);
//	void CreateColliderSphere(void);
//
//
//	int m_nColliderCnt;
//	int m_nColliderTimer;
//	float m_fScale;
//	bool m_bAction;
//	bool m_bInit;
//	int m_nInitCnt;
//};
//
////==============================================
////		地雷型型プレイヤー
////==============================================
//class CTechniquePlayer : public CTutorialPlayer
//{
//public:
//	
//	CTechniquePlayer();
//	~CTechniquePlayer();
//
//	HRESULT Init(D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40], int nNumPlayer, int nType);
//	HRESULT Init(void);
//	void Uninit(void);
//	void Update(void);
//	void Draw(void);
//	void ActionUpdate(void);
//	void PlayerActionPad();				//プレイヤーの移動処理
//	void PlayerActionMouse();				//プレイヤーの移動処理
//	static CTechniquePlayer *Create(D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40], int nNumPlayer, int nType,
//		CHARCTERTYPE charctertype, int nControllerIndx, int nControllerType);
//	void  Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);
//
//private:
//	int m_nMinePoint;			//マインの置けるポイント
//	int m_nReCounter;			//回復カウンター
//	int m_nReTimer;				//回復タイマー
//	int m_nInstallationCounter;	//設置カウンター
//	int m_nInstallationTimer;	//設置タイマー
//	bool m_bMineUse;			//地雷を使用したか
//};
#endif