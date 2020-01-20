//=============================================================================
//
// 三角形ポリゴン3Dの処理 [sceneTriangle3D.h]
// Author : 佐藤安純 Sato_Asumi
//
//=============================================================================
#ifndef _SCENE_TRAIANGLE3D_H_
#define _SCENE_TRAIANGLE3D_H_

#include "main.h"
#include "scene.h"

//=============================================================================
// 3Dポリゴンクラス
//=============================================================================
class CSceneTriangle3D : public CScene
{
public:
	CSceneTriangle3D();
	~CSceneTriangle3D();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CSceneTriangle3D * Create(D3DXVECTOR3 Pos1, D3DXVECTOR3 Pos2, D3DXVECTOR3 Pos3);

	void Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);
	void SetPos(D3DXVECTOR3 Pos1, D3DXVECTOR3 Pos2, D3DXVECTOR3 Pos3);
	void SetColor(D3DXCOLOR col);
	LPDIRECT3DVERTEXBUFFER9 GetVtxBuff(void) { return m_pVtxBuff; }	//バッファの取得

private:
	void SetWorldMatrix(void);

	//メンバ変数
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;								//頂点バッファへのポインタ
	D3DXMATRIX				m_mtxWorld;								//ワールドマトリックス
	int m_nLife;													//表示時間
	D3DXCOLOR m_col;												//色
};
#endif
