//=============================================================================
//
// �Q�[���p�b�h���͏��� [input.cpp]
// Author : ���������@SATOU ASUMI
//
//=============================================================================
#include "gamepad.h"

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================
LPDIRECTINPUTDEVICE8 CGamePad::m_pDevGamePad = NULL;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CGamePad::CGamePad()
{
	//�e�L�[�̏���������
	for (int nCnt = 0; nCnt < GAMEPAD_NUM_KEY_MAX; nCnt++)
	{
		m_aGamePadKeyState[nCnt] = '\0';
		m_aGamePadKeyStateTrigger[nCnt] = '\0';
		m_aGamePadKeyStateRelease[nCnt] = '\0';
	}
	for (int nCnt = 0; nCnt < ANALOG_STICK_MAX; nCnt++)
	{
		m_aLeftAnalogStick[nCnt] = '\0';
		m_aLeftAnalogStickTrigger[nCnt] = '\0';
	}
	for (int nCnt = 0; nCnt < POV_MAX; nCnt++)
	{
		m_aPov[nCnt] = '\0';
		m_aPovTrigger[nCnt] = '\0';
	}
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CGamePad::~CGamePad()
{
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CGamePad::Init(HINSTANCE hInstance, HWND hWnd)
{
	//�ϐ��錾
	HRESULT hr;

	//�������������Ăяo��
	CInput::Init(hInstance, hWnd);

	// ���̓f�o�C�X(�R���g���[���[)�̐���
	if (FAILED(m_pInput->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumJoysticksCallback, NULL, DIEDFL_ATTACHEDONLY)))		// ��4���� (ATTACHEDONLY = ���ݐڑ�����Ă���W���C�X�e�B�b�N�̂݌��o����)
	{
		return FALSE;
	}

	// �f�o�C�X�Ƀf�[�^�t�H�[�}�b�g��ݒ�
	if (m_pDevGamePad != NULL)
	{
		if (FAILED(hr = m_pDevGamePad->SetDataFormat(&c_dfDIJoystick2)))
		{
			return FALSE;
		}

		// �������[�h��ݒ�
		if (FAILED(hr = m_pDevGamePad->SetCooperativeLevel(hWnd, DISCL_EXCLUSIVE | DISCL_FOREGROUND)))
		{
			return FALSE;
		}

		// �Q�[���p�b�h�̋@�\�̏������W
		m_GamepadCaps.dwSize = sizeof(DIDEVCAPS);

		if (FAILED(hr = m_pDevGamePad->GetCapabilities(&m_GamepadCaps)))
		{
			m_pDevGamePad->Release();
			return FALSE;
		}

		// �e���̃��[�h�ݒ�
		hr = m_pDevGamePad->EnumObjects(EnumAxesCallback, (void*)m_pDevGamePad, DIDFT_AXIS);

		m_JoyconAxesNumber++;		// �ԍ���i�߂�

		// �Q�[���p�b�h�ւ̃A�N�Z�X�����l��(���͐���J�n)
		hr = m_pDevGamePad->Poll();

		if (FAILED(hr))
		{
			hr = m_pDevGamePad->Acquire();

			while (hr == DIERR_INPUTLOST)
			{
				hr = m_pDevGamePad->Acquire();
			}
		}
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CGamePad::Uninit(void)
{
	CInput::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CGamePad::Update(void)
{
	if (m_pDevGamePad != NULL)
	{
		DIJOYSTATE2 aGamePadKeyState;								// �R���g���[���[�̓��͏��
		BYTE aLeftAnalogStick[ANALOG_STICK_MAX] = { 0,0,0,0 };		// �X�e�B�b�N�̓��͏��
		BYTE aRightAnalogStick[ANALOG_STICK_MAX] = { 0,0,0,0 };		// �E�X�e�B�b�N�̓��͏��
		BYTE aPov[POV_MAX] = { 0,0,0,0 };							// �\���L�[�̓��͏��

		HRESULT hr;
		hr = m_pDevGamePad->Poll();

		// ���̓f�o�C�X����f�[�^���擾
		if (SUCCEEDED(hr = m_pDevGamePad->GetDeviceState(sizeof(DIJOYSTATE2), &aGamePadKeyState)))
		{
			// �{�^��
			for (int nCntKey = 0; nCntKey < GAMEPAD_NUM_KEY_MAX; nCntKey++)
			{
				// �R���g���[���[�̓��͏��̕ۑ�(�g���K�[)
				m_aGamePadKeyStateTrigger[nCntKey] = (m_aGamePadKeyState[nCntKey] ^ aGamePadKeyState.rgbButtons[nCntKey]) & aGamePadKeyState.rgbButtons[nCntKey];
				// �R���g���[���[�̓��͏��̕ۑ�(�����[�X)
				m_aGamePadKeyStateRelease[nCntKey] = (aGamePadKeyState.rgbButtons[nCntKey] ^ m_aGamePadKeyState[nCntKey]) & m_aGamePadKeyState[nCntKey];
				// �R���g���[���[�̓��͏��̕ۑ�(�v���X)
				m_aGamePadKeyState[nCntKey] = aGamePadKeyState.rgbButtons[nCntKey];
			}

			//--------------------------------------------------------
			// ���A�i���O�X�e�B�b�N�̓��͏��̎擾
			//--------------------------------------------------------
			if (aGamePadKeyState.lY < -GAMEPAD_DEADZONE)
			{// ���X�e�B�b�N����ɓ|����
				aLeftAnalogStick[ANALOG_STICK_UP] = 128;			// ���͂��ꂽ
			}
			else if (aGamePadKeyState.lY > GAMEPAD_DEADZONE)
			{// ���X�e�B�b�N�����ɓ|����
				aLeftAnalogStick[ANALOG_STICK_DOWN] = 128;			// ���͂��ꂽ
			}
			else if (aGamePadKeyState.lX > GAMEPAD_DEADZONE)
			{// ���X�e�B�b�N���E�ɓ|����
				aLeftAnalogStick[ANALOG_STICK_RIGHT] = 128;			// ���͂��ꂽ
			}
			else if (aGamePadKeyState.lX < -GAMEPAD_DEADZONE)
			{// ���X�e�B�b�N�����ɓ|����
				aLeftAnalogStick[ANALOG_STICK_LEFT] = 128;			// ���͂��ꂽ
			}

			for (int nCntAnalogStick = 0; nCntAnalogStick < ANALOG_STICK_MAX; nCntAnalogStick++)
			{// ���A�i���O�X�e�B�b�N�̕����̎�ޕ����[�v
			 // ���X�e�B�b�N�̃g���K�[���
				m_aLeftAnalogStickTrigger[nCntAnalogStick] = (m_aLeftAnalogStick[nCntAnalogStick] ^ aLeftAnalogStick[nCntAnalogStick])&aLeftAnalogStick[nCntAnalogStick];		// ���A�i���O�X�e�B�b�N�̃g���K�[���
																																																// ���X�e�B�b�N�̃v���X���
				m_aLeftAnalogStick[nCntAnalogStick] = aLeftAnalogStick[nCntAnalogStick];																						// ���A�i���O�X�e�B�b�N�̃v���X���
			}

			//--------------------------------------------------------
			// �E�A�i���O�X�e�B�b�N�̓��͏��̎擾
			//--------------------------------------------------------
			if (aGamePadKeyState.lRz < -GAMEPAD_DEADZONE)
			{// �E�X�e�B�b�N����ɓ|����
				aRightAnalogStick[ANALOG_STICK_UP] = 128;			// ���͂��ꂽ
			}
			else if (aGamePadKeyState.lRz > GAMEPAD_DEADZONE)
			{// �E�X�e�B�b�N�����ɓ|����
				aRightAnalogStick[ANALOG_STICK_DOWN] = 128;			// ���͂��ꂽ
			}
			else if (aGamePadKeyState.lZ > GAMEPAD_DEADZONE)
			{// �E�X�e�B�b�N���E�ɓ|����
				aRightAnalogStick[ANALOG_STICK_RIGHT] = 128;			// ���͂��ꂽ
			}
			else if (aGamePadKeyState.lZ < -GAMEPAD_DEADZONE)
			{// �E�X�e�B�b�N�����ɓ|����
				aRightAnalogStick[ANALOG_STICK_LEFT] = 128;			// ���͂��ꂽ
			}

			for (int nCntAnalogStick = 0; nCntAnalogStick < ANALOG_STICK_MAX; nCntAnalogStick++)
			{// �E�A�i���O�X�e�B�b�N�̕����̎�ޕ����[�v
			 // �E�X�e�B�b�N�̃g���K�[���
				m_aRightAnalogStickTrigger[nCntAnalogStick] = (m_aRightAnalogStick[nCntAnalogStick] ^ aRightAnalogStick[nCntAnalogStick])&aRightAnalogStick[nCntAnalogStick];		// ���A�i���O�X�e�B�b�N�̃g���K�[���
																																												// ���X�e�B�b�N�̃v���X���
				m_aRightAnalogStick[nCntAnalogStick] = aRightAnalogStick[nCntAnalogStick];																						// ���A�i���O�X�e�B�b�N�̃v���X���
			}

			//--------------------------------------------------------
			// �\���L�[�̓��͏��̎擾
			//--------------------------------------------------------
			if (aGamePadKeyState.rgdwPOV[0] == 0.0f)
			{// �\���L�[�̏��������
				aPov[POV_UP] = 128;			// ���͂��ꂽ
			}
			if (aGamePadKeyState.rgdwPOV[0] == 18000.0f)
			{// �\���L�[�̉���������
				aPov[POV_DOWN] = 128;			// ���͂��ꂽ
			}
			if (aGamePadKeyState.rgdwPOV[0] == 9000.0f)
			{// �\���L�[�̉E��������
				aPov[POV_RIGHT] = 128;			// ���͂��ꂽ
			}
			if (aGamePadKeyState.rgdwPOV[0] == 27000.0f)
			{// �\���L�[�̍���������
				aPov[POV_LEFT] = 128;			// ���͂��ꂽ
			}

			for (int nCntPov = 0; nCntPov < POV_MAX; nCntPov++)
			{// �\���L�[�̕����̎�ޕ����[�v
			
				m_aPovTrigger[nCntPov] = (m_aPov[nCntPov] ^ aPov[nCntPov])&aPov[nCntPov];	// �\���L�[�̃g���K�[���
				m_aPov[nCntPov] = aPov[nCntPov];											// �\���L�[�̃v���X���
			}
			m_aGamePad = aGamePadKeyState;			// ���݂̏�Ԃ�ۑ�
		}
		else
		{
			m_pDevGamePad->Acquire();				// �Q�[���p�b�h�ւ̃A�N�Z�X�����l��
		}
	}
}

//=============================================================================
// �R�[���o�b�N�֐�
//=============================================================================
BOOL CALLBACK CGamePad::EnumJoysticksCallback(const DIDEVICEINSTANCE* pdidInstance, VOID* pContext)
{ 
	HRESULT hr;

	// �񋓂��ꂽ�W���C�X�e�B�b�N�ւ̃C���^�[�t�F�C�X���擾
	hr = m_pInput->CreateDevice(pdidInstance->guidInstance, &m_pDevGamePad, NULL);

	// �f�o�C�X�����o�o���Ȃ������ꍇ
	if (FAILED(hr))
	{
		return DIENUM_CONTINUE;
	}

	return DIENUM_CONTINUE;
}

//=============================================================================
// �R�[���o�b�N�֐�
//=============================================================================
BOOL CALLBACK CGamePad::EnumAxesCallback(LPCDIDEVICEOBJECTINSTANCE lpddoi, LPVOID pvRef)
{
	//�ϐ��錾
	HRESULT hr;
	DIPROPRANGE diprg;

	ZeroMemory(&diprg, sizeof(diprg));
	diprg.diph.dwSize = sizeof(diprg);
	diprg.diph.dwHeaderSize = sizeof(diprg.diph);
	diprg.diph.dwObj = lpddoi->dwType;
	diprg.diph.dwHow = DIPH_BYID;
	diprg.lMin = -(GAMEPAD_DEADZONE / GAMEPAD_DEADZONE * MAX_ZONE);				// �\���L�[�̍ŏ��l [��] [��]
	diprg.lMax = GAMEPAD_DEADZONE / GAMEPAD_DEADZONE * MAX_ZONE;
	hr = m_pDevGamePad->SetProperty(DIPROP_RANGE, &diprg.diph);

	if (FAILED(hr))
	{
		return DIENUM_STOP;
	}

	return DIENUM_CONTINUE;
}

//=============================================================================
// �v���X����
//=============================================================================
bool CGamePad::GetGamePadPress(int nKey)
{
	if (m_pDevGamePad == NULL)
	{
		return false;
	}

	return (m_aGamePadKeyState[nKey] & 0x80) ? true : false;
}

//========================================================================================
// ���͏��(�g���K�[���)���擾
//========================================================================================
bool CGamePad::GetGamePadTrigger(int nKey)
{
	if (m_pDevGamePad == NULL)
	{
		return false;
	}

	return (m_aGamePadKeyStateTrigger[nKey] & 0x80) ? true : false;
}

//========================================================================================
// �̓��͏��(�����[�X���)���擾
//========================================================================================
bool CGamePad::GetGamePadRelease(int nKey)
{
	if (m_pDevGamePad == NULL)
	{
		return false;
	}

	return (m_aGamePadKeyStateRelease[nKey] & 0x80) ? true : false;
}

//==============================================================================================================================
// �Q�[���p�b�h�̍��X�e�B�b�N���͏��(�v���X���)���擾
//==============================================================================================================================
bool CGamePad::GetGamePadLeftStickPress(int nKey)
{
	if (m_pDevGamePad == NULL)
	{
		return false;
	}

	return (m_aLeftAnalogStick[nKey] & 0x80) ? true : false;
}

//==============================================================================================================================
// �Q�[���p�b�h�̍��X�e�B�b�N���͏��(�g���K�[���)���擾
//==============================================================================================================================
bool CGamePad::GetGamePadLeftStickTrigger(int nKey)
{
	if (m_pDevGamePad == NULL)
	{
		return false;
	}

	return (m_aLeftAnalogStickTrigger[nKey] & 0x80) ? true : false;
}

//==============================================================================================================================
// �Q�[���p�b�h�̉E�X�e�B�b�N���͏��(�v���X���)���擾
//==============================================================================================================================
bool CGamePad::GetGamePadRightStickPress(int nKey)
{
	if (m_pDevGamePad == NULL)
	{
		return false;
	}

	return (m_aRightAnalogStick[nKey] & 0x80) ? true : false;
}

//==============================================================================================================================
// �Q�[���p�b�h�̉E�X�e�B�b�N���͏��(�g���K�[���)���擾
//==============================================================================================================================
bool CGamePad::GetGamePadRightStickTrigger(int nKey)
{
	if (m_pDevGamePad == NULL)
	{
		return false;
	}

	return (m_aRightAnalogStickTrigger[nKey] & 0x80) ? true : false;
}

//=============================================================================
// �\���L�[���͏��(�g���K�[���)���擾
//=============================================================================
bool CGamePad::GetPovTpress(int nAngle)
{
	if (m_pDevGamePad == NULL)
	{
		return false;
	}

	return (m_aPov[nAngle] & 0x80) ? true : false;
}

//=============================================================================
// �\���L�[���͏��(�g���K�[���)���擾
//=============================================================================
bool CGamePad::GetPovTtigger(int nAngle)
{
	if (m_pDevGamePad == NULL)
	{
		return false;
	}

	return (m_aPovTrigger[nAngle] & 0x80) ? true : false;
}