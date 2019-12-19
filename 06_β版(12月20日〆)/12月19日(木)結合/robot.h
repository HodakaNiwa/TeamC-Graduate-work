//====================================================================================
//
// 第9戦力処理	[robot.h]
// Author;荒谷由朗
//
//====================================================================================
#ifndef _ROBOT_H_
#define _ROBOT_H_

#include "main.h"
#include "sceneX.h"
#include "game.h"
#include "scene.h"
#include "character.h"

//-------------------------------------------------------------
// マクロ定義
//-------------------------------------------------------------
#define ROBOT_CHARNUM					(8)
#define ROBOT_VIGILANCESPEED			(10.0f)		// 移動速度
#define ROBOT_ATTACK_RADIUS				(70.0f)		// 攻撃判定の範囲
#define ROBOT_ATTACK_INTERVAL			(320)		// 攻撃(近接)の間隔50
#define ROBOT_SHOOT_MODEINTERVAL		(8)			// 攻撃(遠距離)の間隔5
#define ROBOT_SHOOT_ROTATION			(0.07f)		// 攻撃(遠距離)の回転量
#define ROBOT_SHOOT_BULLETINTERVAL		(14)		// ミサイルを撃つ間隔8
#define ROBOT_SHOOT_MAXBULLET			(12)		// ミサイルを撃つ回数
#define ROBOT_GRAVITY					(-9.8f)		// 重力(-9.8m/s^2)
#define ROBOT_ENDTIME					(2700)		// 出現時間
#define ROBOT_SWITCHTIME				(200)		// 追跡時間

#define ROBOT_UNINITPOS					(1000)		// 消える時のY軸座標
#define ROBOT_ATTACKEFFECT				(50)		// 攻撃エフェクトのタイミング
#define ROBOT_ENDEMITTER				(60)		// 着地エフェクトの消える時間

// コリジョン関係
#define ROBOT_COLLISION_RADIUS			(50.0f)		// 当たり判定の幅
#define ROBOT_COLLISION_HEIGHT			(180.0f)	// 当たり判定の高さ
#define ROBOT_COLLISION_ATTACKRADIUS	(180.0f)	// 攻撃時の当たり判定の幅
#define ROBOT_COLLISION_ATTACKLIFE		(25)		// 攻撃時の当たり判定の寿命

//-------------------------------------------------------------
// 前方宣言
//-------------------------------------------------------------
class CRobot;
class CLoadEffect;
class CModel;
class CMotion;
class CGame;
class CCollider;
class CMeshCollider;
class CBoxCollider;
class CSphereCollider;
class CCylinderCollider;
class CPlayerAttackSphereCollider;
class CRobotUI;
class CEmitter;

//-------------------------------------------------------------
// クラス宣言
//-------------------------------------------------------------
class CRobot : CCharacter
{
public:
	typedef enum
	{// 状態
		ROBOTSTATE_NONE = 0,	// ニュートラル
		ROBOTSTATE_WALK,		// 移動
		ROBOTSTATE_APPEARANCE,	// 出現
		ROBOTSTATE_JUMP,		// ジャンプ
		ROBOTSTATE_ATTACK,		// 近接攻撃
		ROBOTSTATE_SHOOT,		// 射撃攻撃
		ROBOTSTATE_DAWN,		// 落下中
	}ROBOTSTATE;

	CRobot(int nPriority = 3, OBJTYPE objType = OBJTYPE_ROBOT);	// コンストラクタ
	~CRobot();													// デストラクタ

	static CRobot	*Create			(D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40], int nNumPlayer, TYPE type, CHARCTERTYPE charctertype);
	HRESULT			Init			(D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40], int nNumPlayer);
	HRESULT			Init			(void);
	void			Uninit			(void);
	void			Update			(void);
	void			Draw			(void);
	void			Set				(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);

	void			Search			(void);		// 捜索処理
	void			Wake			(void);		// 移動処理
	void			Attack			(void);		// 攻撃処理
	void			Shoot			(void);		// 射撃処理
	void			Gravity			(void);		// 落下処理
	void			Jump			(void);		// ジャンプ処理
	void			GetUp			(void);		// 起き上がり処理

	void			CreateRobotUI	(void);		// 出現テロップの生成処理
	void			ReleaseRobotUI	(void);

	void			SetRobotUI		(CRobotUI *pRobotUI)	{ m_pRobotUI = pRobotUI; }
	CRobotUI		*GetRobotUI		(void)					{ return m_pRobotUI; }
	bool			GetEndGravity	(void)					{ return m_bEndGravity; }

	// ベクトル計算
	double VectorAngle	(D3DXVECTOR3 A, D3DXVECTOR3 B);										// 2つのベクトルABのなす角度θを求める
	double GetVecLength	(D3DXVECTOR3 v) { return pow((v.x * v.x) + (v.z * v.z), 0.5); }		// ベクトルの長さの算出

protected:		// 親と子クラスが使用できる
	//	---<<コリジョン関連関数>>---
	void CreateCylinderCollider(void);

	// 各情報変数
	CModel		**m_pModel;			// モデルの情報
	CMotion		*m_pMotion;			// モーションの情報

	// プレイヤー変数
	int m_nNumPlayer;				// プレイヤーの数

private:
	void CreateColliderSphere	(void);
	void UpdateRobotUI			(void);

	D3DXMATRIX	m_mtxWorld;				// ワールドマトリックス
	ROBOTSTATE	m_RobotState;			// ロボットの状態
	D3DXVECTOR3 m_pos;					// 位置
	D3DXVECTOR3 m_rot;					// 向き
	D3DXVECTOR3 m_move;					// 移動量
	D3DXVECTOR3 m_col;					// 色

	int			m_nNumTaget;			// 標的の番号
	int			m_nNumTagetOld;			// 前回標的の番号
	int			m_nCntEnd;				// 退場カウンター
	int			m_nCntWalk;				// ターゲット切り替え時間
	int			m_nNumAttack;			// ロボットが攻撃(近接)した回数
	int			m_nNumShoot;			// ロボットが攻撃(遠距離)した回数
	int			m_nCntShoot;			// ミサイルを撃つまでのカウンタ

	float		m_fDropTime;			// 落下時間
	float		m_fJumpTime;			// 上昇時間

	bool		m_bDeth;				// 死亡フラグ
	bool		m_bEndGravity;			// 落下終了フラグ
	bool		m_bAttack;				// 攻撃(近接)しているかどうか
	bool		m_bShoot;				// 攻撃(遠距離)しているかどうか

	// ポインタ
	CRobotUI	*m_pRobotUI;			// 出現テロップのポインタ

	// エフェクト関係
	CEmitter	*m_pEmitter;			// エミッタのポインタ
	int			m_nCntEmitter;			// エミッタのカウンタ
	int			m_nCntAttackEffect;		// 攻撃時のエフェクトカウンタ

	//!	---<<ラインの判定に必要>>---
	void		CreateOrbitLine	(void);	// 軌跡の線の生成
	void		UninitOrtbitLine(void);	// ラインの破棄
};
#endif