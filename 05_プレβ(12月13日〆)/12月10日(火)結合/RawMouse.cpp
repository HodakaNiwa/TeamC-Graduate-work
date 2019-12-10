//*****************************************************************************
//
//     RawInput��p�����}�E�X�������̏���[RawMouse.cpp]
//     Author:Hodaka Niwa
//
//*****************************************************************************
#include "RawMouse.h"
#include "strsafe.h"

//*****************************************************************************
//     �}�N����`
//*****************************************************************************
// �}�E�X�̓��͌��m�p
// �{�^��
#define RAWINPUT_MOUSE_BUTTON_LEFT       (1)
#define RAWINPUT_MOUSE_BUTTON_RIGHT      (4)
#define RAWINPUT_MOUSE_BUTTON_MIDDLE     (16)

// �f�b�h�]�[��
#define RAWINPUT_MOUSE_DEADZONE_HORIZON  (25)
#define RAWINPUT_MOUSE_DEADZONE_VERTICAL (25)

// ���s�[�g�p
#define RAWINPUT_REPEAT_FRAME            (20)
#define RAWINPUT_REPEAT_TRIGGER          (7)

//*****************************************************************************
//     �ÓI�����o�ϐ��錾
//*****************************************************************************
CRawMouse *CRawMouse::m_pInstance = NULL;   // RawMouse�N���X�ւ̃|�C���^

//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CRawMouse::CRawMouse()
{
	ZeroMemory(&m_Mouse, sizeof(m_Mouse));
	ZeroMemory(&m_aMousePress, sizeof(m_aMousePress));
	ZeroMemory(&m_aMouseTrigger, sizeof(m_aMouseTrigger));
	ZeroMemory(&m_aMouseRelease, sizeof(m_aMouseRelease));
	ZeroMemory(&m_aMouseRepeat, sizeof(m_aMouseRepeat));
	ZeroMemory(&m_aMouseCounter, sizeof(m_aMouseCounter));
	ZeroMemory(&m_bMouseTrigger, sizeof(m_bMouseTrigger));
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CRawMouse::~CRawMouse()
{

}

//=============================================================================
//    ��������
//=============================================================================
CRawMouse *CRawMouse::Create(HWND hWnd)
{
	if (m_pInstance != NULL) return m_pInstance;
	m_pInstance = new CRawMouse;

	return m_pInstance;
}

//=============================================================================
//    �f�o�C�X��RAWINPUT�ɓo�^���鏈��
//=============================================================================
HRESULT CRawMouse::RegisterRawInput(HWND hWnd)
{
	// �f�o�C�X��WM_INPUT�ɓo�^
	RAWINPUTDEVICE Device[1];
	// HID�}�E�X
	Device[0].usUsagePage = 0x01;
	Device[0].usUsage = 0x02;
	Device[0].dwFlags = RIDEV_INPUTSINK;
	Device[0].hwndTarget = hWnd;

	// �o�^
	if (RegisterRawInputDevices(Device, 1, sizeof(Device[0])) == 0)
	{
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CRawMouse::Init(HWND hWnd)
{
	// ���ݐڑ�����Ă���f�o�C�X�̐����擾
	GetRawInputDeviceList(NULL, &m_nDeviceNum, sizeof(RAWINPUTDEVICELIST));
	RAWINPUTDEVICELIST* deviceList = new RAWINPUTDEVICELIST[m_nDeviceNum];

	// ���X�g���擾
	if(GetRawInputDeviceList(deviceList, &m_nDeviceNum, sizeof(RAWINPUTDEVICELIST)) == -1)
	{
		return E_FAIL;
	}

	// �n���h����ۑ�
	m_nMouseNum = 0;
	for (int nCnt = 0; nCnt < (int)m_nDeviceNum; nCnt++)
	{
		if (deviceList[nCnt].dwType == RIM_TYPEMOUSE)
		{
			m_MouseHandle[m_nMouseNum] = deviceList[nCnt].hDevice;
			m_nMouseNum++;
		}
	}
	delete[] deviceList;

	// �E�B���h�E�n���h����ۑ�
	m_hWnd = hWnd;

	// �L�[�{�[�h��̃}�E�X�͎g�p���Ȃ��̂Ő������炷
	m_nMouseNum--;

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CRawMouse::Uninit(void)
{
	// �[���N���A
	ClearMouseData();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CRawMouse::Update(LPARAM lParam)
{
	// �o�b�t�@�̑傫�����擾
	UINT dwSize = 1;
	GetRawInputData((HRAWINPUT)lParam, RID_INPUT, NULL, &dwSize, sizeof(RAWINPUTHEADER));
	BYTE* lpb = new BYTE[dwSize];
	if (lpb == NULL)
	{
		return;
	}

	// �f�[�^���擾
	if (GetRawInputData((HRAWINPUT)lParam, RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER)) == -1)
	{
		delete[] lpb;
		return;
	}

	// �f�[�^�������o�ϐ��Ɋi�[
	RAWINPUT *raw = (RAWINPUT*)lpb;
	for (int nCntMouse = 0; nCntMouse < (int)m_nMouseNum; nCntMouse++)
	{
		if (raw->header.dwType == RIM_TYPEMOUSE && m_MouseHandle[nCntMouse] == raw->header.hDevice)
		{
			ArrayMouseData(raw, nCntMouse);
		}
	}

// �f�o�b�O�p�̃R�����g�A�E�g
#if 0
	//if (raw->header.dwType == RIM_TYPEMOUSE && m_MouseHandle[] == raw->header.hDevice)
	//{
	//	m_Mouse[] = raw->data.mouse;
	//	m_MouseHeader[] = raw->header;

	//	//char szTempOutput[256];
	//	//HRESULT hResult;
	//	//hResult = StringCchPrintf(szTempOutput, STRSAFE_MAX_CCH, TEXT("Mouse: usFlags=%04x ulButtons=%04x usButtonFlags=%04x usButtonData=%04x ulRawButtons=%04x lLastX=%04x lLastY=%04x ulExtraInformation=%04x\r\n"),
	//	//	raw->data.mouse.usFlags,
	//	//	raw->data.mouse.ulButtons,
	//	//	raw->data.mouse.usButtonFlags,
	//	//	raw->data.mouse.usButtonData,
	//	//	raw->data.mouse.ulRawButtons,
	//	//	raw->data.mouse.lLastX,
	//	//	raw->data.mouse.lLastY,
	//	//	raw->data.mouse.ulExtraInformation);

	//	//if (FAILED(hResult))
	//	//{
	//	//	// TODO: write error handler
	//	//}
	//	//OutputDebugString(szTempOutput);
	//}
	//else if (raw->header.dwType == RIM_TYPEKEYBOARD)
	//{
	//	m_Keyboard = raw->data.keyboard;
	//	m_KeyHeader = raw->header;
	//}
#endif

	delete[] lpb;
}

//=============================================================================
//    �}�E�X�̃f�[�^�����蓖�Ă鏈��
//=============================================================================
void CRawMouse::ArrayMouseData(RAWINPUT *raw, int nCntMouse)
{
	// �}�E�X�f�[�^��ۑ�
	m_Mouse[nCntMouse] = raw->data.mouse;

	// �w�b�_�[(�ڍ׏��)��ۑ�
	m_MouseHeader[nCntMouse] = raw->header;

	// �}�E�X�̏�Ԃ��擾
	BYTE aMouseState[RIMS_BUTTON_MAX] = {};   // �}�E�X�{�^�����͏��

	// �{�^���n
	if (raw->data.mouse.ulButtons >= RAWINPUT_MOUSE_BUTTON_MIDDLE)
	{// ���{�^����������Ă���
		aMouseState[RIMS_BUTTON_MIDDLE] = 0x80;
		raw->data.mouse.ulButtons -= RAWINPUT_MOUSE_BUTTON_MIDDLE;
		if (raw->data.mouse.ulButtons >= RAWINPUT_MOUSE_BUTTON_MIDDLE)
		{
			aMouseState[RIMS_BUTTON_MIDDLE] = 0x00;
			raw->data.mouse.ulButtons -= RAWINPUT_MOUSE_BUTTON_MIDDLE;
			m_bMouseRelease[nCntMouse][RIMS_BUTTON_MIDDLE] = false;
		}
	}
	if (raw->data.mouse.ulButtons >= RAWINPUT_MOUSE_BUTTON_RIGHT)
	{// �E�{�^����������Ă���
		aMouseState[RIMS_BUTTON_RIGHT] = 0x80;
		raw->data.mouse.ulButtons -= RAWINPUT_MOUSE_BUTTON_RIGHT;
		if (raw->data.mouse.ulButtons >= RAWINPUT_MOUSE_BUTTON_RIGHT)
		{
			aMouseState[RIMS_BUTTON_RIGHT] = 0x00;
			raw->data.mouse.ulButtons -= RAWINPUT_MOUSE_BUTTON_RIGHT;
			m_bMouseRelease[nCntMouse][RIMS_BUTTON_RIGHT] = false;
		}
	}
	if (raw->data.mouse.ulButtons >= RAWINPUT_MOUSE_BUTTON_LEFT)
	{// ���{�^����������Ă���
		aMouseState[RIMS_BUTTON_LEFT] = 0x80;
		raw->data.mouse.ulButtons -= RAWINPUT_MOUSE_BUTTON_LEFT;
		if (raw->data.mouse.ulButtons >= RAWINPUT_MOUSE_BUTTON_LEFT)
		{
			aMouseState[RIMS_BUTTON_LEFT] = 0x00;
			raw->data.mouse.ulButtons -= RAWINPUT_MOUSE_BUTTON_LEFT;
			m_bMouseRelease[nCntMouse][RIMS_BUTTON_LEFT] = false;
		}
	}

	// �}�E�X�̓���
	if (raw->data.mouse.lLastY < -RAWINPUT_MOUSE_DEADZONE_VERTICAL)
	{// ��ɓ�����
		aMouseState[RIMS_SLIDE_DOWN] = 0x80;
		m_bMouseRelease[nCntMouse][RIMS_SLIDE_DOWN] = false;
	}
	else if (raw->data.mouse.lLastY > RAWINPUT_MOUSE_DEADZONE_VERTICAL)
	{// ���ɓ�����
		aMouseState[RIMS_SLIDE_UP] = 0x80;
		m_bMouseRelease[nCntMouse][RIMS_SLIDE_UP] = false;
	}
	if (raw->data.mouse.lLastX < -RAWINPUT_MOUSE_DEADZONE_HORIZON)
	{// �E�ɓ�����
		aMouseState[RIMS_SLIDE_RIGHT] = 0x80;
		m_bMouseRelease[nCntMouse][RIMS_SLIDE_RIGHT] = false;
	}
	else if (raw->data.mouse.lLastX > RAWINPUT_MOUSE_DEADZONE_HORIZON)
	{// ���ɓ�����
		aMouseState[RIMS_SLIDE_LEFT] = 0x80;
		m_bMouseRelease[nCntMouse][RIMS_SLIDE_LEFT] = false;
	}

	// �{�^���̏�Ԃ��i�[
	for (int nCntButton = 0; nCntButton < RIMS_BUTTON_MAX; nCntButton++)
	{
		m_aMousePress[nCntMouse][nCntButton] = aMouseState[nCntButton];
		if (aMouseState[nCntButton] == 0x00)
		{
			m_bMouseTrigger[nCntMouse][nCntButton] = false;
		}
	}
}

//=============================================================================
//    �}�E�X�̃f�[�^�𒲐����鏈��
//=============================================================================
void CRawMouse::AdjustMouseData(void)
{
	for (int nCntMouse = 0; nCntMouse < (int)m_nMouseNum; nCntMouse++)
	{// �ڑ�����Ă���}�E�X�̐������J��Ԃ�
		for (int nCntButton = 0; nCntButton < RIMS_BUTTON_MAX; nCntButton++)
		{// �{�^���̐������J��Ԃ�
			// �g���K�[���
			AdjustMouseData_Trigger(nCntMouse, nCntButton);

			// �����[�X���
			AdjustMouseData_Release(nCntMouse, nCntButton);

			// ���s�[�g���
			AdjustMouseData_Repeat(nCntMouse, nCntButton);
		}
	}
}

//=============================================================================
//    �}�E�X�̃f�[�^�𒲐����鏈��(�g���K�[)
//=============================================================================
void CRawMouse::AdjustMouseData_Trigger(int nCntMouse, int nCntButton)
{
	if (m_aMousePress[nCntMouse][nCntButton] == 0x80 && m_bMouseTrigger[nCntMouse][nCntButton] == false)
	{
		m_aMouseTrigger[nCntMouse][nCntButton] = 0x80;
		m_bMouseTrigger[nCntMouse][nCntButton] = true;
	}
	else
	{
		m_aMouseTrigger[nCntMouse][nCntButton] = 0x00;
	}
}

//=============================================================================
//    �}�E�X�̃f�[�^�𒲐����鏈��(�����[�X)
//=============================================================================
void CRawMouse::AdjustMouseData_Release(int nCntMouse, int nCntButton)
{
	if (m_aMousePress[nCntMouse][nCntButton] == 0x00 && m_bMouseRelease[nCntMouse][nCntButton] == false)
	{
		m_aMouseRelease[nCntMouse][nCntButton] = 0x80;
		m_bMouseRelease[nCntMouse][nCntButton] = true;
	}
	else
	{
		m_aMouseRelease[nCntMouse][nCntButton] = 0x00;
	}
}

//=============================================================================
//    �}�E�X�̃f�[�^�𒲐����鏈��(���s�[�g)
//=============================================================================
void CRawMouse::AdjustMouseData_Repeat(int nCntMouse, int nCntButton)
{
	if (m_aMousePress[nCntMouse][nCntButton] == 0x80)
	{// �{�^�������͂���Ă���
		m_aMouseCounter[nCntMouse][nCntButton]++;    // �J�E���^�[��i�߂�
		if (m_aMouseCounter[nCntMouse][nCntButton] >= RAWINPUT_REPEAT_FRAME)
		{// 20�t���[�����L�[��������Ă���
			if (m_aMouseCounter[nCntMouse][nCntButton] % RAWINPUT_REPEAT_TRIGGER == 0)
			{// 7�t���[������
				m_aMouseRepeat[nCntMouse][nCntButton] = 0x80;   // �}�E�X�̓��͏��(���s�[�g���)���N��
			}
			else
			{// ����ȊO�̃t���[��
				m_aMouseRepeat[nCntMouse][nCntButton] = 0;      // �}�E�X�̓��͏��(���s�[�g���)���ꎞ��~
			}
		}
	}
	else
	{// �L�[�{�[�h�����͂���Ă��Ȃ�
		m_aMouseCounter[nCntMouse][nCntButton] = 0;        // �J�E���^�[��߂�
		m_aMouseRepeat[nCntMouse][nCntButton] = 0;         // �}�E�X�̓��͏��(���s�[�g���)���~
	}
}

//=============================================================================
//    �}�E�X�̃f�[�^���N���A���鏈��
//=============================================================================
void CRawMouse::ClearMouseData(void)
{
	ZeroMemory(&m_Mouse, sizeof(m_Mouse));
}

//=============================================================================
//    �}�E�X�̐ڑ�����Ă��鐔������
//=============================================================================
UINT CRawMouse::GetMouseNum(void)
{
	return m_nMouseNum;
}

//=============================================================================
//    �}�E�X�ւ̃n���h�����擾���鏈��
//=============================================================================
HANDLE CRawMouse::GetMouseHandle(int nIdx)
{
	return m_MouseHandle[nIdx];
}

//=============================================================================
//    �}�E�X���擾���鏈��
//=============================================================================
RAWMOUSE CRawMouse::GetMouse(int nIdx)
{
	return m_Mouse[nIdx];
}

//=============================================================================
//    �}�E�X�̃{�^���������ꂽ���ǂ������擾(�v���X���)
//=============================================================================
bool CRawMouse::GetPress(int nIdx, int nButton)
{
	return (m_aMousePress[nIdx][nButton] & 0x80) ? true : false;
}

//=============================================================================
//    �}�E�X�̃{�^���������ꂽ���ǂ������擾(�g���K�[���)
//=============================================================================
bool CRawMouse::GetTrigger(int nIdx, int nButton)
{
	return (m_aMouseTrigger[nIdx][nButton] & 0x80) ? true : false;
}

//=============================================================================
//    �}�E�X�̃{�^���������ꂽ���ǂ������擾(�����[�X���)
//=============================================================================
bool CRawMouse::GetRelease(int nIdx, int nButton)
{
	return (m_aMouseRelease[nIdx][nButton] & 0x80) ? true : false;
}

//=============================================================================
//    �}�E�X�̃{�^���������ꂽ���ǂ������擾(���s�[�g���)
//=============================================================================
bool CRawMouse::GetRepeat(int nIdx, int nButton)
{
	return (m_aMouseRepeat[nIdx][nButton] & 0x80) ? true : false;
}