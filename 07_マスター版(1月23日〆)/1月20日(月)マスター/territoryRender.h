//*****************************************************************************
//
//     テリトリー描画の処理[territoryRender.h]
//     Author:Hodaka Niwa
//
//*****************************************************************************
#ifndef _TERRITORYRENDER_H_
#define _TERRITORYRENDER_H_

//*****************************************************************************
//    インクルードファイル
//*****************************************************************************
#include "scene.h"

//*****************************************************************************
//    前方宣言
//*****************************************************************************
class CTerritory;
class CTerritoryShader;

//*****************************************************************************
//    テリトリー描画クラスの定義
//*****************************************************************************
class CTerritoryRender : public CScene
{
public:    // 誰でもアクセス可能
	CTerritoryRender(int nPriority = 3, OBJTYPE objType = OBJTYPE_TERRITORYRENDER);
	~CTerritoryRender();

	static CTerritoryRender *Create(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void BindModel(const LPD3DXMESH pMesh, const LPDIRECT3DTEXTURE9 pTexture);
	void Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size) {}

	//--------------------
	//	Set & Get
	//--------------------
	void SetVtxBuff(const LPDIRECT3DVERTEXBUFFER9 pVtxBuff) { m_pVtxBuff = pVtxBuff; }
	void SetWorldBuff(const LPDIRECT3DVERTEXBUFFER9 pWorldBuff) { m_pWorldBuff = pWorldBuff; }
	void SetColorBuff(const LPDIRECT3DVERTEXBUFFER9 pColorBuff) { m_pColorBuff = pColorBuff; }
	void SetIdxBuff(const LPDIRECT3DINDEXBUFFER9 pIdxBuff) { m_pIdxBuff = pIdxBuff; }
	void SetVtxBuff(IDirect3DVertexDeclaration9 *pDecl) { m_pDecl = pDecl; }
	void SetTerritoryShader(CTerritoryShader *pTerritoryShader) { m_pTerritoryShader = pTerritoryShader; }
	void BindTexture(const LPDIRECT3DTEXTURE9 pTexture) { m_pTexture = pTexture; }
	void SetNumVertex(const int nNumVertex) { m_nNumVertex = nNumVertex; }
	void SetNumPolygon(const int nNumPolygon) { m_nNumPolygon = nNumPolygon; }
	void SetNumInstance(const int nNumInstance) { m_nNumInstance = nNumInstance; }
	void SetDisp(const bool bDisp) { m_bDisp = bDisp; }
	void SetLighting(const bool bLighting) { m_bLighting = bLighting; }

	LPDIRECT3DVERTEXBUFFER9 GetVtxBuff(void) { return m_pVtxBuff; }
	LPDIRECT3DVERTEXBUFFER9 GetWorldBuff(void) { return m_pWorldBuff; }
	LPDIRECT3DVERTEXBUFFER9 GetColorBuff(void) { return m_pColorBuff; }
	LPDIRECT3DINDEXBUFFER9 GetIdxBuff(void) { return m_pIdxBuff; }
	IDirect3DVertexDeclaration9 *GetDecl(void) { return m_pDecl; }
	CTerritoryShader *GetTerritoryShader(void) { return m_pTerritoryShader; }
	LPDIRECT3DTEXTURE9 GetTexture(void) { return m_pTexture; }
	int GetNumVertex(void) { return m_nNumVertex; }
	int GetNumPolygon(void) { return m_nNumPolygon; }
	int GetNumInstance(void) { return m_nNumInstance; }
	bool GetDisp(void) { return m_bDisp; }
	bool GetLighting(void) { return m_bLighting; }

	//--------------------
	//	頂点データの構造体
	//--------------------
	typedef struct
	{
		D3DXVECTOR3 pos;
		D3DXVECTOR3 nor;
		D3DXVECTOR2 tex;
	}VERTEXDATA;

	typedef struct
	{
		float m1[4];
		float m2[4];
		float m3[4];
		float m4[4];
	}WORLDMATRIX;

	typedef struct
	{
		D3DCOLOR col;
	}COLORDATA;

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	//--------------------
	//	関数
	//--------------------
	HRESULT MakeWorldBuff(const LPDIRECT3DDEVICE9 pDevice);
	HRESULT MakeColorBuff(const LPDIRECT3DDEVICE9 pDevice);
	HRESULT MakeDeclaration(const LPDIRECT3DDEVICE9 pDevice);
	HRESULT LoadShader(void);
	void AddList(CTerritory *pTerritory, WORLDMATRIX *pWorld, COLORDATA *pCol);
	void AddWorld(CTerritory *pTerritory, WORLDMATRIX *pWorld);
	void AddColor(CTerritory *pTerritory, COLORDATA *pCol);

	//--------------------
	//	メンバ変数
	//--------------------
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;			// 頂点バッファへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pWorldBuff;		// ワールドマトリックスバッファへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pColorBuff;		// 頂点カラーバッファへのポインタ
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff;			// インデックスバッファへのポインタ
	IDirect3DVertexDeclaration9 *m_pDecl;		// 頂点宣言データへのポインタ
	CTerritoryShader *m_pTerritoryShader;		// テリトリーシェーダークラスへのポインタ
	LPDIRECT3DTEXTURE9 m_pTexture;				// テクスチャへのポインタ
	int m_nNumVertex;							// 頂点数
	int m_nNumPolygon;							// ポリゴン数
	int m_nNumInstance;							// インスタンス数
	bool m_bDisp;								// 描画するかしないか
	bool m_bLighting;							// ライティングするかどうか
};

#endif