//*****************************************************************************
//
//     RawInputを用いたマウス複数化の処理[RawMouse.cpp]
//     Author:Hodaka Niwa
//
//*****************************************************************************
#include "RawMouse.h"
#include "strsafe.h"

//*****************************************************************************
//     マクロ定義
//*****************************************************************************
// マウスの入力検知用
// ボタン
#define RAWINPUT_MOUSE_BUTTON_LEFT       (1)
#define RAWINPUT_MOUSE_BUTTON_RIGHT      (4)
#define RAWINPUT_MOUSE_BUTTON_MIDDLE     (16)

// デッドゾーン
#define RAWINPUT_MOUSE_DEADZONE_HORIZON  (25)
#define RAWINPUT_MOUSE_DEADZONE_VERTICAL (25)

// リピート用
#define RAWINPUT_REPEAT_FRAME            (20)
#define RAWINPUT_REPEAT_TRIGGER          (7)

//*****************************************************************************
//     静的メンバ変数宣言
//*****************************************************************************
CRawMouse *CRawMouse::m_pInstance = NULL;   // RawMouseクラスへのポインタ

//=============================================================================
//    コンストラクタ
//=============================================================================
CRawMouse::CRawMouse()
{
	ZeroMemory(&m_Mouse, sizeof(m_Mouse));
	ZeroMemory(&m_aMousePress, sizeof(m_aMousePress));
	ZeroMemory(&m_aMouseTrigger, sizeof(m_aMouseTrigger));
	ZeroMemory(&m_aMouseRelease, sizeof(m_aMouseRelease));
	ZeroMemory(&m_aMouseRepeat, sizeof(m_aMouseRepeat));
	ZeroMemory(&m_aMouseCounter, sizeof(m_aMouseCounter));
	ZeroMemory(&m_bMouseTrigger, sizeof(m_bMouseTrigger));
}

//=============================================================================
//    デストラクタ
//=============================================================================
CRawMouse::~CRawMouse()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CRawMouse *CRawMouse::Create(HWND hWnd)
{
	if (m_pInstance != NULL) return m_pInstance;
	m_pInstance = new CRawMouse;

	return m_pInstance;
}

//=============================================================================
//    デバイスをRAWINPUTに登録する処理
//=============================================================================
HRESULT CRawMouse::RegisterRawInput(HWND hWnd)
{
	// デバイスをWM_INPUTに登録
	RAWINPUTDEVICE Device[1];
	// HIDマウス
	Device[0].usUsagePage = 0x01;
	Device[0].usUsage = 0x02;
	Device[0].dwFlags = RIDEV_INPUTSINK;
	Device[0].hwndTarget = hWnd;

	// 登録
	if (RegisterRawInputDevices(Device, 1, sizeof(Device[0])) == 0)
	{
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CRawMouse::Init(HWND hWnd)
{
	// 現在接続されているデバイスの数を取得
	GetRawInputDeviceList(NULL, &m_nDeviceNum, sizeof(RAWINPUTDEVICELIST));
	RAWINPUTDEVICELIST* deviceList = new RAWINPUTDEVICELIST[m_nDeviceNum];

	// リストを取得
	if(GetRawInputDeviceList(deviceList, &m_nDeviceNum, sizeof(RAWINPUTDEVICELIST)) == -1)
	{
		return E_FAIL;
	}

	// ハンドルを保存
	m_nMouseNum = 0;
	for (int nCnt = 0; nCnt < (int)m_nDeviceNum; nCnt++)
	{
		if (deviceList[nCnt].dwType == RIM_TYPEMOUSE)
		{
			m_MouseHandle[m_nMouseNum] = deviceList[nCnt].hDevice;
			m_nMouseNum++;
		}
	}
	delete[] deviceList;

	// ウィンドウハンドルを保存
	m_hWnd = hWnd;

	// キーボード上のマウスは使用しないので数を減らす
	m_nMouseNum--;

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CRawMouse::Uninit(void)
{
	// ゼロクリア
	ClearMouseData();
}

//=============================================================================
//    更新処理
//=============================================================================
void CRawMouse::Update(LPARAM lParam)
{
	// バッファの大きさを取得
	UINT dwSize = 1;
	GetRawInputData((HRAWINPUT)lParam, RID_INPUT, NULL, &dwSize, sizeof(RAWINPUTHEADER));
	BYTE* lpb = new BYTE[dwSize];
	if (lpb == NULL)
	{
		return;
	}

	// データを取得
	if (GetRawInputData((HRAWINPUT)lParam, RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER)) == -1)
	{
		delete[] lpb;
		return;
	}

	// データをメンバ変数に格納
	RAWINPUT *raw = (RAWINPUT*)lpb;
	for (int nCntMouse = 0; nCntMouse < (int)m_nMouseNum; nCntMouse++)
	{
		if (raw->header.dwType == RIM_TYPEMOUSE && m_MouseHandle[nCntMouse] == raw->header.hDevice)
		{
			ArrayMouseData(raw, nCntMouse);
		}
	}

// デバッグ用のコメントアウト
#if 0
	//if (raw->header.dwType == RIM_TYPEMOUSE && m_MouseHandle[] == raw->header.hDevice)
	//{
	//	m_Mouse[] = raw->data.mouse;
	//	m_MouseHeader[] = raw->header;

	//	//char szTempOutput[256];
	//	//HRESULT hResult;
	//	//hResult = StringCchPrintf(szTempOutput, STRSAFE_MAX_CCH, TEXT("Mouse: usFlags=%04x ulButtons=%04x usButtonFlags=%04x usButtonData=%04x ulRawButtons=%04x lLastX=%04x lLastY=%04x ulExtraInformation=%04x\r\n"),
	//	//	raw->data.mouse.usFlags,
	//	//	raw->data.mouse.ulButtons,
	//	//	raw->data.mouse.usButtonFlags,
	//	//	raw->data.mouse.usButtonData,
	//	//	raw->data.mouse.ulRawButtons,
	//	//	raw->data.mouse.lLastX,
	//	//	raw->data.mouse.lLastY,
	//	//	raw->data.mouse.ulExtraInformation);

	//	//if (FAILED(hResult))
	//	//{
	//	//	// TODO: write error handler
	//	//}
	//	//OutputDebugString(szTempOutput);
	//}
	//else if (raw->header.dwType == RIM_TYPEKEYBOARD)
	//{
	//	m_Keyboard = raw->data.keyboard;
	//	m_KeyHeader = raw->header;
	//}
#endif

	delete[] lpb;
}

//=============================================================================
//    マウスのデータを割り当てる処理
//=============================================================================
void CRawMouse::ArrayMouseData(RAWINPUT *raw, int nCntMouse)
{
	// マウスデータを保存
	m_Mouse[nCntMouse] = raw->data.mouse;

	// ヘッダー(詳細情報)を保存
	m_MouseHeader[nCntMouse] = raw->header;

	// マウスの状態を取得
	BYTE aMouseState[RIMS_BUTTON_MAX] = {};   // マウスボタン入力情報

	// ボタン系
	if (raw->data.mouse.ulButtons >= RAWINPUT_MOUSE_BUTTON_MIDDLE)
	{// 中ボタンが押されている
		aMouseState[RIMS_BUTTON_MIDDLE] = 0x80;
		raw->data.mouse.ulButtons -= RAWINPUT_MOUSE_BUTTON_MIDDLE;
		if (raw->data.mouse.ulButtons >= RAWINPUT_MOUSE_BUTTON_MIDDLE)
		{
			aMouseState[RIMS_BUTTON_MIDDLE] = 0x00;
			raw->data.mouse.ulButtons -= RAWINPUT_MOUSE_BUTTON_MIDDLE;
			m_bMouseRelease[nCntMouse][RIMS_BUTTON_MIDDLE] = false;
		}
	}
	if (raw->data.mouse.ulButtons >= RAWINPUT_MOUSE_BUTTON_RIGHT)
	{// 右ボタンが押されている
		aMouseState[RIMS_BUTTON_RIGHT] = 0x80;
		raw->data.mouse.ulButtons -= RAWINPUT_MOUSE_BUTTON_RIGHT;
		if (raw->data.mouse.ulButtons >= RAWINPUT_MOUSE_BUTTON_RIGHT)
		{
			aMouseState[RIMS_BUTTON_RIGHT] = 0x00;
			raw->data.mouse.ulButtons -= RAWINPUT_MOUSE_BUTTON_RIGHT;
			m_bMouseRelease[nCntMouse][RIMS_BUTTON_RIGHT] = false;
		}
	}
	if (raw->data.mouse.ulButtons >= RAWINPUT_MOUSE_BUTTON_LEFT)
	{// 左ボタンが押されている
		aMouseState[RIMS_BUTTON_LEFT] = 0x80;
		raw->data.mouse.ulButtons -= RAWINPUT_MOUSE_BUTTON_LEFT;
		if (raw->data.mouse.ulButtons >= RAWINPUT_MOUSE_BUTTON_LEFT)
		{
			aMouseState[RIMS_BUTTON_LEFT] = 0x00;
			raw->data.mouse.ulButtons -= RAWINPUT_MOUSE_BUTTON_LEFT;
			m_bMouseRelease[nCntMouse][RIMS_BUTTON_LEFT] = false;
		}
	}

	// マウスの動き
	if (raw->data.mouse.lLastY < -RAWINPUT_MOUSE_DEADZONE_VERTICAL)
	{// 上に動いた
		aMouseState[RIMS_SLIDE_DOWN] = 0x80;
		m_bMouseRelease[nCntMouse][RIMS_SLIDE_DOWN] = false;
	}
	else if (raw->data.mouse.lLastY > RAWINPUT_MOUSE_DEADZONE_VERTICAL)
	{// 下に動いた
		aMouseState[RIMS_SLIDE_UP] = 0x80;
		m_bMouseRelease[nCntMouse][RIMS_SLIDE_UP] = false;
	}
	if (raw->data.mouse.lLastX < -RAWINPUT_MOUSE_DEADZONE_HORIZON)
	{// 右に動いた
		aMouseState[RIMS_SLIDE_RIGHT] = 0x80;
		m_bMouseRelease[nCntMouse][RIMS_SLIDE_RIGHT] = false;
	}
	else if (raw->data.mouse.lLastX > RAWINPUT_MOUSE_DEADZONE_HORIZON)
	{// 左に動いた
		aMouseState[RIMS_SLIDE_LEFT] = 0x80;
		m_bMouseRelease[nCntMouse][RIMS_SLIDE_LEFT] = false;
	}

	// ボタンの状態を格納
	for (int nCntButton = 0; nCntButton < RIMS_BUTTON_MAX; nCntButton++)
	{
		m_aMousePress[nCntMouse][nCntButton] = aMouseState[nCntButton];
		if (aMouseState[nCntButton] == 0x00)
		{
			m_bMouseTrigger[nCntMouse][nCntButton] = false;
		}
	}
}

//=============================================================================
//    マウスのデータを調整する処理
//=============================================================================
void CRawMouse::AdjustMouseData(void)
{
	for (int nCntMouse = 0; nCntMouse < (int)m_nMouseNum; nCntMouse++)
	{// 接続されているマウスの数だけ繰り返し
		for (int nCntButton = 0; nCntButton < RIMS_BUTTON_MAX; nCntButton++)
		{// ボタンの数だけ繰り返し
			// トリガー情報
			AdjustMouseData_Trigger(nCntMouse, nCntButton);

			// リリース情報
			AdjustMouseData_Release(nCntMouse, nCntButton);

			// リピート情報
			AdjustMouseData_Repeat(nCntMouse, nCntButton);
		}
	}
}

//=============================================================================
//    マウスのデータを調整する処理(トリガー)
//=============================================================================
void CRawMouse::AdjustMouseData_Trigger(int nCntMouse, int nCntButton)
{
	if (m_aMousePress[nCntMouse][nCntButton] == 0x80 && m_bMouseTrigger[nCntMouse][nCntButton] == false)
	{
		m_aMouseTrigger[nCntMouse][nCntButton] = 0x80;
		m_bMouseTrigger[nCntMouse][nCntButton] = true;
	}
	else
	{
		m_aMouseTrigger[nCntMouse][nCntButton] = 0x00;
	}
}

//=============================================================================
//    マウスのデータを調整する処理(リリース)
//=============================================================================
void CRawMouse::AdjustMouseData_Release(int nCntMouse, int nCntButton)
{
	if (m_aMousePress[nCntMouse][nCntButton] == 0x00 && m_bMouseRelease[nCntMouse][nCntButton] == false)
	{
		m_aMouseRelease[nCntMouse][nCntButton] = 0x80;
		m_bMouseRelease[nCntMouse][nCntButton] = true;
	}
	else
	{
		m_aMouseRelease[nCntMouse][nCntButton] = 0x00;
	}
}

//=============================================================================
//    マウスのデータを調整する処理(リピート)
//=============================================================================
void CRawMouse::AdjustMouseData_Repeat(int nCntMouse, int nCntButton)
{
	if (m_aMousePress[nCntMouse][nCntButton] == 0x80)
	{// ボタンが入力されている
		m_aMouseCounter[nCntMouse][nCntButton]++;    // カウンターを進める
		if (m_aMouseCounter[nCntMouse][nCntButton] >= RAWINPUT_REPEAT_FRAME)
		{// 20フレーム分キーが押されている
			if (m_aMouseCounter[nCntMouse][nCntButton] % RAWINPUT_REPEAT_TRIGGER == 0)
			{// 7フレームごと
				m_aMouseRepeat[nCntMouse][nCntButton] = 0x80;   // マウスの入力情報(リピート情報)を起動
			}
			else
			{// それ以外のフレーム
				m_aMouseRepeat[nCntMouse][nCntButton] = 0;      // マウスの入力情報(リピート情報)を一時停止
			}
		}
	}
	else
	{// キーボードが入力されていない
		m_aMouseCounter[nCntMouse][nCntButton] = 0;        // カウンターを戻す
		m_aMouseRepeat[nCntMouse][nCntButton] = 0;         // マウスの入力情報(リピート情報)を停止
	}
}

//=============================================================================
//    マウスのデータをクリアする処理
//=============================================================================
void CRawMouse::ClearMouseData(void)
{
	ZeroMemory(&m_Mouse, sizeof(m_Mouse));
}

//=============================================================================
//    マウスの接続されている数を処理
//=============================================================================
UINT CRawMouse::GetMouseNum(void)
{
	return m_nMouseNum;
}

//=============================================================================
//    マウスへのハンドルを取得する処理
//=============================================================================
HANDLE CRawMouse::GetMouseHandle(int nIdx)
{
	return m_MouseHandle[nIdx];
}

//=============================================================================
//    マウスを取得する処理
//=============================================================================
RAWMOUSE CRawMouse::GetMouse(int nIdx)
{
	return m_Mouse[nIdx];
}

//=============================================================================
//    マウスのボタンが押されたかどうかを取得(プレス情報)
//=============================================================================
bool CRawMouse::GetPress(int nIdx, int nButton)
{
	return (m_aMousePress[nIdx][nButton] & 0x80) ? true : false;
}

//=============================================================================
//    マウスのボタンが押されたかどうかを取得(トリガー情報)
//=============================================================================
bool CRawMouse::GetTrigger(int nIdx, int nButton)
{
	return (m_aMouseTrigger[nIdx][nButton] & 0x80) ? true : false;
}

//=============================================================================
//    マウスのボタンが押されたかどうかを取得(リリース情報)
//=============================================================================
bool CRawMouse::GetRelease(int nIdx, int nButton)
{
	return (m_aMouseRelease[nIdx][nButton] & 0x80) ? true : false;
}

//=============================================================================
//    マウスのボタンが押されたかどうかを取得(リピート情報)
//=============================================================================
bool CRawMouse::GetRepeat(int nIdx, int nButton)
{
	return (m_aMouseRepeat[nIdx][nButton] & 0x80) ? true : false;
}