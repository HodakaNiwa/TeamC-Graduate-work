#pragma once
//=============================================================================
//
// ミニマップの処理 [minimap.h]
// Author : 山下敦史
//
//=============================================================================
#ifndef _MINIMAP_H_
#define _MINIMAP_H_

#include "main.h"
#include "scene.h"

//前方宣言
class CScene2D;

//=============================================================================
// ミニマップクラス
//=============================================================================
class CMiniMap
{
public:
	CMiniMap(int nPriority, CScene::OBJTYPE type);
	CMiniMap();
	~CMiniMap();

	//メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CMiniMap * Create(void);
private:
	//メンバ変数
	LPDIRECT3DTEXTURE9			m_pTexture;		// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuff;		// 頂点バッファへのポインタ
	D3DXVECTOR3					m_pos;			// ポリゴンの位置
	D3DXVECTOR3					m_Size;			// ポリゴンのサイズ
	D3DXCOLOR					m_Color;		// 色
	CScene2D                   *m_pScene2D;
};

#endif