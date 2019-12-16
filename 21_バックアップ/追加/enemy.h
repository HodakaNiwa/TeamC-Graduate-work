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
	//++ =====構造体=====
	
	//テリトリー関連
	typedef struct
	{
		D3DXVECTOR3 pos;	//拠点の位置
		int nAreaNum;		//拠点の所属するエリア番号
		float fDistance;	//キャラと拠点の距離
		float fDisSort;		//ソートした距離を記憶(短い順)
		float fRadian;		//角度
		bool bFlag;			//拠点を一度は通ったか
	}TERRITORY_INFO;

	//キャラクターの状態
	typedef enum
	{
		STATE_NONE = 0,
		STATE_WALK,
		STATE_ACTION,
		STATE_BLOWAWAY,
		STATE_DAMAGE,
		STATE_MAX
	}STATE;

	//---<<コンストラクタ・デストラクタ>>---
	CEnemy(int nPriority = 3, OBJTYPE objType = OBJTYPE_PLAYER);
	~CEnemy();

	//---<<生成>>---
	static CEnemy *Create(int nNum, TYPE type, D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40], CHARCTERTYPE charatype);

	//---<<デフォルト関数>>---
	HRESULT Init(void);
	void  Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);

	//---<<基盤関数>>---
	HRESULT Init(int nNum, D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40],int nType);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//---<<追加関数>>---
	void InitSort(D3DXVECTOR3 pos);										//初回距離ソート(一回通るのみ)
	void DisSort(D3DXVECTOR3 pos);										//距離のソート(現在のキャラの位置から見て)
	void AIBasicAction(void);											//AIの共通処理
	void Program_Move(D3DXVECTOR3 pos,TERRITORY_INFO territory);		//移動関連
	void Program_Line(void);											//ライン関連
	void Program_State(void);											//ステータス関連
	void Program_Motion(void);											//モーション関連
	
	//---<<イベント>>---
	void Event_Bonus(void);
	void Event_Division(int,int ,int);//(分断タイプ(縦横), 現在いるエリア, 現在いるエリア+1))

	//---<<当たり判定>>---
	void Collision(void);

	//---<<ラインの判定>>---
	void CreateOrbitLine(void);
	void UninitOrtbitLine(void);
	void LineConnect(int);//ラインを繋ぐ手順

	//マージ関連
	void MergeSort(TERRITORY_INFO*, int, int, int);//(TERRITORY_INFO*, 0, 拠点数-1, 拠点数)


protected:
	int m_nEnemyNo;					//キャラ番号(生成順)
	CModel** m_pModel;				//モデル情報
	CMotion* m_pMotion;				//モーション情報
	STATE m_state;					//ステータス情報
	CLoadEffect * m_pLoadEffect;	//エフェクト情報
	bool m_bWalk;					//移動状態
	bool m_bSprintMotion;
	float m_fSpeed;					//移動速度
	bool m_bStop;					//処理を一時的/複数回行うのを阻止
	int m_nDamageCount;				//ダメージ後の時間をカウント
	float m_fBlowAngle;				//吹っ飛ぶ方向
	float m_fBlowLength;			//吹っ飛ぶ距離

	//---<<キャラ同士がぶつかった時>>---
	void BlowAway(D3DXVECTOR3 AnotherPos);

	//---<<ラインを繋ぐ・図形の完成>>---
	int m_nLineNum;					//現在のライン数
	bool m_bFinish;					//図形を完成させるかどうか
	TERRITORY_INFO m_nTerrStart;	//図形となるラインを繋ぐ際の始点・終点を記憶(始点・終点は同じ位置)

	//---<<スキル使用関連>>---
	bool m_bTarget;					//拠点かキャラクター

	//---<<ダメージ処理>>---
	int m_nDamageCounter;			//ダメージカウンター
	bool m_bSuperArmor;				//スーパーアーマ状態
	D3DXVECTOR3 m_rot;				//向き
	int m_nLevel;					//簡易的なレベル分け

private:

	D3DXVECTOR3 m_posOld;			//ワンフレーム前の位置

	//あたり判定関数
	bool CollisionCollider(CCollider *pCollider, D3DXVECTOR3 &pos, D3DXVECTOR3 &posOld, D3DXVECTOR3 &Move, D3DXVECTOR3 &ColRange);
	bool CollisionBoxCollider(CBoxCollider *pBoxCollider, D3DXVECTOR3 &pos, D3DXVECTOR3 &posOld, D3DXVECTOR3 &Move, D3DXVECTOR3 &ColRange);
	bool CollisionCylinderyCollider(CCylinderCollider *pCylinderCollider, D3DXVECTOR3 &pos, D3DXVECTOR3 &posOld, D3DXVECTOR3 &Move, D3DXVECTOR3 &ColRange);
	bool CollisionPlayerAttackSphereCollider(CPlayerAttackSphereCollider *pShere, D3DXVECTOR3 &pos, D3DXVECTOR3 &ColRange);
	bool CollisionRobotAttackSphereCollider(CPlayerAttackSphereCollider *pShere, D3DXVECTOR3 &pos, D3DXVECTOR3 &ColRange); 


	//---<<AI関連>---
	int m_nTargetNum;							//割り振られた拠点の番号　
	int m_nPassCnt;								//拠点の通過回数を記憶
	bool m_bBreak;								//ループ解除用

	//---<<テリトリー関連>>---
	CTerritory* m_pTerritory;					//クラスポインタ
	TERRITORY_INFO* m_TerritoryInfo;			//全体の拠点情報
	TERRITORY_INFO* m_AreaInfo[AREA_MAX];		//エリアごとの拠点情報
	int m_nAreaTerrNum[AREA_MAX];				//各エリアのテリトリー数
	int m_nAreaNow;								//拠点のエリア番号
	int m_nRandNum;
	bool m_nRandStop;

	//イベント関連
	bool m_EventCheck;
	bool m_bReset;
	bool m_EveCheckDivi;

	//マージソート
	float *m_tmp[4];							// [Temporary]一時的に値を記憶する変数

	bool m_bDebugStop;							//フラグでエネミーの移動を止める
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
	int m_nTimingCnt;			// レベルごとに発動するタイミングを変える
	CLoadEffect * m_pLoadEffectSpeed;			//エフェクトの情報

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

	 //---<<スキル発動範囲の判定>>---
	bool CollisionSkillTiming(CCylinderCollider *pCylinderCollider, D3DXVECTOR3 &pos, D3DXVECTOR3 &posOld,
		D3DXVECTOR3 &Move, D3DXVECTOR3 &ColRange);

private:

	void CreateColliderSphere(void); // 衝撃波の判定

	 //---<<スキル使用関連>>---
	bool m_bTrigger;				//	スキルの使用
	bool m_bBreakTime;				//	使用可能か
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
	void ActionUpdate(void);	// スキル処理


	 //! ---<<地雷関連>>---
	void Process();				//	地雷使用時の処理

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