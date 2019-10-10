//*****************************************************************************
//
//     コリジョンの処理[collision.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _COLLISION_H_
#define _COLLISION_H_

//*****************************************************************************
//    インクルードファイル
//*****************************************************************************
#include "meshSphere.h"

//*****************************************************************************
//     前方宣言
//*****************************************************************************
class CScene;

//*****************************************************************************
//    コライダークラスの定義
//*****************************************************************************
class CCollider
{
public:    // 誰でもアクセス可能
	//------------------------
	//  コライダーの種類
	//------------------------
	typedef enum
	{
		TYPE_NONE = -1,            // 無し
		TYPE_SQUARE2D,             // 矩形(2D)
		TYPE_CIRCLE2D,             // 円形(2D)
		TYPE_MESH,                 // メッシュ
		TYPE_BOX,                  // 箱
		TYPE_SPHERE,               // 球
		TYPE_CYLINDER,             // 筒
		TYPE_SPHERE_PLAYERATTACK,  // プレイヤーの攻撃球
		TYPE_SPHERE_ENEMYATTACK,   // 敵の攻撃球
		TYPE_MAX
	}TYPE;

	CCollider(TYPE type = TYPE_NONE, CScene *pParent = NULL);
	virtual ~CCollider();

	virtual HRESULT Init(void) = 0;
	virtual void Uninit(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;

	static void UpdateAll(void);
	static void DrawAll(void);
	static void ReleaseAll(void);
	static void DeathCheck(void);
	static void SwicthDraw(void);
	static bool GetDrawAll(void);
	static CCollider *GetTop(int nType);

	void SetTransformToDevice(const LPDIRECT3DDEVICE9 pDevice);
	void CalcNotScaleTransform(void);
	void SetParent(CScene *pParent);
	void SetPos(const D3DXVECTOR3 pos);
	void SetPosOld(const D3DXVECTOR3 posOld);
	void SetRot(const D3DXVECTOR3 rot);
	void SetScale(const D3DXVECTOR3 scale);
	void SetMtxWorld(const D3DXMATRIX mtxWorld);
	void SetParentMtxWorld(D3DXMATRIX *pParentMtxWorld);
	void SetDisp(const bool bDisp);

	CScene *GetParent(void);
	CCollider *GetNext(void);
	CCollider *GetPrev(void);
	TYPE GetType(void);
	D3DXVECTOR3 GetPos(void);
	D3DXVECTOR3 GetPosOld(void);
	D3DXVECTOR3 GetRot(void);
	D3DXVECTOR3 GetScale(void);
	D3DXMATRIX GetMtxWorld(void);
	D3DXMATRIX *GetParentMtxWorld(void);
	bool GetDisp(void);

protected: // このクラスと派生クラスだけがアクセス可能
	void Release(void);

private:   // このクラスだけがアクセス可能
	static bool      m_bDrawAll;           // 全てのコライダーを描画するかどうか
	static CCollider *m_apTop[TYPE_MAX];   // 先頭のコライダークラスへのポインタ
	static CCollider *m_apCur[TYPE_MAX];   // 現在の最後尾ポインタ保存用

	CScene      *m_pParent;          // コライダーを所持しているオブジェクトへのポインタ
	CCollider   *m_pNext;            // 次のコライダーへのポインタ
	CCollider   *m_pPrev;            // 前のコライダーへのポインタ
	TYPE        m_Type;              // コライダーの種類
	bool        m_bDeath;            // 死亡フラグ
	D3DXVECTOR3 m_Pos;               // 現在の位置(もしくはローカル位置)
	D3DXVECTOR3 m_PosOld;            // 前回の位置
	D3DXVECTOR3 m_Rot;               // 向き
	D3DXVECTOR3 m_Scale;             // 大きさ
	D3DXMATRIX  m_MtxWorld;          // ワールドマトリックス
	D3DXMATRIX  *m_pParentMtxWorld;  // 親のワールドマトリックスへのポインタ
	bool        m_bDisp;             // 描画するかどうか
};

//*****************************************************************************
//    矩形(2D)コライダークラスの定義
//*****************************************************************************
class CSquareCollider2D : public CCollider
{
public:    // 誰でもアクセス可能
	CSquareCollider2D(TYPE type = TYPE_SQUARE2D, CScene *pParent = NULL);
	~CSquareCollider2D();

	static CSquareCollider2D *Create(D3DXVECTOR3 pos, float fWidth, float fHeight, CScene *pParent = NULL);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	bool Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fWidth, float fHeight);

	void SetWidth(const float fWidth);
	void SetHeight(const float fHeight);

	float GetWidth(void);
	float GetHeight(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	float m_fWidth;    // 矩形の幅
	float m_fHeight;   // 矩形の高さ
};

//*****************************************************************************
//    円形(2D)コライダークラスの定義
//*****************************************************************************
class CCircleCollider2D : public CCollider
{
public:    // 誰でもアクセス可能
	CCircleCollider2D(TYPE type = TYPE_CIRCLE2D, CScene *pParent = NULL);
	~CCircleCollider2D();

	static CCircleCollider2D *Create(D3DXVECTOR3 pos, float fRadius, CScene *pParent = NULL);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	bool Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fRadius);

	void SetRadius(const float fRadius);

	float GetRadius(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	float m_fRadius;    // 円の半径
};

//*****************************************************************************
//    ボックスコライダークラスの定義
//*****************************************************************************
class CBoxCollider : public CCollider
{
public:    // 誰でもアクセス可能
	CBoxCollider(TYPE type = TYPE_BOX, CScene *pParent = NULL);
	~CBoxCollider();

	static CBoxCollider *Create(D3DXVECTOR3 pos, D3DXVECTOR3 scale, float fWidth, float fHeight, float fDepth, bool bReturnFlag);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	bool Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 ColRange, CScene *pScene, bool *pLand = NULL);

	void SetWidth(const float fWidth);
	void SetHeight(const float fHeight);
	void SetDepth(const float fDepth);
	void SetReturnFlag(const bool bReturnFlag);

	float GetWidth(void);
	float GetHeight(void);
	float GetDepth(void);
	bool GetReturnFlag(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	void MakeVertex(const LPDIRECT3DDEVICE9 pDevice);
	void MakeIndex(const LPDIRECT3DDEVICE9 pDevice);

	typedef struct
	{
		D3DXVECTOR3 pos;
		D3DCOLOR col;
	}BOX_VERTEX;

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;      // 頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9  m_pIdxBuff;      // インデックスバッファへのポインタ
	float                   m_fWidth;        // 箱の横幅
	float                   m_fHeight;       // 箱の高さ
	float                   m_fDepth;        // 箱の奥行
	bool                    m_bReturnFlag;   // 当たった際に戻すかどうか
};

//*****************************************************************************
//    当たり判定可視化用のメッシュスフィアクラスの定義
//*****************************************************************************
class CColDispSphere : public CMeshSphere
{
public:    // 誰でもアクセス可能
	CColDispSphere(int nPriority = 3, OBJTYPE objType = OBJTYPE_COLDISPSPHERE);
	~CColDispSphere();

	static CColDispSphere *Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fRadius, int nXBlock, int nYBlock, int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos, D3DXCOLOR col, float fRadius, int nXBlock, int nYBlock);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Disp(void);
	void SetVtxBuffValue(void);
	void Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size) {};

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能

};

//*****************************************************************************
//    スフィアコライダークラスの定義
//*****************************************************************************
class CSphereCollider : public CCollider
{
public:    // 誰でもアクセス可能
	CSphereCollider(TYPE type = TYPE_SPHERE, CScene *pParent = NULL);
	~CSphereCollider();

	static CSphereCollider *Create(D3DXVECTOR3 pos, D3DXVECTOR3 scale, float fRadius, bool bReturnFlag);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	bool Collision(D3DXVECTOR3 *pPos, float fColRadius, CScene *pScene);

	void SetRadius(const float fRadius);
	void SetReturnFlag(const bool bReturnFlag);
	void SetColDispSphere(CColDispSphere *pColDispSphere);

	float GetRadius(void);
	bool GetReturnFlag(void);
	CColDispSphere *GetColDispSphere(void);

protected: // このクラスと派生クラスだけがアクセス可能
	virtual void MakeColDispSphere(void);

private:   // このクラスだけがアクセス可能
	CColDispSphere *m_pColDispSphere;   // 当たり判定可視化用メッシュスフィアクラスへのポインタ
	float          m_fRadius;           // 球の半径
	bool           m_bReturnFlag;       // 当たった際に戻すかどうか
};

//*****************************************************************************
//    シリンダーコライダークラスの定義
//*****************************************************************************
class CCylinderCollider : public CCollider
{
public:    // 誰でもアクセス可能
	CCylinderCollider(TYPE type = TYPE_CYLINDER, CScene *pParent = NULL);
	~CCylinderCollider();

	static CCylinderCollider *Create(D3DXVECTOR3 pos, D3DXVECTOR3 scale, float fRadius, float fHeight, bool bReturnFlag, CScene *pParent = NULL);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	bool Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fColRadius, float fHeight, CScene *pScene, bool *pLand = NULL);

	void SetRadius(const float fRadius);
	void SetHeight(const float fHeight);
	void SetReturnFlag(const bool bReturnFlag);

	float GetRadius(void);
	float GetHeight(void);
	bool GetReturnFlag(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	void MakeVertex(const LPDIRECT3DDEVICE9 pDevice);
	void MakeIndex(const LPDIRECT3DDEVICE9 pDevice);
	bool LandCollision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fColRadius, float fHeight, CScene *pScene, bool *pLand = NULL);
	bool RangeCollision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fColRadius, float fHeight, CScene *pScene, bool *pLand = NULL);

	typedef struct
	{
		D3DXVECTOR3 pos;
		D3DCOLOR col;
	}CYLINDER_VERTEX;

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;    // 頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9  m_pIdxBuff;    // インデックスバッファへのポインタ
	float           m_fRadius;             // 筒の半径
	float           m_fHeight;             // 筒の高さ
	bool            m_bReturnFlag;         // 当たった際に戻すかどうか
};

//*****************************************************************************
//    メッシュコライダークラスの定義
//*****************************************************************************
class CMeshCollider : public CCollider
{
public:    // 誰でもアクセス可能
	CMeshCollider(TYPE type = TYPE_MESH, CScene *pParent = NULL);
	~CMeshCollider();

	static CMeshCollider *Create(float fWidth, float fDepth, int nXBlock, int nZBlock, int nNumVertex, LPDIRECT3DVERTEXBUFFER9 pVtxBuffParent, D3DXMATRIX *pMtxParent, bool bReturnFlag, CScene *pParent = NULL);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void MakeVertex(const LPDIRECT3DDEVICE9 pDevice);
	void SetWorldVertexPos(void);

	void SetVerPos(D3DXVECTOR3 *pVerPos);
	void SetWidth(const float fWidth);
	void SetDepth(const float fDepth);
	void SetXBlock(const int nXBlock);
	void SetZBlock(const int nZBlock);
	void SetVtxBuffParent(const LPDIRECT3DVERTEXBUFFER9 pVtxBuffParent);
	void SetReturnFlag(const bool bReturnFlag);

	D3DXVECTOR3 *GetVerPos(void);
	float GetWidth(void);
	float GetDepth(void);
	int GetXBlock(void);
	int GetZBlock(void);
	int GetNumVertex(void);
	LPDIRECT3DVERTEXBUFFER9 GetVtxBuffParent(void);
	bool GetReturnFlag(void);

	float GetPolyHeight(D3DXVECTOR3 pos, bool *pLand = NULL, D3DXVECTOR3 *pPolyNormal = NULL, D3DXVECTOR3 *pVerPos = NULL);
	bool CheckPenetration(D3DXVECTOR3 pos, int *pXBlock = NULL, int *pYBlock = NULL, int *pLeftVerIndex = NULL);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	D3DXVECTOR3             *m_pVerPos;           // 頂点座標へのポインタ
	float                   m_fWidth;             // 1ブロック分の幅
	float                   m_fDepth;             // 1ブロック分の奥行
	int                     m_nXBlock;            // 横の分割数
	int                     m_nZBlock;            // 奥行の分割数
	int                     m_nNumVertex;         // 頂点数
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuffParent;     // 頂点バッファへのポインタ
	bool                    m_bReturnFlag;        // 当たった時に押し戻すかどうか
};


//*****************************************************************************
//    プレイヤーの攻撃用スフィアコライダークラスの定義
//****************************************************************************
class CPlayerAttackSphereCollider : public CSphereCollider
{
public:    // 誰でもアクセス可能
	CPlayerAttackSphereCollider(TYPE type = TYPE_SPHERE_PLAYERATTACK, CScene *pParent = NULL);
	~CPlayerAttackSphereCollider();

	static CPlayerAttackSphereCollider *Create(D3DXVECTOR3 pos, D3DXVECTOR3 scale, float fRadius, int nLife, int nPower);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetPower(const int nPower);
	void SetLife(const int nLife);

	int GetPower(void);
	int GetLife(void);

protected: // このクラスと派生クラスだけがアクセス可能
	void MakeColDispSphere(void);

private:   // このクラスだけがアクセス可能
	int m_nPower;  // 攻撃力
	int m_nLife;   // 寿命
};

//*****************************************************************************
//    敵の攻撃用スフィアコライダークラスの定義
//****************************************************************************
class CEnemyAttackSphereCollider : public CSphereCollider
{
public:    // 誰でもアクセス可能
	CEnemyAttackSphereCollider(TYPE type = TYPE_SPHERE_ENEMYATTACK, CScene *pParent = NULL);
	~CEnemyAttackSphereCollider();

	static CEnemyAttackSphereCollider *Create(D3DXVECTOR3 pos, D3DXVECTOR3 scale, float fRadius, int nLife, int nPower);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetPower(const int nPower);
	void SetLife(const int nLife);

	int GetPower(void);
	int GetLife(void);

protected: // このクラスと派生クラスだけがアクセス可能
	void MakeColDispSphere(void);

private:   // このクラスだけがアクセス可能
	int m_nPower;  // 攻撃力
	int m_nLife;   // 寿命
};

//*****************************************************************************
//    コライダー管轄クラスの定義
//*****************************************************************************
class CColliderManager
{
public:    // 誰でもアクセス可能
	CColliderManager();
	~CColliderManager();

	static CColliderManager *Create(int nNumCollider = 1);

	HRESULT Init(int nNumCollider = 1);
	void Uninit(void);

	void SetNumCollider(const int nNumCollider);
	void SetCollider(CCollider *pCollider, int nIdx);

	int GetNumCollider(void);
	CCollider **GetCollider(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	void MakeMemoryCollider(void);
	void ReleaseCollider(void);

	int       m_nNumCollider; // コライダーの数
	CCollider **m_pCollider;  // コライダークラスへのポインタ
};

#endif