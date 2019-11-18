//=============================================================================
//
// マウス入力処理 [inputmouce.cpp]
// Author : 佐藤安純　SATOU ASUMI
//
//=============================================================================
#include "inputmouce.h"
#include "manager.h"
#include "camera.h"
#include "game.h"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
LPDIRECTINPUTDEVICE8 CInputMouse::m_pMouse = NULL;

//=============================================================================
// コンストラクタ
//=============================================================================
CInputMouse::CInputMouse()
{
	m_pMouse = NULL;
}

//=============================================================================
// デストラクタ
//=============================================================================
CInputMouse::~CInputMouse(){}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CInputMouse::Init(HINSTANCE hInstance, HWND hWnd)
{
	//変数宣言
	HRESULT ret = S_FALSE;

	//画面状態の保存
	m_hWnd = hWnd;

	//変数の初期化
	m_WorldPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_MouseIX = 0;
	m_MouseIY = 0;
	m_MouseIZ = 0;

	//初期化処理
	CInput::Init(hInstance, hWnd);

	//変数の初期化
	SetRect(&m_MouseInfo.moveRect, 10, 10, 630, 470);								// マウスカーソルの動く範囲
	m_MouseInfo.x = m_MouseInfo.moveRect.left;										// マウスカーソルのＸ座標を初期化
	m_MouseInfo.y = m_MouseInfo.moveRect.top;										// マウスカーソルのＹ座標を初期化
	m_MouseInfo.lButton = false;													// 左ボタンの情報を初期化
	m_MouseInfo.rButton = false;													// 右ボタンの情報を初期化
	m_MouseInfo.cButton = false;													// 中央ボタンの情報を初期化
	m_MouseInfo.moveAdd = 2;														// マウスカーソルの移動量を設定
	SetRect(&m_MouseInfo.imgRect, 400, 0, 420, 20);									// マウスカーソル画像の矩形を設定
	m_MouseInfo.imgWidth = m_MouseInfo.imgRect.right - m_MouseInfo.imgRect.left;	// 画像の幅を計算
	m_MouseInfo.imgHeight = m_MouseInfo.imgRect.bottom - m_MouseInfo.imgRect.top;	// 画像の高さを計算

	//入力デバイスの生成
	if (FAILED(m_pInput->CreateDevice(GUID_SysMouse, &m_pMouse, NULL)))
	{
		MessageBox(hWnd, TEXT("マウスデバイスの生成に失敗"), TEXT("メッセージボックス"), MB_OK);
		return false;
	}

	//データフォーマットの設定
	if (FAILED(m_pMouse->SetDataFormat(&c_dfDIMouse2)))
	{
		MessageBox(hWnd, TEXT("マウスフォーマットの設定に失敗"), TEXT("メッセージボックス"), MB_OK);
		return false;
	}

	// モードの設定
	if (FAILED(m_pMouse->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND)))
	{
		MessageBox(hWnd, TEXT("マウスモードの設定に失敗"), TEXT("メッセージボックス"), MB_OK);
		return false;
	}

	// デバイスの設定
	DIPROPDWORD diprop;
	diprop.diph.dwSize = sizeof(diprop);
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	diprop.diph.dwObj = 0;
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.dwData = DIPROPAXISMODE_REL;	// 相対値モードで設定（絶対値はDIPROPAXISMODE_ABS）

	ret = m_pMouse->SetProperty(DIPROP_AXISMODE, &diprop.diph);
	if (FAILED(ret)) 
	{
		// デバイスの設定に失敗
		return false;
	}

	// 入力制御開始
	ret = m_pMouse->Acquire();

	return true;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CInputMouse::Uninit(void)
{
	// DirectInputオブジェクトの開放
	if (m_pMouse != NULL)
	{
		m_pMouse->Unacquire();
		m_pMouse->Release();
		m_pMouse = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void CInputMouse::Update(void)
{
	//変数宣言
	HRESULT ret = S_FALSE;

	if (m_pMouse == NULL) { return; }

	if (SUCCEEDED(ret = m_pMouse->GetDeviceState(sizeof(DIMOUSESTATE2), &m_MouseState)))
	{
		for (int nCntMouse = 0; nCntMouse < NUM_MOUSEMAX; nCntMouse++)
		{
			m_MouseStateRelease.rgbButtons[nCntMouse] = (m_MouseStatePress.rgbButtons[nCntMouse] ^ m_MouseState.rgbButtons[nCntMouse]) & m_MouseStatePress.rgbButtons[nCntMouse];	//リリース
			m_MouseStateTrigger.rgbButtons[nCntMouse] = (m_MouseStatePress.rgbButtons[nCntMouse] ^ m_MouseState.rgbButtons[nCntMouse]) & m_MouseState.rgbButtons[nCntMouse];		//トリガー
			m_MouseStatePress.rgbButtons[nCntMouse] = m_MouseState.rgbButtons[nCntMouse];						//入力情報保存
		}

		m_MouseIX = m_MouseState.lX;
		m_MouseIY = m_MouseState.lY;
		m_MouseIZ = m_MouseState.lZ;
	}
	else
	{
		ret = m_pMouse->Acquire();	//キーボードへのアクセス権を獲得
	}

	GetScreenToWorld();
}

//=============================================================================
// リリース処理
//=============================================================================
bool CInputMouse::GetRelease(int nKey)
{
	if (m_pMouse == NULL) { return false; }
	return (m_MouseStateRelease.rgbButtons[nKey] & 0x80) ? true : false;
}

//=============================================================================
// トリガー処理
//=============================================================================
bool CInputMouse::GetTrigger(int nKey)
{
	if (m_pMouse == NULL) { return false; }
	return (m_MouseStateTrigger.rgbButtons[nKey] & 0x80) ? true : false;
}

//=============================================================================
// プレス処理
//=============================================================================
bool CInputMouse::GetPress(int nKey)
{
	if (m_pMouse == NULL) { return false; }
	return (m_MouseStatePress.rgbButtons[nKey] & 0x80) ? true : false;
}

//=============================================================================
// X軸の処理
//=============================================================================
LONG CInputMouse::GetIX(void)
{
	return m_MouseIX;
}

//=============================================================================
// Y軸の処理
//=============================================================================
LONG CInputMouse::GetIY(void)
{
	return m_MouseIY;
}

//=============================================================================
// Z軸の処理
//=============================================================================
LONG CInputMouse::GetIZ(void)
{
	return m_MouseIZ;
}

//=============================================================================
// スクリーン座標からワールド座標
//=============================================================================
void CInputMouse::GetScreenToWorld(void)
{
	//変数宣言
	D3DXMATRIX mtxView, mtxProjection, mtxViewPort, mtxMouseWorld;

	//マウスの座標を取得
	GetCursorPos(&m_ScreenPos);
	ScreenToClient(m_hWnd, &m_ScreenPos);

	CGame *pGame = CManager::GetGame();
	int nMode = CManager::GetMode();

	//if (nMode == CManager::MODE_GAME)
	//{

	//	//ビュー・プロジェクションの取得
	//	CGameCamera * pCamera = pGame->GetGameCamera(0);
	//	mtxView = pCamera->GetMtxView();
	//	mtxProjection = pCamera->GetMtxProjection();

	//	//ビュー、プロジェクションの逆行列を求める
	//	D3DXMatrixInverse(&mtxView, NULL, &mtxView);
	//	D3DXMatrixInverse(&mtxProjection, NULL, &mtxProjection);

	//	//ビューポートを求める
	//	D3DXMatrixIdentity(&mtxViewPort);
	//	mtxViewPort._11 = SCREEN_WIDTH / 2.0f;
	//	mtxViewPort._41 = SCREEN_WIDTH / 2.0f;
	//	mtxViewPort._22 = -SCREEN_HEIGHT / 2.0f;
	//	mtxViewPort._42 = SCREEN_HEIGHT / 2.0f;

	//	//ビューポートの逆行列を求める
	//	D3DXMatrixInverse(&mtxViewPort, NULL, &mtxViewPort);

	//	//ワールド座標に変換
	//	D3DXMATRIX mtxWorld = mtxViewPort * mtxProjection * mtxView;
	//	D3DXVec3TransformCoord(&m_WorldPos, &D3DXVECTOR3((float)m_ScreenPos.x, (float)m_ScreenPos.y, 1.0f), &mtxWorld);
	//}
}