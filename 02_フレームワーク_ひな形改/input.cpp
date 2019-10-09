//=============================================================================
//
// 入力処理 [input.cpp]
// Author : 佐藤安純　SATOU ASUMI
//
//=============================================================================
#include "input.h"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
LPDIRECTINPUT8 CInput::m_pInput = NULL;

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
CInputKeyboard :: CInputKeyboard()
{
	//変数宣言
	int nCntKey;

	//キーの開放
	for (nCntKey = 0; nCntKey < MAX_KEY; nCntKey++)
	{
		m_aKeyState[nCntKey] = 0;
		m_aKeyStateRelease[nCntKey] = 0;
		m_aKeyStateTrigger[nCntKey] = 0;
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
// キーボードの入力情報(トリガー情報)を取得
//========================================================================================
bool CInputKeyboard::GetKeyboardRelease(int nKey)
{
	return (m_aKeyStateRelease[nKey] & 0x80) ? true : false;
}