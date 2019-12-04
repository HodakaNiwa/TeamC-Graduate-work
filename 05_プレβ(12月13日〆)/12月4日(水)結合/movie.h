//=============================================================================
//
// 動画処理 [movie.cpp]
// Author : 佐藤安純　SATOU ASUMI
//
//=============================================================================
#ifndef _MOVIE_H_
#define _MOVIE_H_

#include "main.h"
#include <dshow.h>
#include <Vmr9.h>
#pragma comment(lib, "strmiids.lib")	//動画再生に必要なライブラリ


//=============================================================================
// 動画クラス
//=============================================================================
class CMovie
{
public:
	CMovie();
	~CMovie();
	static CMovie * Create(WCHAR * pFileName);
	void Init(WCHAR * pFileName);
	void Uninit(void);
	void Update(void);
	
	bool GetEndMovie(void) { return m_bEndMovie; }
	void SetEndMovie(bool bEndMovie) { m_bEndMovie = bEndMovie; }
	static bool GetbRunMovie(void) { return m_bRunMovie; }

private:
	static bool m_bRunMovie;

	IGraphBuilder * m_pGraphBuilder;	//動画を生成
	IMediaControl * m_pMediaControl;	//動画のコントローラー
	IMediaEvent   * m_pMediaEvent;		//動画の状態
	ICaptureGraphBuilder2 * m_pCGB2;	//フィルタ同士を構築する
	IBaseFilter * m_pVMR9;				//ベースフィルター
	IBaseFilter * m_pSource;			//ベースフィルター
	IVMRWindowlessControl9 * m_pVMRWndCont;	//ウインドウレスモードのコントローラー
	bool m_bEndMovie;					//動画の再生フラグ
};

#endif