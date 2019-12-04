//=============================================================================
//
// ���揈�� [movie.cpp]
// Author : ���������@SATOU ASUMI
//
//=============================================================================
#include "movie.h"
#include "manager.h"

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================
bool CMovie::m_bRunMovie = false;

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
// �R���X�g���N�^
//=============================================================================
CMovie::CMovie() {}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CMovie::~CMovie() {}


//=============================================================================
// ����������
//=============================================================================
void CMovie::Init(WCHAR * pFileName)
{
	//����������
	m_pGraphBuilder = NULL;
	m_pMediaControl = NULL;
	m_pMediaEvent = NULL;
	m_pSource = NULL;
	m_pCGB2 = NULL;
	m_pVMR9 = NULL;
	m_pVMRWndCont = NULL;
	m_bEndMovie = false;

	// COM���C�u�����̏�����
	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	HRESULT hr;
	HWND hWnd = CManager::GethWnd();

	// �t�B���^ �O���t �}�l�[�W�����쐬���A�C���^�[�t�F�C�X��₢���킹��B
	hr = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (void **)&m_pGraphBuilder);
	if (FAILED(hr)) { MessageBox(hWnd, TEXT("No_Create_FilterGraph"), TEXT("ERROR"), MB_OK); }

	// VRM9�t�B���^�̍쐬�Ɠo�^
	//IBaseFilter *pVMR9 = NULL;
	hr = CoCreateInstance(CLSID_VideoMixingRenderer9, NULL, CLSCTX_INPROC_SERVER, IID_IBaseFilter, (void**)&/*pVMR9*/m_pVMR9);

	if (FAILED(hr))
	{
		MessageBox(hWnd, TEXT("No_Create_VRM9�t�B���^"), TEXT("ERROR"), MB_OK);
	}
	else
	{
		if (m_pGraphBuilder != NULL) { m_pGraphBuilder->AddFilter(/*pVMR9*/m_pVMR9, L"VMR9"); }     // �t�B���^�O���t�ɓo�^
	}

	// VRM9���E�B���h�E���X���[�h�ɂ���
	IVMRFilterConfig *pVMRCfg = NULL;
	hr = m_pVMR9->QueryInterface(IID_IVMRFilterConfig9, (void**)&pVMRCfg);
	//hr = pVMR9->QueryInterface(IID_IVMRFilterConfig9, (void**)&pVMRCfg);

	if (FAILED(hr))
	{
		MessageBox(hWnd, TEXT("No_Set_WindowLess"), TEXT("ERROR"), MB_OK);
	}
	else
	{
		pVMRCfg->SetRenderingMode(VMRMode_Windowless);
		pVMRCfg->Release();								// IVMRFilterConfig�͂����K�v�Ȃ�
		pVMRCfg = NULL;
	}

	// �`��E�B���h�E�̎w��
	//IVMRWindowlessControl9 *pVMRWndCont = NULL;
	hr = m_pVMR9->QueryInterface(IID_IVMRWindowlessControl9, (void**)&m_pVMRWndCont);
	//hr = pVMR9->QueryInterface(IID_IVMRWindowlessControl9, (void**)&pVMRWndCont);

	if (FAILED(hr))
	{
		MessageBox(hWnd, TEXT("No_Set_Window"), TEXT("ERROR"), MB_OK);
	}
	else
	{
		hr = m_pVMRWndCont->SetVideoClippingWindow(hWnd);
		//hr = pVMRWndCont->SetVideoClippingWindow(hWnd);
	}

	// �\�[�X�t�B���^�̐����Ɠo�^
	//IBaseFilter *pSource = NULL;
	m_pGraphBuilder->AddSourceFilter(pFileName, pFileName, &/*pSource*/m_pSource);

	// CaptureGraphBuilder2�C���^�[�t�F�C�X�̎擾
	hr = CoCreateInstance(CLSID_CaptureGraphBuilder2, NULL, CLSCTX_INPROC_SERVER, IID_ICaptureGraphBuilder2, (void**)&/*pCGB2*/m_pCGB2);

	if (FAILED(hr))
	{
		MessageBox(hWnd, TEXT("No_Create_Graph2"), TEXT("ERROR"), MB_OK);
	}
	else
	{
		m_pCGB2->SetFiltergraph(m_pGraphBuilder);
	}

	// �t�B���^�̐ڑ�
	hr = m_pCGB2->RenderStream(0, 0, /*pSource*/m_pSource, 0, /*pVMR9*/m_pVMR9);

	if (FAILED(hr))
	{
		MessageBox(hWnd, TEXT("No_Set_RenderStream"), TEXT("ERROR"), MB_OK);
	}
	else
	{
		hr = m_pCGB2->RenderStream(0, &MEDIATYPE_Audio, /*pSource*/m_pSource, 0, 0);
	}

	// �`��̈�̐ݒ�i�ڑ���łȂ��ƃG���[�ɂȂ�j
	LONG Width = SCREEN_WIDTH;
	LONG Hight = SCREEN_HEIGHT;
	RECT SrcR = {0, 0, 0, 0};
	RECT DestR = { 0, 0, 0, 0 };

	hr = m_pVMRWndCont->GetNativeVideoSize(&Width, &Hight, NULL, NULL);
	//hr = pVMRWndCont->GetNativeVideoSize(&Width, &Hight, NULL, NULL);
	SetRect(&SrcR, 0, 0, Width, Hight);
	GetClientRect(hWnd, &DestR);

	hr = m_pVMRWndCont->SetVideoPosition(&SrcR, &DestR);
	//hr = pVMRWndCont->SetVideoPosition(&SrcR, &DestR);

	// IGraphBuilder���擾
	//CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC, IID_IGraphBuilder, (LPVOID *)&m_pGraphBuilder);

	hr = m_pGraphBuilder->QueryInterface(IID_IMediaControl, (void **)&m_pMediaControl);	//����̃R���g���[���[�̐���
	hr = m_pGraphBuilder->QueryInterface(IID_IMediaEvent, (void **)&m_pMediaEvent);		//����̃C�x���g�𐶐�

	//m_pMediaControl->RenderFile((BSTR)pFileName);

	if (SUCCEEDED(hr))
	{// ��������s����B
		hr = m_pMediaControl->Run();
		m_bRunMovie = true;
	}

	//pVMRWndCont->Release();		 // �E�B���h�E���X�R���g���[���͂����K�v�Ȃ�
	//pVMRWndCont = NULL;
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

		//�Đ����������瓮����~�߂�
		m_pMediaControl->Stop();
		
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

	if (m_pVMR9 != NULL)
	{//�x�[�X�t�B���^�[�̔j��
		m_pVMR9->Release();
		m_pVMR9 = NULL;
	}

	if (m_pSource != NULL)
	{//�x�[�X�t�B���^�[�̔j��
		m_pSource->Release();
		m_pSource = NULL;
	}
	
	if (m_pCGB2 != NULL)
	{//�t�B���^�[���m�̍\�z��j��
		m_pCGB2->Release();
		m_pCGB2 = NULL;
	}

	if (m_pGraphBuilder != NULL)
	{//�����j��
		m_pGraphBuilder->Release();
		m_pGraphBuilder = NULL;
	}

	if (m_pVMRWndCont != NULL)
	{
		m_pVMRWndCont->Release();
		m_pVMRWndCont = NULL;
	}

	//�j������
	CoUninitialize();
}

//=============================================================================
// �X�V����
//=============================================================================
void CMovie::Update(void)
{
	long lEventCode = 0;
	LONG_PTR lEvParam1 = 0;
	LONG_PTR lEvParam2 = 0;

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
	}

	//�C�x���g���폜
	m_pMediaEvent->FreeEventParams(lEventCode, lEvParam1, lEvParam2);
}