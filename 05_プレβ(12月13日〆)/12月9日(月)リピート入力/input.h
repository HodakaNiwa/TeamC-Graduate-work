//=============================================================================
//
// 入力処理 [input.h]
// Author : 佐藤 安純　[ SATO ASUMI ]
//
//=============================================================================
#ifndef _INPUT_H_
#define _INPUT_H_

#include "main.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define MAX_KEY	(256)	//キーの最大数

#define NUM_BUTTON_MAX		(32)					// ボタンの最大数
#define MAX_CONTROLLERS (4)							// コントローラーの最大数
#define INPUT_DEADZONE  ( 0.44f * FLOAT(0x7FFF) )   // Default to 24% of the +/- 32767 range.   This is a reasonable default value but can be altered if needed.
#define XINPUT_STICK_MAX (32767)					// スティック最大値
#define XINPUT_STICK_MIN (-32768)					// スティック最小値

//=============================================================================
// インプットクラス
//=============================================================================
class CInput
{
public:
	CInput();
	virtual ~CInput();
	virtual HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	virtual void Uninit(void);
	virtual void Update(void) = 0;

protected:
	static LPDIRECTINPUT8 m_pInput;
	LPDIRECTINPUTDEVICE8 m_pDevice;
};

//=============================================================================
// キーボードの入力クラス
//=============================================================================
class CInputKeyboard : public CInput
{
public:
	CInputKeyboard();
	~CInputKeyboard();
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);
	bool GetKeyboardPress(int nKey);
	bool GetKeyboardTrigger(int nKey);
	bool GetKeyboardRelease(int nKey);
	bool GetKeyboardRepeat(int nKey);

private:
	BYTE m_aKeyState[MAX_KEY];
	BYTE m_aKeyStateTrigger[MAX_KEY];			//キーボードの入力情報(トリガー情報)
	BYTE m_aKeyStateRelease[MAX_KEY];			//キーボードの入力情報(リリース情報)
	BYTE m_aKeyStateRepeat[MAX_KEY];			//キーボードの入力情報(リピート情報)
	int m_aKeyRepeatCounter[MAX_KEY];			//キーボードのリピート用カウンター
};

//=============================================================================
// Xインプットの入力クラス
//=============================================================================
class CInputXPad
{// Xパッド
public:
	typedef enum
	{// ボタン以外のタイプ
		XPADOTHER_BUTTON = 0,										// ボタン
		XPADOTHER_TRIGGER_LEFT,										// Lトリガー
		XPADOTHER_TRIGGER_RIGHT,									// Rトリガー
		XPADOTHER_STICK_L_UP,										// Lスティック上
		XPADOTHER_STICK_L_LEFT,										// Lスティック左
		XPADOTHER_STICK_L_RIGHT,									// Lスティック右
		XPADOTHER_STICK_L_DOWN,										// Lスティック下
		XPADOTHER_STICK_R_UP,										// Rスティック上
		XPADOTHER_STICK_R_LEFT,										// Rスティック左
		XPADOTHER_STICK_R_RIGHT,									// Rスティック右
		XPADOTHER_STICK_R_DOWN,										// Rスティック下
		XPADOTHER_KEY_UP,										// Rスティック上
		XPADOTHER_KEY_LEFT,										// Rスティック左
		XPADOTHER_KEY_RIGHT,									// Rスティック右
		XPADOTHER_KEY_DOWN,										// Rスティック下
		XPADOTHER_MAX
	}XPADOTHER;

	typedef struct
	{// XINPUTコントローラー
		XINPUT_STATE state;											// 情報
		bool bConnected;											// 入力有無
	}CONTROLER_STATE;

	CInputXPad();													// コンストラクタ
	virtual ~CInputXPad();											// デストラクタ

	HRESULT Init(HINSTANCE hInstance, HWND hWnd);					// 初期化処理
	void Uninit(void);												// 終了処理
	void Update(void);												// 更新処理

	static int GetInputNum(void);									// 入力数取得

	bool GetALL(int nType, int nIdxPad);							// オール
	bool GetPress(int nButton, int nIdxPad);						// プレス
	bool GetPress(XPADOTHER nButton, int nIdxPad);					// プレス
	bool GetTrigger(int nButton, int nIdxPad);						// トリガー
	bool GetTrigger(XPADOTHER nButton, int nIdxPad);				// トリガー
	bool GetRelease(int nButton, int nIdxPad);						// リリース
	bool GetRelease(XPADOTHER nButton, int nIdxPad);				// リリース
	bool GetRepeat(int nButton, int nIdxPad);						// リピート
	bool GetRepeat(XPADOTHER nButton, int nIdxPad);					// リピート

	bool GetStick(int nLR, int nIdxPad);							// スティック
	float GetStickRot(int nLR, int nIdxPad);						// スティック向き

private:
	HRESULT UpdateControllerState(void);							// コントローラー入力数更新

	CONTROLER_STATE m_Controllers[MAX_CONTROLLERS];					// XINPUTコントローラー
	bool			m_bDeadZoneOn;									// スティック

	static int		m_nInputNum;									// 入力数

	WORD m_aJoyStatePress[MAX_CONTROLLERS][XPADOTHER_MAX];			// プレス
	WORD m_aJoyStateTrigger[MAX_CONTROLLERS][XPADOTHER_MAX];		// トリガー
	WORD m_aJoyStateRelease[MAX_CONTROLLERS][XPADOTHER_MAX];		// リリース
	WORD m_aJoyStateRepeat[MAX_CONTROLLERS][XPADOTHER_MAX];			// リピート
	int m_aJoyRepeatCounter[MAX_CONTROLLERS][XPADOTHER_MAX];		// リピート用カウンター

	float m_LStickRot[MAX_CONTROLLERS];								// Lスティック向き
	float m_RStickRot[MAX_CONTROLLERS];								// Rスティック向き
};
#endif