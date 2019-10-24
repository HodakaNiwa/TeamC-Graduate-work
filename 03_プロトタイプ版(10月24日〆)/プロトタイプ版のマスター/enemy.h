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
#define BASE_MAX	(5)
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
//==============================================
//			キャラクターの派生 → プレイヤー
//==============================================
class CEnemy : public CCharacter
{
public:
	typedef struct
	{
		float fDistance;	//	拠点との距離
		float fChange;
		float fRadian;		//	〃の角度
	}INFO;

	//	---<<コンストラクタ・デストラクタ>>---
	CEnemy();
	~CEnemy();

	//	---<<生成>>---
	static CEnemy *Create(D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40]);

	//	---<<基盤関数>>---
	HRESULT Init(void);	//	デフォルト関数
	HRESULT Init(D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40]);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//	---<<追加関数>>---
	void Move(void);				//	移動処理
	void Collision(void);
	//void CollisionModel(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove);

	//	---<<Set関数>>---
	void  Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);	//	デフォルト関数

	//	---<<小林が追加しました>>★	★	★	★	★	★	★	★	★	★
	float dist(D3DXVECTOR3 obj, D3DXVECTOR3 enemy);
	//	★	★	★	★	★	★	★	★	★	★	★	★	★	★	★	★

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

	//	仮AIに必要な処理
	D3DXVECTOR3 m_beforePos;	//	ワンフレーム前の位置情報
	//D3DXVECTOR3 nextPos;		//	次の位置の予測値

	D3DXVECTOR3 m_objPos[BASE_MAX];	//	(仮)拠点の位置情報
	int m_nNum;						//	(仮)拠点番号の確保
	INFO m_Info[BASE_MAX];			//	(仮)拠点情報
	int m_nNext;
};
#endif