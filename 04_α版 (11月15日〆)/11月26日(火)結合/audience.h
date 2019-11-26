//*****************************************************************************
//
//     インスタンシングを利用した観客の処理[audience.h]
//     Author:Hodaka Niwa
//
//*****************************************************************************
#ifndef _AUDIENCE_H_
#define _AUDIENCE_H_

//*****************************************************************************
//     インクルードファイル
//*****************************************************************************
#include "scene.h"

//*****************************************************************************
//     前方宣言
//*****************************************************************************
class CInstancingShader;

//*****************************************************************************
//    観客クラスの定義
//*****************************************************************************
class CAudience : public CScene
{
public:    // 誰でもアクセス可能
	typedef enum
	{
		PARTS_BODY = 0,
		PARTS_HEAD,
		PARTS_LEFT_HAND,
		PARTS_RIGHT_HAND,
		PARTS_MAX
	}PARTS;

	CAudience(int nPriority = 3, OBJTYPE objType = OBJTYPE_NONE);
	~CAudience();

	static CAudience *Create(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void  Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
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

	void LoadInstanceDataNum(char *pStr);
	void LoadInstanceData(char *pStr, D3DXVECTOR3 *pPos, D3DXVECTOR3 *pRot, D3DXVECTOR3 *pScale, D3DXCOLOR *pCol);
	void MakeWorldBuff(const LPDIRECT3DDEVICE9 pDevice, D3DXVECTOR3 *pPos, D3DXVECTOR3 *pRot, D3DXVECTOR3 *pScale);
	void MakeDeclaration(const LPDIRECT3DDEVICE9 pDevice);
	void LoadShader(void);

	void MakeBody(const LPDIRECT3DDEVICE9 pDevice, D3DXCOLOR *pCol);
	void MakeVertexBody(const LPDIRECT3DDEVICE9 pDevice);
	void MakeIndexBody(const LPDIRECT3DDEVICE9 pDevice);
	void MakeColorBuffBody(const LPDIRECT3DDEVICE9 pDevice, D3DXCOLOR *pCol);
	void MakeMtxLocalBody(void);

	void MakeHead(const LPDIRECT3DDEVICE9 pDevice);
	void MakeVertexHead(const LPDIRECT3DDEVICE9 pDevice);
	void MakeColorBuffHead(const LPDIRECT3DDEVICE9 pDevice);
	void MakeMtxLocalHead(void);

	void MakeLeftHand(const LPDIRECT3DDEVICE9 pDevice);
	void MakeVertexLeftHand(const LPDIRECT3DDEVICE9 pDevice);
	void MakeColorBuffLeftHand(const LPDIRECT3DDEVICE9 pDevice);
	void MakeMtxLocalLeftHand(void);

	void MakeRightHand(const LPDIRECT3DDEVICE9 pDevice);
	void MakeVertexRightHand(const LPDIRECT3DDEVICE9 pDevice);
	void MakeColorBuffRightHand(const LPDIRECT3DDEVICE9 pDevice);
	void MakeMtxLocalRightHand(void);

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff[PARTS_MAX];		// 頂点データへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pColorBuff[PARTS_MAX];	// 頂点カラーへのポインタ
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff[PARTS_MAX];		// インデックスバッファへのポインタ
	IDirect3DVertexDeclaration9 *m_pDecl;				// 頂点宣言データへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pWorldBuff;				// ワールドマトリックスへのポインタ
	CInstancingShader *m_pInsShader;					// インスタンシングシェーダークラスへのポインタ
	D3DXMATRIX m_MtxLocal[PARTS_MAX];					// ローカル座標におけるワールドマトリックス
	int m_nNumInstance;									// インスタンス数
	int m_nNumVertex[PARTS_MAX];						// 頂点数
	int m_nNumIndex[PARTS_MAX];							// インデックス数
	int m_nNumPolygon[PARTS_MAX];						// ポリゴン数
};

#endif