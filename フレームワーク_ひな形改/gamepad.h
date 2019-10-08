//=============================================================================
//
// ゲームパッド入力処理 [gamepad.h]
// Author : 佐藤 安純　[ SATO ASUMI ]
//
//=============================================================================
#ifndef _GAMEPAD_H_
#define _GAMEPAD_H_

#include "main.h"
#include "input.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define GAMEPAD_DEADZONE		(3)
#define MAX_ZONE				(1000)
#define	GAMEPAD_NUM_KEY_MAX		(128)		// キーの最大数
#define WARK_ZONE				(50.0f)		

#define BUTTON_X	   (ELECOM_X)		// Xボタンの番号
#define BUTTON_Y	   (ELECOM_Y)		// Yボタンの番号
#define BUTTON_A	   (ELECOM_A)		// Aボタンの番号
#define BUTTON_B	   (ELECOM_B)		// Bボタンの番号
#define BUTTON_L1	   (ELECOM_L1)		// L1ボタンの番号
#define BUTTON_R1	   (ELECOM_R1)		// R1ボタンの番号
#define BUTTON_L2	   (ELECOM_L2)		// L2ボタンの番号
#define BUTTON_R2	   (ELECOM_R2)		// R2ボタンの番号
#define BUTTON_L_STICK (ELECOM_L_STICK)	// L_STICKボタンの番号
#define BUTTON_R_STICK (ELECOM_R_STICK)	// R_STICKボタンの番号
#define BUTTON_BACK	   (ELECOM_BACK)	// BACKボタンの番号
#define BUTTON_START   (ELECOM_START)	// STARTボタンの番号
#define BUTTON_GUIDE   (ELECOM_GUIDE)	// GUIDEボタンの番号
#define BUTTON_MAX	   (ELECOM_MAX)		// ボタンの最大

//=============================================================================
// ゲームパッドの入力クラス
//=============================================================================
class CGamePad : public CInput
{
public:
	typedef enum
	{// アナログスティックの方向の種類(4方向)
		ANALOG_STICK_UP = 0,
		ANALOG_STICK_RIGHT,
		ANALOG_STICK_LEFT,
		ANALOG_STICK_DOWN,
		ANALOG_STICK_MAX
	}ANALOG_STICK;

	typedef enum
	{// 十字キーの方向の種類
		POV_UP = 0,
		POV_DOWN,
		POV_RIGHT,
		POV_LEFT,
		POV_MAX
	}POV;

	typedef enum
	{//ボタンの種類
		ELECOM_X = 0,
		ELECOM_Y,
		ELECOM_A,
		ELECOM_B,
		ELECOM_L1,
		ELECOM_R1,
		ELECOM_L2,
		ELECOM_R2,
		ELECOM_L_STICK,
		ELECOM_R_STICK,
		ELECOM_BACK,
		ELECOM_START,
		ELECOM_GUIDE,
		ELECOM_MAX
	}ELECOM_CTRL;

	CGamePad();
	~CGamePad();
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);
	bool GetGamePadPress(int nKey);
	bool GetGamePadTrigger(int nKey);
	bool GetGamePadRelease(int nKey);
	bool GetGamePadLeftStickPress(int nKey);
	bool GetGamePadLeftStickTrigger(int nKey);
	bool GetGamePadRightStickPress(int nKey);
	bool GetGamePadRightStickTrigger(int nKey);
	bool GetPovTpress(int nAngle);
	bool GetPovTtigger(int nAngle);
	LPDIRECTINPUTDEVICE8 GetDevGamepad(void) { return m_pDevGamePad; };		//入力デバイスの取得

	DIJOYSTATE2 m_aGamePad;	// コントローラーの情報

private:
	//関数
	static BOOL CALLBACK EnumJoysticksCallback(const DIDEVICEINSTANCE* pdidInstance, VOID* pContext);	//コールバック関数
	static BOOL CALLBACK EnumAxesCallback(LPCDIDEVICEOBJECTINSTANCE lpddoi, LPVOID pvRef);				//コールバック関数

	//変数
	static LPDIRECTINPUTDEVICE8 m_pDevGamePad;															// 入力デバイス(kコントローラー)へのポインタ
	DIDEVCAPS m_GamepadCaps;																		// ゲームパッドの機能情報


	BYTE		m_aGamePadKeyState[GAMEPAD_NUM_KEY_MAX];											// ジョイスティックのプレス入力情報
	BYTE		m_aGamePadKeyStateTrigger[GAMEPAD_NUM_KEY_MAX];									// ジョイスティックのトリガー入力情報
	BYTE		m_aGamePadKeyStateRelease[GAMEPAD_NUM_KEY_MAX];									// ジョイスティックのリリース入力情報
	BYTE		m_aLeftAnalogStick[ANALOG_STICK_MAX];												// 左アナログスティックのプレス情報
	BYTE		m_aLeftAnalogStickTrigger[ANALOG_STICK_MAX];										// 左アナログスティックのトリガー情報
	BYTE		m_aRightAnalogStick[ANALOG_STICK_MAX];												// 右アナログスティックのプレス情報
	BYTE		m_aRightAnalogStickTrigger[ANALOG_STICK_MAX];										// 右アナログスティックのトリガー情報
	BYTE		m_aPov[POV_MAX];																	// 十字キーのプレス情報
	BYTE		m_aPovTrigger[POV_MAX];																// 十字キーのトリガー情報

	int		   	    m_JoyconAxesNumber;		// joystickの機能コールバック関数で使うジョイコンの番号
};
#endif