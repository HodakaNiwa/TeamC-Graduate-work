//=============================================================================
//
// メイン処理 [main.cpp]
// Author : 佐藤安純　SATOU ASUMI
//
//=============================================================================
#include "main.h"
#include "manager.h"
#include "RawMouse.h"
#include "debuglog.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define CLASS_NAME		"AppClass"									// ウインドウのクラス名
#define WINDOW_NAME		"領地奪取 ～テリトリーダッシュ～"			// ウインドウのキャプション名
#define WINDOW_MODE     (false)										// ウィンドウのモード(FALSE)

//*****************************************************************************
// グローバル変数
//*****************************************************************************
int g_nFps;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

//=============================================================================
// メイン関数
//=============================================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	//変数宣言
	g_nFps = 0;

	//メモリリークチェック
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	//インスタンスの生成
	CManager * pManager = NULL;

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WndProc,
		0,
		0,
		hInstance,
		LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1)),
		LoadCursor(NULL, IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		NULL,
		CLASS_NAME,
		(HICON)LoadImage(hInstance,  MAKEINTRESOURCE(IDI_ICON1),  IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_SHARED)
	};

	RECT rect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
	HWND hWnd;
	MSG msg;
	DWORD dwCurrentTime;
	DWORD dwFrameCount;
	DWORD dwExecLastTime;
	DWORD dwFPSLastTime;
	dwCurrentTime = 0;
	dwFrameCount = timeGetTime();
	srand((unsigned int)time(0));

	// ウィンドウクラスの登録
	RegisterClassEx(&wcex);

	// 指定したクライアント領域を確保するために必要なウィンドウ座標を計算
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

	// ウィンドウの作成
	hWnd = CreateWindow(CLASS_NAME,
						WINDOW_NAME,
						WS_OVERLAPPEDWINDOW,
						CW_USEDEFAULT,
						CW_USEDEFAULT,
						(rect.right - rect.left),
						(rect.bottom - rect.top),
						NULL,
						NULL,
						hInstance,
						NULL);

	if (pManager == NULL)
	{
		pManager = new CManager();	//マネージャークラスの生成

		// 初期化処理
		if (FAILED(pManager->Init(hInstance, hWnd, WINDOW_MODE))) //FALSEで全画面
		{
			return-1;
		}

		// 分解能を設定
		timeBeginPeriod(1);

		// フレームカウント初期化
		dwCurrentTime =
			dwFrameCount = 0;
		dwExecLastTime =
			dwFPSLastTime = timeGetTime();

		// ウインドウの表示
		ShowWindow(hWnd, nCmdShow);
		UpdateWindow(hWnd);

		// メッセージループ
		while (1)
		{
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				if (msg.message == WM_QUIT)
				{// PostQuitMessage()が呼ばれたらループ終了
					break;
				}
				else
				{
					// メッセージの翻訳とディスパッチ
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
			}
			else
			{
				//RawInputにデバイスを登録
				CRawMouse::RegisterRawInput(hWnd);

				if (WINDOW_MODE == false)
				{//マウスカーソルを消す
					ShowCursor(FALSE);
				}
				dwCurrentTime = timeGetTime();	// 現在の時間を取得
				if ((dwCurrentTime - dwFPSLastTime) >= 500)
				{// 0.5秒ごとに実行

				// FPSを算出
					g_nFps = dwFrameCount * 1000 / (dwCurrentTime - dwFPSLastTime);

					dwFPSLastTime = dwCurrentTime;	// 現在の時間を保存
					dwFrameCount = 0;
				}

				if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
				{// 1/60秒経過
					dwExecLastTime = dwCurrentTime;	// 現在の時間を保存
					CDebugProc::Print("FPS : %d\n", g_nFps);

					// RawInputのデータを調整
					if (CManager::GetRawMouse() != NULL){ pManager->GetRawMouse()->AdjustMouseData(); }

					// 更新処理
					pManager->Update();

					// 描画処理
					pManager->Draw();

					// RawInputのデータをクリア
					if (CManager::GetRawMouse() != NULL) { CManager::GetRawMouse()->ClearMouseData(); }

					dwFrameCount++;
				}
			}
		}

		// 終了処理
		if (pManager != NULL)
		{
			pManager->Uninit();

			delete pManager;
			pManager = NULL;
		}
	}
	else
	{
		MessageBox(NULL,											// ウィンドウ・ハンドル
				   TEXT(" pManager が NULL になっていません"),      // メッセージボックスのテキスト
			       TEXT("WinMain"),								    // メッセージボックスのタイトル
			       MB_OK);											// メッセージボックスのスタイル
	}

	// ウィンドウクラスの登録を解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	// 分解能を戻す
	timeEndPeriod(1);

	return (int)msg.wParam;
}

//=============================================================================
// ウインドウプロシージャ
//=============================================================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_CREATE:
		//　RawMouseの生成
		CManager::CreateRawMouse(hWnd);
		break;

	case WM_DEVICECHANGE:
		CManager::ReCreateRawMouse(hWnd);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch(wParam)
		{
		case VK_ESCAPE:				// [ESC]キーが押された
			DestroyWindow(hWnd);	// ウィンドウを破棄するよう指示する
			break;
		case VK_DELETE:				// デリートが押されたら破棄する
			DestroyWindow(hWnd);	// ウィンドウを破棄するよう指示する
			break;
		}
		break;

	case WM_INPUT:
		// RawMouseの更新
		CManager::GetRawMouse()->Update(lParam);
		break;

	default:
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

//=============================================================================
// FPSの取得
//=============================================================================
int GetFPS(void)
{
	return g_nFps;
}