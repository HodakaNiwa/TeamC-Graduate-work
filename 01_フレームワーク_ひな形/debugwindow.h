//=============================================================================
//
// �f�o�b�O�E�C���h�E���� [GUI.h]
// Author : �������� Sato_Asumi
//
//=============================================================================
#ifndef _DEBUGWINDOE_H_
#define _DEBUGWINDOE_H_

#include "main.h"
#include "GUI.h"

//=============================================================================
// �f�o�b�O�E�C���h�E�N���X
//=============================================================================
class CDebugWindow : public CGui
{
public:
	CDebugWindow();
	~CDebugWindow();
	void Init(void);
	void Uninit(void);
	void Update(void);
	static CDebugWindow * Create(void);
};
#endif