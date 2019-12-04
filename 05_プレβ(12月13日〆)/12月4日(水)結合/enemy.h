//◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆
//					エネミー処理 [enemy.h]
//			Author : Kobayashi_Sho-hei / 小林 将兵
//◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆
#ifndef _ENEMY_H_
#define _ENEMY_H_

//=================================================
//		include / 他のデータ・コードの呼び出し・利用
//=================================================
#include "main.h"
#include "game.h"
#include "character.h"
//==============================================
//					マクロ定義
//==============================================
#define AREA_MAX	(4)			//	エリアの分割数
#define TERRITORY_MAX	(10)	//	最大でつなげる数

//==============================================
//					前方宣言
//==============================================
class CSound;
class CModel;
class CMotion;
class CEnemy;
class CTerritory;
class CLoadEffect;
class CCollider;
class CBoxCollider;
class CCylinderCollider;
class CPlayerAttackSphereCollider;

//==============================================
//			キャラクターの派生 → プレイヤー
//==============================================
class CEnemy : public CCharacter
{
public:
	//	=============構造体==========================
	// モデルタイプ構造体
	typedef enum
	{
		PLAYERTYPE_SPEED = 0,
		PLAYERTYPE_POWER,
		PLAYERTYPE_TECHNIQUE,
		PLAYERTYPE_MAX
	}MODEL_TYPE;

	//	テリトリー情報構造体
	typedef struct
	{
		D3DXVECTOR3 pos;
		int nAreaNum;		//	エリア番号
		float fDistance;	//	距離
		float fDisSort;		//	ソート後の距離(短い順)
		float fRadian;		//	角度
		bool bFlag;			//	一度通ったか
	}TERRITORY_INFO;

	//	プレイヤーの状態
	typedef enum
	{
		STATE_NONE = 0,
		STATE_WALK,
		STATE_ACTION,
		STATE_BLOWAWAY,
		STATE_DAMAGE,
		STATE_MAX
	}STATE;

	//	====================================================

	//	---<<コンストラクタ・デストラクタ>>---
	CEnemy(int nPriority = 3, OBJTYPE objType = OBJTYPE_PLAYER);
	~CEnemy();

	//	---<<生成>>---
	static CEnemy *Create(int nNum, TYPE type, D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40], CHARCTERTYPE charatype);

	//	---<<デフォルト関数>>---
	HRESULT Init(void);
	void  Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);

	//	---<<基盤関数>>---
	HRESULT Init(int nNum, D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40],int nType);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//	---<<追加関数>>---
	void InitSort(D3DXVECTOR3 pos);	//	ゲーム開始時の近場のエリア・テリトリーを見つける
	void DisSort(D3DXVECTOR3 pos);	//	距離間ソート計算(エネミーの現在地)
	void AIBasicAction(void);		//	AIの共通処理
	void Program_Move(D3DXVECTOR3 pos,TERRITORY_INFO territory);		//	移動・位置変更
	void Program_Line(void);		//	ライン関数まとめ
	void Program_State(void);		//	ステータス管理
	void Program_Motion(void);//	モーションアクション

	// ---<<当たり判定>>---
	void Collision(void);

	//!	---<<ラインの判定>>---
	void CreateOrbitLine(void);
	void UninitOrtbitLine(void);

	// マージ関連
	void MergeSort(TERRITORY_INFO*, int, int, int); // (TERRITORY_INFO*, 0, 拠点数-1, 拠点数)

	// ラインを繋ぐ手順
	void LineConnect(int);

protected:
	int m_nEnemyNo;					//	割り当てられたキャラ番号
	CModel** m_pModel;				//	モデルの情報
	CMotion* m_pMotion;				//	モーションの情報
	STATE m_state;					//	キャラクターの状態
	CLoadEffect * m_pLoadEffect;	//	エフェクトの情報
	bool m_bWalk;					//	移動をしてるか
	bool m_bSprintMotion;
	float m_fSpeed;					//	プレイヤーの速さ
	bool m_bStop;					//	複数回処理を実行させない用(各タイプのAIスキル処理で使用)
	int m_nDamageCount;				// ダメージを受けてからの時間を計測する
	float m_fBlowAngle;				// 吹っ飛ぶ方向
	float m_fBlowLength;			// 吹っ飛ぶ距離

	//!	---<<キャラ同士がぶつかった時>>---
	void BlowAway(D3DXVECTOR3 AnotherPos);

	//!	---<<ラインを繋ぐ・図形の完成>>---
	int m_nLineNum;							//	現在のライン数
	bool m_bFinish;							//	図形を完成させるかどうか
	TERRITORY_INFO m_nTerrStart;			//	図形となるラインを繋ぐ際の始点・終点を記憶(始点・終点は同じ位置)

	//!	---<<スキル使用関連>>---
	bool m_bTarget;							//	ターゲットの切り替え

	//!	---<<ダメージ処理>>---
	int m_nDamageCounter;					//	ダメージカウンター
	bool m_bSuperArmor;						//	スーパーアーマ状態
	D3DXVECTOR3 m_rot;						//	移動量

	int m_nLevel;							// AIごとのレベル

private:
	//D3DXVECTOR3 m_pos;						//	位置
	D3DXVECTOR3 m_posOld;					//	古い位置情報
	D3DXVECTOR3 m_move;						//	移動量
	D3DXCOLOR   m_col;
	D3DXMATRIX	m_mtxWorld;					//	ワールドマトリックス
	bool m_bUse;							//	使用しているかどうか


	bool CollisionCollider(CCollider *pCollider, D3DXVECTOR3 &pos, D3DXVECTOR3 &posOld, D3DXVECTOR3 &Move, D3DXVECTOR3 &ColRange);
	bool CollisionBoxCollider(CBoxCollider *pBoxCollider, D3DXVECTOR3 &pos, D3DXVECTOR3 &posOld, D3DXVECTOR3 &Move, D3DXVECTOR3 &ColRange);
	bool CollisionCylinderyCollider(CCylinderCollider *pCylinderCollider, D3DXVECTOR3 &pos, D3DXVECTOR3 &posOld, D3DXVECTOR3 &Move, D3DXVECTOR3 &ColRange);
	bool CollisionPlayerAttackSphereCollider(CPlayerAttackSphereCollider *pShere, D3DXVECTOR3 &pos, D3DXVECTOR3 &ColRange);
	bool CollisionRobotAttackSphereCollider(CPlayerAttackSphereCollider *pShere, D3DXVECTOR3 &pos, D3DXVECTOR3 &ColRange); 

	// ---<<モデルタイプ>>---
	MODEL_TYPE m_modelType;

	//!	---<<AI関連>---
	int m_nTargetNum;							//	(仮)拠点番号の確保
	int m_nTargetCnt;							//	拠点の通過回数を記憶
	bool m_bBreak;								//	ループ解除用

	//!	---<<テリトリー関連>>---
	CTerritory* m_pTerritory;				//	クラスポインタ
	TERRITORY_INFO* m_TerritoryInfo;		//	構造体ポインタ
	TERRITORY_INFO* m_AreaInfo[AREA_MAX];	//	構造体ポインタ
	int m_nAreaTerrNum[AREA_MAX];			//	各エリアのテリトリー数
	int m_nAreaNow;							//	現在いるエリア番号


	//	マージ関連変数
	float *m_tmp[4]; // [Temporary]一時的に値を記憶する変数
	bool m_bCheck; // 複数回newさせないため
};
//==============================================
//		スピード型
//==============================================
class CTypeSpeed : public CEnemy
{
public:
	CTypeSpeed(int nPriority = 3, OBJTYPE objType = OBJTYPE_ENEMY);
	~CTypeSpeed();

	static CTypeSpeed *Create(int nChara, int nLevel, int country, CHARCTERTYPE type, D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40]);
	HRESULT Init(int nChara, D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40], int country);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SprintUpdate(void);	//スピード型のスキル処理
	void Trigger(void);
	void  Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);

private:
	int m_nCnt;
	bool m_bSkillFlag;
	int m_nTimingCnt;	// レベルごとに発動するタイミングを変える
};

//==============================================
//		脳筋型
//==============================================
class CTypePower : public CEnemy
{
public:
	//	---<<コンストラクタ・デストラクタ>>---
	CTypePower(int nPriority = 3, OBJTYPE objType = OBJTYPE_ENEMY);
	~CTypePower();
	//	---<<生成>>---
	static CTypePower *Create(int nChara, int country, CHARCTERTYPE type, D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40]);
	//	---<<デフォルト関数>>---
	HRESULT Init(void);
	void  Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);
	//	---<<基盤関数>>---
	HRESULT Init(int nChara, D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40], int country);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	//	---<<追加関数>>---
	void ActionUpdate(void); // スキル処理


							 //!	---<<スキル発動範囲の判定>>---
	bool CollisionSkillTiming(CCylinderCollider *pCylinderCollider, D3DXVECTOR3 &pos, D3DXVECTOR3 &posOld,
		D3DXVECTOR3 &Move, D3DXVECTOR3 &ColRange);

private:

	void CreateColliderSphere(void); // 衝撃波の判定

									 //!	---<<スキル使用関連>>---
	bool m_bTrigger;	//	スキルの使用
	bool m_bBreakTime;	//	使用可能か
};

//==============================================
//		地雷型
//==============================================
class CTypeSweeper : public CEnemy
{
public:
	//	---<<コンストラクタ・デストラクタ>>---
	CTypeSweeper();
	~CTypeSweeper();
	//	---<<生成>>---
	static CTypeSweeper *Create(int nChara, int country, CHARCTERTYPE type, D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40]);
	//	---<<デフォルト関数>>---
	HRESULT Init(void);
	void  Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);
	//	---<<基盤関数>>---
	HRESULT Init(int nChara, D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40], int country);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	//	---<<追加関数>>---
	void ActionUpdate(void); // スキル処理


							 //! ---<<地雷関連>>---
	void Process();	//	地雷使用時の処理

private:
	int m_nMinePoint;			//	マインの置けるポイント
	int m_nReCounter;			//	回復カウンター
	int m_nReTimer;				//	回復タイマー
	int m_nInstallationCounter;	//	設置カウンター
	int m_nInstallationTimer;	//	設置タイマー
	int m_nTiming;				//	タイミング
	bool m_nUse;
	int m_nActionCnt;
};


#endif