//◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆
//					プレイヤー処理 [player.h]
//			Author : Kobayashi_Sho-hei / 小林 将兵 : 山下敦史 : 丹羽保貴
//◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆
#ifndef _PLAYER_H_
#define _PLAYER_H_

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
#define PLAYER_SPEED	(2.2f)
#define PLAYER_GRAVITY	(0.5f)
#define PLAYER_MAX_PATTERN		(10)
#define MAX_PLAY (2)
#define MAX_TERRITORY	(20)

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
class CLine;
class CLoadEffect;
class CMotion;

//==============================================
//			キャラクターの派生 → プレイヤー
//==============================================
class CPlayer : public CCharacter
{
public:
	typedef enum
	{//プレイヤーの状態
		PLAYERSTATE_NONE = 0,
		PLAYERSTATE_WALK,			//移動状態
		PLAYERSTATE_ACTION,			//アクション状態
		PLAYERSTATE_BLOWAWAY,		//ぶつかった状態
		PLAYERSTATE_DAMAGE,			//ダメージ状態
		PLAYERSTATE_ROBOTDAMAGE,	// ロボットに殴られた状態
		PLAYERSTATE_MAX
	}PLAYERSTATE;

	CPlayer(int nPriority = 3, OBJTYPE objType = OBJTYPE_PLAYER);
	~CPlayer();

	HRESULT Init(D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40],int nNumPlayer, int nType);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CPlayer *Create(D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40],int nNumPlayer,TYPE type,CHARCTERTYPE playertype);
	void  Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);
	void PlayerMovePad(D3DXVECTOR3 &CameraRot, D3DXVECTOR3 &pos);				//コントローラー(スティック)での移動処理
	void PlayerMoveMouse(D3DXVECTOR3 &CameraRot, D3DXVECTOR3 &pos);				//マウスでの移動処理
	void PlayerMoveKeyboard(D3DXVECTOR3 &CameraRot, D3DXVECTOR3 &pos);			//キーボードでの移動処理

	void PlayerMove(void);														//プレイヤーの移動処理
	void Collision(void);

	//プレイヤーの情報取得関数
	PLAYERSTATE GetPlayerState(void) { return m_PlayerState; }
	bool GetBool(void) { return m_bSprintMotion; }
protected:							//親と子クラスが使用できる

	void BlowAway(D3DXVECTOR3 AnotherPos, int Num);


	int m_nDamageCount;															// ダメージを受けてからの時間を計測する
	float m_fBlowAngle;															// 吹っ飛ぶ方向
	float m_fBlowLength;														// 吹っ飛ぶ距離
	int m_nDownCount;

	//各情報変数
	CModel** m_pModel;															//モデルの情報
	PLAYERSTATE m_PlayerState;													//プレイヤーの状態
	CMotion *m_pMotion;															//モーションの情報
	CLoadEffect * m_pLoadEffect;												//エフェクトの情報

	//	位置・回転情報
	D3DXVECTOR3 m_posOld;														//位置
	D3DXVECTOR3 m_move;															//移動量
	D3DXVECTOR3 m_rot;															//回転量
	D3DXVECTOR3 m_Angle;														//角度

	//プレイヤー変数
	int m_nNumPlayer;															//プレイヤーの数
	bool m_bWalk;																//移動をしてるか
	bool m_bSprint;																//スプリントを使用してるか
	bool m_bSprintMotion;														//スピードタイプのアクション
	float m_fSpeed;																//プレイヤーの速さ
	float m_fInertia;															//慣性

	int m_nControllerIndx;														//使用してるコントローラーの番号
	int m_nControllerType;														//使用してるコントローラーのタイプ
	int m_nDamageCounter;														//ダメージカウンター
	bool m_bSuperArmor;															//スーパーアーマ状態
	bool m_bStateFlag;
	//状態のフラグ
private:
	bool CollisionCollider(CCollider *pCollider, D3DXVECTOR3 &pos, D3DXVECTOR3 &posOld, D3DXVECTOR3 &Move, D3DXVECTOR3 &ColRange);
	bool CollisionBoxCollider(CBoxCollider *pBoxCollider, D3DXVECTOR3 &pos, D3DXVECTOR3 &posOld, D3DXVECTOR3 &Move, D3DXVECTOR3 &ColRange);
	bool CollisionCylinderyCollider(CCylinderCollider *pCylinderCollider, D3DXVECTOR3 &pos, D3DXVECTOR3 &posOld, D3DXVECTOR3 &Move, D3DXVECTOR3 &ColRange);
	bool CollisionPlayerAttackSphereCollider(CPlayerAttackSphereCollider *pShere, D3DXVECTOR3 &pos, D3DXVECTOR3 &ColRange);
	bool CollisionRobotAttackSphereCollider(CPlayerAttackSphereCollider *pShere, D3DXVECTOR3 &pos, D3DXVECTOR3 &ColRange);

	void CreateOrbitLine(void);
	void UninitOrtbitLine(void);
	bool m_bMouse;
	float m_fMouse;
	int m_nCountTime;															//カウンター
	bool m_bWalkFrag;															//歩いてるかどうかのフラグ
};

//==============================================
//		スピード型プレイヤー
//==============================================
class CSpeedPlayer : public CPlayer
{
public:
	CSpeedPlayer();
	~CSpeedPlayer();

	HRESULT Init(D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40], int nNumPlayer, int nType);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void PlayerActionPad();						//コントローラでのアクション処理
	void PlayerActionMouse();					//マウスでのアクション移動処理
	void PlayerActionKeyboard();					//マウスでのアクション移動処理
	void SprintUpdate(void);					//スピード型のスキル処理
	static CSpeedPlayer *Create(D3DXVECTOR3 pos,char ModelTxt[40], char MotionTxt[40],int nNumPlayer,int nType, 
		CHARCTERTYPE charctertype,int nControllerIndx, int nControllerType);
	void  Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);
	void CreateOrbitLine(void);
	void UninitOrtbitLine(void);
	bool GetSprintState(void) { return m_bSprint; }

private:
	int m_nSprintCounter;						//スプリント用カウンター
	int m_nSprintTimer;							//スプリント用タイマー
	CLoadEffect * m_pLoadEffectSpeed;			//エフェクトの情報
};
//==============================================
//		パワー型プレイヤー
//==============================================
class CPowerPlayer : public CPlayer
{
public:
	CPowerPlayer();
	~CPowerPlayer();

	HRESULT Init(D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40], int nNumPlayer, int nType);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void ActionUpdate(void);			//アクション処理
	void PlayerActionPad();				//コントローラでのアクション処理
	void PlayerActionMouse();			//マウスでのアクション移動処理
	void PlayerActionKeyboard();		//キーボードでのアクション移動処理
	void RotAction(int nType);			//コントローラーの向きの処理					
	void InitNum(void);					//変数初期処理
	static CPowerPlayer *Create(D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40], int nNumPlayer, int nType, 
		CHARCTERTYPE charctertype,int nControllerIndx, int nControllerType);
	void  Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);
private:
	void CreateColliderSphere(void);	//衝撃波の当たり判定
	int m_nColliderCnt;					//衝撃波カウンター
	int m_nColliderTimer;				//衝撃波タイマー
	bool m_bAction;						//アクション中かどうか
	bool m_bInit;						//初期化するかどうか
	int m_nInitCnt;						//初期化までのカウンター
	int m_nRotCounter;					//向きを変えれるカウンター
	bool m_bCollision;					//当たり判定
};

//==============================================
//		地雷型型プレイヤー
//==============================================
class CTechniquePlayer : public CPlayer
{
public:
	
	CTechniquePlayer();
	~CTechniquePlayer();

	HRESULT Init(D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40], int nNumPlayer, int nType);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void ActionUpdate(void);			//アクション処理
	void PlayerActionPad();				//コントローラでのアクション処理
	void PlayerActionMouse();			//マウスでのアクション移動処理
	void PlayerActionKeyboard();		//キーボードでのアクション移動処理
	static CTechniquePlayer *Create(D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40], int nNumPlayer, int nType,
		CHARCTERTYPE charctertype, int nControllerIndx, int nControllerType);
	void  Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);

private:
	int m_nMinePoint;					//マインの置けるポイント
	int m_nReCounter;					//回復カウンター
	int m_nReTimer;						//回復タイマー
	int m_nInstallationCounter;			//設置カウンター
	int m_nInstallationTimer;			//設置タイマー
	bool m_bMineUse;					//地雷を使用したか
	int m_nInitCnt;						//初期化までのカウンター
	bool m_bInit;						//初期化するかどうか
};
#endif