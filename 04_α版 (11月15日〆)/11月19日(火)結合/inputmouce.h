//=============================================================================
//
// �}�E�X���͏��� [inputmouce.h]
// Author : ���� �����@[ SATO ASUMI ]
//
//=============================================================================
#ifndef _INPUTMOUCE_H_
#define _INPUTMOUCE_H_

#include "main.h"
#include "input.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define NUM_MOUSEMAX (8)

//=============================================================================
// �L�[�{�[�h�̓��̓N���X
//=============================================================================
class CInputMouse : public CInput
{
public:
	typedef enum
	{
		BUTTON_LEFT = 0,
		BUTTON_RIGHT,
		BUTTON_WHEEL,
		BUTTON_BACK1,
		BUTTON_FRONT,
	}BUTTON;

	typedef struct
	{
		RECT    moveRect;     // ��ʏ�œ�����͈�
		int     x;            // X���W
		int     y;            // Y���W
		bool    lButton;      // ���{�^��
		bool    rButton;      // �E�{�^��
		bool    cButton;      // �^�񒆃{�^��
		int     moveAdd;      // �ړ���
		RECT    imgRect;      // �}�E�X�p�摜��`
		int     imgWidth;     // �}�E�X�摜��
		int     imgHeight;    // �}�E�X�摜����
	} MOUSE_INFO;

	CInputMouse();
	~CInputMouse();
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);
	bool GetPress(int nKey);
	bool GetTrigger(int nKey);
	bool GetRelease(int nKey);

	LONG GetIX(void);
	LONG GetIY(void);
	LONG GetIZ(void);

	void GetScreenToWorld(void);
	D3DXVECTOR3 GetWorldPos(void) { return m_WorldPos; };
	POINT GetScreenPos(void) { return m_ScreenPos; }
	DIMOUSESTATE2 GetMouseState(void) { return m_MouseState; }
private:
	static LPDIRECTINPUTDEVICE8 m_pMouse;
	MOUSE_INFO m_MouseInfo;					// �}�E�X���
	HWND m_hWnd;

	//�}�E�X�̏��
	DIMOUSESTATE2			m_MouseState;
	DIMOUSESTATE2			m_MouseStatePress;		// �}�E�X�̓��͏��i�v���X���j
	DIMOUSESTATE2			m_MouseStateTrigger;	// �}�E�X�̓��͏��i�g���K�[���j
	DIMOUSESTATE2			m_MouseStateRelease;	// �}�E�X�̓��͏��i�����[�X���

	//�����O�ł��
	LONG m_MouseIX;
	LONG m_MouseIY;
	LONG m_MouseIZ;

	//���[���h���W
	D3DXVECTOR3 m_WorldPos;		//���[���h���W
	POINT		m_ScreenPos;	//�X�N���[�����W
};

#endif