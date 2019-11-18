//=============================================================================
//
// �}�E�X���͏��� [inputmouce.cpp]
// Author : ���������@SATOU ASUMI
//
//=============================================================================
#include "inputmouce.h"
#include "manager.h"
#include "camera.h"
#include "game.h"

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================
LPDIRECTINPUTDEVICE8 CInputMouse::m_pMouse = NULL;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CInputMouse::CInputMouse()
{
	m_pMouse = NULL;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CInputMouse::~CInputMouse(){}

//=============================================================================
// ����������
//=============================================================================
HRESULT CInputMouse::Init(HINSTANCE hInstance, HWND hWnd)
{
	//�ϐ��錾
	HRESULT ret = S_FALSE;

	//��ʏ�Ԃ̕ۑ�
	m_hWnd = hWnd;

	//�ϐ��̏�����
	m_WorldPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_MouseIX = 0;
	m_MouseIY = 0;
	m_MouseIZ = 0;

	//����������
	CInput::Init(hInstance, hWnd);

	//�ϐ��̏�����
	SetRect(&m_MouseInfo.moveRect, 10, 10, 630, 470);								// �}�E�X�J�[�\���̓����͈�
	m_MouseInfo.x = m_MouseInfo.moveRect.left;										// �}�E�X�J�[�\���̂w���W��������
	m_MouseInfo.y = m_MouseInfo.moveRect.top;										// �}�E�X�J�[�\���̂x���W��������
	m_MouseInfo.lButton = false;													// ���{�^���̏���������
	m_MouseInfo.rButton = false;													// �E�{�^���̏���������
	m_MouseInfo.cButton = false;													// �����{�^���̏���������
	m_MouseInfo.moveAdd = 2;														// �}�E�X�J�[�\���̈ړ��ʂ�ݒ�
	SetRect(&m_MouseInfo.imgRect, 400, 0, 420, 20);									// �}�E�X�J�[�\���摜�̋�`��ݒ�
	m_MouseInfo.imgWidth = m_MouseInfo.imgRect.right - m_MouseInfo.imgRect.left;	// �摜�̕����v�Z
	m_MouseInfo.imgHeight = m_MouseInfo.imgRect.bottom - m_MouseInfo.imgRect.top;	// �摜�̍������v�Z

	//���̓f�o�C�X�̐���
	if (FAILED(m_pInput->CreateDevice(GUID_SysMouse, &m_pMouse, NULL)))
	{
		MessageBox(hWnd, TEXT("�}�E�X�f�o�C�X�̐����Ɏ��s"), TEXT("���b�Z�[�W�{�b�N�X"), MB_OK);
		return false;
	}

	//�f�[�^�t�H�[�}�b�g�̐ݒ�
	if (FAILED(m_pMouse->SetDataFormat(&c_dfDIMouse2)))
	{
		MessageBox(hWnd, TEXT("�}�E�X�t�H�[�}�b�g�̐ݒ�Ɏ��s"), TEXT("���b�Z�[�W�{�b�N�X"), MB_OK);
		return false;
	}

	// ���[�h�̐ݒ�
	if (FAILED(m_pMouse->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND)))
	{
		MessageBox(hWnd, TEXT("�}�E�X���[�h�̐ݒ�Ɏ��s"), TEXT("���b�Z�[�W�{�b�N�X"), MB_OK);
		return false;
	}

	// �f�o�C�X�̐ݒ�
	DIPROPDWORD diprop;
	diprop.diph.dwSize = sizeof(diprop);
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	diprop.diph.dwObj = 0;
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.dwData = DIPROPAXISMODE_REL;	// ���Βl���[�h�Őݒ�i��Βl��DIPROPAXISMODE_ABS�j

	ret = m_pMouse->SetProperty(DIPROP_AXISMODE, &diprop.diph);
	if (FAILED(ret)) 
	{
		// �f�o�C�X�̐ݒ�Ɏ��s
		return false;
	}

	// ���͐���J�n
	ret = m_pMouse->Acquire();

	return true;

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CInputMouse::Uninit(void)
{
	// DirectInput�I�u�W�F�N�g�̊J��
	if (m_pMouse != NULL)
	{
		m_pMouse->Unacquire();
		m_pMouse->Release();
		m_pMouse = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void CInputMouse::Update(void)
{
	//�ϐ��錾
	HRESULT ret = S_FALSE;

	if (m_pMouse == NULL) { return; }

	if (SUCCEEDED(ret = m_pMouse->GetDeviceState(sizeof(DIMOUSESTATE2), &m_MouseState)))
	{
		for (int nCntMouse = 0; nCntMouse < NUM_MOUSEMAX; nCntMouse++)
		{
			m_MouseStateRelease.rgbButtons[nCntMouse] = (m_MouseStatePress.rgbButtons[nCntMouse] ^ m_MouseState.rgbButtons[nCntMouse]) & m_MouseStatePress.rgbButtons[nCntMouse];	//�����[�X
			m_MouseStateTrigger.rgbButtons[nCntMouse] = (m_MouseStatePress.rgbButtons[nCntMouse] ^ m_MouseState.rgbButtons[nCntMouse]) & m_MouseState.rgbButtons[nCntMouse];		//�g���K�[
			m_MouseStatePress.rgbButtons[nCntMouse] = m_MouseState.rgbButtons[nCntMouse];						//���͏��ۑ�
		}

		m_MouseIX = m_MouseState.lX;
		m_MouseIY = m_MouseState.lY;
		m_MouseIZ = m_MouseState.lZ;
	}
	else
	{
		ret = m_pMouse->Acquire();	//�L�[�{�[�h�ւ̃A�N�Z�X�����l��
	}

	GetScreenToWorld();
}

//=============================================================================
// �����[�X����
//=============================================================================
bool CInputMouse::GetRelease(int nKey)
{
	if (m_pMouse == NULL) { return false; }
	return (m_MouseStateRelease.rgbButtons[nKey] & 0x80) ? true : false;
}

//=============================================================================
// �g���K�[����
//=============================================================================
bool CInputMouse::GetTrigger(int nKey)
{
	if (m_pMouse == NULL) { return false; }
	return (m_MouseStateTrigger.rgbButtons[nKey] & 0x80) ? true : false;
}

//=============================================================================
// �v���X����
//=============================================================================
bool CInputMouse::GetPress(int nKey)
{
	if (m_pMouse == NULL) { return false; }
	return (m_MouseStatePress.rgbButtons[nKey] & 0x80) ? true : false;
}

//=============================================================================
// X���̏���
//=============================================================================
LONG CInputMouse::GetIX(void)
{
	return m_MouseIX;
}

//=============================================================================
// Y���̏���
//=============================================================================
LONG CInputMouse::GetIY(void)
{
	return m_MouseIY;
}

//=============================================================================
// Z���̏���
//=============================================================================
LONG CInputMouse::GetIZ(void)
{
	return m_MouseIZ;
}

//=============================================================================
// �X�N���[�����W���烏�[���h���W
//=============================================================================
void CInputMouse::GetScreenToWorld(void)
{
	//�ϐ��錾
	D3DXMATRIX mtxView, mtxProjection, mtxViewPort, mtxMouseWorld;

	//�}�E�X�̍��W���擾
	GetCursorPos(&m_ScreenPos);
	ScreenToClient(m_hWnd, &m_ScreenPos);

	CGame *pGame = CManager::GetGame();
	int nMode = CManager::GetMode();

	//if (nMode == CManager::MODE_GAME)
	//{

	//	//�r���[�E�v���W�F�N�V�����̎擾
	//	CGameCamera * pCamera = pGame->GetGameCamera(0);
	//	mtxView = pCamera->GetMtxView();
	//	mtxProjection = pCamera->GetMtxProjection();

	//	//�r���[�A�v���W�F�N�V�����̋t�s������߂�
	//	D3DXMatrixInverse(&mtxView, NULL, &mtxView);
	//	D3DXMatrixInverse(&mtxProjection, NULL, &mtxProjection);

	//	//�r���[�|�[�g�����߂�
	//	D3DXMatrixIdentity(&mtxViewPort);
	//	mtxViewPort._11 = SCREEN_WIDTH / 2.0f;
	//	mtxViewPort._41 = SCREEN_WIDTH / 2.0f;
	//	mtxViewPort._22 = -SCREEN_HEIGHT / 2.0f;
	//	mtxViewPort._42 = SCREEN_HEIGHT / 2.0f;

	//	//�r���[�|�[�g�̋t�s������߂�
	//	D3DXMatrixInverse(&mtxViewPort, NULL, &mtxViewPort);

	//	//���[���h���W�ɕϊ�
	//	D3DXMATRIX mtxWorld = mtxViewPort * mtxProjection * mtxView;
	//	D3DXVec3TransformCoord(&m_WorldPos, &D3DXVECTOR3((float)m_ScreenPos.x, (float)m_ScreenPos.y, 1.0f), &mtxWorld);
	//}
}