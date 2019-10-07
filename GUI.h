//=============================================================================
//
// GUI���� [GUI.h]
// Author : �������� Sato_Asumi
//
//=============================================================================
#ifndef _GUI_H_
#define _GUI_H_

#include "main.h"
#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"

//=============================================================================
// �J�����N���X
//=============================================================================
class CGui
{
public:
	CGui();
	~CGui();
	static void SetUp(HWND hWnd);
	static void Teardown(void);
	static void Renderer(void);
	virtual void Init(void) = 0;
	virtual void Uninit(void) = 0;
	virtual void Update(void) = 0;

	//---------------------
	//		Set & Get
	//---------------------
	static void SetDraw(bool bDraw) { m_bDrawGui = bDraw; }	//�`����
	static bool GetDraw(void) { return m_bDrawGui; }

protected:
	void RetouchMinimumInt(int * nRetouch, int nMinimum);
	void RetouchMinimumFloat(float * fRetouch, float fMinimum);

	//---------------------
	//		Set & Get
	//---------------------
	static void SetUse(bool bUse) { m_bUseGui = bUse; }	//�g�p���

	static bool m_bUseGui;	//GUI���g�p���Ă��邩�ǂ���
	static bool m_bDrawGui;	//GUI��`�悷�邩�ǂ���
};
#endif