//=============================================================================
//
// マウス入力処理 [inputmouce.h]
// Author : 佐藤 安純　[ SATO ASUMI ]
//
//=============================================================================
#ifndef _INPUTMOUCE_H_
#define _INPUTMOUCE_H_

#include "main.h"
#include "input.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define NUM_MOUSEMAX (8)

//=============================================================================
// キーボードの入力クラス
//=============================================================================
class CInputMouse : public CInput
{
public:
	typedef enum
	{
		BUTTON_LEFT = 0,
		BUTTON_RIGHT,
		BUTTON_WHEEL,
		BUTTON_BACK1,
		BUTTON_FRONT,
	}BUTTON;

	typedef struct
	{
		RECT    moveRect;     // 画面上で動ける範囲
		int     x;            // X座標
		int     y;            // Y座標
		bool    lButton;      // 左ボタン
		bool    rButton;      // 右ボタン
		bool    cButton;      // 真ん中ボタン
		int     moveAdd;      // 移動量
		RECT    imgRect;      // マウス用画像矩形
		int     imgWidth;     // マウス画像幅
		int     imgHeight;    // マウス画像高さ
	} MOUSE_INFO;

	CInputMouse();
	~CInputMouse();
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);
	bool GetPress(int nKey);
	bool GetTrigger(int nKey);
	bool GetRelease(int nKey);

	LONG GetIX(void);
	LONG GetIY(void);
	LONG GetIZ(void);

	void GetScreenToWorld(void);
	D3DXVECTOR3 GetWorldPos(void) { return m_WorldPos; };
	POINT GetScreenPos(void) { return m_ScreenPos; }
	DIMOUSESTATE2 GetMouseState(void) { return m_MouseState; }
private:
	static LPDIRECTINPUTDEVICE8 m_pMouse;
	MOUSE_INFO m_MouseInfo;					// マウス状態
	HWND m_hWnd;

	//マウスの情報
	DIMOUSESTATE2			m_MouseState;
	DIMOUSESTATE2			m_MouseStatePress;		// マウスの入力情報（プレス情報）
	DIMOUSESTATE2			m_MouseStateTrigger;	// マウスの入力情報（トリガー情報）
	DIMOUSESTATE2			m_MouseStateRelease;	// マウスの入力情報（リリース情報

	//ロングでやる
	LONG m_MouseIX;
	LONG m_MouseIY;
	LONG m_MouseIZ;

	//ワールド座標
	D3DXVECTOR3 m_WorldPos;		//ワールド座標
	POINT		m_ScreenPos;	//スクリーン座標
};

#endif