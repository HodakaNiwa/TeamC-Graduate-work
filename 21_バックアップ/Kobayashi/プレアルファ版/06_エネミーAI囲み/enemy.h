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
#include "sceneX.h"
#include "game.h"
#include "scene.h"
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
class CScene3D;
class CObject;
class CEnemy;
class CSceneX;
class CCollider;
class CMeshCollider;
class CBoxCollider;
class CSphereCollider;
class CCylinderCollider;
class CEnemyAttackSphereCollider;
class CModel;
class CTerritory;
class CLine;
class CSceneOrbit;

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

	//	====================================================



	//	---<<コンストラクタ・デストラクタ>>---
	CEnemy();
	~CEnemy();

	//	---<<生成>>---
	static CEnemy *Create(int nNum, TYPE type, D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40]);

	//	---<<基盤関数>>---
	HRESULT Init(void);	//	デフォルト関数
	HRESULT Init(int nNum, D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40]);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//	---<<Set関数>>---
	void  Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);	//	デフォルト関数

	//	---<<追加関数>>---
	void Move(void);				//	移動処理
	void Collision(void);
	void InitSort(D3DXVECTOR3 pos);	//	ゲーム開始時の近場のエリア・テリトリーを見つける

	// ---<<モデルタイプ>>---
	MODEL_TYPE GetModelType(void) { return m_modelType; }

private:
	D3DXVECTOR3 m_pos;						//	位置
	D3DXVECTOR3 m_posOld;					//	古い位置情報
	D3DXVECTOR3 m_move;						//	移動量
	D3DXVECTOR3 m_rot;						//	移動量
	D3DXCOLOR   m_col;
	D3DXMATRIX	m_mtxWorld;					//	ワールドマトリックス
	D3DXVECTOR3 m_vtxMax;
	D3DXVECTOR3 m_vtxMin;
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// 頂点バッファへのポインタ
	static LPD3DXMESH m_pMeshModel;			//	メッシュ情報へのポインタ
	static LPD3DXBUFFER m_pBufferMatModel;	//	マテリアル情報へのポインタ
	static DWORD m_nNumMatModel;			//	マテリアル情報の数
	bool m_bUse;							//	使用しているかどうか

	static CSceneX *m_apSceneX;
	D3DXVECTOR3 m_Angle;					//	角度

	bool CollisionCollider(CCollider *pCollider, D3DXVECTOR3 &pos, D3DXVECTOR3 &posOld, D3DXVECTOR3 &Move, D3DXVECTOR3 &ColRange);
	bool CollisionBoxCollider(CBoxCollider *pBoxCollider, D3DXVECTOR3 &pos, D3DXVECTOR3 &posOld, D3DXVECTOR3 &Move, D3DXVECTOR3 &ColRange);

	//	位置・回転情報
	CModel** m_pModel;

	// ---<<モデルタイプ>>---
	MODEL_TYPE m_modelType;


	//!	---<<AI関連>---
	D3DXVECTOR3 m_beforePos;					//	ワンフレーム前の位置情報
	int m_nTargetNum;							//	(仮)拠点番号の確保
	int m_nTargetCnt;							//	拠点の通過回数を記憶
	bool m_bBreak;								//	ループ解除用

	//!	---<<テリトリー関連>>---
	CTerritory* m_pTerritory;				//	クラスポインタ
	TERRITORY_INFO* m_TerritoryInfo;		//	構造体ポインタ
	TERRITORY_INFO* m_AreaInfo[AREA_MAX];	//	構造体ポインタ
	int m_nMax;								//	生成数
	int m_nAreaTerrNum[AREA_MAX];			//	各エリアのテリトリー数
	int m_nAreaNow;							//	現在いるエリア番号

	//!	---<<ラインの判定に必要>>---
	void CreateOrbitLine(void);				//	軌跡の線の生成
	void UninitOrtbitLine(void);			//	ラインの破棄


};
#endif