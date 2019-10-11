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
	
private:
	BYTE m_aKeyState[MAX_KEY];
	BYTE m_aKeyStateTrigger[MAX_KEY];			//�L�[�{�[�h�̓��͏��(�g���K�[���)
	BYTE m_aKeyStateRelease[MAX_KEY];			//�L�[�{�[�h�̓��͏��(�����[�X���)
};
#endif