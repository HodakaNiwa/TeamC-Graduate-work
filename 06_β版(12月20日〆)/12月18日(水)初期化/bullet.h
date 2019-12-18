//====================================================================================
//
// ロボットの弾処理	[bullet.h]
// Author;荒谷由朗
//
//====================================================================================
#ifndef _BULLET_H_
#define _BULLET_H_

#include "main.h"
#include "sceneX.h"

#define BULLET_MAX_EFFECT (2)

//====================================================================================
// 前方宣言
//====================================================================================
class CLoadEffect;
class CCollider;
class CBoxCollider;

//-------------------------------------------------------------
// クラス宣言
//-------------------------------------------------------------
class CBullet : public CSceneX
{
public:
	typedef enum
	{
		TYPE_NONE = 0,	// 通常状態
		TYPE_MOVE,		// 移動状態
		TYPE_EXPLOSION,	// 爆発状態
		TYPE_MAX
	}TYPE;

	CBullet(int nPriority = 3, OBJTYPE objtype = OBJTYPE_ROBOTBULLET);
	~CBullet();
	static CBullet * Create	(const D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale);
	HRESULT Init			(void);
	void	Uninit			(void);
	void	Update			(void);
	void	Draw			(void);

	void	Collision		(void);

	static void LoadModel	(void);
	static void UnloadModel	(void);

	//--------------------
	//	Set & Get
	//--------------------
	void	SetType(const int nType)	{ m_nType = nType; }
	void	SetLife(const int nLife)	{ m_nLife = nLife; }

	int		GetType(void)				{ return m_nType; }
	int		GetLife(void)				{ return m_nLife; }

private:
	//--------------------
	// 関数宣言
	//--------------------
	void Move					(void);		// 移動処理
	void Explosion				(void);		// 爆発処理
	void CreateCollider			(void);		// 当たり判定の生成
	void CreateExplosionEffect	(void);		// 爆発エフェクトの生成
	void CreateExplosionCollider(void);		// 爆発当たり判定の生成
	bool CollisionCollider		(CCollider *pCollider, D3DXVECTOR3 &pos, D3DXVECTOR3 &posOld, D3DXVECTOR3 &Move, D3DXVECTOR3 &ColRange);
	bool CollisionBoxCollider	(CBoxCollider *pBoxCollider, D3DXVECTOR3 &pos, D3DXVECTOR3 &posOld, D3DXVECTOR3 &Move, D3DXVECTOR3 &ColRange);

	//--------------------
	// 静的メンバ変数
	//--------------------
	static LPDIRECT3DTEXTURE9	m_ppTexture;		// テクスチャへのポインタ
	static LPD3DXMESH 			m_pMesh;			// メッシュ情報へのポインタ
	static LPD3DXBUFFER 		m_pBuffMat;			// マテリアル情報へのポインタ
	static DWORD 				m_pNumMat;			// マテリアル情報の数
	static D3DXVECTOR3 			m_pVtxMin;			// モデル最小値
	static D3DXVECTOR3 			m_pVtxMax;			// モデル最大値
	static int					m_nMaxTex;			// テクスチャの最大数
	static int					m_nMaxModel;		// モデルの最大数

	//--------------------
	// メンバ変数
	//--------------------
	D3DXVECTOR3					m_pos;				// 位置
	D3DXVECTOR3					m_posOld;			// 位置(過去)
	D3DXVECTOR3					m_rot;				// 向き
	D3DXVECTOR3					m_move;				// 移動量
	D3DXVECTOR3					m_Angle;			// 角度
	int							m_nType;			// 状態
	int							m_nLife;			// 寿命
	bool						m_bDeth;			// 死亡フラグ
	int							m_nExplosionTimer;	// 爆発までの時間を計測する
	bool						m_bExplosion;		// 爆発フラグ
	CLoadEffect				*	m_pLoadEffect[BULLET_MAX_EFFECT];	// エフェクトの情報

};


#endif