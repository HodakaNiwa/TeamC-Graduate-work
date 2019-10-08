//=============================================================================
//
// ゲームパッド入力処理 [input.cpp]
// Author : 佐藤安純　SATOU ASUMI
//
//=============================================================================
#include "gamepad.h"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
LPDIRECTINPUTDEVICE8 CGamePad::m_pDevGamePad = NULL;

//=============================================================================
// コンストラクタ
//=============================================================================
CGamePad::CGamePad()
{
	//各キーの初期化処理
	for (int nCnt = 0; nCnt < GAMEPAD_NUM_KEY_MAX; nCnt++)
	{
		m_aGamePadKeyState[nCnt] = '\0';
		m_aGamePadKeyStateTrigger[nCnt] = '\0';
		m_aGamePadKeyStateRelease[nCnt] = '\0';
	}
	for (int nCnt = 0; nCnt < ANALOG_STICK_MAX; nCnt++)
	{
		m_aLeftAnalogStick[nCnt] = '\0';
		m_aLeftAnalogStickTrigger[nCnt] = '\0';
	}
	for (int nCnt = 0; nCnt < POV_MAX; nCnt++)
	{
		m_aPov[nCnt] = '\0';
		m_aPovTrigger[nCnt] = '\0';
	}
}

//=============================================================================
// デストラクタ
//=============================================================================
CGamePad::~CGamePad()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CGamePad::Init(HINSTANCE hInstance, HWND hWnd)
{
	//変数宣言
	HRESULT hr;

	//初期化処理を呼び出す
	CInput::Init(hInstance, hWnd);

	// 入力デバイス(コントローラー)の生成
	if (FAILED(m_pInput->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumJoysticksCallback, NULL, DIEDFL_ATTACHEDONLY)))		// 第4引数 (ATTACHEDONLY = 現在接続されているジョイスティックのみ検出する)
	{
		return FALSE;
	}

	// デバイスにデータフォーマットを設定
	if (m_pDevGamePad != NULL)
	{
		if (FAILED(hr = m_pDevGamePad->SetDataFormat(&c_dfDIJoystick2)))
		{
			return FALSE;
		}

		// 協調モードを設定
		if (FAILED(hr = m_pDevGamePad->SetCooperativeLevel(hWnd, DISCL_EXCLUSIVE | DISCL_FOREGROUND)))
		{
			return FALSE;
		}

		// ゲームパッドの機能の情報を収集
		m_GamepadCaps.dwSize = sizeof(DIDEVCAPS);

		if (FAILED(hr = m_pDevGamePad->GetCapabilities(&m_GamepadCaps)))
		{
			m_pDevGamePad->Release();
			return FALSE;
		}

		// 各軸のモード設定
		hr = m_pDevGamePad->EnumObjects(EnumAxesCallback, (void*)m_pDevGamePad, DIDFT_AXIS);

		m_JoyconAxesNumber++;		// 番号を進める

		// ゲームパッドへのアクセス権を獲得(入力制御開始)
		hr = m_pDevGamePad->Poll();

		if (FAILED(hr))
		{
			hr = m_pDevGamePad->Acquire();

			while (hr == DIERR_INPUTLOST)
			{
				hr = m_pDevGamePad->Acquire();
			}
		}
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CGamePad::Uninit(void)
{
	CInput::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CGamePad::Update(void)
{
	if (m_pDevGamePad != NULL)
	{
		DIJOYSTATE2 aGamePadKeyState;								// コントローラーの入力情報
		BYTE aLeftAnalogStick[ANALOG_STICK_MAX] = { 0,0,0,0 };		// スティックの入力情報
		BYTE aRightAnalogStick[ANALOG_STICK_MAX] = { 0,0,0,0 };		// 右スティックの入力情報
		BYTE aPov[POV_MAX] = { 0,0,0,0 };							// 十字キーの入力情報

		HRESULT hr;
		hr = m_pDevGamePad->Poll();

		// 入力デバイスからデータを取得
		if (SUCCEEDED(hr = m_pDevGamePad->GetDeviceState(sizeof(DIJOYSTATE2), &aGamePadKeyState)))
		{
			// ボタン
			for (int nCntKey = 0; nCntKey < GAMEPAD_NUM_KEY_MAX; nCntKey++)
			{
				// コントローラーの入力情報の保存(トリガー)
				m_aGamePadKeyStateTrigger[nCntKey] = (m_aGamePadKeyState[nCntKey] ^ aGamePadKeyState.rgbButtons[nCntKey]) & aGamePadKeyState.rgbButtons[nCntKey];
				// コントローラーの入力情報の保存(リリース)
				m_aGamePadKeyStateRelease[nCntKey] = (aGamePadKeyState.rgbButtons[nCntKey] ^ m_aGamePadKeyState[nCntKey]) & m_aGamePadKeyState[nCntKey];
				// コントローラーの入力情報の保存(プレス)
				m_aGamePadKeyState[nCntKey] = aGamePadKeyState.rgbButtons[nCntKey];
			}

			//--------------------------------------------------------
			// 左アナログスティックの入力情報の取得
			//--------------------------------------------------------
			if (aGamePadKeyState.lY < -GAMEPAD_DEADZONE)
			{// 左スティックを上に倒した
				aLeftAnalogStick[ANALOG_STICK_UP] = 128;			// 入力された
			}
			else if (aGamePadKeyState.lY > GAMEPAD_DEADZONE)
			{// 左スティックを下に倒した
				aLeftAnalogStick[ANALOG_STICK_DOWN] = 128;			// 入力された
			}
			else if (aGamePadKeyState.lX > GAMEPAD_DEADZONE)
			{// 左スティックを右に倒した
				aLeftAnalogStick[ANALOG_STICK_RIGHT] = 128;			// 入力された
			}
			else if (aGamePadKeyState.lX < -GAMEPAD_DEADZONE)
			{// 左スティックを左に倒した
				aLeftAnalogStick[ANALOG_STICK_LEFT] = 128;			// 入力された
			}

			for (int nCntAnalogStick = 0; nCntAnalogStick < ANALOG_STICK_MAX; nCntAnalogStick++)
			{// 左アナログスティックの方向の種類分ループ
			 // 左スティックのトリガー情報
				m_aLeftAnalogStickTrigger[nCntAnalogStick] = (m_aLeftAnalogStick[nCntAnalogStick] ^ aLeftAnalogStick[nCntAnalogStick])&aLeftAnalogStick[nCntAnalogStick];		// 左アナログスティックのトリガー情報
																																																// 左スティックのプレス情報
				m_aLeftAnalogStick[nCntAnalogStick] = aLeftAnalogStick[nCntAnalogStick];																						// 左アナログスティックのプレス情報
			}

			//--------------------------------------------------------
			// 右アナログスティックの入力情報の取得
			//--------------------------------------------------------
			if (aGamePadKeyState.lRz < -GAMEPAD_DEADZONE)
			{// 右スティックを上に倒した
				aRightAnalogStick[ANALOG_STICK_UP] = 128;			// 入力された
			}
			else if (aGamePadKeyState.lRz > GAMEPAD_DEADZONE)
			{// 右スティックを下に倒した
				aRightAnalogStick[ANALOG_STICK_DOWN] = 128;			// 入力された
			}
			else if (aGamePadKeyState.lZ > GAMEPAD_DEADZONE)
			{// 右スティックを右に倒した
				aRightAnalogStick[ANALOG_STICK_RIGHT] = 128;			// 入力された
			}
			else if (aGamePadKeyState.lZ < -GAMEPAD_DEADZONE)
			{// 右スティックを左に倒した
				aRightAnalogStick[ANALOG_STICK_LEFT] = 128;			// 入力された
			}

			for (int nCntAnalogStick = 0; nCntAnalogStick < ANALOG_STICK_MAX; nCntAnalogStick++)
			{// 右アナログスティックの方向の種類分ループ
			 // 右スティックのトリガー情報
				m_aRightAnalogStickTrigger[nCntAnalogStick] = (m_aRightAnalogStick[nCntAnalogStick] ^ aRightAnalogStick[nCntAnalogStick])&aRightAnalogStick[nCntAnalogStick];		// 左アナログスティックのトリガー情報
																																												// 左スティックのプレス情報
				m_aRightAnalogStick[nCntAnalogStick] = aRightAnalogStick[nCntAnalogStick];																						// 左アナログスティックのプレス情報
			}

			//--------------------------------------------------------
			// 十字キーの入力情報の取得
			//--------------------------------------------------------
			if (aGamePadKeyState.rgdwPOV[0] == 0.0f)
			{// 十字キーの上を押した
				aPov[POV_UP] = 128;			// 入力された
			}
			if (aGamePadKeyState.rgdwPOV[0] == 18000.0f)
			{// 十字キーの下を押した
				aPov[POV_DOWN] = 128;			// 入力された
			}
			if (aGamePadKeyState.rgdwPOV[0] == 9000.0f)
			{// 十字キーの右を押した
				aPov[POV_RIGHT] = 128;			// 入力された
			}
			if (aGamePadKeyState.rgdwPOV[0] == 27000.0f)
			{// 十字キーの左を押した
				aPov[POV_LEFT] = 128;			// 入力された
			}

			for (int nCntPov = 0; nCntPov < POV_MAX; nCntPov++)
			{// 十字キーの方向の種類分ループ
			
				m_aPovTrigger[nCntPov] = (m_aPov[nCntPov] ^ aPov[nCntPov])&aPov[nCntPov];	// 十字キーのトリガー情報
				m_aPov[nCntPov] = aPov[nCntPov];											// 十字キーのプレス情報
			}
			m_aGamePad = aGamePadKeyState;			// 現在の状態を保存
		}
		else
		{
			m_pDevGamePad->Acquire();				// ゲームパッドへのアクセス権を獲得
		}
	}
}

//=============================================================================
// コールバック関数
//=============================================================================
BOOL CALLBACK CGamePad::EnumJoysticksCallback(const DIDEVICEINSTANCE* pdidInstance, VOID* pContext)
{ 
	HRESULT hr;

	// 列挙されたジョイスティックへのインターフェイスを取得
	hr = m_pInput->CreateDevice(pdidInstance->guidInstance, &m_pDevGamePad, NULL);

	// デバイスを検出出来なかった場合
	if (FAILED(hr))
	{
		return DIENUM_CONTINUE;
	}

	return DIENUM_CONTINUE;
}

//=============================================================================
// コールバック関数
//=============================================================================
BOOL CALLBACK CGamePad::EnumAxesCallback(LPCDIDEVICEOBJECTINSTANCE lpddoi, LPVOID pvRef)
{
	//変数宣言
	HRESULT hr;
	DIPROPRANGE diprg;

	ZeroMemory(&diprg, sizeof(diprg));
	diprg.diph.dwSize = sizeof(diprg);
	diprg.diph.dwHeaderSize = sizeof(diprg.diph);
	diprg.diph.dwObj = lpddoi->dwType;
	diprg.diph.dwHow = DIPH_BYID;
	diprg.lMin = -(GAMEPAD_DEADZONE / GAMEPAD_DEADZONE * MAX_ZONE);				// 十字キーの最小値 [上] [左]
	diprg.lMax = GAMEPAD_DEADZONE / GAMEPAD_DEADZONE * MAX_ZONE;
	hr = m_pDevGamePad->SetProperty(DIPROP_RANGE, &diprg.diph);

	if (FAILED(hr))
	{
		return DIENUM_STOP;
	}

	return DIENUM_CONTINUE;
}

//=============================================================================
// プレス処理
//=============================================================================
bool CGamePad::GetGamePadPress(int nKey)
{
	if (m_pDevGamePad == NULL)
	{
		return false;
	}

	return (m_aGamePadKeyState[nKey] & 0x80) ? true : false;
}

//========================================================================================
// 入力情報(トリガー情報)を取得
//========================================================================================
bool CGamePad::GetGamePadTrigger(int nKey)
{
	if (m_pDevGamePad == NULL)
	{
		return false;
	}

	return (m_aGamePadKeyStateTrigger[nKey] & 0x80) ? true : false;
}

//========================================================================================
// の入力情報(リリース情報)を取得
//========================================================================================
bool CGamePad::GetGamePadRelease(int nKey)
{
	if (m_pDevGamePad == NULL)
	{
		return false;
	}

	return (m_aGamePadKeyStateRelease[nKey] & 0x80) ? true : false;
}

//==============================================================================================================================
// ゲームパッドの左スティック入力情報(プレス情報)を取得
//==============================================================================================================================
bool CGamePad::GetGamePadLeftStickPress(int nKey)
{
	if (m_pDevGamePad == NULL)
	{
		return false;
	}

	return (m_aLeftAnalogStick[nKey] & 0x80) ? true : false;
}

//==============================================================================================================================
// ゲームパッドの左スティック入力情報(トリガー情報)を取得
//==============================================================================================================================
bool CGamePad::GetGamePadLeftStickTrigger(int nKey)
{
	if (m_pDevGamePad == NULL)
	{
		return false;
	}

	return (m_aLeftAnalogStickTrigger[nKey] & 0x80) ? true : false;
}

//==============================================================================================================================
// ゲームパッドの右スティック入力情報(プレス情報)を取得
//==============================================================================================================================
bool CGamePad::GetGamePadRightStickPress(int nKey)
{
	if (m_pDevGamePad == NULL)
	{
		return false;
	}

	return (m_aRightAnalogStick[nKey] & 0x80) ? true : false;
}

//==============================================================================================================================
// ゲームパッドの右スティック入力情報(トリガー情報)を取得
//==============================================================================================================================
bool CGamePad::GetGamePadRightStickTrigger(int nKey)
{
	if (m_pDevGamePad == NULL)
	{
		return false;
	}

	return (m_aRightAnalogStickTrigger[nKey] & 0x80) ? true : false;
}

//=============================================================================
// 十字キー入力情報(トリガー情報)を取得
//=============================================================================
bool CGamePad::GetPovTpress(int nAngle)
{
	if (m_pDevGamePad == NULL)
	{
		return false;
	}

	return (m_aPov[nAngle] & 0x80) ? true : false;
}

//=============================================================================
// 十字キー入力情報(トリガー情報)を取得
//=============================================================================
bool CGamePad::GetPovTtigger(int nAngle)
{
	if (m_pDevGamePad == NULL)
	{
		return false;
	}

	return (m_aPovTrigger[nAngle] & 0x80) ? true : false;
}