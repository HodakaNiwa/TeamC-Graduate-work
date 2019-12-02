//*****************************************************************************
//
//     エフェクト(3D)の処理[effect3D.h]
//     Author:Hodaka Niwa
//
//*****************************************************************************
#ifndef _EFFECT3D_H_
#define _EFFECT3D_H_

//*****************************************************************************
//    インクルードファイル
//*****************************************************************************
#include "scene.h"

//*****************************************************************************
//    エフェクト(3D)クラスの定義
//*****************************************************************************
class CEffect3D : public CScene
{
public:		// 誰でもアクセス可能
	CEffect3D(int nPriority = 3, OBJTYPE objType = OBJTYPE_EFFECT3D);
	~CEffect3D();

	static CEffect3D *Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, int nLife, float fCutAlpha, float fCutLength, bool bLighting = false, int nPriority = 3, float fRot = 0.0f, float fTexU = 0.0f, float fTexV = 0.0f, float fTexWidth = 1.0f, float fTexHeight = 1.0f);
	static HRESULT Load(void);
	static void UnLoad(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size) {}
	virtual void MakeVertex(const LPDIRECT3DDEVICE9 pDevice);
	virtual void CalcNotScaleTransform(const LPDIRECT3DDEVICE9 pDevice);
	void SetTransformToDevice(const LPDIRECT3DDEVICE9 pDevice);
	void SetScale(float fWidth, float fHeight);
	virtual void SetVtxBuffValue(void);
	virtual void SetVtxBuffPos(void);
	void SetVtxBuffNor(void);
	void SetVtxBuffCol(void);
	void SetVtxBuffTex(void);


	//------------------------
	//  Set & Get
	//------------------------
	void SetVtxBuff(const LPDIRECT3DVERTEXBUFFER9 pVtxBuff) { m_pVtxBuff = pVtxBuff; }
	void SetPos(const D3DXVECTOR3 pos) { m_Pos = pos; }
	void SetCol(const D3DXCOLOR col) { m_Col = col; }
	void SetRot(const float fRot) { m_fRot = fRot; }
	void SetWidth(const float fWidth) { m_fWidth = fWidth; }
	void SetHeight(const float fHeight) { m_fHeight = fHeight; }
	void SetAngle(const float fAngle) { m_fAngle = fAngle; }
	void SetLength(const float fLength) { m_fLength = fLength; }
	void SetTexU(const float fTexU) { m_fTexU = fTexU; }
	void SetTexV(const float fTexV) { m_fTexV = fTexV; }
	void SetTexWidth(const float fTexWidth) { m_fTexWidth = fTexWidth; }
	void SetTexHeight(const float fTexHeight) { m_fTexHeight = fTexHeight; }
	void SetMtxWorld(const D3DXMATRIX mtxWorld) { m_MtxWorld = mtxWorld; }
	void SetLighting(const bool bLighting) { m_bLighting = bLighting; }
	void SetLife(const int nLife) { m_nLife = nLife; }
	void SetCutAlpha(const float fCutAlpha) { m_fCutAlpha = fCutAlpha; }
	void SetCutLength(const float fCutLength) { m_fCutLength = fCutLength; }

	LPDIRECT3DVERTEXBUFFER9 GetVtxBuff(void) { return m_pVtxBuff; }
	D3DXVECTOR3 GetPos(void) { return m_Pos; }
	D3DXCOLOR GetCol(void) { return m_Col; }
	float GetRot(void) { return m_fRot; }
	float GetWidth(void) { return m_fWidth; }
	float GetHeight(void) { return m_fHeight; }
	float GetAngle(void) { return m_fAngle; }
	float GetLength(void) { return m_fLength; }
	float GetTexU(void) { return m_fTexU; }
	float GetTexV(void) { return m_fTexV; }
	float GetTexWidth(void) { return m_fTexWidth; }
	float GetTexHeight(void) { return m_fTexHeight; }
	D3DXMATRIX GetMtxWorld(void) { return m_MtxWorld; }
	bool GetLighting(void) { return m_bLighting; }
	int GetLife(void) { return m_nLife; }
	float GetCutAlpha(void) { return m_fCutAlpha; }
	float GetCutLength(void) { return m_fCutLength; }

protected:	// このクラスと派生クラスだけがアクセス可能
	void ClearVariable(void);

private:	// このクラスだけがアクセス可能
	//------------------------
	//  関数
	//------------------------
	void SetValue(void);

	//------------------------
	//  静的メンバ変数
	//------------------------
	static LPDIRECT3DTEXTURE9 m_pTexture;

	//------------------------
	//  メンバ変数
	//------------------------
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// 頂点バッファへのポインタ
	D3DXVECTOR3             m_Pos;			// ポリゴンの座標
	D3DXCOLOR               m_Col;			// ポリゴンの色
	float                   m_fRot;			// ポリゴンの向き
	float                   m_fWidth;		// ポリゴンの幅
	float                   m_fHeight;		// ポリゴンの高さ
	float                   m_fAngle;		// ポリゴンを出す角度
	float                   m_fLength;		// ポリゴンの長さ
	float                   m_fTexU;		// ポリゴンの左上テクスチャU座標
	float                   m_fTexV;		// ポリゴンの左上テクスチャV座標
	float                   m_fTexWidth;	// ポリゴンのテクスチャ座標の横幅
	float                   m_fTexHeight;	// ポリゴンのテクスチャ座標の縦幅
	D3DXMATRIX              m_MtxWorld;		// ワールドマトリックス
	bool                    m_bLighting;	// ライティングするかしないか
	int                     m_nLife;		// 寿命
	float                   m_fCutAlpha;	// 透明度を削る量
	float                   m_fCutLength;	// 大きさを削る量
};

#endif