//=============================================================================
//
// レンダリング処理 [renderer.h]
// Author : 佐藤 安純　[ SATO ASUMI ]
//
//=============================================================================
#ifndef _RENDERER_H_
#define _RENDERER_H_

#include "main.h"
#include "modebase.h"

//=============================================================================
// 前方宣言
//=============================================================================
class CSky;

//=============================================================================
// レンダリングクラス
//=============================================================================
class CRenderer
{
public:
	CRenderer();
	~CRenderer();

	//メンバ関数
	HRESULT Init(HWND hWnd, bool bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(CModeBase *pModeBase);
	LPDIRECT3DDEVICE9 GetDevice(void) { return m_pD3DDevice; };

private:
	//メンバ変数
	LPDIRECT3D9				m_pD3D;				// Direct3Dオブジェクト
	LPDIRECT3DDEVICE9		m_pD3DDevice;		// Deviceオブジェクト(描画に必要)

	// 背景描画用
	HRESULT CreateBG(void);
	void ReleaseBG(void);

	LPDIRECT3DTEXTURE9 m_pTextureBg;			// 背景テクスチャへのポインタ
	CSky *m_pSky;								// 背景用球クラスへのポインタ
};
#endif