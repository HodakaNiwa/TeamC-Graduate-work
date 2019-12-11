//*****************************************************************************
//
//     RawInput��p�����}�E�X�������̏���[RawMouse.h]
//      Author:Hodaka Niwa
//
//*****************************************************************************
#ifndef _RAWMOUSE_H_
#define _RAWMOUSE_H_

//*****************************************************************************
//    �}�N����`
//*****************************************************************************
#define RAWINPUT_DEVMOUSE_MAX (4)

//*****************************************************************************
//    �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"

//*****************************************************************************
//    RawMouse�N���X�̒�`
//*****************************************************************************
class CRawMouse
{
public:    // �N�ł��A�N�Z�X�\
	//------------------------
	//  �{�^���̎��
	//------------------------
	typedef enum
	{
		// �{�^��
		RIMS_BUTTON_LEFT,
		RIMS_BUTTON_RIGHT,
		RIMS_BUTTON_MIDDLE,

		// �X���C�h
		RIMS_SLIDE_UP,
		RIMS_SLIDE_DOWN,
		RIMS_SLIDE_LEFT,
		RIMS_SLIDE_RIGHT,

		RIMS_BUTTON_MAX
	}RIMS_BUTTON;

	~CRawMouse();

	static CRawMouse *Create(HWND hWnd);
	static CRawMouse *ReCreate(HWND hWnd);
	static HRESULT RegisterRawInput(HWND hWnd);

	HRESULT Init(HWND hWnd);
	void Uninit(void);
	void Update(LPARAM lParam);
	void AdjustMouseData(void);
	void ClearMouseData(void);

	UINT GetMouseNum(void);
	HANDLE GetMouseHandle(int nIdx);
	RAWMOUSE GetMouse(int nIdx);

	bool GetPress(int nIdx, int nButton);
	bool GetTrigger(int nIdx, int nButton);
	bool GetRelease(int nIdx, int nButton);
	bool GetRepeat(int nIdx, int nButton);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	CRawMouse();
	void ArrayMouseData(RAWINPUT *raw, int nCntMouse);
	void AdjustMouseData_Trigger(int nCntMouse, int nCntButton);
	void AdjustMouseData_Release(int nCntMouse, int nCntButton);
	void AdjustMouseData_Repeat(int nCntMouse, int nCntButton);

	static CRawMouse *m_pInstance;										// RawMouse�N���X�̃C���X�^���X
	UINT m_nDeviceNum;													// ���ݐڑ�����Ă���f�o�C�X�̐�
	UINT m_nMouseNum;													// ���ݐڑ�����Ă���}�E�X�̐�
	HANDLE m_MouseHandle[RAWINPUT_DEVMOUSE_MAX];						// �ڑ�����Ă���}�E�X�ւ̃n���h��
	RAWINPUTHEADER m_MouseHeader[RAWINPUT_DEVMOUSE_MAX];				// �}�E�X�̃w�b�_�[(�ڍ׏��)���i�[����
	RAWMOUSE m_Mouse[RAWINPUT_DEVMOUSE_MAX];							// �}�E�X�̏����i�[����
	HWND m_hWnd;														// �E�B���h�E�n���h���ۑ��p
	BYTE m_aMousePress[RAWINPUT_DEVMOUSE_MAX][RIMS_BUTTON_MAX];			// ���݃}�E�X�̃{�^���������ꂽ���ǂ���(�v���X���)
	BYTE m_aMouseTrigger[RAWINPUT_DEVMOUSE_MAX][RIMS_BUTTON_MAX];		// �}�E�X�̃{�^���������ꂽ���ǂ���(�g���K�[���)
	BYTE m_aMouseRelease[RAWINPUT_DEVMOUSE_MAX][RIMS_BUTTON_MAX];		// �}�E�X�̃{�^���������ꂽ���ǂ���(�����[�X���)
	BYTE m_aMouseRepeat[RAWINPUT_DEVMOUSE_MAX][RIMS_BUTTON_MAX];		// �}�E�X�̃{�^���������ꂽ���ǂ���(���s�[�g���)
	DWORD m_aMouseCounter[RAWINPUT_DEVMOUSE_MAX][RIMS_BUTTON_MAX];		// �}�E�X�����͂���Ă�Ԃ𐔂���J�E���^�[
	bool m_bMouseTrigger[RAWINPUT_DEVMOUSE_MAX][RIMS_BUTTON_MAX];		// �}�E�X�̃g���K�[�����N���������ǂ���
	bool m_bMouseRelease[RAWINPUT_DEVMOUSE_MAX][RIMS_BUTTON_MAX];		// �}�E�X�̃����[�X�����N���������ǂ���
};

#endif