//=============================================================================
//
// ���C������ [main.cpp]
// Author : ���������@SATOU ASUMI
//
//=============================================================================
#include "main.h"
#include "manager.h"
#include "RawMouse.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define CLASS_NAME		"AppClass"			// �E�C���h�E�̃N���X��
#define WINDOW_NAME		"�̒n�D�� �`�e���g���[�_�b�V���`"			// �E�C���h�E�̃L���v�V������

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
int g_nFps;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);	//imgui�p

//=============================================================================
// ���C���֐�
//=============================================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	//�ϐ��錾
	g_nFps = 0;

	//���������[�N�`�F�b�N
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	//�C���X�^���X�̐���
	CManager * pManager = NULL;

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WndProc,
		0,
		0,
		hInstance,
		NULL,
		LoadCursor(NULL, IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		NULL,
		CLASS_NAME,
		NULL
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

	// �E�B���h�E�N���X�̓o�^
	RegisterClassEx(&wcex);

	// �w�肵���N���C�A���g�̈���m�ۂ��邽�߂ɕK�v�ȃE�B���h�E���W���v�Z
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

	// �E�B���h�E�̍쐬
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
		pManager = new CManager();	//�}�l�[�W���[�N���X�̐���
		
		// ����������
		if (FAILED(pManager->Init(hInstance, hWnd, TRUE))) //FALSE�őS���
		{
			return-1;
		}

		// ����\��ݒ�
		timeBeginPeriod(1);

		// �t���[���J�E���g������
		dwCurrentTime =
			dwFrameCount = 0;
		dwExecLastTime =
			dwFPSLastTime = timeGetTime();

		// �E�C���h�E�̕\��
		ShowWindow(hWnd, nCmdShow);
		UpdateWindow(hWnd);

		// ���b�Z�[�W���[�v
		while (1)
		{
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				if (msg.message == WM_QUIT)
				{// PostQuitMessage()���Ă΂ꂽ�烋�[�v�I��
					break;
				}
				else
				{
					// ���b�Z�[�W�̖|��ƃf�B�X�p�b�`
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
			}
			else
			{
				//RawInput�Ƀf�o�C�X��o�^
				CRawMouse::RegisterRawInput(hWnd);

				dwCurrentTime = timeGetTime();	// ���݂̎��Ԃ��擾
				if ((dwCurrentTime - dwFPSLastTime) >= 500)
				{// 0.5�b���ƂɎ��s

#ifdef _DEBUG
				// FPS���Z�o
					g_nFps = dwFrameCount * 1000 / (dwCurrentTime - dwFPSLastTime);
#endif
					dwFPSLastTime = dwCurrentTime;	// ���݂̎��Ԃ�ۑ�
					dwFrameCount = 0;
				}

				if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
				{// 1/60�b�o��
					dwExecLastTime = dwCurrentTime;	// ���݂̎��Ԃ�ۑ�

					//�@RawInput�̃f�[�^�𒲐�
					pManager->GetRawMouse()->AdjustMouseData();

					// �X�V����
					pManager->Update();

					// �`�揈��
					pManager->Draw();

					if (CManager::GetRawMouse() != NULL) { CManager::GetRawMouse()->ClearMouseData(); }

					dwFrameCount++;
				}
			}
		}

		// �I������
		if (pManager != NULL)
		{
			pManager->Uninit();

			delete pManager;
			pManager = NULL;
		}
	}
	else
	{
		MessageBox(NULL,											// �E�B���h�E�E�n���h��
				   TEXT(" pManager �� NULL �ɂȂ��Ă��܂���"),      // ���b�Z�[�W�{�b�N�X�̃e�L�X�g
			       TEXT("WinMain"),								    // ���b�Z�[�W�{�b�N�X�̃^�C�g��
			       MB_OK);											// ���b�Z�[�W�{�b�N�X�̃X�^�C��
	}

	// �E�B���h�E�N���X�̓o�^������
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	// ����\��߂�
	timeEndPeriod(1);

	return (int)msg.wParam;
}

//=============================================================================
// �E�C���h�E�v���V�[�W��
//=============================================================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
	{
		return true;
	}

	switch(uMsg)
	{
	case WM_CREATE:
		//�@RawMouse�̐���
		CManager::CreateRawMouse(hWnd);
		CManager::GetRawMouse()->Init(hWnd);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch(wParam)
		{
		case VK_ESCAPE:				// [ESC]�L�[�������ꂽ
			DestroyWindow(hWnd);	// �E�B���h�E��j������悤�w������
			break;
		}
		break;

	case WM_INPUT:
		// RawMouse�̍X�V
		CManager::GetRawMouse()->Update(lParam);
		break;

	default:
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

//=============================================================================
// FPS�̎擾
//=============================================================================
int GetFPS(void)
{
	return g_nFps;
}