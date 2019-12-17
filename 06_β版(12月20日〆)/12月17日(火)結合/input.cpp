//=============================================================================
//
// ���͏��� [input.cpp]
// Author : ���������@SATOU ASUMI :: ���я��� : �R���֎j : �O�H�ۋM
//
//=============================================================================
#include "input.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define REPEAT_FRAME     (20)    // ���s�[�g�����N�����n�߂�t���[�����̍��v
#define REPEAT_TRIGGER   (7)     // ���s�[�g�����N������t���[���̊Ԋu

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================
LPDIRECTINPUT8 CInput::m_pInput = NULL;
int CInputXPad::m_nInputNum = 0;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CInput::CInput()
{
	m_pDevice = NULL;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CInput::~CInput()
{
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CInput::Init(HINSTANCE hInstance, HWND hWnd)
{
	if (m_pInput == NULL)
	{
		// DirectInput�I�u�W�F�N�g�̐���
		if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInput, NULL)))
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CInput::Uninit(void)
{
	// ���̓f�o�C�X(�L�[�{�[�h)�̊J��
	if (m_pDevice != NULL)
	{
		m_pDevice->Unacquire();	//�L�[�{�[�h�ւ̃A�N�Z�X�������
		m_pDevice->Release();
		m_pDevice = NULL;
	}

	// DirectInput�I�u�W�F�N�g�̊J��
	if (m_pInput != NULL)
	{
		m_pInput->Release();
		m_pInput = NULL;
	}
}

/*****************************************************************************/
// �L�[�{�[�h�̓��͏���                                                      //
/*****************************************************************************/
//=============================================================================
// �R���X�g���N�^
//=============================================================================
CInputKeyboard::CInputKeyboard()
{
	//�ϐ��錾
	int nCntKey;

	//�L�[�̊J��
	for (nCntKey = 0; nCntKey < MAX_KEY; nCntKey++)
	{
		m_aKeyState[nCntKey] = 0;
		m_aKeyStateRelease[nCntKey] = 0;
		m_aKeyStateTrigger[nCntKey] = 0;
		m_aKeyStateRepeat[nCntKey] = 0;
		m_aKeyRepeatCounter[nCntKey] = 0;
	}
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CInputKeyboard::~CInputKeyboard()
{
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CInputKeyboard::Init(HINSTANCE hInstance, HWND hWnd)
{
	//�������������Ăяo��
	CInput::Init(hInstance, hWnd);

	// ���̓f�o�C�X(�L�[�{�[�h)�̐���
	if (FAILED(m_pInput->CreateDevice(GUID_SysKeyboard, &m_pDevice, NULL)))
	{
		return E_FAIL;
	}

	// �f�[�^�t�H�[�}�b�g��ݒ�
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	// �������[�h��ݒ�
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	// �L�[�{�[�h�ւ̃A�N�Z�X�����l��(���͐���J�n)
	m_pDevice->Acquire();

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CInputKeyboard::Uninit(void)
{
	CInput::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CInputKeyboard::Update(void)
{
	//�ϐ��錾
	BYTE aKeyState[MAX_KEY];	//�L�[�{�[�h�̓��͏��
	int nCntKey = 0;

	// ���̓f�o�C�X����f�[�^���擾
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (nCntKey = 0; nCntKey < MAX_KEY; nCntKey++)
		{
			m_aKeyStateRelease[nCntKey] = (m_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & m_aKeyState[nCntKey];	//�����[�X
			m_aKeyStateTrigger[nCntKey] = (m_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & aKeyState[nCntKey];		//�g���K�[
			m_aKeyState[nCntKey] = aKeyState[nCntKey];															//�L�[�{�[�h�̓��͏��ۑ�

			if (m_aKeyState[nCntKey] == 0x80)
			{// �v���X��񂪋N�����Ă���
				m_aKeyRepeatCounter[nCntKey]++;
				if (m_aKeyRepeatCounter[nCntKey] >= REPEAT_FRAME)
				{// �v���X��񂪋N�����Ĉ�莞�Ԍo��
					if (m_aKeyRepeatCounter[nCntKey] % REPEAT_TRIGGER == 0)
					{// 7�t���[�����ɏ����N������
						m_aKeyStateRepeat[nCntKey] = 0x80;
					}
					else
					{// ����ȊO�̃t���[���̓��s�[�g�����ꎞ��~
						m_aKeyStateRepeat[nCntKey] = 0x00;
					}
				}
			}
			else
			{// �v���X��񂪋N�����Ă��Ȃ�
				m_aKeyStateRepeat[nCntKey] = 0x00;
				m_aKeyRepeatCounter[nCntKey] = 0;
			}
		}
	}
	else
	{
		m_pDevice->Acquire();	//�L�[�{�[�h�ւ̃A�N�Z�X�����l��
	}
}

//=============================================================================
// �L�[�{�[�h�v���X����
//=============================================================================
bool CInputKeyboard::GetKeyboardPress(int nKey)
{
	return (m_aKeyState[nKey] & 0x80) ? true : false;
}

//========================================================================================
// �L�[�{�[�h�̓��͏��(�g���K�[���)���擾
//========================================================================================
bool CInputKeyboard::GetKeyboardTrigger(int nKey)
{
	return (m_aKeyStateTrigger[nKey] & 0x80) ? true : false;
}

//========================================================================================
// �L�[�{�[�h�̓��͏��(�����[�X���)���擾
//========================================================================================
bool CInputKeyboard::GetKeyboardRelease(int nKey)
{
	return (m_aKeyStateRelease[nKey] & 0x80) ? true : false;
}

//========================================================================================
// �L�[�{�[�h�̓��͏��(���s�[�g���)���擾
//========================================================================================
bool CInputKeyboard::GetKeyboardRepeat(int nKey)
{
	return (m_aKeyStateRepeat[nKey] & 0x80) ? true : false;
}

//=============================================================================
// �R���X�g���N�^							(public)	*** CInputXPad ***
//=============================================================================
CInputXPad::CInputXPad()
{
	// �����l�ݒ�
	for (int nCntCont = 0; nCntCont < MAX_CONTROLLERS; nCntCont++)
	{
		m_LStickRot[nCntCont] = 0.0f;		// L�X�e�B�b�N
		m_RStickRot[nCntCont] = 0.0f;		// R�X�e�B�b�N
	}

	m_nInputNum = 0;						// ���͐�

	ZeroMemory(m_aJoyRepeatCounter, sizeof(int) * XPADOTHER_MAX * MAX_CONTROLLERS);
}

//=============================================================================
// �f�X�g���N�^								(public)	*** CInputXPad ***
//=============================================================================
CInputXPad::~CInputXPad()
{

}

//=============================================================================
// ����������								(public)	*** CInputXPad ***
//=============================================================================
HRESULT CInputXPad::Init(HINSTANCE hInstance, HWND hWnd)
{
	// �R���g���[���[�̏�����
	ZeroMemory(m_Controllers, sizeof(CONTROLER_STATE) * MAX_CONTROLLERS);

	return S_OK;
}

//=============================================================================
// �I������									(public)	*** CInputXPad ***
//=============================================================================
void CInputXPad::Uninit(void)
{

}

//=============================================================================
// �X�V����									(public)	*** CInputXPad ***
//=============================================================================
void CInputXPad::Update(void)
{
	UpdateControllerState();	// �r������

	for (DWORD nCntCont = 0; nCntCont < MAX_CONTROLLERS; nCntCont++)
	{// �R���g���[���[�J�E���g
		if (m_Controllers[nCntCont].bConnected == true)
		{// �ڑ�����Ă�����
			if (m_bDeadZoneOn == true)
			{// �X�e�B�b�N
			 // Zero value if thumbsticks are within the dead zone
				if ((m_Controllers[nCntCont].state.Gamepad.sThumbLX < INPUT_DEADZONE &&
					m_Controllers[nCntCont].state.Gamepad.sThumbLX > -INPUT_DEADZONE) &&
					(m_Controllers[nCntCont].state.Gamepad.sThumbLY < INPUT_DEADZONE &&
						m_Controllers[nCntCont].state.Gamepad.sThumbLY > -INPUT_DEADZONE))
				{
					m_Controllers[nCntCont].state.Gamepad.sThumbLX = 0;
					m_Controllers[nCntCont].state.Gamepad.sThumbLY = 0;
				}
				//m_Controllers[nCntCont].state.Gamepad.wButtons
				if ((m_Controllers[nCntCont].state.Gamepad.sThumbRX < INPUT_DEADZONE &&
					m_Controllers[nCntCont].state.Gamepad.sThumbRX > -INPUT_DEADZONE) &&
					(m_Controllers[nCntCont].state.Gamepad.sThumbRY < INPUT_DEADZONE &&
						m_Controllers[nCntCont].state.Gamepad.sThumbRY > -INPUT_DEADZONE))
				{
					m_Controllers[nCntCont].state.Gamepad.sThumbRX = 0;
					m_Controllers[nCntCont].state.Gamepad.sThumbRY = 0;
				}
			}

			WORD wButtons = 0;		// XINPUT�R���g���[���[�̓��͏��

			for (int nCount = 0; nCount < XPADOTHER_MAX; nCount++)
			{// ���͏��J�E���g
				switch (nCount)
				{
				case 0:		// �{�^��
					wButtons = m_Controllers[nCntCont].state.Gamepad.wButtons;
					break;
				case 1:		// ���g���K�[
					wButtons = m_Controllers[nCntCont].state.Gamepad.bLeftTrigger;
					break;
				case 2:		// �E�g���K�[
					wButtons = m_Controllers[nCntCont].state.Gamepad.bRightTrigger;
					break;
				case 3:		// ���X�e�B�b�N��
					if (m_Controllers[nCntCont].state.Gamepad.sThumbLY > XINPUT_STICK_MAX * 0.1f)
						wButtons = 0x80;
					else
						wButtons = 0;
					break;
				case 4:		// ���X�e�B�b�N��
					if (m_Controllers[nCntCont].state.Gamepad.sThumbLX < XINPUT_STICK_MIN * 0.1f)
						wButtons = 0x80;
					else
						wButtons = 0;
					break;
				case 5:		// ���X�e�B�b�N�E
					if (m_Controllers[nCntCont].state.Gamepad.sThumbLX > XINPUT_STICK_MAX * 0.1f)
						wButtons = 0x80;
					else
						wButtons = 0;
					break;
				case 6:		// ���X�e�B�b�N��
					if (m_Controllers[nCntCont].state.Gamepad.sThumbLY < XINPUT_STICK_MIN * 0.1f)
						wButtons = 0x80;
					else
						wButtons = 0;
					break;
				case 7:		// �E�X�e�B�b�N��
					if (m_Controllers[nCntCont].state.Gamepad.sThumbRY > XINPUT_STICK_MAX * 0.1f)
						wButtons = 0x80;
					else
						wButtons = 0;
					break;
				case 8:		// �E�X�e�B�b�N��
					if (m_Controllers[nCntCont].state.Gamepad.sThumbRX < XINPUT_STICK_MIN * 0.1f)
						wButtons = 0x80;
					else
						wButtons = 0;
					break;
				case 9:		// �E�X�e�B�b�N�E
					if (m_Controllers[nCntCont].state.Gamepad.sThumbRX > XINPUT_STICK_MAX * 0.1f)
						wButtons = 0x80;
					else
						wButtons = 0;
					break;
				case 10:		// �E�X�e�B�b�N��
					if (m_Controllers[nCntCont].state.Gamepad.sThumbRY < XINPUT_STICK_MIN * 0.1f)
						wButtons = 0x80;
					else
						wButtons = 0;
					break;
				}

				// �X�e�B�b�N����
				m_LStickRot[nCntCont] = atan2f((float)m_Controllers[nCntCont].state.Gamepad.sThumbLX, (float)m_Controllers[nCntCont].state.Gamepad.sThumbLY);
				m_RStickRot[nCntCont] = atan2f((float)m_Controllers[nCntCont].state.Gamepad.sThumbRX, (float)m_Controllers[nCntCont].state.Gamepad.sThumbRY);

				// �W���C�p�b�h�̓��͏��
				m_aJoyStateTrigger[nCntCont][nCount] = (m_aJoyStatePress[nCntCont][nCount] ^ wButtons) & wButtons;
				m_aJoyStateRelease[nCntCont][nCount] = (m_aJoyStatePress[nCntCont][nCount] ^ wButtons) & m_aJoyStatePress[nCntCont][nCount];
				m_aJoyStatePress[nCntCont][nCount] = wButtons;

				if (m_aJoyStatePress[nCntCont][nCount] == 0x80)
				{// �v���X��񂪋N�����Ă���
					m_aJoyRepeatCounter[nCntCont][nCount]++;
					if (m_aJoyRepeatCounter[nCntCont][nCount] >= REPEAT_FRAME)
					{// �v���X��񂪋N�����Ĉ�莞�Ԍo��
						if (m_aJoyRepeatCounter[nCntCont][nCount] % REPEAT_TRIGGER == 0)
						{// 7�t���[�����ɏ����N������
							m_aJoyStateRepeat[nCntCont][nCount] = 0x80;
						}
						else
						{// ����ȊO�̃t���[���̓��s�[�g�����ꎞ��~
							m_aJoyStateRepeat[nCntCont][nCount] = 0x00;
						}
					}
				}
				else
				{// �v���X��񂪋N�����Ă��Ȃ�
					m_aJoyStateRepeat[nCntCont][nCount] = 0x00;
					m_aJoyRepeatCounter[nCntCont][nCount] = 0;
				}
			}
		}
	}
}

//=============================================================================
// X�p�b�h�̓��͏��(���͊m�F)���擾	(private)	*** CInputXPad ***
//=============================================================================
HRESULT CInputXPad::UpdateControllerState(void)
{
	DWORD dwResult;
	m_nInputNum = 0;
	for (DWORD nCntCont = 0; nCntCont < MAX_CONTROLLERS; nCntCont++)
	{
		// Simply get the state of the controller from XInput.
		dwResult = XInputGetState(nCntCont, &m_Controllers[nCntCont].state);

		if (dwResult == ERROR_SUCCESS)
		{
			m_Controllers[nCntCont].bConnected = true;
			m_nInputNum++;
		}
		else
		{
			m_Controllers[nCntCont].bConnected = false;
		}
	}

	return S_OK;
}
//=============================================================================
// X�p�b�h�̓��͏��(���͐����)���擾	(private)	*** CInputXPad ***
//=============================================================================
int CInputXPad::GetInputNum(void)
{
	return m_nInputNum;
}

//=============================================================================
// X�p�b�h�̓��͏��(�I�[�����)���擾	(private)	*** CInputXPad ***
//=============================================================================
bool CInputXPad::GetALL(int nType, int nIdxPad)
{
	if (m_Controllers[nIdxPad].bConnected)
	{
		switch (nType)
		{
		case 0:
			for (int nCount = 0; nCount < XPADOTHER_MAX; nCount++)
			{// ���͏��J�E���g
				if (nCount == XPADOTHER_TRIGGER_LEFT || nCount == XPADOTHER_TRIGGER_RIGHT ||
					nCount == XPADOTHER_STICK_L_UP || nCount == XPADOTHER_STICK_L_LEFT || nCount == XPADOTHER_STICK_L_RIGHT || nCount == XPADOTHER_STICK_L_DOWN ||
					nCount == XPADOTHER_STICK_R_UP || nCount == XPADOTHER_STICK_R_LEFT || nCount == XPADOTHER_STICK_R_RIGHT || nCount == XPADOTHER_STICK_R_DOWN)
				{
					if (m_aJoyStatePress[nIdxPad][nCount] == 0x80)
					{
						return true;
					}
				}
				else
				{
					if (m_aJoyStatePress[nIdxPad][nCount] != 0)
					{
						return true;
					}
				}
			}

			break;
		case 1:
			for (int nCount = 0; nCount < XPADOTHER_MAX; nCount++)
			{// ���͏��J�E���g
				if (nCount == XPADOTHER_TRIGGER_LEFT || nCount == XPADOTHER_TRIGGER_RIGHT ||
					nCount == XPADOTHER_STICK_L_UP || nCount == XPADOTHER_STICK_L_LEFT || nCount == XPADOTHER_STICK_L_RIGHT || nCount == XPADOTHER_STICK_L_DOWN ||
					nCount == XPADOTHER_STICK_R_UP || nCount == XPADOTHER_STICK_R_LEFT || nCount == XPADOTHER_STICK_R_RIGHT || nCount == XPADOTHER_STICK_R_DOWN)
				{
					if (m_aJoyStateTrigger[nIdxPad][nCount] == 0x80)
					{
						return true;
					}
				}
				else
				{
					if (m_aJoyStateTrigger[nIdxPad][nCount] != 0)
					{
						return true;
					}
				}
			}

			break;
		default:
			for (int nCount = 0; nCount < XPADOTHER_MAX; nCount++)
			{// ���͏��J�E���g
				if (nCount == XPADOTHER_TRIGGER_LEFT || nCount == XPADOTHER_TRIGGER_RIGHT ||
					nCount == XPADOTHER_STICK_L_UP || nCount == XPADOTHER_STICK_L_LEFT || nCount == XPADOTHER_STICK_L_RIGHT || nCount == XPADOTHER_STICK_L_DOWN ||
					nCount == XPADOTHER_STICK_R_UP || nCount == XPADOTHER_STICK_R_LEFT || nCount == XPADOTHER_STICK_R_RIGHT || nCount == XPADOTHER_STICK_R_DOWN)
				{
					if (m_aJoyStateRelease[nIdxPad][nCount] == 0x80)
					{
						return true;
					}
				}
				else
				{
					if (m_aJoyStateRelease[nIdxPad][nCount] != 0)
					{
						return true;
					}
				}
			}

			break;
		}
	}

	return false;
}

//=============================================================================
// X�p�b�h�̓��͏��(�v���X���)���擾	(private)	*** CInputXPad ***
//=============================================================================
bool CInputXPad::GetPress(int nButton, int nIdxPad)
{
	if (m_Controllers[nIdxPad].bConnected)
		return (m_aJoyStatePress[nIdxPad][0] & nButton) ? true : false;
	else
		return false;
}
bool CInputXPad::GetPress(XPADOTHER nButton, int nIdxPad)
{
	if (m_Controllers[nIdxPad].bConnected)
		return (m_aJoyStatePress[nIdxPad][nButton] & 0x80) ? true : false;
	else
		return false;
}

//=============================================================================
// X�p�b�h�̓��͏��(�g���K�[���)���擾	(private)	*** CInputXPad ***
//=============================================================================
bool CInputXPad::GetTrigger(int nButton, int nIdxPad)
{
	if (m_Controllers[nIdxPad].bConnected)
		return (m_aJoyStateTrigger[nIdxPad][0] & nButton) ? true : false;
	else
		return false;
}
bool CInputXPad::GetTrigger(XPADOTHER nButton, int nIdxPad)
{
	if (m_Controllers[nIdxPad].bConnected)
		return (m_aJoyStateTrigger[nIdxPad][nButton] & 0x80) ? true : false;
	else
		return false;
}

//=============================================================================
// X�p�b�h�̓��͏��(�����[�X���)���擾	(private)	*** CInputXPad ***
//=============================================================================
bool CInputXPad::GetRelease(int nButton, int nIdxPad)
{
	if (m_Controllers[nIdxPad].bConnected)
		return (m_aJoyStateRelease[nIdxPad][0] & nButton) ? true : false;
	else
		return false;
}
bool CInputXPad::GetRelease(XPADOTHER nButton, int nIdxPad)
{
	if (m_Controllers[nIdxPad].bConnected)
		return (m_aJoyStateRelease[nIdxPad][nButton] & 0x80) ? true : false;
	else
		return false;
}

//=============================================================================
// X�p�b�h�̓��͏��(�����[�X���)���擾	(private)	*** CInputXPad ***
//=============================================================================
bool CInputXPad::GetRepeat(int nButton, int nIdxPad)
{
	if (m_Controllers[nIdxPad].bConnected)
		return (m_aJoyStateRepeat[nIdxPad][0] & nButton) ? true : false;
	else
		return false;
}
bool CInputXPad::GetRepeat(XPADOTHER nButton, int nIdxPad)
{
	if (m_Controllers[nIdxPad].bConnected)
		return (m_aJoyStateRepeat[nIdxPad][nButton] & 0x80) ? true : false;
	else
		return false;
}

//=============================================================================
// X�p�b�h�̓��͏��(�X�e�B�b�N���)���擾	(private)	*** CInputXPad ***
//=============================================================================
bool CInputXPad::GetStick(int nLR, int nIdxPad)
{
	if (m_Controllers[nIdxPad].bConnected)
	{
		if (nLR == 0)
		{
			for (int nCntStick = XPADOTHER_STICK_L_UP; nCntStick <= XPADOTHER_STICK_L_DOWN; nCntStick++)
			{
				if (m_aJoyStatePress[nIdxPad][nCntStick] == 0x80)
				{
					return true;
				}
			}
		}
		else
		{
			for (int nCntStick = XPADOTHER_STICK_R_UP; nCntStick <= XPADOTHER_STICK_R_DOWN; nCntStick++)
			{
				if (m_aJoyStatePress[nIdxPad][nCntStick] == 0x80)
				{
					return true;
				}
			}
		}
	}

	return false;
}

//=============================================================================
// X�p�b�h�̓��͏��(�X�e�B�b�N�������)���擾	(private)	*** CInputXPad ***
//=============================================================================
float CInputXPad::GetStickRot(int nLR, int nIdxPad)
{
	if (m_Controllers[nIdxPad].bConnected)
	{
		if (nLR == 0)
		{
			return m_LStickRot[nIdxPad];
		}
		else
		{
			return m_RStickRot[nIdxPad];
		}
	}

	return false;
}