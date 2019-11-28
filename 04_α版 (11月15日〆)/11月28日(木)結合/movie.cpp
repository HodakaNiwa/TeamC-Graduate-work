//=============================================================================
//
// 動画処理 [movie.cpp]
// Author : 佐藤安純　SATOU ASUMI
//
//=============================================================================
#include "movie.h"
#include <Vmr9.h>
#pragma comment(lib, "strmiids.lib")	//動画再生に必要なライブラリ

#include "manager.h"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
bool CMovie::m_bRunMovie = false;

//=============================================================================
// コンストラクタ
//=============================================================================
CMovie::CMovie(){}

//=============================================================================
// デストラクタ
//=============================================================================
CMovie::~CMovie(){}

//=============================================================================
// 生成処理
//=============================================================================
CMovie * CMovie::Create(WCHAR * pFileName)
{
	//インスタンス生成
	CMovie * pMovie = new CMovie;
	pMovie->Init(pFileName);

	return pMovie;
}

//=============================================================================
// 初期化処理
//=============================================================================
void CMovie::Init(WCHAR * pFileName)
{
	//初期化処理
	m_pGraphBuilder = NULL;
	m_pMediaControl = NULL;
	m_pMediaEvent = NULL;
	m_bEndMovie = false;

	// COMライブラリの初期化
	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	HRESULT hr;
	HWND hWnd = CManager::GethWnd();

	// フィルタ グラフ マネージャを作成し、インターフェイスを問い合わせる。
	hr = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (void **)&m_pGraphBuilder);
	if (FAILED(hr)) { MessageBox(hWnd, TEXT("No_Create_FilterGraph"), TEXT("ERROR"), MB_OK); }

	// VRM9フィルタの作成と登録
	IBaseFilter *pVMR9 = NULL;
	hr = CoCreateInstance(CLSID_VideoMixingRenderer9, NULL, CLSCTX_INPROC_SERVER, IID_IBaseFilter, (void**)&pVMR9);

	if (FAILED(hr))
	{
		MessageBox(hWnd, TEXT("No_Create_VRM9フィルタ"), TEXT("ERROR"), MB_OK);
	}
	else
	{
		if (m_pGraphBuilder != NULL) { m_pGraphBuilder->AddFilter(pVMR9, L"VMR9"); }     // フィルタグラフに登録
	}

	// VRM9をウィンドウレスモードにする
	IVMRFilterConfig *pVMRCfg = NULL;
	hr = pVMR9->QueryInterface(IID_IVMRFilterConfig9, (void**)&pVMRCfg);

	if (FAILED(hr))
	{
		MessageBox(hWnd, TEXT("No_Set_WindowLess"), TEXT("ERROR"), MB_OK);
	}
	else
	{
		pVMRCfg->SetRenderingMode(VMRMode_Windowless);
		pVMRCfg->Release();								// IVMRFilterConfigはもう必要ない
	}

	// 描画ウィンドウの指定
	IVMRWindowlessControl9 *pVMRWndCont = NULL;
	hr = pVMR9->QueryInterface(IID_IVMRWindowlessControl9, (void**)&pVMRWndCont);

	if (FAILED(hr))
	{
		MessageBox(hWnd, TEXT("No_Set_Window"), TEXT("ERROR"), MB_OK);
	}
	else
	{
		hr = pVMRWndCont->SetVideoClippingWindow(hWnd);
	}

	// ソースフィルタの生成と登録
	IBaseFilter *pSource = NULL;
	m_pGraphBuilder->AddSourceFilter(pFileName, pFileName, &pSource);

	// CaptureGraphBuilder2インターフェイスの取得
	ICaptureGraphBuilder2 *pCGB2 = NULL;
	hr = CoCreateInstance(CLSID_CaptureGraphBuilder2, NULL, CLSCTX_INPROC_SERVER, IID_ICaptureGraphBuilder2, (void**)&pCGB2);

	if (FAILED(hr))
	{
		MessageBox(hWnd, TEXT("No_Create_Graph2"), TEXT("ERROR"), MB_OK);
	}
	else
	{
		pCGB2->SetFiltergraph(m_pGraphBuilder);
	}

	// フィルタの接続
	hr = pCGB2->RenderStream(0, 0, pSource, 0, pVMR9);

	if (FAILED(hr))
	{
		MessageBox(hWnd, TEXT("No_Set_RenderStream"), TEXT("ERROR"), MB_OK);
	}
	else
	{
		hr = pCGB2->RenderStream(0, &MEDIATYPE_Audio, pSource, 0, 0);
	}

	// 描画領域の設定（接続後でないとエラーになる）
	LONG Width = SCREEN_WIDTH;
	LONG Hight = SCREEN_HEIGHT;
	RECT SrcR, DestR;

	hr = pVMRWndCont->GetNativeVideoSize(&Width, &Hight, NULL, NULL);
	SetRect(&SrcR, 0, 0, Width, Hight);
	GetClientRect(hWnd, &DestR);

	hr = pVMRWndCont->SetVideoPosition(&SrcR, &DestR);
	pVMRWndCont->Release();								 // ウィンドウレスコントロールはもう必要ない

	hr = m_pGraphBuilder->QueryInterface(IID_IMediaControl, (void **)&m_pMediaControl);	//動画のコントローラーの生成
	hr = m_pGraphBuilder->QueryInterface(IID_IMediaEvent, (void **)&m_pMediaEvent);		//動画のイベントを生成

	if (SUCCEEDED(hr))
	{// 動画を実行する。
		hr = m_pMediaControl->Run();
		m_bRunMovie = true;
	}

	/*pVMR9->Release();
	pSource->Release();*/
}

//=============================================================================
// 終了処理
//=============================================================================
void CMovie::Uninit(void)
{
	//動画の停止フラグ
	m_bRunMovie = false;

	if (m_pMediaControl != NULL)
	{//動画コントローラーの破棄

		if (!m_bEndMovie)
		{//再生中だったら動画を止める
			m_pMediaControl->Stop();
		}
		// グラフのフィルタを列挙
		IEnumFilters *pEnum = NULL;
		HRESULT hr = m_pGraphBuilder->EnumFilters(&pEnum);
		if (SUCCEEDED(hr))
		{
			IBaseFilter *pFilter = NULL;
			while (S_OK == pEnum->Next(1, &pFilter, NULL))
			{
				// フィルタを削除する
				m_pGraphBuilder->RemoveFilter(pFilter);
				// 列挙子をリセットする
				pEnum->Reset();
				pFilter->Release();
			}
			pEnum->Release();
		}

		m_pMediaControl->Release();
		m_pMediaControl = NULL;
	}

	if (m_pMediaEvent != NULL)
	{//動画の状態
		m_pMediaEvent->Release();
		m_pMediaEvent = NULL;
	}

	if (m_pGraphBuilder != NULL)
	{//動画を破棄
		m_pGraphBuilder->Release();
		m_pGraphBuilder = NULL;
	}

	//破棄する
	CoUninitialize();
}

//=============================================================================
// 更新処理
//=============================================================================
void CMovie::Update(void)
{
	long lEventCode;
	LONG_PTR lEvParam1, lEvParam2;

	//イベントを取得
	HRESULT hr = m_pMediaEvent->GetEvent(&lEventCode, &lEvParam1, &lEvParam2, 0);
	if (hr == S_OK)
	{
		//再生終了であったときフラグを立てる
		if (lEventCode == EC_COMPLETE)
		{ 
			m_bEndMovie = true;		//動画の再生終了
			m_bRunMovie = false;	//動画再生していない状態
		}

		//イベントを削除
		m_pMediaEvent->FreeEventParams(lEventCode, lEvParam1, lEvParam2);
	}
}