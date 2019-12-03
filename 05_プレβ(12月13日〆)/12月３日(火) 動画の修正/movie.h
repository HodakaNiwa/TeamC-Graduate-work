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
#include <Vmr9.h>
#pragma comment(lib, "strmiids.lib")	//����Đ��ɕK�v�ȃ��C�u����


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
	void SetEndMovie(bool bEndMovie) { m_bEndMovie = bEndMovie; }
	static bool GetbRunMovie(void) { return m_bRunMovie; }

private:
	static bool m_bRunMovie;

	IGraphBuilder * m_pGraphBuilder;	//����𐶐�
	IMediaControl * m_pMediaControl;	//����̃R���g���[���[
	IMediaEvent   * m_pMediaEvent;		//����̏��
	ICaptureGraphBuilder2 * m_pCGB2;	//�t�B���^���m���\�z����
	IBaseFilter * m_pVMR9;				//�x�[�X�t�B���^�[
	IBaseFilter * m_pSource;			//�x�[�X�t�B���^�[
	IVMRWindowlessControl9 * m_pVMRWndCont;	//�E�C���h�E���X���[�h�̃R���g���[���[
	bool m_bEndMovie;					//����̍Đ��t���O
};

#endif