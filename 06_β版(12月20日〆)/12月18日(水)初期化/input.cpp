//=============================================================================
//
// 入力処理 [input.cpp]
// Author : 佐藤安純　SATOU ASUMI :: 小林将兵 : 山下敦史 : 丹羽保貴
//
//=============================================================================
#include "input.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define REPEAT_FRAME     (20)    // リピート情報を起動し始めるフレーム数の合計
#define REPEAT_TRIGGER   (7)     // リピート情報を起動するフレームの間隔

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
LPDIRECTINPUT8 CInput::m_pInput = NULL;
int CInputXPad::m_nInputNum = 0;

//=============================================================================
// コンストラクタ
//=============================================================================
CInput::CInput()
{
	m_pDevice = NULL;
}

//=============================================================================
// デストラクタ
//=============================================================================
CInput::~CInput()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CInput::Init(HINSTANCE hInstance, HWND hWnd)
{
	if (m_pInput == NULL)
	{
		// DirectInputオブジェクトの生成
		if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInput, NULL)))
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CInput::Uninit(void)
{
	// 入力デバイス(キーボード)の開放
	if (m_pDevice != NULL)
	{
		m_pDevice->Unacquire();	//キーボードへのアクセス権を放棄
		m_pDevice->Release();
		m_pDevice = NULL;
	}

	// DirectInputオブジェクトの開放
	if (m_pInput != NULL)
	{
		m_pInput->Release();
		m_pInput = NULL;
	}
}

/*****************************************************************************/
// キーボードの入力処理                                                      //
/*****************************************************************************/
//=============================================================================
// コンストラクタ
//=============================================================================
CInputKeyboard::CInputKeyboard()
{
	//変数宣言
	int nCntKey;

	//キーの開放
	for (nCntKey = 0; nCntKey < MAX_KEY; nCntKey++)
	{
		m_aKeyState[nCntKey] = 0;
		m_aKeyStateRelease[nCntKey] = 0;
		m_aKeyStateTrigger[nCntKey] = 0;
		m_aKeyStateRepeat[nCntKey] = 0;
		m_aKeyRepeatCounter[nCntKey] = 0;
	}
}

//=============================================================================
// デストラクタ
//=============================================================================
CInputKeyboard::~CInputKeyboard()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CInputKeyboard::Init(HINSTANCE hInstance, HWND hWnd)
{
	//初期化処理を呼び出す
	CInput::Init(hInstance, hWnd);

	// 入力デバイス(キーボード)の生成
	if (FAILED(m_pInput->CreateDevice(GUID_SysKeyboard, &m_pDevice, NULL)))
	{
		return E_FAIL;
	}

	// データフォーマットを設定
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	// 協調モードを設定
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	// キーボードへのアクセス権を獲得(入力制御開始)
	m_pDevice->Acquire();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CInputKeyboard::Uninit(void)
{
	CInput::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CInputKeyboard::Update(void)
{
	//変数宣言
	BYTE aKeyState[MAX_KEY];	//キーボードの入力情報
	int nCntKey = 0;

	// 入力デバイスからデータを取得
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (nCntKey = 0; nCntKey < MAX_KEY; nCntKey++)
		{
			m_aKeyStateRelease[nCntKey] = (m_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & m_aKeyState[nCntKey];	//リリース
			m_aKeyStateTrigger[nCntKey] = (m_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & aKeyState[nCntKey];		//トリガー
			m_aKeyState[nCntKey] = aKeyState[nCntKey];															//キーボードの入力情報保存

			if (m_aKeyState[nCntKey] == 0x80)
			{// プレス情報が起動している
				m_aKeyRepeatCounter[nCntKey]++;
				if (m_aKeyRepeatCounter[nCntKey] >= REPEAT_FRAME)
				{// プレス情報が起動して一定時間経過
					if (m_aKeyRepeatCounter[nCntKey] % REPEAT_TRIGGER == 0)
					{// 7フレーム毎に情報を起動する
						m_aKeyStateRepeat[nCntKey] = 0x80;
					}
					else
					{// それ以外のフレームはリピート情報を一時停止
						m_aKeyStateRepeat[nCntKey] = 0x00;
					}
				}
			}
			else
			{// プレス情報が起動していない
				m_aKeyStateRepeat[nCntKey] = 0x00;
				m_aKeyRepeatCounter[nCntKey] = 0;
			}
		}
	}
	else
	{
		m_pDevice->Acquire();	//キーボードへのアクセス権を獲得
	}
}

//=============================================================================
// キーボードプレス処理
//=============================================================================
bool CInputKeyboard::GetKeyboardPress(int nKey)
{
	return (m_aKeyState[nKey] & 0x80) ? true : false;
}

//========================================================================================
// キーボードの入力情報(トリガー情報)を取得
//========================================================================================
bool CInputKeyboard::GetKeyboardTrigger(int nKey)
{
	return (m_aKeyStateTrigger[nKey] & 0x80) ? true : false;
}

//========================================================================================
// キーボードの入力情報(リリース情報)を取得
//========================================================================================
bool CInputKeyboard::GetKeyboardRelease(int nKey)
{
	return (m_aKeyStateRelease[nKey] & 0x80) ? true : false;
}

//========================================================================================
// キーボードの入力情報(リピート情報)を取得
//========================================================================================
bool CInputKeyboard::GetKeyboardRepeat(int nKey)
{
	return (m_aKeyStateRepeat[nKey] & 0x80) ? true : false;
}

//=============================================================================
// コンストラクタ							(public)	*** CInputXPad ***
//=============================================================================
CInputXPad::CInputXPad()
{
	// 初期値設定
	for (int nCntCont = 0; nCntCont < MAX_CONTROLLERS; nCntCont++)
	{
		m_LStickRot[nCntCont] = 0.0f;		// Lスティック
		m_RStickRot[nCntCont] = 0.0f;		// Rスティック
	}

	m_nInputNum = 0;						// 入力数

	ZeroMemory(m_aJoyRepeatCounter, sizeof(int) * XPADOTHER_MAX * MAX_CONTROLLERS);
}

//=============================================================================
// デストラクタ								(public)	*** CInputXPad ***
//=============================================================================
CInputXPad::~CInputXPad()
{

}

//=============================================================================
// 初期化処理								(public)	*** CInputXPad ***
//=============================================================================
HRESULT CInputXPad::Init(HINSTANCE hInstance, HWND hWnd)
{
	// コントローラーの初期化
	ZeroMemory(m_Controllers, sizeof(CONTROLER_STATE) * MAX_CONTROLLERS);

	return S_OK;
}

//=============================================================================
// 終了処理									(public)	*** CInputXPad ***
//=============================================================================
void CInputXPad::Uninit(void)
{

}

//=============================================================================
// 更新処理									(public)	*** CInputXPad ***
//=============================================================================
void CInputXPad::Update(void)
{
	UpdateControllerState();	// 途中入力

	for (DWORD nCntCont = 0; nCntCont < MAX_CONTROLLERS; nCntCont++)
	{// コントローラーカウント
		if (m_Controllers[nCntCont].bConnected == true)
		{// 接続されていたら
			if (m_bDeadZoneOn == true)
			{// スティック
			 // Zero value if thumbsticks are within the dead zone
				if ((m_Controllers[nCntCont].state.Gamepad.sThumbLX < INPUT_DEADZONE &&
					m_Controllers[nCntCont].state.Gamepad.sThumbLX > -INPUT_DEADZONE) &&
					(m_Controllers[nCntCont].state.Gamepad.sThumbLY < INPUT_DEADZONE &&
						m_Controllers[nCntCont].state.Gamepad.sThumbLY > -INPUT_DEADZONE))
				{
					m_Controllers[nCntCont].state.Gamepad.sThumbLX = 0;
					m_Controllers[nCntCont].state.Gamepad.sThumbLY = 0;
				}
				//m_Controllers[nCntCont].state.Gamepad.wButtons
				if ((m_Controllers[nCntCont].state.Gamepad.sThumbRX < INPUT_DEADZONE &&
					m_Controllers[nCntCont].state.Gamepad.sThumbRX > -INPUT_DEADZONE) &&
					(m_Controllers[nCntCont].state.Gamepad.sThumbRY < INPUT_DEADZONE &&
						m_Controllers[nCntCont].state.Gamepad.sThumbRY > -INPUT_DEADZONE))
				{
					m_Controllers[nCntCont].state.Gamepad.sThumbRX = 0;
					m_Controllers[nCntCont].state.Gamepad.sThumbRY = 0;
				}
			}

			WORD wButtons = 0;		// XINPUTコントローラーの入力情報

			for (int nCount = 0; nCount < XPADOTHER_MAX; nCount++)
			{// 入力情報カウント
				switch (nCount)
				{
				case 0:		// ボタン
					wButtons = m_Controllers[nCntCont].state.Gamepad.wButtons;
					break;
				case 1:		// 左トリガー
					wButtons = m_Controllers[nCntCont].state.Gamepad.bLeftTrigger;
					break;
				case 2:		// 右トリガー
					wButtons = m_Controllers[nCntCont].state.Gamepad.bRightTrigger;
					break;
				case 3:		// 左スティック上
					if (m_Controllers[nCntCont].state.Gamepad.sThumbLY > XINPUT_STICK_MAX * 0.1f)
						wButtons = 0x80;
					else
						wButtons = 0;
					break;
				case 4:		// 左スティック左
					if (m_Controllers[nCntCont].state.Gamepad.sThumbLX < XINPUT_STICK_MIN * 0.1f)
						wButtons = 0x80;
					else
						wButtons = 0;
					break;
				case 5:		// 左スティック右
					if (m_Controllers[nCntCont].state.Gamepad.sThumbLX > XINPUT_STICK_MAX * 0.1f)
						wButtons = 0x80;
					else
						wButtons = 0;
					break;
				case 6:		// 左スティック下
					if (m_Controllers[nCntCont].state.Gamepad.sThumbLY < XINPUT_STICK_MIN * 0.1f)
						wButtons = 0x80;
					else
						wButtons = 0;
					break;
				case 7:		// 右スティック上
					if (m_Controllers[nCntCont].state.Gamepad.sThumbRY > XINPUT_STICK_MAX * 0.1f)
						wButtons = 0x80;
					else
						wButtons = 0;
					break;
				case 8:		// 右スティック左
					if (m_Controllers[nCntCont].state.Gamepad.sThumbRX < XINPUT_STICK_MIN * 0.1f)
						wButtons = 0x80;
					else
						wButtons = 0;
					break;
				case 9:		// 右スティック右
					if (m_Controllers[nCntCont].state.Gamepad.sThumbRX > XINPUT_STICK_MAX * 0.1f)
						wButtons = 0x80;
					else
						wButtons = 0;
					break;
				case 10:		// 右スティック下
					if (m_Controllers[nCntCont].state.Gamepad.sThumbRY < XINPUT_STICK_MIN * 0.1f)
						wButtons = 0x80;
					else
						wButtons = 0;
					break;
				}

				// スティック向き
				m_LStickRot[nCntCont] = atan2f((float)m_Controllers[nCntCont].state.Gamepad.sThumbLX, (float)m_Controllers[nCntCont].state.Gamepad.sThumbLY);
				m_RStickRot[nCntCont] = atan2f((float)m_Controllers[nCntCont].state.Gamepad.sThumbRX, (float)m_Controllers[nCntCont].state.Gamepad.sThumbRY);

				// ジョイパッドの入力情報
				m_aJoyStateTrigger[nCntCont][nCount] = (m_aJoyStatePress[nCntCont][nCount] ^ wButtons) & wButtons;
				m_aJoyStateRelease[nCntCont][nCount] = (m_aJoyStatePress[nCntCont][nCount] ^ wButtons) & m_aJoyStatePress[nCntCont][nCount];
				m_aJoyStatePress[nCntCont][nCount] = wButtons;

				if (m_aJoyStatePress[nCntCont][nCount] == 0x80)
				{// プレス情報が起動している
					m_aJoyRepeatCounter[nCntCont][nCount]++;
					if (m_aJoyRepeatCounter[nCntCont][nCount] >= REPEAT_FRAME)
					{// プレス情報が起動して一定時間経過
						if (m_aJoyRepeatCounter[nCntCont][nCount] % REPEAT_TRIGGER == 0)
						{// 7フレーム毎に情報を起動する
							m_aJoyStateRepeat[nCntCont][nCount] = 0x80;
						}
						else
						{// それ以外のフレームはリピート情報を一時停止
							m_aJoyStateRepeat[nCntCont][nCount] = 0x00;
						}
					}
				}
				else
				{// プレス情報が起動していない
					m_aJoyStateRepeat[nCntCont][nCount] = 0x00;
					m_aJoyRepeatCounter[nCntCont][nCount] = 0;
				}
			}
		}
	}
}

//=============================================================================
// Xパッドの入力情報(入力確認)を取得	(private)	*** CInputXPad ***
//=============================================================================
HRESULT CInputXPad::UpdateControllerState(void)
{
	DWORD dwResult;
	m_nInputNum = 0;
	for (DWORD nCntCont = 0; nCntCont < MAX_CONTROLLERS; nCntCont++)
	{
		// Simply get the state of the controller from XInput.
		dwResult = XInputGetState(nCntCont, &m_Controllers[nCntCont].state);

		if (dwResult == ERROR_SUCCESS)
		{
			m_Controllers[nCntCont].bConnected = true;
			m_nInputNum++;
		}
		else
		{
			m_Controllers[nCntCont].bConnected = false;
		}
	}

	return S_OK;
}
//=============================================================================
// Xパッドの入力情報(入力数情報)を取得	(private)	*** CInputXPad ***
//=============================================================================
int CInputXPad::GetInputNum(void)
{
	return m_nInputNum;
}

//=============================================================================
// Xパッドの入力情報(オール情報)を取得	(private)	*** CInputXPad ***
//=============================================================================
bool CInputXPad::GetALL(int nType, int nIdxPad)
{
	if (m_Controllers[nIdxPad].bConnected)
	{
		switch (nType)
		{
		case 0:
			for (int nCount = 0; nCount < XPADOTHER_MAX; nCount++)
			{// 入力情報カウント
				if (nCount == XPADOTHER_TRIGGER_LEFT || nCount == XPADOTHER_TRIGGER_RIGHT ||
					nCount == XPADOTHER_STICK_L_UP || nCount == XPADOTHER_STICK_L_LEFT || nCount == XPADOTHER_STICK_L_RIGHT || nCount == XPADOTHER_STICK_L_DOWN ||
					nCount == XPADOTHER_STICK_R_UP || nCount == XPADOTHER_STICK_R_LEFT || nCount == XPADOTHER_STICK_R_RIGHT || nCount == XPADOTHER_STICK_R_DOWN)
				{
					if (m_aJoyStatePress[nIdxPad][nCount] == 0x80)
					{
						return true;
					}
				}
				else
				{
					if (m_aJoyStatePress[nIdxPad][nCount] != 0)
					{
						return true;
					}
				}
			}

			break;
		case 1:
			for (int nCount = 0; nCount < XPADOTHER_MAX; nCount++)
			{// 入力情報カウント
				if (nCount == XPADOTHER_TRIGGER_LEFT || nCount == XPADOTHER_TRIGGER_RIGHT ||
					nCount == XPADOTHER_STICK_L_UP || nCount == XPADOTHER_STICK_L_LEFT || nCount == XPADOTHER_STICK_L_RIGHT || nCount == XPADOTHER_STICK_L_DOWN ||
					nCount == XPADOTHER_STICK_R_UP || nCount == XPADOTHER_STICK_R_LEFT || nCount == XPADOTHER_STICK_R_RIGHT || nCount == XPADOTHER_STICK_R_DOWN)
				{
					if (m_aJoyStateTrigger[nIdxPad][nCount] == 0x80)
					{
						return true;
					}
				}
				else
				{
					if (m_aJoyStateTrigger[nIdxPad][nCount] != 0)
					{
						return true;
					}
				}
			}

			break;
		default:
			for (int nCount = 0; nCount < XPADOTHER_MAX; nCount++)
			{// 入力情報カウント
				if (nCount == XPADOTHER_TRIGGER_LEFT || nCount == XPADOTHER_TRIGGER_RIGHT ||
					nCount == XPADOTHER_STICK_L_UP || nCount == XPADOTHER_STICK_L_LEFT || nCount == XPADOTHER_STICK_L_RIGHT || nCount == XPADOTHER_STICK_L_DOWN ||
					nCount == XPADOTHER_STICK_R_UP || nCount == XPADOTHER_STICK_R_LEFT || nCount == XPADOTHER_STICK_R_RIGHT || nCount == XPADOTHER_STICK_R_DOWN)
				{
					if (m_aJoyStateRelease[nIdxPad][nCount] == 0x80)
					{
						return true;
					}
				}
				else
				{
					if (m_aJoyStateRelease[nIdxPad][nCount] != 0)
					{
						return true;
					}
				}
			}

			break;
		}
	}

	return false;
}

//=============================================================================
// Xパッドの入力情報(プレス情報)を取得	(private)	*** CInputXPad ***
//=============================================================================
bool CInputXPad::GetPress(int nButton, int nIdxPad)
{
	if (m_Controllers[nIdxPad].bConnected)
		return (m_aJoyStatePress[nIdxPad][0] & nButton) ? true : false;
	else
		return false;
}
bool CInputXPad::GetPress(XPADOTHER nButton, int nIdxPad)
{
	if (m_Controllers[nIdxPad].bConnected)
		return (m_aJoyStatePress[nIdxPad][nButton] & 0x80) ? true : false;
	else
		return false;
}

//=============================================================================
// Xパッドの入力情報(トリガー情報)を取得	(private)	*** CInputXPad ***
//=============================================================================
bool CInputXPad::GetTrigger(int nButton, int nIdxPad)
{
	if (m_Controllers[nIdxPad].bConnected)
		return (m_aJoyStateTrigger[nIdxPad][0] & nButton) ? true : false;
	else
		return false;
}
bool CInputXPad::GetTrigger(XPADOTHER nButton, int nIdxPad)
{
	if (m_Controllers[nIdxPad].bConnected)
		return (m_aJoyStateTrigger[nIdxPad][nButton] & 0x80) ? true : false;
	else
		return false;
}

//=============================================================================
// Xパッドの入力情報(リリース情報)を取得	(private)	*** CInputXPad ***
//=============================================================================
bool CInputXPad::GetRelease(int nButton, int nIdxPad)
{
	if (m_Controllers[nIdxPad].bConnected)
		return (m_aJoyStateRelease[nIdxPad][0] & nButton) ? true : false;
	else
		return false;
}
bool CInputXPad::GetRelease(XPADOTHER nButton, int nIdxPad)
{
	if (m_Controllers[nIdxPad].bConnected)
		return (m_aJoyStateRelease[nIdxPad][nButton] & 0x80) ? true : false;
	else
		return false;
}

//=============================================================================
// Xパッドの入力情報(リリース情報)を取得	(private)	*** CInputXPad ***
//=============================================================================
bool CInputXPad::GetRepeat(int nButton, int nIdxPad)
{
	if (m_Controllers[nIdxPad].bConnected)
		return (m_aJoyStateRepeat[nIdxPad][0] & nButton) ? true : false;
	else
		return false;
}
bool CInputXPad::GetRepeat(XPADOTHER nButton, int nIdxPad)
{
	if (m_Controllers[nIdxPad].bConnected)
		return (m_aJoyStateRepeat[nIdxPad][nButton] & 0x80) ? true : false;
	else
		return false;
}

//=============================================================================
// Xパッドの入力情報(スティック情報)を取得	(private)	*** CInputXPad ***
//=============================================================================
bool CInputXPad::GetStick(int nLR, int nIdxPad)
{
	if (m_Controllers[nIdxPad].bConnected)
	{
		if (nLR == 0)
		{
			for (int nCntStick = XPADOTHER_STICK_L_UP; nCntStick <= XPADOTHER_STICK_L_DOWN; nCntStick++)
			{
				if (m_aJoyStatePress[nIdxPad][nCntStick] == 0x80)
				{
					return true;
				}
			}
		}
		else
		{
			for (int nCntStick = XPADOTHER_STICK_R_UP; nCntStick <= XPADOTHER_STICK_R_DOWN; nCntStick++)
			{
				if (m_aJoyStatePress[nIdxPad][nCntStick] == 0x80)
				{
					return true;
				}
			}
		}
	}

	return false;
}

//=============================================================================
// Xパッドの入力情報(スティック向き情報)を取得	(private)	*** CInputXPad ***
//=============================================================================
float CInputXPad::GetStickRot(int nLR, int nIdxPad)
{
	if (m_Controllers[nIdxPad].bConnected)
	{
		if (nLR == 0)
		{
			return m_LStickRot[nIdxPad];
		}
		else
		{
			return m_RStickRot[nIdxPad];
		}
	}

	return false;
}