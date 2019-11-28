//=============================================================================
//
// ���揈�� [movie.cpp]
// Author : ���������@SATOU ASUMI
//
//=============================================================================
#ifndef _MOVIE_H_
#define _MOVIE_H_

#include "main.h"
#include <dshow.h>

//=============================================================================
// ����N���X
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
	static bool GetbRunMovie(void) { return m_bRunMovie; }

private:
	static bool m_bRunMovie;

	IGraphBuilder * m_pGraphBuilder;	//����𐶐�
	IMediaControl * m_pMediaControl;	//����̃R���g���[���[
	IMediaEvent   * m_pMediaEvent;		//����̏��
	bool m_bEndMovie;					//����̍Đ��t���O
};

#endif