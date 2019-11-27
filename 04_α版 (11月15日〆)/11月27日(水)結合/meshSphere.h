//*****************************************************************************
//
//     メッシュスフィアの処理[meshSphere.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _MESHSPHERE_H_
#define _MESHSPHERE_H_

//*****************************************************************************
//    インクルードファイル
//*****************************************************************************
#include "scene.h"

//*****************************************************************************
//    メッシュスフィアクラスの定義
//*****************************************************************************
class CMeshSphere : public CScene
{
public:    // 誰でもアクセス可能
	CMeshSphere(int nPriority = 3, OBJTYPE objType = OBJTYPE_MESHSPHERE);
	~CMeshSphere();

	static CMeshSphere *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fRadius, int nXBlock, int nYBlock, int nPriority = 3);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	virtual void CalcNotScaleTransform(void);
	void SetTransformToDevice(const LPDIRECT3DDEVICE9 pDevice);
	virtual void MakeVertex(const LPDIRECT3DDEVICE9 pDevice);
	void MakeIndex(const LPDIRECT3DDEVICE9 pDevice);
	void SetVtxBuffValue(void);
	void SetVtxBuffPos(void);
	void SetVtxBuffCol(void);
	void SetVtxBuffNor(void);
	void SetVtxBuffTex(void);

	void BindTexture(const LPDIRECT3DTEXTURE9 pTexture);
	void SetVtxBuff(const LPDIRECT3DVERTEXBUFFER9 pVtxBuff);
	void SetIdxBuff(const LPDIRECT3DINDEXBUFFER9 pIdxBuff);
	void SetPos(const D3DXVECTOR3 pos);
	void SetRot(const D3DXVECTOR3 rot);
	void SetMtxWorld(const D3DXMATRIX mtxWorld);
	void SetCol(const D3DXCOLOR col);
	void SetRadius(const float fRadius);
	void SetXBlock(const int nXBlock);
	void SetYBlock(const int nYBlock);
	void Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size) {};

	LPDIRECT3DVERTEXBUFFER9 GetVtxBuff(void);
	LPDIRECT3DINDEXBUFFER9 GetIdxBuff(void);
	LPDIRECT3DTEXTURE9 GetTexture(void);
	D3DXVECTOR3 GetPos(void);
	D3DXVECTOR3 GetRot(void);
	D3DXMATRIX GetMtxWorld(void);
	D3DXCOLOR GetCol(void);
	float GetRadius(void);
	int GetXBlock(void);
	int GetYBlock(void);
	int GetNumVertex(void);
	int GetNumIndex(void);
	int GetNumPolygon(void);

protected: // このクラスと派生クラスだけがアクセス可能
	void ClearVariable(void);

private:   // このクラスだけがアクセス可能
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;      // 頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9  m_pIdxBuff;      // インデックスバッファへのポインタ
	LPDIRECT3DTEXTURE9		m_pTexture;	     // テクスチャへのポインタ
	D3DXVECTOR3             m_Pos;           // メッシュスフィアの座標
	D3DXVECTOR3             m_Rot;           // メッシュスフィアの向き
	D3DXMATRIX              m_MtxWorld;      // メッシュスフィアのワールドマトリックス
	D3DXCOLOR               m_Col;           // メッシュスフィアの色
	float                   m_fRadius;       // メッシュスフィアの半径
	int                     m_nXBlock;       // 横の分割数
	int                     m_nYBlock;       // 縦の分割数
	int                     m_nNumVertex;    // 頂点数
	int                     m_nNumIndex;     // インデックス数
	int                     m_nNumPolygon;   // ポリゴン数
};

#endif