//*****************************************************************************
//
//     メッシュリングの処理[meshRing.h]
//     Author:Hodaka Niwa
//
//*****************************************************************************
#ifndef _MESHRING_H_
#define _MESHRING_H_

//*****************************************************************************
//    インクルードファイル
//*****************************************************************************
#include "scene.h"

//*****************************************************************************
//    メッシュリングクラスの定義
//*****************************************************************************
class CMeshRing : public CScene
{
public:    // 誰でもアクセス可能
	CMeshRing(int nPriority = 3, OBJTYPE objType = OBJTYPE_MESHRING);
	~CMeshRing();

	static CMeshRing *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fHeight, float fRadius, float fDiffusion, int nXBlock, int nYBlock, int nPriority = 3);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size) {}
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
	void SetHeight(const float fHeight);
	void SetRadius(const float fRadius);
	void SetDiffusion(const float fDiffusion);
	void SetXBlock(const int nXBlock);
	void SetYBlock(const int nYBlock);

	LPDIRECT3DVERTEXBUFFER9 GetVtxBuff(void);
	LPDIRECT3DINDEXBUFFER9 GetIdxBuff(void);
	LPDIRECT3DTEXTURE9 GetTexture(void);
	D3DXVECTOR3 GetPos(void);
	D3DXVECTOR3 GetRot(void);
	D3DXMATRIX GetMtxWorld(void);
	D3DXCOLOR GetCol(void);
	float GetHeight(void);
	float GetRadius(void);
	float GetDiffusion(void);
	int GetXBlock(void);
	int GetYBlock(void);
	int GetNumVertex(void);
	int GetNumIndex(void);
	int GetNumPolygon(void);

protected: // このクラスと派生クラスだけがアクセス可能
	void ClearVariable(void);

private:   // このクラスだけがアクセス可能
	void CalcNotScaleTransform(void);
	void SetTransformToDevice(const LPDIRECT3DDEVICE9 pDevice);

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// 頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9  m_pIdxBuff;		// インデックスバッファへのポインタ
	LPDIRECT3DTEXTURE9		m_pTexture;		// テクスチャへのポインタ
	D3DXVECTOR3             m_Pos;			// メッシュリングの位置
	D3DXVECTOR3             m_Rot;			// メッシュリングの向き
	D3DXMATRIX              m_MtxWorld;		// メッシュリングのワールドマトリックス
	D3DXCOLOR               m_Col;			// メッシュリングの色
	float                   m_fHeight;		// メッシュリングの高さ
	float                   m_fRadius;		// メッシュリングの半径
	float                   m_fDiffusion;	// メッシュリングの拡散値
	int                     m_nXBlock;		// 横の分割数
	int                     m_nYBlock;		// 縦の分割数
	int                     m_nNumVertex;	// 頂点数
	int                     m_nNumIndex;	// インデックス数
	int                     m_nNumPolygon;	// ポリゴン数
};

#endif