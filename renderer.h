//=============================================================================
//
// レンダリング処理 [renderer.h]
// Author : 佐藤 安純　[ SATO ASUMI ]
//
//=============================================================================
#ifndef _RENDERER_H_
#define _RENDERER_H_

#include "main.h"

//=============================================================================
// 前方宣言
//=============================================================================
class CDebugProc;
class CDebugWindow;

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
	void Draw(void);
	LPDIRECT3DDEVICE9 GetDevice(void) { return m_pD3DDevice; };

	static CDebugProc * GetDebugLeft(void);								//デバック表示ポインタの取得
	static CDebugProc * GetDebugRight(void) { return m_pDebugRight;};	//デバック表示ポインタの取得
	static bool GetDrawDebugRight(void) { return m_bDrawDebugRight;};	//表示状態を取得

private:
	//メンバ変数
	LPDIRECT3D9				m_pD3D;				// Direct3Dオブジェクト
	LPDIRECT3DDEVICE9		m_pD3DDevice;		// Deviceオブジェクト(描画に必要)

	static CDebugProc * m_pDebugLeft;
	static CDebugProc * m_pDebugRight;
	static bool m_bDrawDebugLeft;				//デバックを表示しているかどうか
	static bool m_bDrawDebugRight;				//デバックを表示しているかどうか
	static bool m_bWireFram;					//ワイヤーフレームを表示する
	CDebugWindow *	m_pDebugWindow;				// デバッグウインドウ
};
#endif