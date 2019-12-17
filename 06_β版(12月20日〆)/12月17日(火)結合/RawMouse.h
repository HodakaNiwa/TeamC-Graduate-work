//*****************************************************************************
//
//     RawInputを用いたマウス複数化の処理[RawMouse.h]
//      Author:Hodaka Niwa
//
//*****************************************************************************
#ifndef _RAWMOUSE_H_
#define _RAWMOUSE_H_

//*****************************************************************************
//    マクロ定義
//*****************************************************************************
#define RAWINPUT_DEVMOUSE_MAX (4)

//*****************************************************************************
//    インクルードファイル
//*****************************************************************************
#include "main.h"

//*****************************************************************************
//    RawMouseクラスの定義
//*****************************************************************************
class CRawMouse
{
public:    // 誰でもアクセス可能
	//------------------------
	//  ボタンの種類
	//------------------------
	typedef enum
	{
		// ボタン
		RIMS_BUTTON_LEFT,
		RIMS_BUTTON_RIGHT,
		RIMS_BUTTON_MIDDLE,

		// スライド
		RIMS_SLIDE_UP,
		RIMS_SLIDE_DOWN,
		RIMS_SLIDE_LEFT,
		RIMS_SLIDE_RIGHT,

		RIMS_BUTTON_MAX
	}RIMS_BUTTON;

	~CRawMouse();

	static CRawMouse *Create(HWND hWnd);
	static CRawMouse *ReCreate(HWND hWnd);
	static HRESULT RegisterRawInput(HWND hWnd);

	HRESULT Init(HWND hWnd);
	void Uninit(void);
	void Update(LPARAM lParam);
	void AdjustMouseData(void);
	void ClearMouseData(void);

	UINT GetMouseNum(void);
	HANDLE GetMouseHandle(int nIdx);
	RAWMOUSE GetMouse(int nIdx);

	bool GetPress(int nIdx, int nButton);
	bool GetTrigger(int nIdx, int nButton);
	bool GetRelease(int nIdx, int nButton);
	bool GetRepeat(int nIdx, int nButton);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	CRawMouse();
	void ArrayMouseData(RAWINPUT *raw, int nCntMouse);
	void AdjustMouseData_Trigger(int nCntMouse, int nCntButton);
	void AdjustMouseData_Release(int nCntMouse, int nCntButton);
	void AdjustMouseData_Repeat(int nCntMouse, int nCntButton);

	static CRawMouse *m_pInstance;										// RawMouseクラスのインスタンス
	UINT m_nDeviceNum;													// 現在接続されているデバイスの数
	UINT m_nMouseNum;													// 現在接続されているマウスの数
	HANDLE m_MouseHandle[RAWINPUT_DEVMOUSE_MAX];						// 接続されているマウスへのハンドル
	RAWINPUTHEADER m_MouseHeader[RAWINPUT_DEVMOUSE_MAX];				// マウスのヘッダー(詳細情報)を格納する
	RAWMOUSE m_Mouse[RAWINPUT_DEVMOUSE_MAX];							// マウスの情報を格納する
	HWND m_hWnd;														// ウィンドウハンドル保存用
	BYTE m_aMousePress[RAWINPUT_DEVMOUSE_MAX][RIMS_BUTTON_MAX];			// 現在マウスのボタンが押されたかどうか(プレス情報)
	BYTE m_aMouseTrigger[RAWINPUT_DEVMOUSE_MAX][RIMS_BUTTON_MAX];		// マウスのボタンが押されたかどうか(トリガー情報)
	BYTE m_aMouseRelease[RAWINPUT_DEVMOUSE_MAX][RIMS_BUTTON_MAX];		// マウスのボタンが押されたかどうか(リリース情報)
	BYTE m_aMouseRepeat[RAWINPUT_DEVMOUSE_MAX][RIMS_BUTTON_MAX];		// マウスのボタンが押されたかどうか(リピート情報)
	DWORD m_aMouseCounter[RAWINPUT_DEVMOUSE_MAX][RIMS_BUTTON_MAX];		// マウスが入力されてる間を数えるカウンター
	bool m_bMouseTrigger[RAWINPUT_DEVMOUSE_MAX][RIMS_BUTTON_MAX];		// マウスのトリガー情報を起動したかどうか
	bool m_bMouseRelease[RAWINPUT_DEVMOUSE_MAX][RIMS_BUTTON_MAX];		// マウスのリリース情報を起動したかどうか
};

#endif