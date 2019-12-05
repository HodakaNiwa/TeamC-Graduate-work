//=============================================================================
//
// メッシュフィールド処理 [sceneMeshfiled.h]
// Author : 佐藤安純 Sato_Asumi
//
//=============================================================================
#ifndef _SCENEMESHFILED_H_
#define _SCENEMESHFILED_H_

#include "main.h"
#include "scene.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define MAX_TEX	(1)

//=============================================================================
// メッシュフィールドクラス
//=============================================================================
class CSceneMeshFiled : public CScene
{
public:
	CSceneMeshFiled();
	~CSceneMeshFiled();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CSceneMeshFiled * Create(const D3DXVECTOR3 pos);
	static void Load(void);
	static void Unload(void);
	float GetHight(D3DXVECTOR3 pos);

#ifdef _DEBUG
	void SetHight(D3DXVECTOR3 pos, float fValue, float fRange);
#endif


private:
	//変数宣言
	void Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);
	void SaveData(void);
	void LoadData(void);
	void SetNor(void);		//法線を更新する

#ifdef _DEBUG
	void UpdatePrimitiv(void);
	void SetPosY(int nVertex);
#endif

	//メンバ変数
	static LPDIRECT3DTEXTURE9	m_pTexture;			// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuff;			//頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9		m_pIdxBuff;			// インデックスバッファへのポインタ
	D3DXMATRIX					m_mtxWorld;			//ワールドマトリックス
	D3DXVECTOR3					m_pos;				//中心座標
	D3DXVECTOR3					m_rot;				//向き
	int							m_nHight;			//楯列の数
	int							m_nWidth;			//横列の数
	int							m_nIndex;			//インデックス数
	int							m_nPolygon;			//ポリゴン数
	int							m_nPrimitiv;		//頂点数
	int							m_nSelectPrimitiv;	//現在選択されている頂点
	bool						m_bUpdatePrimitiv;	//頂点情報が更新されいるかどうか

	D3DXVECTOR3					m_LoadPos[3000];	//読み込み用の頂点

	D3DXVECTOR3 m_nor[2];
};
#endif
