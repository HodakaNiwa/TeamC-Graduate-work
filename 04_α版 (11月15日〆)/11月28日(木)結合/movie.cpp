//=============================================================================
//
// ���揈�� [movie.cpp]
// Author : ���������@SATOU ASUMI
//
//=============================================================================
#include "movie.h"
#include <Vmr9.h>
#pragma comment(lib, "strmiids.lib")	//����Đ��ɕK�v�ȃ��C�u����

#include "manager.h"

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================
bool CMovie::m_bRunMovie = false;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CMovie::CMovie(){}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CMovie::~CMovie(){}

//=============================================================================
// ��������
//=============================================================================
CMovie * CMovie::Create(WCHAR * pFileName)
{
	//�C���X�^���X����
	CMovie * pMovie = new CMovie;
	pMovie->Init(pFileName);

	return pMovie;
}

//=============================================================================
// ����������
//=============================================================================
void CMovie::Init(WCHAR * pFileName)
{
	//����������
	m_pGraphBuilder = NULL;
	m_pMediaControl = NULL;
	m_pMediaEvent = NULL;
	m_bEndMovie = false;

	// COM���C�u�����̏�����
	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	HRESULT hr;
	HWND hWnd = CManager::GethWnd();

	// �t�B���^ �O���t �}�l�[�W�����쐬���A�C���^�[�t�F�C�X��₢���킹��B
	hr = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (void **)&m_pGraphBuilder);
	if (FAILED(hr)) { MessageBox(hWnd, TEXT("No_Create_FilterGraph"), TEXT("ERROR"), MB_OK); }

	// VRM9�t�B���^�̍쐬�Ɠo�^
	IBaseFilter *pVMR9 = NULL;
	hr = CoCreateInstance(CLSID_VideoMixingRenderer9, NULL, CLSCTX_INPROC_SERVER, IID_IBaseFilter, (void**)&pVMR9);

	if (FAILED(hr))
	{
		MessageBox(hWnd, TEXT("No_Create_VRM9�t�B���^"), TEXT("ERROR"), MB_OK);
	}
	else
	{
		if (m_pGraphBuilder != NULL) { m_pGraphBuilder->AddFilter(pVMR9, L"VMR9"); }     // �t�B���^�O���t�ɓo�^
	}

	// VRM9���E�B���h�E���X���[�h�ɂ���
	IVMRFilterConfig *pVMRCfg = NULL;
	hr = pVMR9->QueryInterface(IID_IVMRFilterConfig9, (void**)&pVMRCfg);

	if (FAILED(hr))
	{
		MessageBox(hWnd, TEXT("No_Set_WindowLess"), TEXT("ERROR"), MB_OK);
	}
	else
	{
		pVMRCfg->SetRenderingMode(VMRMode_Windowless);
		pVMRCfg->Release();								// IVMRFilterConfig�͂����K�v�Ȃ�
	}

	// �`��E�B���h�E�̎w��
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

	// �\�[�X�t�B���^�̐����Ɠo�^
	IBaseFilter *pSource = NULL;
	m_pGraphBuilder->AddSourceFilter(pFileName, pFileName, &pSource);

	// CaptureGraphBuilder2�C���^�[�t�F�C�X�̎擾
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

	// �t�B���^�̐ڑ�
	hr = pCGB2->RenderStream(0, 0, pSource, 0, pVMR9);

	if (FAILED(hr))
	{
		MessageBox(hWnd, TEXT("No_Set_RenderStream"), TEXT("ERROR"), MB_OK);
	}
	else
	{
		hr = pCGB2->RenderStream(0, &MEDIATYPE_Audio, pSource, 0, 0);
	}

	// �`��̈�̐ݒ�i�ڑ���łȂ��ƃG���[�ɂȂ�j
	LONG Width = SCREEN_WIDTH;
	LONG Hight = SCREEN_HEIGHT;
	RECT SrcR, DestR;

	hr = pVMRWndCont->GetNativeVideoSize(&Width, &Hight, NULL, NULL);
	SetRect(&SrcR, 0, 0, Width, Hight);
	GetClientRect(hWnd, &DestR);

	hr = pVMRWndCont->SetVideoPosition(&SrcR, &DestR);
	pVMRWndCont->Release();								 // �E�B���h�E���X�R���g���[���͂����K�v�Ȃ�

	hr = m_pGraphBuilder->QueryInterface(IID_IMediaControl, (void **)&m_pMediaControl);	//����̃R���g���[���[�̐���
	hr = m_pGraphBuilder->QueryInterface(IID_IMediaEvent, (void **)&m_pMediaEvent);		//����̃C�x���g�𐶐�

	if (SUCCEEDED(hr))
	{// ��������s����B
		hr = m_pMediaControl->Run();
		m_bRunMovie = true;
	}

	/*pVMR9->Release();
	pSource->Release();*/
}

//=============================================================================
// �I������
//=============================================================================
void CMovie::Uninit(void)
{
	//����̒�~�t���O
	m_bRunMovie = false;

	if (m_pMediaControl != NULL)
	{//����R���g���[���[�̔j��

		if (!m_bEndMovie)
		{//�Đ����������瓮����~�߂�
			m_pMediaControl->Stop();
		}
		// �O���t�̃t�B���^���
		IEnumFilters *pEnum = NULL;
		HRESULT hr = m_pGraphBuilder->EnumFilters(&pEnum);
		if (SUCCEEDED(hr))
		{
			IBaseFilter *pFilter = NULL;
			while (S_OK == pEnum->Next(1, &pFilter, NULL))
			{
				// �t�B���^���폜����
				m_pGraphBuilder->RemoveFilter(pFilter);
				// �񋓎q�����Z�b�g����
				pEnum->Reset();
				pFilter->Release();
			}
			pEnum->Release();
		}

		m_pMediaControl->Release();
		m_pMediaControl = NULL;
	}

	if (m_pMediaEvent != NULL)
	{//����̏��
		m_pMediaEvent->Release();
		m_pMediaEvent = NULL;
	}

	if (m_pGraphBuilder != NULL)
	{//�����j��
		m_pGraphBuilder->Release();
		m_pGraphBuilder = NULL;
	}

	//�j������
	CoUninitialize();
}

//=============================================================================
// �X�V����
//=============================================================================
void CMovie::Update(void)
{
	long lEventCode;
	LONG_PTR lEvParam1, lEvParam2;

	//�C�x���g���擾
	HRESULT hr = m_pMediaEvent->GetEvent(&lEventCode, &lEvParam1, &lEvParam2, 0);
	if (hr == S_OK)
	{
		//�Đ��I���ł������Ƃ��t���O�𗧂Ă�
		if (lEventCode == EC_COMPLETE)
		{ 
			m_bEndMovie = true;		//����̍Đ��I��
			m_bRunMovie = false;	//����Đ����Ă��Ȃ����
		}

		//�C�x���g���폜
		m_pMediaEvent->FreeEventParams(lEventCode, lEvParam1, lEvParam2);
	}
}