#pragma once
//=============================================================================
//
// ミニマップの処理 [minimap.h]
// Author : 山下敦史
//
//=============================================================================
#ifndef _TIMERLOGO_H_
#define _TIMERLOGO_H_

#include "main.h"
#include "scene.h"
#include "scene2D.h"

//前方宣言
class CScene2D;

//=============================================================================
// ミニマップクラス
//=============================================================================
class CTimerLogo : public CScene2D
{
public:
	typedef enum
	{
		TIMERLOGO_START = 0,
		TIMERLOGO_2MIN,
		TIMERLOGO_1MIN,
		TIMERLOGO_30SEC,
		TIMERLOGO_FIN,
		TIMERLOGO_MAX
	}TIMERLOGO;

	CTimerLogo(int nPriority, CScene::OBJTYPE type);
	CTimerLogo();
	~CTimerLogo();

	//メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CTimerLogo * Create(const D3DXVECTOR3 pos,const D3DXVECTOR3 size,TIMERLOGO timerlogo);

	static HRESULT Load(void);							//テクスチャ読み込み
	static void Unload(void);
private:
	//メンバ変数
	static LPDIRECT3DTEXTURE9			m_pTexture[TIMERLOGO_MAX];		// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuff;		// 頂点バッファへのポインタ
	D3DXVECTOR3					m_pos;			// ポリゴンの位置
	D3DXVECTOR3					m_Size;			// ポリゴンのサイズ
	D3DXCOLOR					m_Color;		// 色
	TIMERLOGO					m_TimerLogo;
};

#endif