//=============================================================================
//
// �Q�[���p�b�h���͏��� [gamepad.h]
// Author : ���� �����@[ SATO ASUMI ]
//
//=============================================================================
#ifndef _GAMEPAD_H_
#define _GAMEPAD_H_

#include "main.h"
#include "input.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define GAMEPAD_DEADZONE		(3)
#define MAX_ZONE				(1000)
#define	GAMEPAD_NUM_KEY_MAX		(128)		// �L�[�̍ő吔
#define WARK_ZONE				(50.0f)		

#define BUTTON_X	   (ELECOM_X)		// X�{�^���̔ԍ�
#define BUTTON_Y	   (ELECOM_Y)		// Y�{�^���̔ԍ�
#define BUTTON_A	   (ELECOM_A)		// A�{�^���̔ԍ�
#define BUTTON_B	   (ELECOM_B)		// B�{�^���̔ԍ�
#define BUTTON_L1	   (ELECOM_L1)		// L1�{�^���̔ԍ�
#define BUTTON_R1	   (ELECOM_R1)		// R1�{�^���̔ԍ�
#define BUTTON_L2	   (ELECOM_L2)		// L2�{�^���̔ԍ�
#define BUTTON_R2	   (ELECOM_R2)		// R2�{�^���̔ԍ�
#define BUTTON_L_STICK (ELECOM_L_STICK)	// L_STICK�{�^���̔ԍ�
#define BUTTON_R_STICK (ELECOM_R_STICK)	// R_STICK�{�^���̔ԍ�
#define BUTTON_BACK	   (ELECOM_BACK)	// BACK�{�^���̔ԍ�
#define BUTTON_START   (ELECOM_START)	// START�{�^���̔ԍ�
#define BUTTON_GUIDE   (ELECOM_GUIDE)	// GUIDE�{�^���̔ԍ�
#define BUTTON_MAX	   (ELECOM_MAX)		// �{�^���̍ő�

//=============================================================================
// �Q�[���p�b�h�̓��̓N���X
//=============================================================================
class CGamePad : public CInput
{
public:
	typedef enum
	{// �A�i���O�X�e�B�b�N�̕����̎��(4����)
		ANALOG_STICK_UP = 0,
		ANALOG_STICK_RIGHT,
		ANALOG_STICK_LEFT,
		ANALOG_STICK_DOWN,
		ANALOG_STICK_MAX
	}ANALOG_STICK;

	typedef enum
	{// �\���L�[�̕����̎��
		POV_UP = 0,
		POV_DOWN,
		POV_RIGHT,
		POV_LEFT,
		POV_MAX
	}POV;

	typedef enum
	{//�{�^���̎��
		ELECOM_X = 0,
		ELECOM_Y,
		ELECOM_A,
		ELECOM_B,
		ELECOM_L1,
		ELECOM_R1,
		ELECOM_L2,
		ELECOM_R2,
		ELECOM_L_STICK,
		ELECOM_R_STICK,
		ELECOM_BACK,
		ELECOM_START,
		ELECOM_GUIDE,
		ELECOM_MAX
	}ELECOM_CTRL;

	CGamePad();
	~CGamePad();
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);
	bool GetGamePadPress(int nKey);
	bool GetGamePadTrigger(int nKey);
	bool GetGamePadRelease(int nKey);
	bool GetGamePadLeftStickPress(int nKey);
	bool GetGamePadLeftStickTrigger(int nKey);
	bool GetGamePadRightStickPress(int nKey);
	bool GetGamePadRightStickTrigger(int nKey);
	bool GetPovTpress(int nAngle);
	bool GetPovTtigger(int nAngle);
	LPDIRECTINPUTDEVICE8 GetDevGamepad(void) { return m_pDevGamePad; };		//���̓f�o�C�X�̎擾

	DIJOYSTATE2 m_aGamePad;	// �R���g���[���[�̏��

private:
	//�֐�
	static BOOL CALLBACK EnumJoysticksCallback(const DIDEVICEINSTANCE* pdidInstance, VOID* pContext);	//�R�[���o�b�N�֐�
	static BOOL CALLBACK EnumAxesCallback(LPCDIDEVICEOBJECTINSTANCE lpddoi, LPVOID pvRef);				//�R�[���o�b�N�֐�

	//�ϐ�
	static LPDIRECTINPUTDEVICE8 m_pDevGamePad;															// ���̓f�o�C�X(k�R���g���[���[)�ւ̃|�C���^
	DIDEVCAPS m_GamepadCaps;																		// �Q�[���p�b�h�̋@�\���


	BYTE		m_aGamePadKeyState[GAMEPAD_NUM_KEY_MAX];											// �W���C�X�e�B�b�N�̃v���X���͏��
	BYTE		m_aGamePadKeyStateTrigger[GAMEPAD_NUM_KEY_MAX];									// �W���C�X�e�B�b�N�̃g���K�[���͏��
	BYTE		m_aGamePadKeyStateRelease[GAMEPAD_NUM_KEY_MAX];									// �W���C�X�e�B�b�N�̃����[�X���͏��
	BYTE		m_aLeftAnalogStick[ANALOG_STICK_MAX];												// ���A�i���O�X�e�B�b�N�̃v���X���
	BYTE		m_aLeftAnalogStickTrigger[ANALOG_STICK_MAX];										// ���A�i���O�X�e�B�b�N�̃g���K�[���
	BYTE		m_aRightAnalogStick[ANALOG_STICK_MAX];												// �E�A�i���O�X�e�B�b�N�̃v���X���
	BYTE		m_aRightAnalogStickTrigger[ANALOG_STICK_MAX];										// �E�A�i���O�X�e�B�b�N�̃g���K�[���
	BYTE		m_aPov[POV_MAX];																	// �\���L�[�̃v���X���
	BYTE		m_aPovTrigger[POV_MAX];																// �\���L�[�̃g���K�[���

	int		   	    m_JoyconAxesNumber;		// joystick�̋@�\�R�[���o�b�N�֐��Ŏg���W���C�R���̔ԍ�
};
#endif