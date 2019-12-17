//=============================================================================
//
// ���͏��� [input.h]
// Author : ���� �����@[ SATO ASUMI ]
//
//=============================================================================
#ifndef _INPUT_H_
#define _INPUT_H_

#include "main.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define MAX_KEY	(256)	//�L�[�̍ő吔

#define NUM_BUTTON_MAX		(32)					// �{�^���̍ő吔
#define MAX_CONTROLLERS (4)							// �R���g���[���[�̍ő吔
#define INPUT_DEADZONE  ( 0.44f * FLOAT(0x7FFF) )   // Default to 24% of the +/- 32767 range.   This is a reasonable default value but can be altered if needed.
#define XINPUT_STICK_MAX (32767)					// �X�e�B�b�N�ő�l
#define XINPUT_STICK_MIN (-32768)					// �X�e�B�b�N�ŏ��l

//=============================================================================
// �C���v�b�g�N���X
//=============================================================================
class CInput
{
public:
	CInput();
	virtual ~CInput();
	virtual HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	virtual void Uninit(void);
	virtual void Update(void) = 0;

protected:
	static LPDIRECTINPUT8 m_pInput;
	LPDIRECTINPUTDEVICE8 m_pDevice;
};

//=============================================================================
// �L�[�{�[�h�̓��̓N���X
//=============================================================================
class CInputKeyboard : public CInput
{
public:
	CInputKeyboard();
	~CInputKeyboard();
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);
	bool GetKeyboardPress(int nKey);
	bool GetKeyboardTrigger(int nKey);
	bool GetKeyboardRelease(int nKey);
	bool GetKeyboardRepeat(int nKey);

private:
	BYTE m_aKeyState[MAX_KEY];
	BYTE m_aKeyStateTrigger[MAX_KEY];			//�L�[�{�[�h�̓��͏��(�g���K�[���)
	BYTE m_aKeyStateRelease[MAX_KEY];			//�L�[�{�[�h�̓��͏��(�����[�X���)
	BYTE m_aKeyStateRepeat[MAX_KEY];			//�L�[�{�[�h�̓��͏��(���s�[�g���)
	int m_aKeyRepeatCounter[MAX_KEY];			//�L�[�{�[�h�̃��s�[�g�p�J�E���^�[
};

//=============================================================================
// X�C���v�b�g�̓��̓N���X
//=============================================================================
class CInputXPad
{// X�p�b�h
public:
	typedef enum
	{// �{�^���ȊO�̃^�C�v
		XPADOTHER_BUTTON = 0,										// �{�^��
		XPADOTHER_TRIGGER_LEFT,										// L�g���K�[
		XPADOTHER_TRIGGER_RIGHT,									// R�g���K�[
		XPADOTHER_STICK_L_UP,										// L�X�e�B�b�N��
		XPADOTHER_STICK_L_LEFT,										// L�X�e�B�b�N��
		XPADOTHER_STICK_L_RIGHT,									// L�X�e�B�b�N�E
		XPADOTHER_STICK_L_DOWN,										// L�X�e�B�b�N��
		XPADOTHER_STICK_R_UP,										// R�X�e�B�b�N��
		XPADOTHER_STICK_R_LEFT,										// R�X�e�B�b�N��
		XPADOTHER_STICK_R_RIGHT,									// R�X�e�B�b�N�E
		XPADOTHER_STICK_R_DOWN,										// R�X�e�B�b�N��
		XPADOTHER_KEY_UP,										// R�X�e�B�b�N��
		XPADOTHER_KEY_LEFT,										// R�X�e�B�b�N��
		XPADOTHER_KEY_RIGHT,									// R�X�e�B�b�N�E
		XPADOTHER_KEY_DOWN,										// R�X�e�B�b�N��
		XPADOTHER_MAX
	}XPADOTHER;

	typedef struct
	{// XINPUT�R���g���[���[
		XINPUT_STATE state;											// ���
		bool bConnected;											// ���͗L��
	}CONTROLER_STATE;

	CInputXPad();													// �R���X�g���N�^
	virtual ~CInputXPad();											// �f�X�g���N�^

	HRESULT Init(HINSTANCE hInstance, HWND hWnd);					// ����������
	void Uninit(void);												// �I������
	void Update(void);												// �X�V����

	static int GetInputNum(void);									// ���͐��擾

	bool GetALL(int nType, int nIdxPad);							// �I�[��
	bool GetPress(int nButton, int nIdxPad);						// �v���X
	bool GetPress(XPADOTHER nButton, int nIdxPad);					// �v���X
	bool GetTrigger(int nButton, int nIdxPad);						// �g���K�[
	bool GetTrigger(XPADOTHER nButton, int nIdxPad);				// �g���K�[
	bool GetRelease(int nButton, int nIdxPad);						// �����[�X
	bool GetRelease(XPADOTHER nButton, int nIdxPad);				// �����[�X
	bool GetRepeat(int nButton, int nIdxPad);						// ���s�[�g
	bool GetRepeat(XPADOTHER nButton, int nIdxPad);					// ���s�[�g

	bool GetStick(int nLR, int nIdxPad);							// �X�e�B�b�N
	float GetStickRot(int nLR, int nIdxPad);						// �X�e�B�b�N����

private:
	HRESULT UpdateControllerState(void);							// �R���g���[���[���͐��X�V

	CONTROLER_STATE m_Controllers[MAX_CONTROLLERS];					// XINPUT�R���g���[���[
	bool			m_bDeadZoneOn;									// �X�e�B�b�N

	static int		m_nInputNum;									// ���͐�

	WORD m_aJoyStatePress[MAX_CONTROLLERS][XPADOTHER_MAX];			// �v���X
	WORD m_aJoyStateTrigger[MAX_CONTROLLERS][XPADOTHER_MAX];		// �g���K�[
	WORD m_aJoyStateRelease[MAX_CONTROLLERS][XPADOTHER_MAX];		// �����[�X
	WORD m_aJoyStateRepeat[MAX_CONTROLLERS][XPADOTHER_MAX];			// ���s�[�g
	int m_aJoyRepeatCounter[MAX_CONTROLLERS][XPADOTHER_MAX];		// ���s�[�g�p�J�E���^�[

	float m_LStickRot[MAX_CONTROLLERS];								// L�X�e�B�b�N����
	float m_RStickRot[MAX_CONTROLLERS];								// R�X�e�B�b�N����
};
#endif